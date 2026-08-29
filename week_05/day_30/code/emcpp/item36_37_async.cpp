#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace {
int compute(int value) {
    return value * value;
}

template <typename Function, typename... Arguments>
auto reallyAsync(Function&& function, Arguments&&... arguments) {
    return std::async(std::launch::async,
                      std::forward<Function>(function),
                      std::forward<Arguments>(arguments)...);
}

class JoiningThread {
public:
    explicit JoiningThread(std::thread thread) : thread_(std::move(thread)) {}

    ~JoiningThread() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;

private:
    std::thread thread_;
};

bool joinsDuringStackUnwinding() {
    int result = 0;
    try {
        JoiningThread worker(std::thread([&result] { result = 42; }));
        throw std::runtime_error("demonstrate an early exit");
    } catch (const std::runtime_error&) {
        return result == 42;
    }
}
}  // namespace

int main() {
    auto deferred = std::async(std::launch::deferred, compute, 3);
    const bool deferredDetected =
        deferred.wait_for(std::chrono::seconds(0)) == std::future_status::deferred;
    const int deferredResult = deferred.get();

    auto asynchronous = reallyAsync(compute, 4);
    const bool isNotDeferred =
        asynchronous.wait_for(std::chrono::seconds(0)) != std::future_status::deferred;
    const int asynchronousResult = asynchronous.get();

    const bool passed = deferredDetected && deferredResult == 9 && isNotDeferred &&
                        asynchronousResult == 16 && joinsDuringStackUnwinding();
    std::cout << "launch policy is tested by status, not elapsed time.\n";
    std::cout << (passed ? "Item 36-37 checks passed\n"
                         : "Item 36-37 checks failed\n");
    return passed ? 0 : 1;
}
