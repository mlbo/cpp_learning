#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include "thread_pool.h"

int main() {
    ThreadPool pool(2U);

    auto owned_value = std::make_unique<int>(21);
    std::future<int> moved_result = pool.submit(
        [](std::unique_ptr<int> value) { return *value * 2; },
        std::move(owned_value));

    int referenced_value = 10;
    std::future<int> referenced_result = pool.submit(
        [](int& value) {
            value += 5;
            return value;
        },
        std::ref(referenced_value));

    const bool accepted_move_transferred_ownership = owned_value == nullptr;
    const bool move_only_result_correct = moved_result.get() == 42;
    const bool reference_result_correct = referenced_result.get() == 15 &&
                                          referenced_value == 15;

    pool.shutdown();

    auto rejected_value = std::make_unique<int>(7);
    bool rejected_after_stop = false;
    try {
        static_cast<void>(pool.submit(
            [](std::unique_ptr<int> value) { return *value; },
            std::move(rejected_value)));
    } catch (const std::runtime_error&) {
        rejected_after_stop = true;
    }

    const bool rejected_argument_preserved = rejected_value != nullptr &&
                                             *rejected_value == 7;
    if (!accepted_move_transferred_ownership || !move_only_result_correct ||
        !reference_result_correct || !rejected_after_stop ||
        !rejected_argument_preserved) {
        std::cerr << "thread pool argument ownership regression test failed\n";
        return 1;
    }

    std::cout << "move-only、std::ref 与停止拒绝的参数所有权契约通过\n";
    return 0;
}
