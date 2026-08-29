/**
 * @file layers.h
 * @brief 教学用神经网络层及其输入契约
 */

#ifndef MODEL_INFERENCE_LAYERS_H
#define MODEL_INFERENCE_LAYERS_H

#include "tensor.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace inference {

class Layer {
public:
    virtual ~Layer() = default;

    // 统一契约：公开层只接受非空、有限输入；成功返回时输出也必须有限。
    virtual Tensor forward(const Tensor& input) const = 0;
    virtual std::string name() const = 0;
    virtual size_t numParameters() const { return 0; }
};

class Linear : public Layer {
public:
    Linear(size_t in_features, size_t out_features, bool use_bias = true)
        : in_features_(in_features), out_features_(out_features), use_bias_(use_bias) {
        if (in_features_ == 0 || out_features_ == 0) {
            throw std::invalid_argument("Linear feature counts must be greater than zero");
        }
        if (in_features_ > std::numeric_limits<size_t>::max() - out_features_) {
            throw std::overflow_error("Linear feature count sum overflows size_t");
        }
        const float scale = std::sqrt(
            2.0f / static_cast<float>(in_features_ + out_features_));
        weight_ = Tensor::randn({out_features_, in_features_}, 0.0f, scale);
        if (use_bias_) {
            bias_ = Tensor::zeros({out_features_});
        }
    }

    Linear(const Tensor& weight, const Tensor& bias)
        : weight_(validatedWeight(weight)),
          bias_(validatedBias(bias, weight_.shape()[0])),
          in_features_(weight_.shape()[1]),
          out_features_(weight_.shape()[0]),
          use_bias_(true) {}

    explicit Linear(const Tensor& weight)
        : weight_(validatedWeight(weight)),
          in_features_(weight_.shape()[1]),
          out_features_(weight_.shape()[0]),
          use_bias_(false) {}

    Tensor forward(const Tensor& input) const override {
        validateInput(input);

        if (input.ndim() == 1) {
            Tensor input_2d({1, in_features_});
            std::copy(input.data(), input.data() + input.size(), input_2d.data());
            Tensor output = input_2d.matmul(weight_.transpose());
            if (use_bias_) {
                for (size_t i = 0; i < out_features_; ++i) {
                    output.at(0, i) += bias_[i];
                }
            }
            requireFiniteOutput(output);

            Tensor result({out_features_});
            std::copy(output.data(), output.data() + out_features_, result.data());
            return result;
        }

        Tensor output = input.matmul(weight_.transpose());
        if (use_bias_) {
            for (size_t batch = 0; batch < input.shape()[0]; ++batch) {
                for (size_t feature = 0; feature < out_features_; ++feature) {
                    output.at(batch, feature) += bias_[feature];
                }
            }
        }
        requireFiniteOutput(output);
        return output;
    }

    std::string name() const override { return "Linear"; }

    size_t numParameters() const override {
        return weight_.size() + (use_bias_ ? bias_.size() : 0);
    }

    const Tensor& weight() const noexcept { return weight_; }
    const Tensor& bias() const noexcept { return bias_; }
    size_t inFeatures() const noexcept { return in_features_; }
    size_t outFeatures() const noexcept { return out_features_; }

    void setWeight(const Tensor& weight) {
        if (weight.shape() != weight_.shape()) {
            throw std::invalid_argument("Linear weight shape mismatch");
        }
        requireFinite(weight, "Linear weight");
        weight_ = weight;
    }

    void setBias(const Tensor& bias) {
        if (!use_bias_) {
            throw std::logic_error("Cannot set bias on a bias-free Linear layer");
        }
        if (bias.shape() != bias_.shape()) {
            throw std::invalid_argument("Linear bias shape mismatch");
        }
        requireFinite(bias, "Linear bias");
        bias_ = bias;
    }

private:
    Tensor weight_;
    Tensor bias_;
    size_t in_features_;
    size_t out_features_;
    bool use_bias_;

    static Tensor validatedWeight(const Tensor& weight) {
        if (weight.ndim() != 2 || weight.empty()) {
            throw std::invalid_argument("Linear weight must be a non-empty 2D tensor");
        }
        requireFinite(weight, "Linear weight");
        return weight;
    }

    static Tensor validatedBias(const Tensor& bias, size_t out_features) {
        if (bias.ndim() != 1 || bias.shape()[0] != out_features) {
            throw std::invalid_argument("Linear bias must be a 1D tensor matching out_features");
        }
        requireFinite(bias, "Linear bias");
        return bias;
    }

    static void requireFinite(const Tensor& tensor, const char* parameter_name) {
        if (!tensor.allFinite()) {
            throw std::invalid_argument(std::string(parameter_name) +
                                        " must contain only finite values");
        }
    }

    static void requireFiniteOutput(const Tensor& output) {
        if (!output.allFinite()) {
            throw std::overflow_error("Linear produced a non-finite output");
        }
    }

    void validateInput(const Tensor& input) const {
        if (input.ndim() == 1) {
            if (input.shape()[0] != in_features_) {
                throw std::invalid_argument("Linear 1D input feature count mismatch");
            }
        } else if (input.ndim() == 2) {
            if (input.shape()[1] != in_features_) {
                throw std::invalid_argument("Linear 2D input feature count mismatch");
            }
        } else {
            throw std::invalid_argument("Linear expects a non-empty 1D or 2D input");
        }
        if (!input.allFinite()) {
            throw std::invalid_argument("Linear requires finite input values");
        }
    }
};

class ReLU : public Layer {
public:
    Tensor forward(const Tensor& input) const override {
        requireFiniteInput(input, "ReLU");
        return input.relu();
    }
    std::string name() const override { return "ReLU"; }

private:
    static void requireFiniteInput(const Tensor& input, const char* layer_name) {
        if (input.empty() || !input.allFinite()) {
            throw std::invalid_argument(std::string(layer_name) +
                                        " requires non-empty finite input");
        }
    }
};

class Sigmoid : public Layer {
public:
    Tensor forward(const Tensor& input) const override {
        if (input.empty() || !input.allFinite()) {
            throw std::invalid_argument("Sigmoid requires non-empty finite input");
        }
        return input.sigmoid();
    }
    std::string name() const override { return "Sigmoid"; }
};

class Softmax : public Layer {
public:
    Tensor forward(const Tensor& input) const override { return input.softmax(); }
    std::string name() const override { return "Softmax"; }
};

class Dropout : public Layer {
public:
    explicit Dropout(float probability = 0.5f)
        : probability_(probability) {
        if (!std::isfinite(probability_) ||
            probability_ < 0.0f || probability_ >= 1.0f) {
            throw std::invalid_argument("Dropout probability must be in [0, 1)");
        }
    }

    // 本项目只实现推理路径，所以 Dropout 始终是恒等映射。
    Tensor forward(const Tensor& input) const override {
        if (input.empty() || !input.allFinite()) {
            throw std::invalid_argument("Dropout requires non-empty finite input");
        }
        return input;
    }

    std::string name() const override { return "Dropout"; }
    float probability() const noexcept { return probability_; }
    void setTraining(bool training) {
        if (training) {
            throw std::logic_error(
                "Dropout is inference-only and cannot enter training mode");
        }
    }
    bool training() const noexcept { return false; }

private:
    float probability_;
};

class BatchNorm : public Layer {
public:
    explicit BatchNorm(size_t num_features, float eps = 1e-5f)
        : num_features_(num_features), eps_(validatedEps(eps)) {
        if (num_features_ == 0) {
            throw std::invalid_argument("BatchNorm num_features must be greater than zero");
        }
        gamma_ = Tensor::ones({num_features_});
        beta_ = Tensor::zeros({num_features_});
        running_mean_ = Tensor::zeros({num_features_});
        running_var_ = Tensor::ones({num_features_});
    }

    BatchNorm(const Tensor& gamma,
              const Tensor& beta,
              const Tensor& running_mean,
              const Tensor& running_var,
              float eps = 1e-5f)
        : num_features_(validatedParameterCount(
              gamma, beta, running_mean, running_var)),
          eps_(validatedEps(eps)),
          gamma_(gamma),
          beta_(beta),
          running_mean_(running_mean),
          running_var_(running_var) {}

    Tensor forward(const Tensor& input) const override {
        validateFeatureInput(input, num_features_, "BatchNorm");
        Tensor output(input.shape());

        if (input.ndim() == 1) {
            normalizeRow(input.data(), output.data());
            return output;
        }

        for (size_t batch = 0; batch < input.shape()[0]; ++batch) {
            normalizeRow(input.data() + batch * num_features_,
                         output.data() + batch * num_features_);
        }
        return output;
    }

    std::string name() const override { return "BatchNorm"; }
    size_t numParameters() const override { return gamma_.size() + beta_.size(); }

    void setInferenceParameters(const Tensor& gamma,
                                const Tensor& beta,
                                const Tensor& running_mean,
                                const Tensor& running_var) {
        if (validatedParameterCount(gamma, beta, running_mean, running_var) !=
            num_features_) {
            throw std::invalid_argument("BatchNorm parameter feature count mismatch");
        }

        Tensor gamma_copy(gamma);
        Tensor beta_copy(beta);
        Tensor mean_copy(running_mean);
        Tensor variance_copy(running_var);
        gamma_ = std::move(gamma_copy);
        beta_ = std::move(beta_copy);
        running_mean_ = std::move(mean_copy);
        running_var_ = std::move(variance_copy);
    }

    const Tensor& gamma() const noexcept { return gamma_; }
    const Tensor& beta() const noexcept { return beta_; }
    const Tensor& runningMean() const noexcept { return running_mean_; }
    const Tensor& runningVariance() const noexcept { return running_var_; }
    float eps() const noexcept { return eps_; }

private:
    size_t num_features_;
    float eps_;
    Tensor gamma_;
    Tensor beta_;
    Tensor running_mean_;
    Tensor running_var_;

    static void validateFeatureInput(const Tensor& input,
                                     size_t features,
                                     const char* layer_name) {
        const bool shape_matches =
            (input.ndim() == 1 && input.shape()[0] == features) ||
            (input.ndim() == 2 && input.shape()[1] == features);
        if (!shape_matches) {
            throw std::invalid_argument(std::string(layer_name) +
                                        " expects 1D/2D input with matching last dimension");
        }
        if (!input.allFinite()) {
            throw std::invalid_argument(std::string(layer_name) +
                                        " requires finite input values");
        }
    }

    void normalizeRow(const float* input, float* output) const {
        for (size_t feature = 0; feature < num_features_; ++feature) {
            const double normalized =
                (static_cast<double>(input[feature]) - running_mean_[feature]) /
                std::sqrt(static_cast<double>(running_var_[feature]) + eps_);
            const double value =
                static_cast<double>(gamma_[feature]) * normalized + beta_[feature];
            output[feature] = checkedOutput(value, "BatchNorm");
        }
    }

    static float validatedEps(float eps) {
        if (!std::isfinite(eps) || !(eps > 0.0f)) {
            throw std::invalid_argument("BatchNorm eps must be finite and greater than zero");
        }
        return eps;
    }

    static size_t validatedParameterCount(const Tensor& gamma,
                                          const Tensor& beta,
                                          const Tensor& running_mean,
                                          const Tensor& running_var) {
        if (gamma.ndim() != 1 || gamma.empty()) {
            throw std::invalid_argument("BatchNorm gamma must be a non-empty 1D tensor");
        }
        const auto expected_shape = gamma.shape();
        if (beta.shape() != expected_shape || running_mean.shape() != expected_shape ||
            running_var.shape() != expected_shape) {
            throw std::invalid_argument(
                "BatchNorm beta and running statistics must match gamma shape");
        }
        if (!gamma.allFinite() || !beta.allFinite() || !running_mean.allFinite() ||
            !running_var.allFinite()) {
            throw std::invalid_argument(
                "BatchNorm parameters and running statistics must be finite");
        }
        for (size_t i = 0; i < running_var.size(); ++i) {
            if (running_var[i] < 0.0f) {
                throw std::invalid_argument(
                    "BatchNorm running variance must be non-negative");
            }
        }
        return gamma.shape()[0];
    }

    static float checkedOutput(double value, const char* layer_name) {
        const double limit = static_cast<double>(std::numeric_limits<float>::max());
        if (!std::isfinite(value) || value < -limit || value > limit) {
            throw std::overflow_error(std::string(layer_name) +
                                      " produced a non-finite or out-of-range output");
        }
        return static_cast<float>(value);
    }
};

class LayerNorm : public Layer {
public:
    explicit LayerNorm(size_t normalized_shape, float eps = 1e-5f)
        : normalized_shape_(normalized_shape), eps_(validatedEps(eps)) {
        if (normalized_shape_ == 0) {
            throw std::invalid_argument("LayerNorm normalized_shape must be greater than zero");
        }
        gamma_ = Tensor::ones({normalized_shape_});
        beta_ = Tensor::zeros({normalized_shape_});
    }

    LayerNorm(const Tensor& gamma, const Tensor& beta, float eps = 1e-5f)
        : normalized_shape_(validatedParameterCount(gamma, beta)),
          eps_(validatedEps(eps)),
          gamma_(gamma),
          beta_(beta) {}

    Tensor forward(const Tensor& input) const override {
        validateInput(input);
        Tensor output(input.shape());
        const size_t rows = input.ndim() == 1 ? 1 : input.shape()[0];
        for (size_t row = 0; row < rows; ++row) {
            normalizeRow(input.data() + row * normalized_shape_,
                         output.data() + row * normalized_shape_);
        }
        return output;
    }

    std::string name() const override { return "LayerNorm"; }
    size_t numParameters() const override { return gamma_.size() + beta_.size(); }

    void setAffineParameters(const Tensor& gamma, const Tensor& beta) {
        if (validatedParameterCount(gamma, beta) != normalized_shape_) {
            throw std::invalid_argument("LayerNorm parameter feature count mismatch");
        }
        Tensor gamma_copy(gamma);
        Tensor beta_copy(beta);
        gamma_ = std::move(gamma_copy);
        beta_ = std::move(beta_copy);
    }

    const Tensor& gamma() const noexcept { return gamma_; }
    const Tensor& beta() const noexcept { return beta_; }
    float eps() const noexcept { return eps_; }

private:
    size_t normalized_shape_;
    float eps_;
    Tensor gamma_;
    Tensor beta_;

    void validateInput(const Tensor& input) const {
        const bool shape_matches =
            (input.ndim() == 1 && input.shape()[0] == normalized_shape_) ||
            (input.ndim() == 2 && input.shape()[1] == normalized_shape_);
        if (!shape_matches) {
            throw std::invalid_argument(
                "LayerNorm expects 1D/2D input whose last dimension matches normalized_shape");
        }
        if (!input.allFinite()) {
            throw std::invalid_argument("LayerNorm requires finite input values");
        }
    }

    void normalizeRow(const float* input, float* output) const {
        // Welford + double 中间量避免 float 求和、平方溢出和灾难性抵消。
        double mean = 0.0;
        double squared_deviations = 0.0;
        for (size_t i = 0; i < normalized_shape_; ++i) {
            const double value = input[i];
            const double delta = value - mean;
            mean += delta / static_cast<double>(i + 1);
            const double adjusted_delta = value - mean;
            squared_deviations += delta * adjusted_delta;
        }
        const double variance = std::max(
            0.0, squared_deviations / static_cast<double>(normalized_shape_));
        const double inverse_stddev = 1.0 / std::sqrt(variance + eps_);

        for (size_t i = 0; i < normalized_shape_; ++i) {
            const double value = static_cast<double>(gamma_[i]) *
                                     (static_cast<double>(input[i]) - mean) * inverse_stddev +
                                 beta_[i];
            output[i] = checkedOutput(value);
        }
    }

    static float validatedEps(float eps) {
        if (!std::isfinite(eps) || !(eps > 0.0f)) {
            throw std::invalid_argument("LayerNorm eps must be finite and greater than zero");
        }
        return eps;
    }

    static size_t validatedParameterCount(const Tensor& gamma, const Tensor& beta) {
        if (gamma.ndim() != 1 || gamma.empty() || beta.shape() != gamma.shape()) {
            throw std::invalid_argument(
                "LayerNorm gamma and beta must be matching non-empty 1D tensors");
        }
        if (!gamma.allFinite() || !beta.allFinite()) {
            throw std::invalid_argument("LayerNorm gamma and beta must be finite");
        }
        return gamma.shape()[0];
    }

    static float checkedOutput(double value) {
        const double limit = static_cast<double>(std::numeric_limits<float>::max());
        if (!std::isfinite(value) || value < -limit || value > limit) {
            throw std::overflow_error(
                "LayerNorm produced a non-finite or out-of-range output");
        }
        return static_cast<float>(value);
    }
};

class Sequential : public Layer {
public:
    Sequential() = default;

    template <typename... Layers>
    explicit Sequential(Layers&&... layers) {
        (add(std::forward<Layers>(layers)), ...);
    }

    template <typename LayerType>
    void add(LayerType&& layer) {
        using StoredLayer = std::decay_t<LayerType>;
        static_assert(std::is_base_of<Layer, StoredLayer>::value,
                      "Sequential::add requires a Layer-derived type");
        layers_.push_back(
            std::make_unique<StoredLayer>(std::forward<LayerType>(layer)));
    }

    Tensor forward(const Tensor& input) const override {
        if (input.empty()) {
            throw std::invalid_argument("Sequential expects a non-empty input");
        }
        if (!input.allFinite()) {
            throw std::invalid_argument("Sequential requires finite input values");
        }
        Tensor output = input;
        for (const auto& layer : layers_) {
            output = layer->forward(output);
            if (!output.allFinite()) {
                throw std::domain_error("Sequential layer " + layer->name() +
                                        " produced a non-finite output");
            }
        }
        return output;
    }

    std::string name() const override { return "Sequential"; }

    size_t numParameters() const override {
        size_t total = 0;
        for (const auto& layer : layers_) {
            total += layer->numParameters();
        }
        return total;
    }

    size_t numLayers() const noexcept { return layers_.size(); }

    void printStructure() const {
        std::cout << "Sequential Model:\n================\n";
        for (size_t i = 0; i < layers_.size(); ++i) {
            std::cout << "  [" << i << "] " << layers_[i]->name();
            if (layers_[i]->numParameters() > 0) {
                std::cout << " (params: " << layers_[i]->numParameters() << ")";
            }
            std::cout << '\n';
        }
        std::cout << "Total parameters: " << numParameters() << '\n';
    }

private:
    std::vector<std::unique_ptr<Layer>> layers_;
};

}  // namespace inference

#endif  // MODEL_INFERENCE_LAYERS_H
