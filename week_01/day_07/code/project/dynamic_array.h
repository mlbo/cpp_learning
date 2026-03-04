/**
 * @file dynamic_array.h
 * @brief 动态数组类 - 类似于简化版 std::vector
 * 
 * 演示知识点：
 * - RAII: 资源获取即初始化
 * - 移动语义: 移动构造和移动赋值
 * - 模板编程: 类模板设计
 * - 异常安全: 强异常安全保证
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <iterator>

namespace cpp_learning {

/**
 * @brief 动态数组模板类
 * @tparam T 元素类型
 * 
 * 实现了一个简化版的动态数组，支持：
 * - 动态扩容
 * - 随机访问
 * - 范围迭代
 * - 移动语义
 */
template<typename T>
class DynamicArray {
public:
    // ========== 类型别名 ==========
    using value_type        = T;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = T*;
    using const_pointer     = const T*;
    using iterator          = T*;
    using const_iterator    = const T*;

    // ========== 构造函数 ==========
    
    /**
     * @brief 默认构造函数
     */
    DynamicArray() noexcept = default;

    /**
     * @brief 构造指定大小的数组
     * @param count 元素数量
     */
    explicit DynamicArray(size_type count);

    /**
     * @brief 构造指定大小的数组，填充指定值
     * @param count 元素数量
     * @param value 填充值
     */
    DynamicArray(size_type count, const T& value);

    /**
     * @brief 从初始化列表构造
     * @param init 初始化列表
     */
    DynamicArray(std::initializer_list<T> init);

    /**
     * @brief 从迭代器范围构造
     * @tparam InputIt 输入迭代器类型
     * @param first 起始迭代器
     * @param last 结束迭代器
     */
    template<typename InputIt,
             // C++11写法（注释）:
             // typename = typename std::enable_if<
             //     std::is_convertible<typename std::iterator_traits<InputIt>::iterator_category,
             //                         std::input_iterator_tag>::value>::type
             typename = std::enable_if_t<std::is_convertible_v<
                 typename std::iterator_traits<InputIt>::iterator_category,
                 std::input_iterator_tag>>>
    DynamicArray(InputIt first, InputIt last);

    /**
     * @brief 拷贝构造函数
     */
    DynamicArray(const DynamicArray& other);

    /**
     * @brief 移动构造函数
     */
    DynamicArray(DynamicArray&& other) noexcept;

    /**
     * @brief 析构函数
     */
    ~DynamicArray();

    // ========== 赋值操作 ==========
    
    /**
     * @brief 拷贝赋值运算符
     */
    DynamicArray& operator=(const DynamicArray& other);

    /**
     * @brief 移动赋值运算符
     */
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    /**
     * @brief 初始化列表赋值
     */
    DynamicArray& operator=(std::initializer_list<T> init);

    // ========== 元素访问 ==========
    
    /**
     * @brief 下标访问（无边界检查）
     */
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }

    /**
     * @brief 带边界检查的访问
     * @throws std::out_of_range 如果 pos >= size()
     */
    reference at(size_type pos);
    const_reference at(size_type pos) const;

    /**
     * @brief 访问首元素
     */
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    /**
     * @brief 访问末元素
     */
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    /**
     * @brief 获取底层数据指针
     */
    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    // ========== 容量 ==========
    
    /**
     * @brief 检查是否为空
     */
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief 获取元素数量
     */
    size_type size() const noexcept { return size_; }

    /**
     * @brief 获取容量
     */
    size_type capacity() const noexcept { return capacity_; }

    /**
     * @brief 预留容量
     */
    void reserve(size_type new_cap);

    /**
     * @brief 缩减容量以适应大小
     */
    void shrink_to_fit();

    // ========== 修改操作 ==========
    
    /**
     * @brief 在末尾添加元素（拷贝）
     */
    void push_back(const T& value);

    /**
     * @brief 在末尾添加元素（移动）
     */
    void push_back(T&& value);

    /**
     * @brief 原地构造元素
     */
    template<typename... Args>
    reference emplace_back(Args&&... args);

    /**
     * @brief 移除末尾元素
     */
    void pop_back();

    /**
     * @brief 清空所有元素
     */
    void clear() noexcept;

    /**
     * @brief 调整大小
     */
    void resize(size_type count);
    void resize(size_type count, const T& value);

    /**
     * @brief 交换内容
     */
    void swap(DynamicArray& other) noexcept;

    // ========== 迭代器 ==========
    
    iterator begin() noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return data_ + size_; }

private:
    T* data_ = nullptr;          ///< 数据指针
    size_type size_ = 0;         ///< 当前元素数量
    size_type capacity_ = 0;     ///< 当前容量

    /**
     * @brief 重新分配内存
     */
    void reallocate(size_type new_cap);

    /**
     * @brief 销毁所有元素
     */
    void destroy_elements() noexcept;

    /**
     * @brief 移动元素到新位置
     */
    void move_elements(T* dest, T* src, size_type count);
};

// ========== 非成员函数 ==========

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template<typename T>
bool operator!=(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template<typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept;

} // namespace cpp_learning

#endif // DYNAMIC_ARRAY_H
