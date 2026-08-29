#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "joining_thread_group.h"

namespace {
std::mutex printMutex;

void safePrint(const std::string& message) {
    const std::lock_guard<std::mutex> lock(printMutex);
    std::cout << message << '\n';
}

void workerFunction(int id, int& output) {
    output = id * id;
    safePrint("函数线程完成任务 " + std::to_string(id));
}

class Task {
public:
    Task(int id, int& output) : id_(id), output_(output) {}

    void operator()() const {
        output_ = id_ + 10;
        safePrint("函数对象完成任务 " + std::to_string(id_));
    }

private:
    int id_;
    int& output_;
};

class ThreadGuard {
public:
    explicit ThreadGuard(std::thread& thread) : thread_(thread) {}

    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

private:
    std::thread& thread_;
};
}  // namespace

int main() {
    int functionResult = 0;
    std::thread functionThread(workerFunction, 4, std::ref(functionResult));
    functionThread.join();

    int lambdaResult = 0;
    std::thread lambdaThread([&lambdaResult] { lambdaResult = 42; });
    lambdaThread.join();

    int functorResult = 0;
    std::thread functorThread{Task(3, functorResult)};
    functorThread.join();

    std::vector<int> slots(3, -1);
    week5::JoiningThreadGroup workers;
    workers.reserve(slots.size());
    for (std::size_t i = 0; i < slots.size(); ++i) {
        workers.start([i, &slots] { slots[i] = static_cast<int>(i); });
    }
    workers.join_all();

    int guardedResult = 0;
    {
        std::thread guardedThread([&guardedResult] { guardedResult = 7; });
        ThreadGuard guard(guardedThread);
    }

    const bool passed = functionResult == 16 && lambdaResult == 42 &&
                        functorResult == 13 && slots == std::vector<int>({0, 1, 2}) &&
                        guardedResult == 7;
    std::cout << "hardware_concurrency hint: " << std::thread::hardware_concurrency() << '\n';
    std::cout << (passed ? "thread checks passed\n" : "thread checks failed\n");
    return passed ? 0 : 1;
}
