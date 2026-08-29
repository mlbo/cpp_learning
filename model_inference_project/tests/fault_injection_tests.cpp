#include "inference_engine.h"

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

namespace {

using namespace inference;

int failures = 0;

void fail(const std::string& message, int line) {
    ++failures;
    std::cerr << "FAIL line " << line << ": " << message << '\n';
}

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            fail(#condition, __LINE__); \
        } \
    } while (false)

EngineConfig quietConfig() {
    EngineConfig config;
    config.num_threads = 5;
    config.enable_profiling = false;
    return config;
}

void testPartialWorkerCreationRollback() {
    Sequential model;
    std::atomic<int> live_workers{0};
    int launch_attempts = 0;

    ParallelInferenceEngine::WorkerLauncher launcher =
        [&live_workers, &launch_attempts](ParallelInferenceEngine::WorkerTask task) {
            if (launch_attempts++ == 2) {
                throw std::runtime_error("injected worker creation failure");
            }
            return std::thread([&live_workers, task = std::move(task)]() mutable {
                ++live_workers;
                task();
                --live_workers;
            });
        };

    bool threw = false;
    try {
        ParallelInferenceEngine engine(&model, quietConfig(), std::move(launcher));
    } catch (const std::runtime_error&) {
        threw = true;
    }

    CHECK(threw);
    CHECK(launch_attempts == 3);
    CHECK(live_workers.load() == 0);
}

void testNonJoinableWorkerRejected() {
    Sequential model;
    ParallelInferenceEngine::WorkerLauncher launcher =
        [](ParallelInferenceEngine::WorkerTask) { return std::thread(); };

    bool threw = false;
    try {
        ParallelInferenceEngine engine(&model, quietConfig(), std::move(launcher));
    } catch (const std::runtime_error&) {
        threw = true;
    }
    CHECK(threw);
}

}  // namespace

int main() {
    testPartialWorkerCreationRollback();
    testNonJoinableWorkerRejected();

    if (failures != 0) {
        std::cerr << failures << " fault-injection check(s) failed\n";
        return 1;
    }
    std::cout << "All model inference fault-injection tests passed\n";
    return 0;
}
