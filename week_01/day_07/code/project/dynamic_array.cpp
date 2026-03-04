/**
 * @file dynamic_array.cpp
 * @brief 动态数组类实现
 */

#include "dynamic_array.h"
#include <utility>
#include <new>
#include <string>

namespace cpp_learning {

// ========== 构造函数实现 ==========

template<typename T>
DynamicArray<T>::DynamicArray(size_type count) 
    : data_(count > 0 ? static_cast<T*>(::operator new(count * sizeof(T))) : nullptr),
      size_(count), 
      capacity_(count) {
    // 使用 placement new 逐个构造对象
    for (size_type i = 0; i < count; ++i) {
        new (data_ + i) T();
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(size_type count, const T& value)
    : data_(count > 0 ? static_cast<T*>(::operator new(count * sizeof(T))) : nullptr),
      size_(count),
      capacity_(count) {
    for (size_type i = 0; i < count; ++i) {
        new (data_ + i) T(value);
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> init)
    : data_(init.size() > 0 ? static_cast<T*>(::operator new(init.size() * sizeof(T))) : nullptr),
      size_(init.size()),
      capacity_(init.size()) {
    size_type i = 0;
    for (const auto& elem : init) {
        new (data_ + i) T(elem);
        ++i;
    }
}

template<typename T>
template<typename InputIt, typename>
DynamicArray<T>::DynamicArray(InputIt first, InputIt last) {
    // 计算距离
    auto distance = std::distance(first, last);
    if (distance > 0) {
        size_type count = static_cast<size_type>(distance);
        data_ = static_cast<T*>(::operator new(count * sizeof(T)));
        capacity_ = count;
        
        for (size_type i = 0; first != last; ++first, ++i) {
            new (data_ + i) T(*first);
            ++size_;
        }
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other)
    : data_(other.size_ > 0 ? static_cast<T*>(::operator new(other.size_ * sizeof(T))) : nullptr),
      size_(other.size_),
      capacity_(other.size_) {
    for (size_type i = 0; i < size_; ++i) {
        new (data_ + i) T(other.data_[i]);
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept
    // C++11写法（注释）:
    // : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    //     other.data_ = nullptr; other.size_ = 0; other.capacity_ = 0;
    // }
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0)),
      capacity_(std::exchange(other.capacity_, 0)) {
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    destroy_elements();
    ::operator delete(data_);
}

// ========== 赋值操作实现 ==========

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        DynamicArray temp(other);       // 拷贝构造
        swap(temp);                      // 交换
    }
    return *this;
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& other) noexcept {
    if (this != &other) {
        destroy_elements();
        ::operator delete(data_);
        
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }
    return *this;
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(std::initializer_list<T> init) {
    DynamicArray temp(init);
    swap(temp);
    return *this;
}

// ========== 元素访问实现 ==========

template<typename T>
typename DynamicArray<T>::reference DynamicArray<T>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("DynamicArray::at: index out of range");
    }
    return data_[pos];
}

template<typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::at(size_type pos) const {
    if (pos >= size_) {
        throw std::out_of_range("DynamicArray::at: index out of range");
    }
    return data_[pos];
}

// ========== 容量实现 ==========

template<typename T>
void DynamicArray<T>::reserve(size_type new_cap) {
    if (new_cap > capacity_) {
        reallocate(new_cap);
    }
}

template<typename T>
void DynamicArray<T>::shrink_to_fit() {
    if (size_ < capacity_) {
        if (size_ == 0) {
            ::operator delete(data_);
            data_ = nullptr;
            capacity_ = 0;
        } else {
            reallocate(size_);
        }
    }
}

// ========== 修改操作实现 ==========

template<typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        // 2倍扩容策略
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(value);
    ++size_;
}

template<typename T>
void DynamicArray<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
}

template<typename T>
template<typename... Args>
typename DynamicArray<T>::reference DynamicArray<T>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::forward<Args>(args)...);
    return data_[size_++];
}

template<typename T>
void DynamicArray<T>::pop_back() {
    if (size_ > 0) {
        --size_;
        data_[size_].~T();
    }
}

template<typename T>
void DynamicArray<T>::clear() noexcept {
    destroy_elements();
    size_ = 0;
}

template<typename T>
void DynamicArray<T>::resize(size_type count) {
    if (count < size_) {
        // 缩小：销毁多余元素
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = count;
    } else if (count > size_) {
        // 扩大：添加新元素
        reserve(count);
        for (size_type i = size_; i < count; ++i) {
            new (data_ + i) T();
        }
        size_ = count;
    }
}

template<typename T>
void DynamicArray<T>::resize(size_type count, const T& value) {
    if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = count;
    } else if (count > size_) {
        reserve(count);
        for (size_type i = size_; i < count; ++i) {
            new (data_ + i) T(value);
        }
        size_ = count;
    }
}

template<typename T>
void DynamicArray<T>::swap(DynamicArray& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

// ========== 私有方法实现 ==========

template<typename T>
void DynamicArray<T>::reallocate(size_type new_cap) {
    // 分配新内存
    T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
    
    // 移动旧元素
    for (size_type i = 0; i < size_; ++i) {
        new (new_data + i) T(std::move(data_[i]));
        data_[i].~T();
    }
    
    // 释放旧内存
    ::operator delete(data_);
    
    data_ = new_data;
    capacity_ = new_cap;
}

template<typename T>
void DynamicArray<T>::destroy_elements() noexcept {
    for (size_type i = 0; i < size_; ++i) {
        data_[i].~T();
    }
}

// ========== 非成员函数实现 ==========

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

template<typename T>
bool operator!=(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept {
    lhs.swap(rhs);
}

// ========== 显式实例化常用类型 ==========

template class DynamicArray<int>;
template class DynamicArray<double>;
template class DynamicArray<std::string>;

} // namespace cpp_learning
