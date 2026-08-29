/**
 * @file tensor.h
 * @brief 教学用张量类：动态形状、基本运算与确定性的边界检查
 */

#ifndef MODEL_INFERENCE_TENSOR_H
#define MODEL_INFERENCE_TENSOR_H

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace inference {

class Tensor {
public:
    // 默认构造的 Tensor 是唯一的空哨兵；带 shape 的 Tensor 必须每一维都大于 0。
    Tensor() noexcept : size_(0), data_(nullptr) {}

    explicit Tensor(const std::vector<size_t>& shape)
        : shape_(shape), size_(computeSize(shape)), data_(std::make_unique<float[]>(size_)) {
        std::fill(data_.get(), data_.get() + size_, 0.0f);
    }

    Tensor(const std::vector<size_t>& shape, const std::vector<float>& data)
        : shape_(shape), size_(computeSize(shape)) {
        if (size_ != data.size()) {
            throw std::invalid_argument("Tensor data size does not match shape");
        }
        data_ = std::make_unique<float[]>(size_);
        std::copy(data.begin(), data.end(), data_.get());
    }

    Tensor(const Tensor& other) : shape_(other.shape_), size_(other.size_) {
        if (!other.empty()) {
            data_ = std::make_unique<float[]>(size_);
            std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
        }
    }

    Tensor(Tensor&& other) noexcept
        : shape_(std::move(other.shape_)),
          size_(other.size_),
          data_(std::move(other.data_)) {
        other.shape_.clear();
        other.size_ = 0;
    }

    Tensor& operator=(const Tensor& other) {
        if (this == &other) {
            return *this;
        }
        Tensor copy(other);
        swap(copy);
        return *this;
    }

    Tensor& operator=(Tensor&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            shape_ = std::move(other.shape_);
            size_ = other.size_;
            other.shape_.clear();
            other.size_ = 0;
        }
        return *this;
    }

    void swap(Tensor& other) noexcept {
        data_.swap(other.data_);
        shape_.swap(other.shape_);
        std::swap(size_, other.size_);
    }

    const std::vector<size_t>& shape() const noexcept { return shape_; }
    size_t ndim() const noexcept { return shape_.size(); }
    size_t size() const noexcept { return size_; }
    float* data() noexcept { return data_.get(); }
    const float* data() const noexcept { return data_.get(); }
    bool empty() const noexcept { return size_ == 0; }
    bool allFinite() const noexcept {
        for (size_t i = 0; i < size_; ++i) {
            if (!std::isfinite(data_[i])) {
                return false;
            }
        }
        return true;
    }

    // 为教学项目选择“检查式 []”：越界统一抛出 std::out_of_range。
    float& operator[](size_t index) {
        checkFlatIndex(index);
        return data_[index];
    }

    const float& operator[](size_t index) const {
        checkFlatIndex(index);
        return data_[index];
    }

    float& at(size_t i, size_t j) {
        return data_[checkedOffset(i, j)];
    }

    const float& at(size_t i, size_t j) const {
        return data_[checkedOffset(i, j)];
    }

    static Tensor zeros(const std::vector<size_t>& shape) { return Tensor(shape); }

    static Tensor ones(const std::vector<size_t>& shape) {
        Tensor tensor(shape);
        std::fill(tensor.data_.get(), tensor.data_.get() + tensor.size_, 1.0f);
        return tensor;
    }

    static Tensor random(const std::vector<size_t>& shape,
                         float min_val = -1.0f,
                         float max_val = 1.0f,
                         unsigned int seed = 42) {
        if (!std::isfinite(min_val) || !std::isfinite(max_val) || min_val > max_val) {
            throw std::invalid_argument(
                "Tensor::random requires finite bounds with min_val <= max_val");
        }
        Tensor tensor(shape);
        std::mt19937 generator(seed);
        std::uniform_real_distribution<float> distribution(min_val, max_val);
        for (size_t i = 0; i < tensor.size_; ++i) {
            tensor.data_[i] = distribution(generator);
            if (!std::isfinite(tensor.data_[i])) {
                throw std::overflow_error("Tensor::random produced a non-finite value");
            }
        }
        return tensor;
    }

    static Tensor randn(const std::vector<size_t>& shape,
                        float mean = 0.0f,
                        float stddev = 1.0f,
                        unsigned int seed = 42) {
        if (!std::isfinite(mean) || !std::isfinite(stddev) || !(stddev > 0.0f)) {
            throw std::invalid_argument(
                "Tensor::randn requires a finite mean and finite stddev > 0");
        }
        Tensor tensor(shape);
        std::mt19937 generator(seed);
        std::normal_distribution<float> distribution(mean, stddev);
        for (size_t i = 0; i < tensor.size_; ++i) {
            tensor.data_[i] = distribution(generator);
            if (!std::isfinite(tensor.data_[i])) {
                throw std::overflow_error("Tensor::randn produced a non-finite value");
            }
        }
        return tensor;
    }

    Tensor operator+(const Tensor& other) const {
        checkSameShape(other);
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    Tensor operator-(const Tensor& other) const {
        checkSameShape(other);
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    Tensor operator*(const Tensor& other) const {
        checkSameShape(other);
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] * other.data_[i];
        }
        return result;
    }

    Tensor operator*(float scalar) const {
        requireNotEmpty("scalar multiplication");
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i] * scalar;
        }
        return result;
    }

    Tensor matmul(const Tensor& other) const {
        requireNotEmpty("matmul");
        other.requireNotEmpty("matmul");
        if (ndim() != 2 || other.ndim() != 2) {
            throw std::invalid_argument("Tensor::matmul requires two 2D tensors");
        }
        if (shape_[1] != other.shape_[0]) {
            throw std::invalid_argument("Tensor::matmul received incompatible shapes");
        }

        const size_t rows = shape_[0];
        const size_t inner = shape_[1];
        const size_t columns = other.shape_[1];
        Tensor result({rows, columns});
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                float value = 0.0f;
                for (size_t k = 0; k < inner; ++k) {
                    value += at(i, k) * other.at(k, j);
                }
                result.at(i, j) = value;
            }
        }
        return result;
    }

    Tensor transpose() const {
        requireNotEmpty("transpose");
        if (ndim() != 2) {
            throw std::invalid_argument("Tensor::transpose requires a 2D tensor");
        }
        Tensor result({shape_[1], shape_[0]});
        for (size_t i = 0; i < shape_[0]; ++i) {
            for (size_t j = 0; j < shape_[1]; ++j) {
                result.at(j, i) = at(i, j);
            }
        }
        return result;
    }

    Tensor relu() const {
        requireNotEmpty("relu");
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = std::max(0.0f, data_[i]);
        }
        return result;
    }

    Tensor sigmoid() const {
        requireNotEmpty("sigmoid");
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = 1.0f / (1.0f + std::exp(-data_[i]));
        }
        return result;
    }

    // 1D 沿唯一维度计算；2D 逐行计算。其他维度明确拒绝。
    Tensor softmax() const {
        requireNotEmpty("softmax");
        if (ndim() != 1 && ndim() != 2) {
            throw std::invalid_argument("Tensor::softmax supports only 1D or 2D tensors");
        }

        Tensor result(shape_);
        if (ndim() == 1) {
            softmaxRange(data_.get(), result.data_.get(), size_);
            return result;
        }

        const size_t rows = shape_[0];
        const size_t columns = shape_[1];
        for (size_t row = 0; row < rows; ++row) {
            softmaxRange(data_.get() + row * columns,
                         result.data_.get() + row * columns,
                         columns);
        }
        return result;
    }

    // 空集合的和定义为加法单位元 0；需要元素的归约则抛出异常。
    float sum() const noexcept {
        float result = 0.0f;
        for (size_t i = 0; i < size_; ++i) {
            result += data_[i];
        }
        return result;
    }

    float mean() const {
        requireNotEmpty("mean");
        return sum() / static_cast<float>(size_);
    }

    float max() const {
        requireNotEmpty("max");
        return *std::max_element(data_.get(), data_.get() + size_);
    }

    float min() const {
        requireNotEmpty("min");
        return *std::min_element(data_.get(), data_.get() + size_);
    }

    size_t argmax() const {
        requireNotEmpty("argmax");
        return static_cast<size_t>(std::distance(
            data_.get(), std::max_element(data_.get(), data_.get() + size_)));
    }

    void print(const std::string& name = "Tensor") const {
        std::cout << name << " [";
        for (size_t i = 0; i < shape_.size(); ++i) {
            std::cout << shape_[i];
            if (i + 1 < shape_.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "] =\n";
        if (empty()) {
            std::cout << "<empty>\n";
            return;
        }

        if (ndim() == 1) {
            std::cout << "[";
            const size_t displayed = std::min(size_, size_t{10});
            for (size_t i = 0; i < displayed; ++i) {
                std::cout << std::fixed << std::setprecision(4) << data_[i];
                if (i + 1 < displayed) {
                    std::cout << ", ";
                }
            }
            if (size_ > displayed) {
                std::cout << "...";
            }
            std::cout << "]\n";
        } else if (ndim() == 2) {
            const size_t displayed_rows = std::min(shape_[0], size_t{5});
            const size_t displayed_columns = std::min(shape_[1], size_t{5});
            for (size_t i = 0; i < displayed_rows; ++i) {
                std::cout << "  [";
                for (size_t j = 0; j < displayed_columns; ++j) {
                    std::cout << std::fixed << std::setprecision(4) << at(i, j);
                    if (j + 1 < displayed_columns) {
                        std::cout << ", ";
                    }
                }
                if (shape_[1] > displayed_columns) {
                    std::cout << "...";
                }
                std::cout << "]\n";
            }
            if (shape_[0] > displayed_rows) {
                std::cout << "  ...\n";
            }
        }
    }

private:
    std::vector<size_t> shape_;
    size_t size_;
    std::unique_ptr<float[]> data_;

    static size_t computeSize(const std::vector<size_t>& shape) {
        if (shape.empty()) {
            throw std::invalid_argument("Tensor shape must contain at least one dimension");
        }
        size_t size = 1;
        for (size_t dimension : shape) {
            if (dimension == 0) {
                throw std::invalid_argument("Tensor dimensions must be greater than zero");
            }
            if (size > std::numeric_limits<size_t>::max() / dimension) {
                throw std::overflow_error("Tensor shape size overflows size_t");
            }
            size *= dimension;
        }
        return size;
    }

    void requireNotEmpty(const char* operation) const {
        if (empty()) {
            throw std::invalid_argument(std::string("Tensor::") + operation +
                                        " is undefined for an empty tensor");
        }
    }

    void checkFlatIndex(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Tensor flat index is out of range");
        }
    }

    size_t checkedOffset(size_t i, size_t j) const {
        if (ndim() != 2) {
            throw std::invalid_argument("Tensor::at(i, j) requires a 2D tensor");
        }
        if (i >= shape_[0] || j >= shape_[1]) {
            throw std::out_of_range("Tensor 2D index is out of range");
        }
        return i * shape_[1] + j;
    }

    void checkSameShape(const Tensor& other) const {
        requireNotEmpty("element-wise operation");
        other.requireNotEmpty("element-wise operation");
        if (shape_ != other.shape_) {
            throw std::invalid_argument("Tensor shapes do not match");
        }
    }

    static void softmaxRange(const float* input, float* output, size_t count) {
        for (size_t i = 0; i < count; ++i) {
            if (!std::isfinite(input[i])) {
                throw std::invalid_argument(
                    "Tensor::softmax requires finite input values");
            }
        }

        const double max_value = static_cast<double>(
            *std::max_element(input, input + count));
        double total = 0.0;
        for (size_t i = 0; i < count; ++i) {
            const double value = std::exp(static_cast<double>(input[i]) - max_value);
            output[i] = static_cast<float>(value);
            total += value;
        }
        for (size_t i = 0; i < count; ++i) {
            output[i] = static_cast<float>(static_cast<double>(output[i]) / total);
        }
    }
};

}  // namespace inference

#endif  // MODEL_INFERENCE_TENSOR_H
