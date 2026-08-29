#include <exception>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace {
int computeValue(int input) {
    if (input < 0) {
        throw std::invalid_argument("input must be non-negative");
    }
    return input * input;
}

struct ThreadOutcome {
    int value{0};
    std::exception_ptr error;
};

ThreadOutcome runWithThread(int input) {
    ThreadOutcome outcome;
    std::thread worker([input, &outcome] {
        try {
            outcome.value = computeValue(input);
        } catch (...) {
            outcome.error = std::current_exception();
        }
    });
    worker.join();
    return outcome;
}

bool demonstratesExceptionTransport() {
    auto result = std::async(std::launch::async, computeValue, -1);
    try {
        static_cast<void>(result.get());
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}
}  // namespace

int main() {
    const ThreadOutcome threadOutcome = runWithThread(10);
    if (threadOutcome.error != nullptr) {
        std::cerr << "thread approach unexpectedly failed\n";
        return 1;
    }

    auto taskResult = std::async(std::launch::async, computeValue, 10);
    const int value = taskResult.get();

    std::vector<std::future<int>> tasks;
    for (int input = 0; input < 5; ++input) {
        tasks.push_back(std::async(std::launch::async, computeValue, input));
    }
    std::vector<int> values;
    values.reserve(tasks.size());
    for (std::future<int>& task : tasks) {
        values.push_back(task.get());
    }

    const bool passed = threadOutcome.value == 100 && value == 100 &&
                        values == std::vector<int>({0, 1, 4, 9, 16}) &&
                        demonstratesExceptionTransport();
    std::cout << "std::future transports a typed value or a stored exception.\n";
    std::cout << (passed ? "Item 35 checks passed\n" : "Item 35 checks failed\n");
    return passed ? 0 : 1;
}
