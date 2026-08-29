#include <future>
#include <iostream>
#include <stdexcept>

#include "joining_thread_group.h"

namespace {

class ThrowOnCopy final {
public:
    ThrowOnCopy() = default;
    ThrowOnCopy(const ThrowOnCopy&) {
        throw std::runtime_error("simulated thread argument copy failure");
    }

    void operator()() const noexcept {}
};

}  // namespace

int main() {
    std::promise<void> release_promise;
    std::shared_future<void> release = release_promise.get_future().share();
    bool creation_failure_observed = false;

    week5::JoiningThreadGroup workers;
    workers.start([release] { release.wait(); });

    try {
        const ThrowOnCopy throwing_callable;
        workers.start(throwing_callable);
    } catch (const std::runtime_error&) {
        creation_failure_observed = true;
    }

    release_promise.set_value();
    workers.join_all();

    if (!creation_failure_observed || workers.size() != 1U) {
        std::cerr << "partial thread construction rollback test failed\n";
        return 1;
    }

    week5::JoiningThreadGroup self_join_workers;
    std::promise<void> start_self_join;
    const std::shared_future<void> start_signal = start_self_join.get_future().share();
    std::promise<bool> self_join_rejected_promise;
    std::future<bool> self_join_rejected = self_join_rejected_promise.get_future();
    self_join_workers.start(
        [&self_join_workers, start_signal, &self_join_rejected_promise] {
            start_signal.wait();
            try {
                self_join_workers.join_all();
                self_join_rejected_promise.set_value(false);
            } catch (const std::logic_error&) {
                self_join_rejected_promise.set_value(true);
            }
        });
    start_self_join.set_value();
    const bool public_self_join_rejected = self_join_rejected.get();
    self_join_workers.join_all();

    if (!public_self_join_rejected) {
        std::cerr << "thread group public self-join contract test failed\n";
        return 1;
    }

    std::cout << "partial construction stays owned and public self-join is rejected\n";
    return 0;
}
