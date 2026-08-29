#include "inference_engine.h"
#include "layers.h"
#include "tensor.h"

#include <atomic>
#include <chrono>
#include <cmath>
#include <exception>
#include <future>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

using namespace inference;
using namespace std::chrono_literals;

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

template <typename Exception, typename Function>
void checkThrows(Function&& function, const char* expression, int line) {
    try {
        function();
        fail(std::string("expected exception from ") + expression, line);
    } catch (const Exception&) {
    } catch (const std::exception& error) {
        fail(std::string("wrong exception from ") + expression + ": " + error.what(), line);
    }
}

#define CHECK_THROWS(exception_type, expression) \
    checkThrows<exception_type>([&]() { static_cast<void>(expression); }, #expression, __LINE__)

template <typename Result>
void checkReady(std::future<Result>& future, const char* label) {
    if (future.wait_for(2s) != std::future_status::ready) {
        throw std::runtime_error(std::string(label) +
                                 " did not become ready within two seconds");
    }
}

void testTensorContracts() {
    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float infinity = std::numeric_limits<float>::infinity();
    Tensor empty;
    CHECK(empty.empty());
    CHECK(empty.allFinite());
    CHECK(empty.size() == 0);
    CHECK(empty.data() == nullptr);
    CHECK(empty.sum() == 0.0f);
    CHECK_THROWS(std::invalid_argument, Tensor(std::vector<size_t>{}));
    CHECK_THROWS(std::invalid_argument, Tensor(std::vector<size_t>{2, 0}));
    CHECK_THROWS(std::overflow_error,
                 Tensor(std::vector<size_t>{std::numeric_limits<size_t>::max(), 2}));
    CHECK_THROWS(std::invalid_argument, empty.mean());
    CHECK_THROWS(std::invalid_argument, empty.max());
    CHECK_THROWS(std::invalid_argument, empty.softmax());
    CHECK_THROWS(std::out_of_range, empty[0]);

    Tensor matrix({2, 3}, {1, 2, 3, 4, 5, 6});
    CHECK(matrix.at(1, 2) == 6.0f);
    CHECK_THROWS(std::out_of_range, matrix[6]);
    CHECK_THROWS(std::out_of_range, matrix.at(2, 0));
    CHECK_THROWS(std::invalid_argument, Tensor::ones({3}).at(0, 0));
    CHECK_THROWS(std::invalid_argument, matrix + Tensor::ones({3, 2}));

    Tensor rhs({3, 2}, {1, 0, 0, 1, 1, 1});
    Tensor product = matrix.matmul(rhs);
    CHECK(product.shape() == std::vector<size_t>({2, 2}));
    CHECK(product.at(0, 0) == 4.0f);
    CHECK(product.at(1, 1) == 11.0f);
    CHECK_THROWS(std::invalid_argument, matrix.matmul(Tensor::ones({2, 2})));
    CHECK_THROWS(std::invalid_argument, Tensor::ones({2, 2, 2}).softmax());
    CHECK_THROWS(std::invalid_argument, Tensor::random({1}, nan, 1.0f));
    CHECK_THROWS(std::invalid_argument, Tensor::random({1}, 0.0f, infinity));
    CHECK_THROWS(std::invalid_argument, Tensor::randn({1}, nan, 1.0f));
    CHECK_THROWS(std::invalid_argument, Tensor::randn({1}, 0.0f, infinity));
    CHECK_THROWS(std::invalid_argument, Tensor({2}, {infinity, 0.0f}).softmax());
    CHECK_THROWS(std::invalid_argument, Tensor({2}, {-infinity, -infinity}).softmax());
    CHECK_THROWS(std::invalid_argument, Tensor({2}, {nan, 0.0f}).softmax());

    Tensor probabilities = Tensor({3}, {1.0f, 2.0f, 3.0f}).softmax();
    CHECK(probabilities.allFinite());
    CHECK(std::fabs(probabilities.sum() - 1.0f) < 1e-5f);
}

void testLayerContracts() {
    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float infinity = std::numeric_limits<float>::infinity();
    const float max_float = std::numeric_limits<float>::max();
    CHECK_THROWS(std::invalid_argument, Linear(0, 2));
    CHECK_THROWS(std::invalid_argument, Linear(Tensor::ones({3})));
    CHECK_THROWS(std::invalid_argument,
                 Linear(Tensor::ones({2, 3}), Tensor::zeros({3})));

    Linear linear(4, 2);
    CHECK_THROWS(std::invalid_argument, linear.forward(Tensor::ones({3})));
    CHECK_THROWS(std::invalid_argument, linear.forward(Tensor::ones({2, 3})));
    CHECK_THROWS(std::invalid_argument,
                 linear.forward(Tensor({4}, {infinity, 0.0f, 0.0f, 0.0f})));
    CHECK(linear.forward(Tensor::ones({4})).shape() == std::vector<size_t>({2}));
    CHECK(linear.forward(Tensor::ones({3, 4})).shape() ==
          std::vector<size_t>({3, 2}));
    CHECK_THROWS(std::overflow_error,
                 Linear(Tensor({1, 1}, {max_float}), Tensor::zeros({1}))
                     .forward(Tensor({1}, {2.0f})));

    Tensor invalid_weight = Tensor::ones({2, 4});
    invalid_weight[0] = nan;
    CHECK_THROWS(std::invalid_argument, Linear(invalid_weight));
    CHECK_THROWS(std::invalid_argument, linear.setWeight(invalid_weight));
    Tensor invalid_bias = Tensor::zeros({2});
    invalid_bias[1] = infinity;
    CHECK_THROWS(std::invalid_argument, linear.setBias(invalid_bias));

    CHECK_THROWS(std::invalid_argument, Dropout(nan));
    CHECK_THROWS(std::invalid_argument, Dropout(infinity));
    Dropout dropout(0.25f);
    CHECK(!dropout.training());
    dropout.setTraining(false);
    CHECK_THROWS(std::logic_error, dropout.setTraining(true));
    CHECK(dropout.forward(Tensor::ones({2})).sum() == 2.0f);
    CHECK_THROWS(std::invalid_argument,
                 dropout.forward(Tensor({1}, {infinity})));
    CHECK_THROWS(std::invalid_argument,
                 ReLU().forward(Tensor({1}, {infinity})));
    CHECK_THROWS(std::invalid_argument,
                 Sigmoid().forward(Tensor({1}, {nan})));

    CHECK_THROWS(std::invalid_argument, BatchNorm(4, nan));
    CHECK_THROWS(std::invalid_argument, BatchNorm(4, infinity));
    BatchNorm batch_norm(4);
    CHECK_THROWS(std::invalid_argument, batch_norm.forward(Tensor::ones({3})));
    CHECK(batch_norm.forward(Tensor::ones({2, 4})).shape() ==
          std::vector<size_t>({2, 4}));

    const Tensor batch_gamma({2}, {2.0f, 3.0f});
    const Tensor batch_beta({2}, {1.0f, -1.0f});
    const Tensor running_mean({2}, {10.0f, 20.0f});
    const Tensor running_variance({2}, {4.0f, 9.0f});
    BatchNorm loaded_batch_norm(
        batch_gamma, batch_beta, running_mean, running_variance);
    Tensor batch_normalized = loaded_batch_norm.forward(Tensor({2}, {12.0f, 23.0f}));
    CHECK(std::fabs(batch_normalized[0] - 3.0f) < 1e-4f);
    CHECK(std::fabs(batch_normalized[1] - 2.0f) < 1e-4f);
    CHECK(loaded_batch_norm.gamma().shape() == std::vector<size_t>({2}));
    CHECK(loaded_batch_norm.runningMean()[0] == 10.0f);
    loaded_batch_norm.setInferenceParameters(
        Tensor::ones({2}), Tensor::zeros({2}), Tensor::zeros({2}), Tensor::ones({2}));
    CHECK(loaded_batch_norm.gamma()[0] == 1.0f);
    CHECK(loaded_batch_norm.runningVariance()[1] == 1.0f);

    Tensor invalid_running_variance = running_variance;
    invalid_running_variance[0] = -1.0f;
    CHECK_THROWS(std::invalid_argument,
                 BatchNorm(batch_gamma,
                           batch_beta,
                           running_mean,
                           invalid_running_variance));
    Tensor invalid_batch_gamma = batch_gamma;
    invalid_batch_gamma[0] = nan;
    CHECK_THROWS(std::invalid_argument,
                 loaded_batch_norm.setInferenceParameters(
                     invalid_batch_gamma, batch_beta, running_mean, running_variance));
    CHECK(loaded_batch_norm.gamma()[0] == 1.0f);
    CHECK_THROWS(std::invalid_argument,
                 loaded_batch_norm.forward(Tensor({2}, {nan, 0.0f})));
    CHECK_THROWS(std::overflow_error,
                 BatchNorm(Tensor({2}, {max_float, max_float}),
                           Tensor({2}, {max_float, max_float}),
                           Tensor::zeros({2}),
                           Tensor::ones({2}))
                     .forward(Tensor::ones({2})));

    CHECK_THROWS(std::invalid_argument, LayerNorm(3, infinity));
    LayerNorm layer_norm(3);
    Tensor normalized = layer_norm.forward(Tensor({2, 3}, {1, 2, 3, 4, 5, 6}));
    CHECK(normalized.shape() == std::vector<size_t>({2, 3}));
    CHECK(std::fabs(normalized.at(0, 0) - normalized.at(1, 0)) < 1e-5f);
    CHECK_THROWS(std::invalid_argument, layer_norm.forward(Tensor::ones({2, 2})));

    LayerNorm loaded_layer_norm(
        Tensor({2}, {2.0f, 3.0f}), Tensor({2}, {1.0f, -1.0f}));
    Tensor affine_output = loaded_layer_norm.forward(Tensor({2}, {1.0f, 3.0f}));
    CHECK(std::fabs(affine_output[0] + 1.0f) < 1e-4f);
    CHECK(std::fabs(affine_output[1] - 2.0f) < 1e-4f);
    CHECK(loaded_layer_norm.gamma()[0] == 2.0f);
    CHECK(loaded_layer_norm.beta()[1] == -1.0f);
    loaded_layer_norm.setAffineParameters(Tensor::ones({2}), Tensor::zeros({2}));
    CHECK(loaded_layer_norm.gamma()[1] == 1.0f);
    CHECK(loaded_layer_norm.beta()[0] == 0.0f);

    LayerNorm stable_layer_norm(2);
    Tensor equal_max = stable_layer_norm.forward(Tensor({2}, {max_float, max_float}));
    CHECK(equal_max.allFinite());
    CHECK(std::fabs(equal_max[0]) < 1e-6f);
    CHECK(std::fabs(equal_max[1]) < 1e-6f);
    Tensor wide = stable_layer_norm.forward(Tensor({2}, {1e20f, -1e20f}));
    CHECK(wide.allFinite());
    CHECK(std::fabs(wide[0] - 1.0f) < 1e-5f);
    CHECK(std::fabs(wide[1] + 1.0f) < 1e-5f);
    CHECK_THROWS(std::invalid_argument,
                 stable_layer_norm.forward(Tensor({2}, {nan, 0.0f})));
    CHECK_THROWS(std::invalid_argument,
                 loaded_layer_norm.setAffineParameters(
                     Tensor({2}, {nan, 1.0f}), Tensor::zeros({2})));
    CHECK(loaded_layer_norm.gamma()[0] == 1.0f);
    CHECK_THROWS(std::overflow_error,
                 LayerNorm(Tensor({2}, {max_float, max_float}),
                           Tensor({2}, {max_float, max_float}))
                     .forward(Tensor({2}, {1.0f, 3.0f})));
}

struct GateState {
    GateState()
        : first_started(first_started_signal.get_future().share()),
          release(release_signal.get_future().share()) {}

    std::atomic<size_t> started{0};
    std::atomic<size_t> completed{0};
    std::promise<void> first_started_signal;
    std::shared_future<void> first_started;
    std::promise<void> release_signal;
    std::shared_future<void> release;
};

struct WorkerStartState {
    explicit WorkerStartState(size_t expected_workers)
        : expected(expected_workers), all_ready(all_ready_signal.get_future().share()) {}

    const size_t expected;
    std::atomic<size_t> ready{0};
    std::promise<void> all_ready_signal;
    std::shared_future<void> all_ready;

    void arrive() {
        if (ready.fetch_add(1, std::memory_order_acq_rel) + 1 == expected) {
            all_ready_signal.set_value();
        }
    }
};

class GateLayer final : public Layer {
public:
    explicit GateLayer(std::shared_ptr<GateState> state) : state_(std::move(state)) {}

    Tensor forward(const Tensor& input) const override {
        if (state_->started.fetch_add(1, std::memory_order_acq_rel) == 0) {
            state_->first_started_signal.set_value();
        }
        state_->release.wait();
        state_->completed.fetch_add(1, std::memory_order_release);
        return input;
    }

    std::string name() const override { return "GateLayer"; }

private:
    std::shared_ptr<GateState> state_;
};

bool waitForFirstStart(const std::shared_ptr<GateState>& state) {
    return state->first_started.wait_for(2s) == std::future_status::ready;
}

bool waitForReadyWorkers(const std::shared_ptr<WorkerStartState>& state) {
    return state->all_ready.wait_for(2s) == std::future_status::ready;
}

void testAsyncLifecycleAndQueueContracts() {
    const EngineStatus default_status;
    CHECK(default_status.lifecycle == EngineLifecycle::accepting);
    CHECK(default_status.accepting_requests);
    CHECK(default_status.queued_requests == 0);
    CHECK(default_status.active_requests == 0);
    CHECK(default_status.in_flight_calls == 0);

    Sequential passthrough;
    CHECK_THROWS(std::invalid_argument, ParallelInferenceEngine(&passthrough, 0));
    CHECK_THROWS(std::invalid_argument, InferenceEngine(nullptr));

    EngineConfig invalid_config;
    invalid_config.batch_size = 0;
    CHECK_THROWS(std::invalid_argument, InferenceEngine(&passthrough, invalid_config));
    invalid_config.batch_size = 1;
    invalid_config.max_queue_size = 0;
    CHECK_THROWS(std::invalid_argument, ParallelInferenceEngine(&passthrough, invalid_config));

    auto state = std::make_shared<GateState>();
    Sequential gated;
    gated.add(GateLayer(state));

    EngineConfig config;
    config.num_threads = 1;
    config.batch_size = 1;
    config.max_queue_size = 1;
    config.enable_profiling = false;
    ParallelInferenceEngine engine(&gated, config);

    auto active = engine.inferAsync(Tensor::ones({1}));
    CHECK(waitForFirstStart(state));
    EngineStatus status = engine.status();
    CHECK(status.lifecycle == EngineLifecycle::accepting);
    CHECK(status.accepting_requests);
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 0);
    CHECK(status.in_flight_calls == 0);

    auto queued = engine.inferAsync(Tensor::ones({1}));
    status = engine.status();
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 1);
    auto rejected = engine.inferAsync(Tensor::ones({1}));
    checkReady(rejected, "full-queue rejection");
    CHECK(!rejected.get().success);

    engine.close();
    status = engine.status();
    CHECK(status.lifecycle == EngineLifecycle::draining);
    CHECK(engine.closed());
    CHECK(!engine.stopped());

    auto after_stop = engine.inferAsync(Tensor::ones({1}));
    checkReady(after_stop, "post-stop rejection");
    CHECK(!after_stop.get().success);

    state->release_signal.set_value();
    checkReady(active, "active request");
    checkReady(queued, "queued request");
    CHECK(active.get().success);
    CHECK(queued.get().success);
    engine.stop();
    engine.join();
    status = engine.status();
    CHECK(status.lifecycle == EngineLifecycle::joined);
    CHECK(!status.accepting_requests);
    CHECK(status.active_requests == 0);
    CHECK(status.queued_requests == 0);
    CHECK(status.in_flight_calls == 0);
    CHECK(engine.stopped());
}

void testExternalThreadContextAndNonBlockingStop() {
    const InferenceExecutionContext external_context =
        InferenceExecutionContext::capture();
    CHECK(!external_context.carriesCallbackContext());

    auto state = std::make_shared<GateState>();
    Sequential gated;
    gated.add(GateLayer(state));
    ParallelInferenceEngine engine(&gated, EngineConfig{1, 1, false, 2});

    auto inference = std::async(std::launch::async, [&]() {
        return external_context.run([&]() {
            return engine.inferAsync(Tensor::ones({1})).get();
        });
    });
    CHECK(waitForFirstStart(state));

    std::promise<void> stop_returned_signal;
    std::future<void> stop_returned = stop_returned_signal.get_future();
    std::promise<bool> stopper_context_signal;
    std::future<bool> stopper_context_result = stopper_context_signal.get_future();
    std::thread stopper([&]() {
        const InferenceExecutionContext stopper_context =
            InferenceExecutionContext::capture();
        stopper_context_signal.set_value(
            stopper_context.carriesCallbackContext());
        engine.stop();
        stop_returned_signal.set_value();
    });

    checkReady(stop_returned, "external non-blocking stop");
    stop_returned.get();
    CHECK(!stopper_context_result.get());
    const EngineStatus draining = engine.status();
    CHECK(draining.lifecycle == EngineLifecycle::draining);
    CHECK(draining.active_requests == 1);
    CHECK(draining.queued_requests == 0);
    CHECK(draining.in_flight_calls == 0);
    CHECK(!engine.stopped());

    state->release_signal.set_value();
    checkReady(inference, "external threaded inference completion");
    CHECK(inference.get().success);
    stopper.join();
    engine.join();
    CHECK(engine.stopped());
}

void testSynchronousLifecycleContracts() {
    auto state = std::make_shared<GateState>();
    Sequential gated;
    gated.add(GateLayer(state));

    EngineConfig config;
    config.num_threads = 1;
    config.batch_size = 1;
    config.max_queue_size = 2;
    config.enable_profiling = false;
    ParallelInferenceEngine engine(&gated, config);

    auto synchronous = std::async(std::launch::async, [&]() {
        return engine.infer(Tensor::ones({1}));
    });
    CHECK(waitForFirstStart(state));

    EngineStatus status = engine.status();
    CHECK(status.lifecycle == EngineLifecycle::accepting);
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 0);
    CHECK(status.in_flight_calls == 1);

    std::promise<void> closed_signal;
    std::future<void> closed = closed_signal.get_future();
    auto stopping = std::async(std::launch::async, [&]() {
        engine.close();
        closed_signal.set_value();
        engine.join();
        return state->completed.load(std::memory_order_acquire);
    });
    checkReady(closed, "synchronous close transition");
    closed.get();

    status = engine.status();
    CHECK(status.lifecycle == EngineLifecycle::draining);
    CHECK(!status.accepting_requests);
    CHECK(status.active_requests == 1);
    CHECK(status.in_flight_calls == 1);
    CHECK(engine.closed());
    CHECK(!engine.stopped());

    InferenceEngine& polymorphic_engine = engine;
    CHECK_THROWS(std::logic_error,
                 polymorphic_engine.infer(Tensor::ones({1})));
    CHECK_THROWS(std::logic_error,
                 engine.InferenceEngine::infer(Tensor::ones({1})));
    CHECK_THROWS(std::logic_error,
                 engine.inferBatch(std::vector<Tensor>{Tensor::ones({1})}));
    CHECK_THROWS(std::logic_error,
                 engine.InferenceEngine::inferBatch(
                     std::vector<Tensor>{Tensor::ones({1})}));
    CHECK_THROWS(std::logic_error,
                 engine.InferenceEngine::setConfig(config));
    CHECK_THROWS(std::logic_error,
                 engine.inferBatchParallel(std::vector<Tensor>{}));
    auto rejected = engine.inferAsync(Tensor::ones({1}));
    checkReady(rejected, "closed async rejection during synchronous drain");
    CHECK(!rejected.get().success);

    state->release_signal.set_value();
    checkReady(synchronous, "blocked synchronous inference");
    CHECK(synchronous.get().shape() == std::vector<size_t>({1}));
    checkReady(stopping, "stop waiting for synchronous inference");
    CHECK(stopping.get() == 1);

    status = engine.status();
    CHECK(status.lifecycle == EngineLifecycle::joined);
    CHECK(status.active_requests == 0);
    CHECK(status.queued_requests == 0);
    CHECK(status.in_flight_calls == 0);
    CHECK(engine.stopped());
}

void testBatchSizeControlsSubmissionWindow() {
    auto state = std::make_shared<GateState>();
    EngineConfig config;
    config.num_threads = 2;
    config.batch_size = 1;
    config.max_queue_size = 2;
    config.enable_profiling = false;
    auto worker_start = std::make_shared<WorkerStartState>(config.num_threads);
    Sequential gated;
    gated.add(GateLayer(state));

    ParallelInferenceEngine::WorkerLauncher launcher =
        [worker_start](ParallelInferenceEngine::WorkerTask task) {
            return std::thread([worker_start, task = std::move(task)]() mutable {
                worker_start->arrive();
                task();
            });
        };
    ParallelInferenceEngine engine(&gated, config, std::move(launcher));
    CHECK(waitForReadyWorkers(worker_start));

    std::vector<Tensor> inputs(3, Tensor::ones({1}));
    auto batch = std::async(std::launch::async, [&engine, &inputs]() {
        return engine.inferBatchParallel(inputs);
    });

    CHECK(waitForFirstStart(state));
    const EngineStatus blocked_window = engine.status();
    CHECK(blocked_window.active_requests == 1);
    CHECK(blocked_window.queued_requests == 0);
    CHECK(blocked_window.in_flight_calls == 1);

    engine.close();
    state->release_signal.set_value();
    checkReady(batch, "batch-window result");
    const auto results = batch.get();
    CHECK(results.size() == inputs.size());
    CHECK(results[0].success);
    CHECK(!results[1].success);
    CHECK(!results[1].error_message.empty());
    CHECK(!results[2].success);
    CHECK(!results[2].error_message.empty());
    engine.join();
}

void testExternalStopAndBatchCallComposeSafely() {
    auto state = std::make_shared<GateState>();
    Sequential gated;
    gated.add(GateLayer(state));

    EngineConfig config;
    config.num_threads = 1;
    config.batch_size = 1;
    config.max_queue_size = 2;
    config.enable_profiling = false;
    auto engine = std::make_unique<ParallelInferenceEngine>(&gated, config);
    ParallelInferenceEngine* const engine_view = engine.get();

    auto batch = std::async(std::launch::async, [engine_view]() {
        return engine_view->inferBatchParallel({Tensor::ones({1})});
    });
    CHECK(waitForFirstStart(state));
    auto queued = engine->inferAsync(Tensor::ones({1}));

    EngineStatus status = engine->status();
    CHECK(status.lifecycle == EngineLifecycle::accepting);
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 1);
    CHECK(status.in_flight_calls == 1);

    std::promise<void> closed_signal;
    std::future<void> closed = closed_signal.get_future();
    auto stopping = std::async(std::launch::async, [&]() {
        engine->close();
        closed_signal.set_value();
        engine->join();
        return engine->status();
    });
    checkReady(closed, "batch close transition");
    closed.get();

    status = engine->status();
    CHECK(status.lifecycle == EngineLifecycle::draining);
    CHECK(status.active_requests == 1);
    CHECK(status.queued_requests == 1);
    CHECK(status.in_flight_calls == 1);

    state->release_signal.set_value();
    checkReady(stopping, "external stop with active batch");
    const EngineStatus joined = stopping.get();
    CHECK(joined.lifecycle == EngineLifecycle::joined);
    CHECK(joined.active_requests == 0);
    CHECK(joined.queued_requests == 0);
    CHECK(joined.in_flight_calls == 0);

    checkReady(batch, "parallel batch caller completion");
    const auto batch_results = batch.get();
    CHECK(batch_results.size() == 1);
    CHECK(batch_results[0].success);
    checkReady(queued, "queued request drained by external stop");
    CHECK(queued.get().success);
    CHECK(state->completed.load(std::memory_order_acquire) == 2);

    engine.reset();
}

void testBatchAndFailureResults() {
    Sequential model;
    model.add(Linear(2, 1));

    EngineConfig config;
    config.num_threads = 2;
    config.batch_size = 2;
    config.max_queue_size = 2;
    config.enable_profiling = false;

    InferenceEngine serial(&model, config);
    CHECK(serial.inferBatch({}).empty());

    ParallelInferenceEngine parallel(&model, config);
    std::vector<Tensor> inputs(5, Tensor::ones({2}));
    const auto results = parallel.inferBatchParallel(inputs);
    CHECK(results.size() == inputs.size());
    for (const auto& result : results) {
        CHECK(result.success);
    }

    auto invalid = parallel.inferAsync(Tensor::ones({1}));
    checkReady(invalid, "invalid-input result");
    CHECK(!invalid.get().success);
    parallel.stop();
    parallel.join();

    Sequential softmax_model;
    softmax_model.add(Softmax());
    ParallelInferenceEngine numerical_engine(&softmax_model, config);
    auto non_finite = numerical_engine.inferAsync(Tensor(
        {2}, {std::numeric_limits<float>::infinity(), 0.0f}));
    checkReady(non_finite, "non-finite softmax input result");
    const InferenceResult numerical_result = non_finite.get();
    CHECK(!numerical_result.success);
    CHECK(!numerical_result.error_message.empty());
    numerical_engine.stop();
    numerical_engine.join();
}

void testStatsContracts() {
    InferenceStats stats;
    stats.update(InferenceResult::error("expected"), 0.0);
    CHECK(stats.total_requests == 1);
    CHECK(stats.successful_requests == 0);
    CHECK(stats.failed_requests == 1);
    CHECK(stats.avg_latency_ms == 0.0);
    CHECK(stats.min_latency_ms == 0.0);
    CHECK(stats.throughput == 0.0);

    stats.update(InferenceResult(Tensor::ones({1}), 2500us), 2.0);
    CHECK(stats.successful_requests == 1);
    CHECK(std::fabs(stats.avg_latency_ms - 2.5) < 1e-9);
    CHECK(std::fabs(stats.throughput - 1.0) < 1e-9);
}

}  // namespace

int main() {
    testTensorContracts();
    testLayerContracts();
    testAsyncLifecycleAndQueueContracts();
    testExternalThreadContextAndNonBlockingStop();
    testSynchronousLifecycleContracts();
    testBatchSizeControlsSubmissionWindow();
    testExternalStopAndBatchCallComposeSafely();
    testBatchAndFailureResults();
    testStatsContracts();

    if (failures != 0) {
        std::cerr << failures << " contract check(s) failed\n";
        return 1;
    }
    std::cout << "All model inference contract tests passed\n";
    return 0;
}
