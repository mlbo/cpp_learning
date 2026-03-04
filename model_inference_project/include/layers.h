/**
 * @file layers.h
 * @brief 神经网络层定义
 * 
 * 包含：Linear、ReLU、Softmax、Dropout等常用层
 */

#ifndef MODEL_INFERENCE_LAYERS_H
#define MODEL_INFERENCE_LAYERS_H

#include "tensor.h"
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

namespace inference {

/**
 * @brief 层基类
 */
class Layer {
public:
    virtual ~Layer() = default;
    
    /**
     * @brief 前向传播
     * @param input 输入张量
     * @return 输出张量
     */
    virtual Tensor forward(const Tensor& input) = 0;
    
    /**
     * @brief 获取层名称
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief 获取参数数量
     */
    virtual size_t numParameters() const { return 0; }
};

/**
 * @brief 全连接层（线性层）
 * 
 * 执行操作：output = input @ weight^T + bias
 */
class Linear : public Layer {
public:
    /**
     * @brief 构造函数
     * @param in_features 输入特征数
     * @param out_features 输出特征数
     * @param use_bias 是否使用偏置
     */
    Linear(size_t in_features, size_t out_features, bool use_bias = true)
        : in_features_(in_features), 
          out_features_(out_features),
          use_bias_(use_bias) {
        // 初始化权重（Xavier初始化）
        float scale = std::sqrt(2.0f / (in_features + out_features));
        weight_ = Tensor::randn({out_features, in_features}, 0.0f, scale);
        
        if (use_bias_) {
            bias_ = Tensor::zeros({out_features});
        }
    }
    
    /**
     * @brief 从权重数据构造
     */
    Linear(const Tensor& weight, const Tensor& bias)
        : weight_(weight), bias_(bias),
          in_features_(weight.shape().back()),
          out_features_(weight.shape()[0]),
          use_bias_(true) {}
    
    /**
     * @brief 从权重数据构造（无偏置）
     */
    explicit Linear(const Tensor& weight)
        : weight_(weight),
          in_features_(weight.shape().back()),
          out_features_(weight.shape()[0]),
          use_bias_(false) {}
    
    Tensor forward(const Tensor& input) override {
        // input: [batch_size, in_features]
        // weight: [out_features, in_features]
        // output: [batch_size, out_features]
        
        Tensor output;
        
        if (input.ndim() == 1) {
            // 单样本：[in_features]
            // 需要扩展为 [1, in_features]
            Tensor input_2d({1, in_features_});
            std::copy(input.data(), input.data() + in_features_, input_2d.data());
            
            output = input_2d.matmul(weight_.transpose());
            
            if (use_bias_) {
                for (size_t i = 0; i < out_features_; ++i) {
                    output.data()[i] += bias_[i];
                }
            }
            
            // 返回1D结果
            Tensor result({out_features_});
            std::copy(output.data(), output.data() + out_features_, result.data());
            return result;
        } else if (input.ndim() == 2) {
            // 批量样本：[batch_size, in_features]
            output = input.matmul(weight_.transpose());
            
            if (use_bias_) {
                size_t batch_size = input.shape()[0];
                for (size_t b = 0; b < batch_size; ++b) {
                    for (size_t i = 0; i < out_features_; ++i) {
                        output.at(b, i) += bias_[i];
                    }
                }
            }
        } else {
            throw std::invalid_argument("Linear layer expects 1D or 2D input");
        }
        
        return output;
    }
    
    std::string name() const override { return "Linear"; }
    
    size_t numParameters() const override {
        size_t params = weight_.size();
        if (use_bias_) params += bias_.size();
        return params;
    }
    
    // 访问器
    const Tensor& weight() const { return weight_; }
    const Tensor& bias() const { return bias_; }
    size_t inFeatures() const { return in_features_; }
    size_t outFeatures() const { return out_features_; }
    
    /**
     * @brief 设置权重
     */
    void setWeight(const Tensor& weight) {
        if (weight.shape() != weight_.shape()) {
            throw std::invalid_argument("Weight shape mismatch");
        }
        weight_ = weight;
    }
    
    void setBias(const Tensor& bias) {
        if (bias.shape() != bias_.shape()) {
            throw std::invalid_argument("Bias shape mismatch");
        }
        bias_ = bias;
    }

private:
    Tensor weight_;
    Tensor bias_;
    size_t in_features_;
    size_t out_features_;
    bool use_bias_;
};

/**
 * @brief ReLU激活层
 */
class ReLU : public Layer {
public:
    Tensor forward(const Tensor& input) override {
        return input.relu();
    }
    
    std::string name() const override { return "ReLU"; }
};

/**
 * @brief Sigmoid激活层
 */
class Sigmoid : public Layer {
public:
    Tensor forward(const Tensor& input) override {
        return input.sigmoid();
    }
    
    std::string name() const override { return "Sigmoid"; }
};

/**
 * @brief Softmax层
 */
class Softmax : public Layer {
public:
    Tensor forward(const Tensor& input) override {
        return input.softmax();
    }
    
    std::string name() const override { return "Softmax"; }
};

/**
 * @brief Dropout层（仅训练时生效）
 */
class Dropout : public Layer {
public:
    explicit Dropout(float prob = 0.5f) : prob_(prob), training_(true) {}
    
    Tensor forward(const Tensor& input) override {
        if (!training_ || prob_ == 0.0f) {
            return input;
        }
        
        // 推理阶段直接返回输入
        // 训练阶段需要随机mask（这里简化处理）
        return input;
    }
    
    std::string name() const override { return "Dropout"; }
    
    void setTraining(bool training) { training_ = training; }

private:
    float prob_;
    bool training_;
};

/**
 * @brief Batch Normalization层（简化版）
 */
class BatchNorm : public Layer {
public:
    BatchNorm(size_t num_features, float eps = 1e-5f, float momentum = 0.1f)
        : num_features_(num_features), eps_(eps), momentum_(momentum) {
        gamma_ = Tensor::ones({num_features});
        beta_ = Tensor::zeros({num_features});
        running_mean_ = Tensor::zeros({num_features});
        running_var_ = Tensor::ones({num_features});
    }
    
    Tensor forward(const Tensor& input) override {
        // 简化版：使用running statistics进行推理
        Tensor output(input.shape());
        
        if (input.ndim() == 1) {
            for (size_t i = 0; i < num_features_; ++i) {
                float normalized = (input[i] - running_mean_[i]) / 
                                   std::sqrt(running_var_[i] + eps_);
                output[i] = gamma_[i] * normalized + beta_[i];
            }
        } else if (input.ndim() == 2) {
            size_t batch_size = input.shape()[0];
            for (size_t b = 0; b < batch_size; ++b) {
                for (size_t i = 0; i < num_features_; ++i) {
                    float normalized = (input.at(b, i) - running_mean_[i]) / 
                                       std::sqrt(running_var_[i] + eps_);
                    output.at(b, i) = gamma_[i] * normalized + beta_[i];
                }
            }
        }
        
        return output;
    }
    
    std::string name() const override { return "BatchNorm"; }
    
    size_t numParameters() const override {
        return gamma_.size() + beta_.size();
    }

private:
    size_t num_features_;
    float eps_;
    float momentum_;
    Tensor gamma_;
    Tensor beta_;
    Tensor running_mean_;
    Tensor running_var_;
};

/**
 * @brief 层归一化
 */
class LayerNorm : public Layer {
public:
    explicit LayerNorm(size_t normalized_shape, float eps = 1e-5f)
        : normalized_shape_(normalized_shape), eps_(eps) {
        gamma_ = Tensor::ones({normalized_shape});
        beta_ = Tensor::zeros({normalized_shape});
    }
    
    Tensor forward(const Tensor& input) override {
        Tensor output(input.shape());
        
        float mean = input.mean();
        float var = 0.0f;
        for (size_t i = 0; i < input.size(); ++i) {
            float diff = input[i] - mean;
            var += diff * diff;
        }
        var /= input.size();
        
        float std_inv = 1.0f / std::sqrt(var + eps_);
        
        for (size_t i = 0; i < input.size(); ++i) {
            float normalized = (input[i] - mean) * std_inv;
            output[i] = gamma_[i % normalized_shape_] * normalized + 
                        beta_[i % normalized_shape_];
        }
        
        return output;
    }
    
    std::string name() const override { return "LayerNorm"; }

private:
    size_t normalized_shape_;
    float eps_;
    Tensor gamma_;
    Tensor beta_;
};

/**
 * @brief Sequential容器
 */
class Sequential : public Layer {
public:
    Sequential() = default;
    
    /**
     * @brief 从层列表构造
     */
    template<typename... Layers>
    explicit Sequential(Layers&&... layers) {
        (add(std::forward<Layers>(layers)), ...);
    }
    
    /**
     * @brief 添加层
     */
    template<typename LayerType>
    void add(LayerType&& layer) {
        layers_.push_back(std::make_unique<std::decay_t<LayerType>>(
            std::forward<LayerType>(layer)));
    }
    
    Tensor forward(const Tensor& input) override {
        Tensor output = input;
        for (auto& layer : layers_) {
            output = layer->forward(output);
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
    
    /**
     * @brief 获取层数
     */
    size_t numLayers() const { return layers_.size(); }
    
    /**
     * @brief 打印模型结构
     */
    void printStructure() const {
        std::cout << "Sequential Model:\n";
        std::cout << "================\n";
        for (size_t i = 0; i < layers_.size(); ++i) {
            std::cout << "  [" << i << "] " << layers_[i]->name();
            if (layers_[i]->numParameters() > 0) {
                std::cout << " (params: " << layers_[i]->numParameters() << ")";
            }
            std::cout << "\n";
        }
        std::cout << "Total parameters: " << numParameters() << "\n";
    }

private:
    std::vector<std::unique_ptr<Layer>> layers_;
};

} // namespace inference

#endif // MODEL_INFERENCE_LAYERS_H
