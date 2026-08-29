#include "inference_engine.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

using namespace inference;
using namespace std::chrono_literals;

std::atomic<int> failures{0};

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

template <typename Future>
void checkReady(Future& future, const char* label) {
    if (future.wait_for(2s) != std::future_status::ready) {
        throw std::runtime_error(std::string(label) +
                                 " did not become ready within two seconds");
    }
}

enum class DelegationKind {
    thread,
    async
};

template <typename Function>
void runDelegated(DelegationKind kind, Function function) {
    if (kind == DelegationKind::thread) {
        std::thread child(std::move(function));
        child.join();
        return;
    }

    auto child = std::async(std::launch::async, std::move(function));
    child.get();
}

struct CallbackStopState {
    CallbackStopState()
        : first_started(first_started_signal.get_future().share()),
          release_first(release_first_signal.get_future().share()),
          second_started(second_started_signal.get_future().share()),
          release_second(release_second_signal.get_future().share()) {}

    std::atomic<size_t> calls{0};
    std::atomic<size_t> completed{0};
    std::promise<void> first_started_signal;
    std::shared_future<void> first_started;
    std::promise<void> release_first_signal;
    std::shared_future<void> release_first;
    std::promise<void> second_started_signal;
    std::shared_future<void> second_started;
    std::promise<void> release_second_signal;
    std::shared_future<void> release_second;
};

class CallbackStopLayer final : public Layer {
public:
    CallbackStopLayer(ParallelInferenceEngine** engine,
                      std::shared_ptr<CallbackStopState> state)
        : engine_(engine), state_(std::move(state)) {}

    Tensor forward(const Tensor& input) const override {
        const size_t call = state_->calls.fetch_add(1, std::memory_order_acq_rel);
        if (call == 0) {
            state_->first_started_signal.set_value();
            state_->release_first.wait();
            (*engine_)->stop();
        } else if (call == 1) {
            state_->second_started_signal.set_value();
            state_->release_second.wait();
        }
        state_->completed.fetch_add(1, std::memory_order_release);
        return input;
    }

    std::string name() const override { return "CallbackStopLayer"; }

private:
    ParallelInferenceEngine** engine_;
    std::shared_ptr<CallbackStopState> state_;
};

class CallbackBarrier {
public:
    explicit CallbackBarrier(size_t expected) : expected_(expected) {}

    void arriveAndWait() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++arrived_;
        condition_.notify_all();
        // 加超时保护：若 expected_ 与实际线程数不符（用例被改但忘了同步），
        // 会超时失败而非永久挂起，便于在 CI 快速定位。
        if (!condition_.wait_for(lock, std::chrono::seconds(30),
                                  [this]() { return arrived_ == expected_; })) {
            throw std::runtime_error(
                "CallbackBarrier 超时：expected_=" + std::to_string(expected_) +
                " 但 arrived_=" + std::to_string(arrived_) +
                "，检查用例线程数是否与 expected 一致");
        }
    }

private:
    const size_t expected_;
    size_t arrived_{0};
    std::mutex mutex_;
    std::condition_variable condition_;
};

class CallbackInferenceProbeLayer final : public Layer {
public:
    CallbackInferenceProbeLayer(ParallelInferenceEngine** target,
                                std::shared_ptr<std::atomic<size_t>> callbacks,
                                std::shared_ptr<CallbackBarrier> barrier = {})
        : target_(target),
          callbacks_(std::move(callbacks)),
          barrier_(std::move(barrier)) {}

    Tensor forward(const Tensor& input) const override {
        if (barrier_) {
            barrier_->arriveAndWait();
        }
        const auto require_leaf_exception = [](auto&& operation, const char* name) {
            try {
                operation();
                throw std::runtime_error(std::string(name) +
                                         " callback inference was not rejected");
            } catch (const std::logic_error& error) {
                if (std::string(error.what()).find("leaf computations") ==
                    std::string::npos) {
                    throw std::runtime_error(std::string(name) +
                                             " rejection did not explain the leaf rule");
                }
            }
        };

        require_leaf_exception(
            [&]() { static_cast<void>((*target_)->infer(input)); }, "infer");
        require_leaf_exception(
            [&]() { static_cast<void>((*target_)->inferBatch({input})); }, "inferBatch");

        InferenceResult nested = (*target_)->inferAsync(input).get();
        if (nested.success ||
            nested.error_message.find("leaf computations") == std::string::npos) {
            throw std::runtime_error("callback inferAsync was not rejected by the leaf rule");
        }

        require_leaf_exception(
            [&]() { static_cast<void>((*target_)->inferBatchParallel({input})); },
            "inferBatchParallel");

        try {
            (*target_)->join();
            throw std::runtime_error("callback join was not rejected");
        } catch (const std::logic_error& error) {
            if (std::string(error.what()).find("must not wait for workers") ==
                std::string::npos) {
                throw std::runtime_error(
                    "callback join rejection did not explain the blocking wait");
            }
        }
        callbacks_->fetch_add(1, std::memory_order_release);
        return input;
    }

    std::string name() const override { return "CallbackInferenceProbeLayer"; }

private:
    ParallelInferenceEngine** target_;
    std::shared_ptr<std::atomic<size_t>> callbacks_;
    std::shared_ptr<CallbackBarrier> barrier_;
};

class CrossStopLayer final : public Layer {
public:
    CrossStopLayer(ParallelInferenceEngine** target,
                   std::shared_ptr<CallbackBarrier> barrier)
        : target_(target), barrier_(std::move(barrier)) {}

    Tensor forward(const Tensor& input) const override {
        barrier_->arriveAndWait();
        (*target_)->stop();
        return input;
    }

    std::string name() const override { return "CrossStopLayer"; }

private:
    ParallelInferenceEngine** target_;
    std::shared_ptr<CallbackBarrier> barrier_;
};

struct DelegatedInferenceState {
    std::atomic<size_t> model_calls{0};
    bool captured_callback_context{false};
    bool nested_success{false};
    std::string nested_error;
};

class DelegatedInferenceLayer final : public Layer {
public:
    DelegatedInferenceLayer(ParallelInferenceEngine** target,
                            std::shared_ptr<DelegatedInferenceState> state,
                            DelegationKind delegation,
                            bool propagate_context)
        : target_(target),
          state_(std::move(state)),
          delegation_(delegation),
          propagate_context_(propagate_context) {}

    Tensor forward(const Tensor& input) const override {
        const size_t call =
            state_->model_calls.fetch_add(1, std::memory_order_acq_rel);
        if (call != 0) {
            return input;
        }

        const InferenceExecutionContext context =
            InferenceExecutionContext::capture();
        state_->captured_callback_context = context.carriesCallbackContext();

        runDelegated(delegation_, [this, &input, context]() {
            const auto infer = [this, &input]() {
                const InferenceResult nested = (*target_)->inferAsync(input).get();
                state_->nested_success = nested.success;
                state_->nested_error = nested.error_message;
            };
            if (propagate_context_) {
                context.run(infer);
            } else {
                infer();
            }
        });
        return input;
    }

    std::string name() const override { return "DelegatedInferenceLayer"; }

private:
    ParallelInferenceEngine** target_;
    std::shared_ptr<DelegatedInferenceState> state_;
    DelegationKind delegation_;
    bool propagate_context_;
};

struct DelegatedStopState {
    std::atomic<size_t> completions{0};
    std::atomic<bool> captured_callback_context{false};
};

class DelegatedStopLayer final : public Layer {
public:
    DelegatedStopLayer(ParallelInferenceEngine** target,
                       std::shared_ptr<DelegatedStopState> state,
                       DelegationKind delegation,
                       bool propagate_context,
                       std::shared_ptr<CallbackBarrier> barrier = {})
        : target_(target),
          state_(std::move(state)),
          delegation_(delegation),
          propagate_context_(propagate_context),
          barrier_(std::move(barrier)) {}

    Tensor forward(const Tensor& input) const override {
        if (barrier_) {
            barrier_->arriveAndWait();
        }
        const InferenceExecutionContext context =
            InferenceExecutionContext::capture();
        state_->captured_callback_context.store(
            context.carriesCallbackContext(), std::memory_order_release);

        runDelegated(delegation_, [this, context]() {
            const auto request_stop = [this]() { (*target_)->stop(); };
            if (propagate_context_) {
                context.run(request_stop);
            } else {
                request_stop();
            }
        });
        state_->completions.fetch_add(1, std::memory_order_release);
        return input;
    }

    std::string name() const override { return "DelegatedStopLayer"; }

private:
    ParallelInferenceEngine** target_;
    std::shared_ptr<DelegatedStopState> state_;
    DelegationKind delegation_;
    bool propagate_context_;
    std::shared_ptr<CallbackBarrier> barrier_;
};

struct DelegatedJoinState {
    bool captured_callback_context{false};
    bool join_rejected{false};
};

class PropagatedJoinLayer final : public Layer {
public:
    PropagatedJoinLayer(ParallelInferenceEngine** target,
                        std::shared_ptr<DelegatedJoinState> state)
        : target_(target), state_(std::move(state)) {}

    Tensor forward(const Tensor& input) const override {
        const InferenceExecutionContext context =
            InferenceExecutionContext::capture();
        state_->captured_callback_context = context.carriesCallbackContext();

        std::thread child([this, context]() {
            context.run([this]() {
                try {
                    (*target_)->join();
                } catch (const std::logic_error& error) {
                    if (std::string(error.what()).find("must not wait for workers") !=
                        std::string::npos) {
                        state_->join_rejected = true;
                        return;
                    }
                    throw;
                }
                throw std::runtime_error("propagated callback join was not rejected");
            });
        });
        child.join();
        return input;
    }

    std::string name() const override { return "PropagatedJoinLayer"; }

private:
    ParallelInferenceEngine** target_;
    std::shared_ptr<DelegatedJoinState> state_;
};

class CountingLayer final : public Layer {
public:
    explicit CountingLayer(std::shared_ptr<std::atomic<size_t>> executions)
        : executions_(std::move(executions)) {}

    Tensor forward(const Tensor& input) const override {
        executions_->fetch_add(1, std::memory_order_relaxed);
        return input;
    }

    std::string name() const override { return "CountingLayer"; }

private:
    std::shared_ptr<std::atomic<size_t>> executions_;
};

class ModelLifetimeLayer final : public Layer {
public:
    explicit ModelLifetimeLayer(std::shared_ptr<int> lifetime)
        : lifetime_(std::move(lifetime)) {}

    Tensor forward(const Tensor& input) const override {
        if (!lifetime_) {
            throw std::runtime_error("model lifetime token unexpectedly expired");
        }
        return input;
    }

    std::string name() const override { return "ModelLifetimeLayer"; }

private:
    std::shared_ptr<int> lifetime_;
};

class StartGate {
public:
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() { return open_; });
    }

    void open() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            open_ = true;
        }
        condition_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    bool open_{false};
};

struct DestroyControl {
    DestroyControl()
        : started(started_signal.get_future().share()),
          release(release_signal.get_future().share()) {}

    std::promise<void> started_signal;
    std::shared_future<void> started;
    std::promise<void> release_signal;
    std::shared_future<void> release;
    ParallelInferenceEngine** engine{nullptr};
};

class DestroyEngineLayer final : public Layer {
public:
    explicit DestroyEngineLayer(std::shared_ptr<DestroyControl> control)
        : control_(std::move(control)) {}

    Tensor forward(const Tensor& input) const override {
        control_->started_signal.set_value();
        control_->release.wait();
        ParallelInferenceEngine* engine = *control_->engine;
        *control_->engine = nullptr;
        delete engine;
        return input;
    }

    std::string name() const override { return "DestroyEngineLayer"; }

private:
    std::shared_ptr<DestroyControl> control_;
};

struct HoldControl {
    HoldControl()
        : started(started_signal.get_future().share()),
          release(release_signal.get_future().share()) {}

    std::promise<void> started_signal;
    std::shared_future<void> started;
    std::promise<void> release_signal;
    std::shared_future<void> release;
};

class HoldLayer final : public Layer {
public:
    explicit HoldLayer(std::shared_ptr<HoldControl> control)
        : control_(std::move(control)) {}

    Tensor forward(const Tensor& input) const override {
        control_->started_signal.set_value();
        control_->release.wait();
        return input;
    }

    std::string name() const override { return "HoldLayer"; }

private:
    std::shared_ptr<HoldControl> control_;
};

EngineConfig quietConfig(size_t threads, size_t queue_size = 256) {
    EngineConfig config;
    config.num_threads = threads;
    config.batch_size = 8;
    config.max_queue_size = queue_size;
    config.enable_profiling = false;
    return config;
}

void testWorkerCanRequestStop() {
    auto state = std::make_shared<CallbackStopState>();
    ParallelInferenceEngine* engine_view = nullptr;
    Sequential model;
    model.add(CallbackStopLayer(&engine_view, state));
    auto engine =
        std::make_unique<ParallelInferenceEngine>(&model, quietConfig(1, 2));
    engine_view = engine.get();

    auto first = engine->inferAsync(Tensor::ones({1}));
    checkReady(state->first_started, "callback stop first request start");
    auto second = engine->inferAsync(Tensor::ones({1}));
    EngineStatus status = engine->status();
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 1);
    CHECK(status.in_flight_calls == 0);

    state->release_first_signal.set_value();
    checkReady(first, "worker self-stop first result");
    CHECK(first.get().success);
    checkReady(state->second_started, "callback stop queued request start");

    status = engine->status();
    CHECK(engine->closed());
    CHECK(!engine->stopped());
    CHECK(status.lifecycle == EngineLifecycle::draining);
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 0);
    CHECK(status.in_flight_calls == 0);
    CHECK(state->completed.load(std::memory_order_acquire) == 1);

    auto rejected = engine->inferAsync(Tensor::ones({1}));
    checkReady(rejected, "callback stop post-close rejection");
    CHECK(!rejected.get().success);

    state->release_second_signal.set_value();
    checkReady(second, "callback stop queued result");
    CHECK(second.get().success);
    engine->join();
    CHECK(engine->stopped());
    CHECK(state->completed.load(std::memory_order_acquire) == 2);
}

void testSameEngineCallbackInferenceIsRejected() {
    auto callbacks = std::make_shared<std::atomic<size_t>>(0);
    ParallelInferenceEngine* engine_view = nullptr;
    Sequential model;
    model.add(CallbackInferenceProbeLayer(&engine_view, callbacks));
    auto engine = std::make_unique<ParallelInferenceEngine>(&model, quietConfig(1));
    engine_view = engine.get();

    auto future = engine->inferAsync(Tensor::ones({1}));
    checkReady(future, "same-engine callback inference result");
    const InferenceResult result = future.get();
    CHECK(result.success);
    CHECK(callbacks->load(std::memory_order_acquire) == 1);
    CHECK(engine->status().lifecycle == EngineLifecycle::accepting);

    engine->join();
}

void testOneWayCrossEngineCallbackInferenceIsRejected() {
    auto target_executions = std::make_shared<std::atomic<size_t>>(0);
    Sequential target_model;
    target_model.add(CountingLayer(target_executions));
    auto target_engine =
        std::make_unique<ParallelInferenceEngine>(&target_model, quietConfig(1));
    ParallelInferenceEngine* target_view = target_engine.get();

    auto callbacks = std::make_shared<std::atomic<size_t>>(0);
    Sequential outer_model;
    outer_model.add(CallbackInferenceProbeLayer(&target_view, callbacks));
    ParallelInferenceEngine outer_engine(&outer_model, quietConfig(1));

    auto future = outer_engine.inferAsync(Tensor::ones({1}));
    checkReady(future, "one-way cross-engine callback result");
    const InferenceResult result = future.get();
    CHECK(result.success);
    CHECK(callbacks->load(std::memory_order_acquire) == 1);
    CHECK(target_executions->load(std::memory_order_acquire) == 0);
    CHECK(target_engine->status().lifecycle == EngineLifecycle::accepting);

    outer_engine.join();
    target_engine->join();
}

void testCrossEngineCallbackCycleIsRejectedAtBothEdges() {
    auto barrier = std::make_shared<CallbackBarrier>(2);
    auto first_callbacks = std::make_shared<std::atomic<size_t>>(0);
    auto second_callbacks = std::make_shared<std::atomic<size_t>>(0);
    ParallelInferenceEngine* first_view = nullptr;
    ParallelInferenceEngine* second_view = nullptr;

    Sequential first_model;
    first_model.add(
        CallbackInferenceProbeLayer(&second_view, first_callbacks, barrier));
    Sequential second_model;
    second_model.add(
        CallbackInferenceProbeLayer(&first_view, second_callbacks, barrier));

    auto first_engine =
        std::make_unique<ParallelInferenceEngine>(&first_model, quietConfig(1));
    auto second_engine =
        std::make_unique<ParallelInferenceEngine>(&second_model, quietConfig(1));
    first_view = first_engine.get();
    second_view = second_engine.get();

    auto first = first_engine->inferAsync(Tensor::ones({1}));
    auto second = second_engine->inferAsync(Tensor::ones({1}));
    checkReady(first, "first cross-engine cycle edge");
    checkReady(second, "second cross-engine cycle edge");
    CHECK(first.get().success);
    CHECK(second.get().success);
    CHECK(first_callbacks->load(std::memory_order_acquire) == 1);
    CHECK(second_callbacks->load(std::memory_order_acquire) == 1);

    first_engine->join();
    second_engine->join();
}

void testUnpropagatedDelegatedInferenceShowsCallerPrecondition() {
    auto same_state = std::make_shared<DelegatedInferenceState>();
    ParallelInferenceEngine* same_view = nullptr;
    Sequential same_model;
    same_model.add(DelegatedInferenceLayer(
        &same_view, same_state, DelegationKind::thread, false));
    auto same_engine =
        std::make_unique<ParallelInferenceEngine>(&same_model, quietConfig(2));
    same_view = same_engine.get();

    auto same = same_engine->inferAsync(Tensor::ones({1}));
    checkReady(same, "unpropagated same-engine delegated inference");
    CHECK(same.get().success);
    CHECK(same_state->captured_callback_context);
    CHECK(same_state->nested_success);
    CHECK(same_state->nested_error.empty());
    CHECK(same_state->model_calls.load(std::memory_order_acquire) == 2);
    same_engine->join();

    auto target_executions = std::make_shared<std::atomic<size_t>>(0);
    Sequential target_model;
    target_model.add(CountingLayer(target_executions));
    ParallelInferenceEngine target_engine(&target_model, quietConfig(1));
    ParallelInferenceEngine* target_view = &target_engine;

    auto cross_state = std::make_shared<DelegatedInferenceState>();
    Sequential outer_model;
    outer_model.add(DelegatedInferenceLayer(
        &target_view, cross_state, DelegationKind::async, false));
    ParallelInferenceEngine outer_engine(&outer_model, quietConfig(1));

    auto cross = outer_engine.inferAsync(Tensor::ones({1}));
    checkReady(cross, "unpropagated cross-engine delegated inference");
    CHECK(cross.get().success);
    CHECK(cross_state->captured_callback_context);
    CHECK(cross_state->nested_success);
    CHECK(cross_state->nested_error.empty());
    CHECK(cross_state->model_calls.load(std::memory_order_acquire) == 1);
    CHECK(target_executions->load(std::memory_order_acquire) == 1);

    outer_engine.join();
    target_engine.join();
}

void testPropagatedDelegatedInferenceIsRejected() {
    auto same_state = std::make_shared<DelegatedInferenceState>();
    ParallelInferenceEngine* same_view = nullptr;
    Sequential same_model;
    same_model.add(DelegatedInferenceLayer(
        &same_view, same_state, DelegationKind::thread, true));
    auto same_engine =
        std::make_unique<ParallelInferenceEngine>(&same_model, quietConfig(1));
    same_view = same_engine.get();

    auto same = same_engine->inferAsync(Tensor::ones({1}));
    checkReady(same, "propagated same-engine delegated inference rejection");
    CHECK(same.get().success);
    CHECK(same_state->captured_callback_context);
    CHECK(!same_state->nested_success);
    CHECK(same_state->nested_error.find("leaf computations") != std::string::npos);
    CHECK(same_state->model_calls.load(std::memory_order_acquire) == 1);
    same_engine->join();

    auto target_executions = std::make_shared<std::atomic<size_t>>(0);
    Sequential target_model;
    target_model.add(CountingLayer(target_executions));
    ParallelInferenceEngine target_engine(&target_model, quietConfig(1));
    ParallelInferenceEngine* target_view = &target_engine;

    auto cross_state = std::make_shared<DelegatedInferenceState>();
    Sequential outer_model;
    outer_model.add(DelegatedInferenceLayer(
        &target_view, cross_state, DelegationKind::async, true));
    ParallelInferenceEngine outer_engine(&outer_model, quietConfig(1));

    auto cross = outer_engine.inferAsync(Tensor::ones({1}));
    checkReady(cross, "propagated cross-engine delegated inference rejection");
    CHECK(cross.get().success);
    CHECK(cross_state->captured_callback_context);
    CHECK(!cross_state->nested_success);
    CHECK(cross_state->nested_error.find("leaf computations") != std::string::npos);
    CHECK(target_executions->load(std::memory_order_acquire) == 0);

    outer_engine.join();
    target_engine.join();
}

void testDelegatedSameEngineStopIsAlwaysNonBlocking() {
    auto state = std::make_shared<DelegatedStopState>();
    ParallelInferenceEngine* engine_view = nullptr;
    Sequential model;
    model.add(DelegatedStopLayer(
        &engine_view, state, DelegationKind::thread, false));
    auto engine =
        std::make_unique<ParallelInferenceEngine>(&model, quietConfig(1));
    engine_view = engine.get();

    auto result = engine->inferAsync(Tensor::ones({1}));
    checkReady(result, "delegated same-engine stop result");
    CHECK(result.get().success);
    CHECK(state->captured_callback_context.load(std::memory_order_acquire));
    CHECK(state->completions.load(std::memory_order_acquire) == 1);
    CHECK(engine->status().lifecycle == EngineLifecycle::draining);
    CHECK(!engine->stopped());

    engine->join();
    CHECK(engine->stopped());
}

void testPropagatedThreadSameEngineStopIsAlwaysNonBlocking() {
    auto state = std::make_shared<DelegatedStopState>();
    ParallelInferenceEngine* engine_view = nullptr;
    Sequential model;
    model.add(DelegatedStopLayer(
        &engine_view, state, DelegationKind::thread, true));
    auto engine =
        std::make_unique<ParallelInferenceEngine>(&model, quietConfig(1));
    engine_view = engine.get();

    auto result = engine->inferAsync(Tensor::ones({1}));
    checkReady(result, "propagated thread same-engine stop result");
    CHECK(result.get().success);
    CHECK(state->captured_callback_context.load(std::memory_order_acquire));
    CHECK(state->completions.load(std::memory_order_acquire) == 1);
    CHECK(engine->status().lifecycle == EngineLifecycle::draining);
    CHECK(!engine->stopped());

    engine->join();
    CHECK(engine->stopped());
}

void testDelegatedCrossEngineStopCycleIsNonBlocking() {
    auto barrier = std::make_shared<CallbackBarrier>(2);
    auto first_state = std::make_shared<DelegatedStopState>();
    auto second_state = std::make_shared<DelegatedStopState>();
    ParallelInferenceEngine* first_view = nullptr;
    ParallelInferenceEngine* second_view = nullptr;

    Sequential first_model;
    first_model.add(
        DelegatedStopLayer(
            &second_view, first_state, DelegationKind::thread, false, barrier));
    Sequential second_model;
    second_model.add(
        DelegatedStopLayer(
            &first_view, second_state, DelegationKind::thread, false, barrier));

    auto first_engine =
        std::make_unique<ParallelInferenceEngine>(&first_model, quietConfig(1));
    auto second_engine =
        std::make_unique<ParallelInferenceEngine>(&second_model, quietConfig(1));
    first_view = first_engine.get();
    second_view = second_engine.get();

    auto first = first_engine->inferAsync(Tensor::ones({1}));
    auto second = second_engine->inferAsync(Tensor::ones({1}));
    checkReady(first, "delegated cross-engine stop first result");
    checkReady(second, "delegated cross-engine stop second result");
    CHECK(first.get().success);
    CHECK(second.get().success);
    CHECK(first_state->captured_callback_context.load(std::memory_order_acquire));
    CHECK(second_state->captured_callback_context.load(std::memory_order_acquire));
    CHECK(first_state->completions.load(std::memory_order_acquire) == 1);
    CHECK(second_state->completions.load(std::memory_order_acquire) == 1);
    CHECK(first_engine->closed());
    CHECK(second_engine->closed());
    CHECK(!first_engine->stopped());
    CHECK(!second_engine->stopped());

    first_engine->join();
    second_engine->join();
}

void testPropagatedAsyncCrossEngineStopCycleIsNonBlocking() {
    auto barrier = std::make_shared<CallbackBarrier>(2);
    auto first_state = std::make_shared<DelegatedStopState>();
    auto second_state = std::make_shared<DelegatedStopState>();
    ParallelInferenceEngine* first_view = nullptr;
    ParallelInferenceEngine* second_view = nullptr;

    Sequential first_model;
    first_model.add(
        DelegatedStopLayer(
            &second_view, first_state, DelegationKind::async, true, barrier));
    Sequential second_model;
    second_model.add(
        DelegatedStopLayer(
            &first_view, second_state, DelegationKind::async, true, barrier));

    auto first_engine =
        std::make_unique<ParallelInferenceEngine>(&first_model, quietConfig(1));
    auto second_engine =
        std::make_unique<ParallelInferenceEngine>(&second_model, quietConfig(1));
    first_view = first_engine.get();
    second_view = second_engine.get();

    auto first = first_engine->inferAsync(Tensor::ones({1}));
    auto second = second_engine->inferAsync(Tensor::ones({1}));
    checkReady(first, "propagated async cross-engine stop first result");
    checkReady(second, "propagated async cross-engine stop second result");
    CHECK(first.get().success);
    CHECK(second.get().success);
    CHECK(first_state->captured_callback_context.load(std::memory_order_acquire));
    CHECK(second_state->captured_callback_context.load(std::memory_order_acquire));
    CHECK(first_state->completions.load(std::memory_order_acquire) == 1);
    CHECK(second_state->completions.load(std::memory_order_acquire) == 1);
    CHECK(first_engine->closed());
    CHECK(second_engine->closed());
    CHECK(!first_engine->stopped());
    CHECK(!second_engine->stopped());

    first_engine->join();
    second_engine->join();
    CHECK(first_engine->stopped());
    CHECK(second_engine->stopped());
}

void testPropagatedDelegatedJoinIsRejectedWithoutClosing() {
    auto same_state = std::make_shared<DelegatedJoinState>();
    ParallelInferenceEngine* same_view = nullptr;
    Sequential same_model;
    same_model.add(PropagatedJoinLayer(&same_view, same_state));
    auto same_engine =
        std::make_unique<ParallelInferenceEngine>(&same_model, quietConfig(1));
    same_view = same_engine.get();

    auto same = same_engine->inferAsync(Tensor::ones({1}));
    checkReady(same, "propagated same-engine join rejection");
    CHECK(same.get().success);
    CHECK(same_state->captured_callback_context);
    CHECK(same_state->join_rejected);
    CHECK(same_engine->status().lifecycle == EngineLifecycle::accepting);
    same_engine->join();

    Sequential target_model;
    ParallelInferenceEngine target_engine(&target_model, quietConfig(1));
    ParallelInferenceEngine* target_view = &target_engine;
    auto cross_state = std::make_shared<DelegatedJoinState>();
    Sequential outer_model;
    outer_model.add(PropagatedJoinLayer(&target_view, cross_state));
    ParallelInferenceEngine outer_engine(&outer_model, quietConfig(1));

    auto cross = outer_engine.inferAsync(Tensor::ones({1}));
    checkReady(cross, "propagated cross-engine join rejection");
    CHECK(cross.get().success);
    CHECK(cross_state->captured_callback_context);
    CHECK(cross_state->join_rejected);
    CHECK(target_engine.status().lifecycle == EngineLifecycle::accepting);

    outer_engine.join();
    target_engine.join();
}

void testCrossEngineCallbackStopCycleOnlyCloses() {
    auto barrier = std::make_shared<CallbackBarrier>(2);
    ParallelInferenceEngine* first_view = nullptr;
    ParallelInferenceEngine* second_view = nullptr;

    Sequential first_model;
    first_model.add(CrossStopLayer(&second_view, barrier));
    Sequential second_model;
    second_model.add(CrossStopLayer(&first_view, barrier));

    auto first_engine =
        std::make_unique<ParallelInferenceEngine>(&first_model, quietConfig(1));
    auto second_engine =
        std::make_unique<ParallelInferenceEngine>(&second_model, quietConfig(1));
    first_view = first_engine.get();
    second_view = second_engine.get();

    auto first = first_engine->inferAsync(Tensor::ones({1}));
    auto second = second_engine->inferAsync(Tensor::ones({1}));
    checkReady(first, "first cross-engine callback stop");
    checkReady(second, "second cross-engine callback stop");
    CHECK(first.get().success);
    CHECK(second.get().success);
    CHECK(first_engine->closed());
    CHECK(second_engine->closed());
    CHECK(!first_engine->stopped());
    CHECK(!second_engine->stopped());

    first_engine->join();
    second_engine->join();
    CHECK(first_engine->stopped());
    CHECK(second_engine->stopped());
}

void testConcurrentStopAndSubmitStress() {
    constexpr size_t rounds = 40;
    constexpr size_t submitters = 4;
    constexpr size_t submissions_per_thread = 80;

    for (size_t round = 0; round < rounds; ++round) {
        auto executions = std::make_shared<std::atomic<size_t>>(0);
        Sequential model;
        model.add(CountingLayer(executions));
        ParallelInferenceEngine engine(&model, quietConfig(4, 64));
        StartGate start;
        std::vector<std::vector<std::future<InferenceResult>>> submitted(submitters);
        std::vector<std::thread> producers;

        for (size_t producer = 0; producer < submitters; ++producer) {
            producers.emplace_back([&, producer]() {
                start.wait();
                auto& futures = submitted[producer];
                futures.reserve(submissions_per_thread);
                for (size_t i = 0; i < submissions_per_thread; ++i) {
                    futures.push_back(engine.inferAsync(Tensor::ones({1})));
                }
            });
        }

        auto first_stop = std::async(std::launch::async, [&]() {
            start.wait();
            engine.stop();
            engine.join();
        });
        auto second_stop = std::async(std::launch::async, [&]() {
            start.wait();
            engine.stop();
            engine.join();
        });

        start.open();
        for (auto& producer : producers) {
            producer.join();
        }
        checkReady(first_stop, "first concurrent stop");
        checkReady(second_stop, "second concurrent stop");
        first_stop.get();
        second_stop.get();
        const EngineStatus stopped_status = engine.status();
        CHECK(stopped_status.lifecycle == EngineLifecycle::joined);
        CHECK(stopped_status.active_requests == 0);
        CHECK(stopped_status.queued_requests == 0);
        CHECK(stopped_status.in_flight_calls == 0);

        size_t successful_results = 0;
        size_t rejected_results = 0;
        for (auto& futures : submitted) {
            for (auto& future : futures) {
                checkReady(future, "submit/stop race result");
                const InferenceResult result = future.get();
                CHECK(result.success || !result.error_message.empty());
                if (result.success) {
                    ++successful_results;
                } else {
                    ++rejected_results;
                }
            }
        }
        CHECK(successful_results == executions->load(std::memory_order_relaxed));
        CHECK(successful_results + rejected_results ==
              submitters * submissions_per_thread);

        auto rejected = engine.inferAsync(Tensor::ones({1}));
        checkReady(rejected, "post-race rejection");
        CHECK(!rejected.get().success);
    }
}

void testExplicitJoinProvesModelCanBeReleased() {
    auto lifetime = std::make_shared<int>(1);
    const std::weak_ptr<int> lifetime_observer = lifetime;
    auto model = std::make_unique<Sequential>();
    model->add(ModelLifetimeLayer(lifetime));
    lifetime.reset();

    auto engine =
        std::make_unique<ParallelInferenceEngine>(model.get(), quietConfig(2));
    std::vector<std::future<InferenceResult>> results;
    for (int i = 0; i < 32; ++i) {
        results.push_back(engine->inferAsync(Tensor::ones({1})));
    }

    engine->stop();
    CHECK(engine->status().lifecycle == EngineLifecycle::draining);
    engine->join();
    const EngineStatus joined = engine->status();
    CHECK(joined.lifecycle == EngineLifecycle::joined);
    CHECK(joined.queued_requests == 0);
    CHECK(joined.active_requests == 0);
    CHECK(joined.in_flight_calls == 0);

    model.reset();
    CHECK(lifetime_observer.expired());
    CHECK(engine->status().lifecycle == EngineLifecycle::joined);
    for (auto& result : results) {
        checkReady(result, "explicit-join model-lifetime result");
        CHECK(result.get().success);
    }
    engine.reset();
}

void testDestructorDrainsAcceptedTasksAndPermitsModelRelease() {
    auto executions = std::make_shared<std::atomic<size_t>>(0);
    auto lifetime = std::make_shared<int>(1);
    const std::weak_ptr<int> lifetime_observer = lifetime;
    auto model = std::make_unique<Sequential>();
    model->add(CountingLayer(executions));
    model->add(ModelLifetimeLayer(lifetime));
    lifetime.reset();
    std::vector<std::future<InferenceResult>> futures;
    {
        auto engine = std::make_unique<ParallelInferenceEngine>(
            model.get(), quietConfig(3, 128));
        for (int i = 0; i < 90; ++i) {
            futures.push_back(engine->inferAsync(Tensor::ones({1})));
        }
    }

    CHECK(!lifetime_observer.expired());
    model.reset();
    CHECK(lifetime_observer.expired());
    for (auto& future : futures) {
        checkReady(future, "destructor-drained result");
        CHECK(future.get().success);
    }
    CHECK(executions->load(std::memory_order_relaxed) == futures.size());
}

[[noreturn]] void exitFromExpectedTermination() { std::_Exit(86); }

[[noreturn]] void exitProbeFailure(int code) { std::_Exit(code); }

int runCallbackDestructionProbe() {
    std::set_terminate(exitFromExpectedTermination);
    auto control = std::make_shared<DestroyControl>();
    ParallelInferenceEngine* engine = nullptr;
    control->engine = &engine;

    Sequential model;
    model.add(DestroyEngineLayer(control));
    engine = new ParallelInferenceEngine(&model, quietConfig(1, 2));
    auto active = engine->inferAsync(Tensor::ones({1}));

    if (control->started.wait_for(2s) != std::future_status::ready) {
        exitProbeFailure(2);
    }
    auto queued = engine->inferAsync(Tensor::ones({1}));
    if (!queued.valid()) {
        exitProbeFailure(3);
    }
    const EngineStatus status = engine->status();
    if (status.active_requests != 1 || status.queued_requests != 1 ||
        status.in_flight_calls != 0) {
        exitProbeFailure(4);
    }

    control->release_signal.set_value();
    active.wait();
    exitProbeFailure(5);
}

int runBatchDestructionProbe() {
    std::set_terminate(exitFromExpectedTermination);
    auto control = std::make_shared<HoldControl>();
    Sequential model;
    model.add(HoldLayer(control));
    auto* engine = new ParallelInferenceEngine(&model, quietConfig(1, 2));

    auto batch = std::async(std::launch::async, [engine]() {
        return engine->inferBatchParallel({Tensor::ones({1})});
    });
    if (control->started.wait_for(2s) != std::future_status::ready) {
        exitProbeFailure(6);
    }
    auto queued = engine->inferAsync(Tensor::ones({1}));
    if (!queued.valid() || !batch.valid()) {
        exitProbeFailure(7);
    }
    const EngineStatus status = engine->status();
    if (status.active_requests != 1 || status.queued_requests != 1 ||
        status.in_flight_calls != 1) {
        exitProbeFailure(8);
    }

    delete engine;
    exitProbeFailure(9);
}

void testConcurrentConfigSnapshots() {
    Sequential model;
    ParallelInferenceEngine engine(&model, quietConfig(3, 256));
    StartGate start;
    std::vector<std::future<InferenceResult>> futures;

    std::thread writer([&]() {
        start.wait();
        for (int i = 0; i < 10000; ++i) {
            EngineConfig config = quietConfig(3, static_cast<size_t>((i % 64) + 1));
            config.batch_size = static_cast<size_t>((i % 16) + 1);
            engine.setConfig(config);
        }
    });
    std::thread reader([&]() {
        start.wait();
        for (int i = 0; i < 10000; ++i) {
            const EngineConfig snapshot = engine.config();
            CHECK(snapshot.num_threads == 3);
            CHECK(snapshot.batch_size > 0);
            CHECK(snapshot.max_queue_size > 0);
        }
    });
    std::thread submitter([&]() {
        start.wait();
        std::vector<std::future<InferenceResult>> local;
        local.reserve(2000);
        for (int i = 0; i < 2000; ++i) {
            local.push_back(engine.inferAsync(Tensor::ones({1})));
        }
        futures = std::move(local);
    });

    start.open();
    writer.join();
    reader.join();
    submitter.join();
    engine.stop();
    engine.join();

    CHECK(futures.size() == 2000);
    for (auto& future : futures) {
        checkReady(future, "config/submit race result");
        const InferenceResult result = future.get();
        CHECK(result.success || !result.error_message.empty());
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--callback-destruction-probe") {
        return runCallbackDestructionProbe();
    }
    if (argc == 2 && std::string(argv[1]) == "--batch-destruction-probe") {
        return runBatchDestructionProbe();
    }
    if (argc != 1) {
        std::cerr << "unexpected argument\n";
        return 2;
    }

    testWorkerCanRequestStop();
    testSameEngineCallbackInferenceIsRejected();
    testOneWayCrossEngineCallbackInferenceIsRejected();
    testCrossEngineCallbackCycleIsRejectedAtBothEdges();
    testCrossEngineCallbackStopCycleOnlyCloses();
    testUnpropagatedDelegatedInferenceShowsCallerPrecondition();
    testPropagatedDelegatedInferenceIsRejected();
    testDelegatedSameEngineStopIsAlwaysNonBlocking();
    testPropagatedThreadSameEngineStopIsAlwaysNonBlocking();
    testDelegatedCrossEngineStopCycleIsNonBlocking();
    testPropagatedAsyncCrossEngineStopCycleIsNonBlocking();
    testPropagatedDelegatedJoinIsRejectedWithoutClosing();
    testConcurrentStopAndSubmitStress();
    testExplicitJoinProvesModelCanBeReleased();
    testDestructorDrainsAcceptedTasksAndPermitsModelRelease();
    testConcurrentConfigSnapshots();

    if (failures.load() != 0) {
        std::cerr << failures.load() << " concurrency check(s) failed\n";
        return 1;
    }
    std::cout << "All model inference concurrency tests passed\n";
    return 0;
}
