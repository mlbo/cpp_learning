/**
 * @file dynamic_array.tpp
 * @brief DynamicArray 模板实现（由 dynamic_array.h 包含）
 */

#include <utility>
#include <new>
#include <string>

namespace cpp_learning {

// ========== 构造函数实现 ==========

template<typename T>
DynamicArray<T>::DynamicArray(size_type count)
    : data_(allocate(count)),
      size_(0),
      capacity_(count) {
    try {
        for (; size_ < count; ++size_) {
            new (data_ + size_) T();
        }
    } catch (...) {
        destroy_elements();
        deallocate(data_);
        throw;
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(size_type count, const T& value)
    : data_(allocate(count)),
      size_(0),
      capacity_(count) {
    try {
        for (; size_ < count; ++size_) {
            new (data_ + size_) T(value);
        }
    } catch (...) {
        destroy_elements();
        deallocate(data_);
        throw;
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> init)
    : data_(allocate(init.size())),
      size_(0),
      capacity_(init.size()) {
    try {
        for (const auto& elem : init) {
            new (data_ + size_) T(elem);
            ++size_;
        }
    } catch (...) {
        destroy_elements();
        deallocate(data_);
        throw;
    }
}

template<typename T>
template<typename InputIt, typename>
DynamicArray<T>::DynamicArray(InputIt first, InputIt last) {
    // 计算距离
    auto distance = std::distance(first, last);
    if (distance < 0) {
        throw std::invalid_argument("DynamicArray range is reversed");
    }
    if (distance > 0) {
        const size_type count = static_cast<size_type>(distance);
        data_ = allocate(count);
        capacity_ = count;

        try {
            for (; first != last; ++first) {
                new (data_ + size_) T(*first);
                ++size_;
            }
        } catch (...) {
            destroy_elements();
            deallocate(data_);
            throw;
        }
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other)
    : data_(allocate(other.size_)),
      size_(0),
      capacity_(other.size_) {
    try {
        for (; size_ < other.size_; ++size_) {
            new (data_ + size_) T(other.data_[size_]);
        }
    } catch (...) {
        destroy_elements();
        deallocate(data_);
        throw;
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
    deallocate(data_);
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
        deallocate(data_);

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
            deallocate(data_);
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
        // 先复制，保证 arr.push_back(arr[0]) 在扩容时不会读取失效引用。
        T value_copy(value);
        reserve(next_capacity());
        new (data_ + size_) T(std::move_if_noexcept(value_copy));
    } else {
        new (data_ + size_) T(value);
    }
    ++size_;
}

template<typename T>
void DynamicArray<T>::push_back(T&& value) {
    // value 可能就是本数组中的元素。必须在 reserve 释放旧存储前
    // 先取得一个独立临时对象，例如 arr.push_back(std::move(arr[0]))。
    T value_copy(std::move(value));
    if (size_ == capacity_) {
        reserve(next_capacity());
    }
    new (data_ + size_) T(std::move(value_copy));
    ++size_;
}

template<typename T>
template<typename... Args>
typename DynamicArray<T>::reference DynamicArray<T>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        // 参数可能引用本数组中的元素或其子对象，扩容前先完成构造。
        T value(std::forward<Args>(args)...);
        reserve(next_capacity());
        new (data_ + size_) T(std::move_if_noexcept(value));
    } else {
        // 容量足够时直接在最终地址构造，保持 emplace 的核心语义。
        new (data_ + size_) T(std::forward<Args>(args)...);
    }
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
        while (size_ < count) {
            new (data_ + size_) T();
            ++size_;
        }
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
        // value 可能引用当前数组元素，扩容前先复制。
        T value_copy(value);
        reserve(count);
        while (size_ < count) {
            new (data_ + size_) T(value_copy);
            ++size_;
        }
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
    T* new_data = allocate(new_cap);

    size_type constructed = 0;
    try {
        // 若移动可能抛异常而拷贝可用，move_if_noexcept 会选择拷贝，
        // 以保留“失败时原容器不变”的强保证。
        for (; constructed < size_; ++constructed) {
            new (new_data + constructed) T(std::move_if_noexcept(data_[constructed]));
        }
    } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
            new_data[i].~T();
        }
        deallocate(new_data);
        throw;
    }

    destroy_elements();
    // 释放旧内存
    deallocate(data_);

    data_ = new_data;
    capacity_ = new_cap;
}

template<typename T>
void DynamicArray<T>::destroy_elements() noexcept {
    for (size_type i = 0; i < size_; ++i) {
        data_[i].~T();
    }
}

template<typename T>
typename DynamicArray<T>::pointer DynamicArray<T>::allocate(size_type count) {
    if (count > max_size()) {
        throw std::length_error("DynamicArray capacity exceeds max_size");
    }
    if (count == 0) {
        return nullptr;
    }
    if constexpr (alignof(T) > alignof(std::max_align_t)) {
        return static_cast<pointer>(
            ::operator new(count * sizeof(T), std::align_val_t{alignof(T)}));
    }
    return static_cast<pointer>(::operator new(count * sizeof(T)));
}

template<typename T>
void DynamicArray<T>::deallocate(pointer ptr) noexcept {
    if constexpr (alignof(T) > alignof(std::max_align_t)) {
        ::operator delete(ptr, std::align_val_t{alignof(T)});
    } else {
        ::operator delete(ptr);
    }
}

template<typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::next_capacity() const {
    if (capacity_ == 0) {
        return 1;
    }
    if (capacity_ > max_size() / 2) {
        if (capacity_ == max_size()) {
            throw std::length_error("DynamicArray cannot grow beyond max_size");
        }
        return max_size();
    }
    return capacity_ * 2;
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

} // namespace cpp_learning
