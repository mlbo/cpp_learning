/**
 * @file tensor.h
 * @brief 张量类定义 - 支持多维数组操作
 * 
 * 这是一个简化版的张量库，用于演示模型推理
 * 支持：创建、拷贝、移动、基本运算、多维索引
 */

#ifndef MODEL_INFERENCE_TENSOR_H
#define MODEL_INFERENCE_TENSOR_H

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>

namespace inference {

/**
 * @brief 多维张量类
 * 
 * 支持动态形状的张量，提供基本的数学运算
 */
class Tensor {
public:
    // ==================== 构造函数 ====================
    
    /**
     * @brief 默认构造函数
     */
    Tensor() : data_(nullptr), size_(0) {}
    
    /**
     * @brief 从形状构造张量
     * @param shape 各维度大小
     */
    explicit Tensor(const std::vector<size_t>& shape)
        : shape_(shape), size_(computeSize(shape)) {
        if (size_ > 0) {
            data_ = std::make_unique<float[]>(size_);
            std::fill(data_.get(), data_.get() + size_, 0.0f);
        }
    }
    
    /**
     * @brief 从形状和数据构造张量
     * @param shape 各维度大小
     * @param data 初始数据
     */
    Tensor(const std::vector<size_t>& shape, const std::vector<float>& data)
        : shape_(shape), size_(computeSize(shape)) {
        if (size_ != data.size()) {
            throw std::invalid_argument("Data size does not match shape");
        }
        data_ = std::make_unique<float[]>(size_);
        std::copy(data.begin(), data.end(), data_.get());
    }
    
    /**
     * @brief 拷贝构造函数
     */
    Tensor(const Tensor& other)
        : shape_(other.shape_), size_(other.size_) {
        if (size_ > 0) {
            data_ = std::make_unique<float[]>(size_);
            std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
        }
    }
    
    /**
     * @brief 移动构造函数
     */
    Tensor(Tensor&& other) noexcept
        : data_(std::move(other.data_)),
          shape_(std::move(other.shape_)),
          size_(other.size_) {
        other.size_ = 0;
    }
    
    /**
     * @brief 拷贝赋值运算符
     */
    Tensor& operator=(const Tensor& other) {
        if (this != &other) {
            shape_ = other.shape_;
            size_ = other.size_;
            if (size_ > 0) {
                data_ = std::make_unique<float[]>(size_);
                std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
            } else {
                data_.reset();
            }
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值运算符
     */
    Tensor& operator=(Tensor&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            shape_ = std::move(other.shape_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // ==================== 访问器 ====================
    
    /**
     * @brief 获取形状
     */
    const std::vector<size_t>& shape() const { return shape_; }
    
    /**
     * @brief 获取维度数
     */
    size_t ndim() const { return shape_.size(); }
    
    /**
     * @brief 获取总元素数
     */
    size_t size() const { return size_; }
    
    /**
     * @brief 获取原始数据指针
     */
    float* data() { return data_.get(); }
    const float* data() const { return data_.get(); }
    
    /**
     * @brief 检查是否为空
     */
    bool empty() const { return size_ == 0; }
    
    /**
     * @brief 一维索引访问
     */
    float& operator[](size_t index) { return data_[index]; }
    const float& operator[](size_t index) const { return data_[index]; }
    
    /**
     * @brief 二维索引访问
     */
    float& at(size_t i, size_t j) {
        return data_[i * shape_[1] + j];
    }
    const float& at(size_t i, size_t j) const {
        return data_[i * shape_[1] + j];
    }
    
    // ==================== 工厂方法 ====================
    
    /**
     * @brief 创建全零张量
     */
    static Tensor zeros(const std::vector<size_t>& shape) {
        return Tensor(shape);
    }
    
    /**
     * @brief 创建全一张量
     */
    static Tensor ones(const std::vector<size_t>& shape) {
        Tensor t(shape);
        std::fill(t.data_.get(), t.data_.get() + t.size_, 1.0f);
        return t;
    }
    
    /**
     * @brief 创建随机张量（均匀分布）
     */
    static Tensor random(const std::vector<size_t>& shape, 
                         float min_val = -1.0f, 
                         float max_val = 1.0f,
                         unsigned int seed = 42) {
        Tensor t(shape);
        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> dist(min_val, max_val);
        for (size_t i = 0; i < t.size_; ++i) {
            t.data_[i] = dist(gen);
        }
        return t;
    }
    
    /**
     * @brief 创建随机张量（正态分布）
     */
    static Tensor randn(const std::vector<size_t>& shape,
                        float mean = 0.0f,
                        float std = 1.0f,
                        unsigned int seed = 42) {
        Tensor t(shape);
        std::mt19937 gen(seed);
        std::normal_distribution<float> dist(mean, std);
        for (size_t i = 0; i < t.size_; ++i) {
            t.data_[i] = dist(gen);
        }
        return t;
    }
    
    // ==================== 数学运算 ====================
    
    /**
     * @brief 逐元素加法
     */
    Tensor operator+(const Tensor& other) const {
        checkSameShape(other);
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }
    
    /**
     * @brief 逐元素减法
     */
    Tensor operator-(const Tensor& other) const {
        checkSameShape(other);
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }
    
    /**
     * @brief 逐元素乘法
     */
    Tensor operator*(const Tensor& other) const {
        checkSameShape(other);
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] * other.data_[i];
        }
        return result;
    }
    
    /**
     * @brief 标量乘法
     */
    Tensor operator*(float scalar) const {
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] * scalar;
        }
        return result;
    }
    
    /**
     * @brief 矩阵乘法（仅支持2D张量）
     */
    Tensor matmul(const Tensor& other) const {
        if (ndim() != 2 || other.ndim() != 2) {
            throw std::invalid_argument("matmul only supports 2D tensors");
        }
        if (shape_[1] != other.shape_[0]) {
            throw std::invalid_argument("Incompatible shapes for matrix multiplication");
        }
        
        size_t m = shape_[0];
        size_t k = shape_[1];
        size_t n = other.shape_[1];
        
        Tensor result({m, n});
        
        // 简单的三重循环矩阵乘法
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                float sum = 0.0f;
                for (size_t p = 0; p < k; ++p) {
                    sum += at(i, p) * other.at(p, j);
                }
                result.at(i, j) = sum;
            }
        }
        
        return result;
    }
    
    /**
     * @brief 转置（仅支持2D张量）
     */
    Tensor transpose() const {
        if (ndim() != 2) {
            throw std::invalid_argument("transpose only supports 2D tensors");
        }
        Tensor result({shape_[1], shape_[0]});
        for (size_t i = 0; i < shape_[0]; ++i) {
            for (size_t j = 0; j < shape_[1]; ++j) {
                result.at(j, i) = at(i, j);
            }
        }
        return result;
    }
    
    /**
     * @brief 逐元素ReLU
     */
    Tensor relu() const {
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = std::max(0.0f, data_[i]);
        }
        return result;
    }
    
    /**
     * @brief 逐元素Sigmoid
     */
    Tensor sigmoid() const {
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = 1.0f / (1.0f + std::exp(-data_[i]));
        }
        return result;
    }
    
    /**
     * @brief Softmax（沿最后一维）
     */
    Tensor softmax() const {
        Tensor result(shape_);
        
        if (ndim() == 1) {
            // 1D softmax
            float max_val = *std::max_element(data_.get(), data_.get() + size_);
            float sum = 0.0f;
            for (size_t i = 0; i < size_; ++i) {
                result.data_[i] = std::exp(data_[i] - max_val);
                sum += result.data_[i];
            }
            for (size_t i = 0; i < size_; ++i) {
                result.data_[i] /= sum;
            }
        } else if (ndim() == 2) {
            // 2D softmax (per row)
            size_t rows = shape_[0];
            size_t cols = shape_[1];
            for (size_t i = 0; i < rows; ++i) {
                float max_val = data_[i * cols];
                for (size_t j = 1; j < cols; ++j) {
                    max_val = std::max(max_val, data_[i * cols + j]);
                }
                float sum = 0.0f;
                for (size_t j = 0; j < cols; ++j) {
                    result.data_[i * cols + j] = std::exp(data_[i * cols + j] - max_val);
                    sum += result.data_[i * cols + j];
                }
                for (size_t j = 0; j < cols; ++j) {
                    result.data_[i * cols + j] /= sum;
                }
            }
        }
        
        return result;
    }
    
    /**
     * @brief 求和
     */
    float sum() const {
        float result = 0.0f;
        for (size_t i = 0; i < size_; ++i) {
            result += data_[i];
        }
        return result;
    }
    
    /**
     * @brief 均值
     */
    float mean() const {
        return sum() / static_cast<float>(size_);
    }
    
    /**
     * @brief 最大值
     */
    float max() const {
        return *std::max_element(data_.get(), data_.get() + size_);
    }
    
    /**
     * @brief 最小值
     */
    float min() const {
        return *std::min_element(data_.get(), data_.get() + size_);
    }
    
    /**
     * @brief 获取最大值索引
     */
    size_t argmax() const {
        return std::distance(data_.get(), 
                            std::max_element(data_.get(), data_.get() + size_));
    }
    
    // ==================== 输出 ====================
    
    /**
     * @brief 打印张量信息
     */
    void print(const std::string& name = "Tensor") const {
        std::cout << name << " [";
        for (size_t i = 0; i < shape_.size(); ++i) {
            std::cout << shape_[i];
            if (i < shape_.size() - 1) std::cout << ", ";
        }
        std::cout << "] = \n";
        
        if (ndim() == 1) {
            std::cout << "[";
            for (size_t i = 0; i < std::min(size_, size_t(10)); ++i) {
                std::cout << std::fixed << std::setprecision(4) << data_[i];
                if (i < size_ - 1) std::cout << ", ";
            }
            if (size_ > 10) std::cout << "...";
            std::cout << "]\n";
        } else if (ndim() == 2) {
            size_t rows = shape_[0];
            size_t cols = shape_[1];
            for (size_t i = 0; i < std::min(rows, size_t(5)); ++i) {
                std::cout << "  [";
                for (size_t j = 0; j < std::min(cols, size_t(5)); ++j) {
                    std::cout << std::fixed << std::setprecision(4) << at(i, j);
                    if (j < cols - 1) std::cout << ", ";
                }
                if (cols > 5) std::cout << "...";
                std::cout << "]\n";
            }
            if (rows > 5) std::cout << "  ...\n";
        }
    }

private:
    std::unique_ptr<float[]> data_;
    std::vector<size_t> shape_;
    size_t size_;
    
    static size_t computeSize(const std::vector<size_t>& shape) {
        if (shape.empty()) return 0;
        size_t size = 1;
        for (size_t dim : shape) {
            size *= dim;
        }
        return size;
    }
    
    void checkSameShape(const Tensor& other) const {
        if (shape_ != other.shape_) {
            throw std::invalid_argument("Tensor shapes do not match");
        }
    }
};

} // namespace inference

#endif // MODEL_INFERENCE_TENSOR_H
