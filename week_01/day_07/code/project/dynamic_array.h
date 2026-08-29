/**
 * @file dynamic_array.h
 * @brief 动态数组类 - 类似于简化版 std::vector
 * 
 * 演示知识点：
 * - RAII: 资源获取即初始化
 * - 移动语义: 移动构造和移动赋值
 * - 模板编程: 类模板设计
 * - 异常安全: 构造失败不泄漏，扩容在可拷贝或无抛出移动时提供强保证
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <limits>

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
     * @tparam InputIt 前向迭代器类型（实现需要先求距离，再遍历一次）
     * @param first 起始迭代器
     * @param last 结束迭代器
     */
    template<typename InputIt,
             // C++11写法（注释）:
             // typename = typename std::enable_if<
             //     std::is_convertible<typename std::iterator_traits<InputIt>::iterator_category,
             //                         std::forward_iterator_tag>::value>::type
             typename = std::enable_if_t<std::is_convertible_v<
                 typename std::iterator_traits<InputIt>::iterator_category,
                 std::forward_iterator_tag>>>
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
     * @pre pos < size()；需要失败报告时使用 at()。
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
     * @throws std::out_of_range 容器为空
     */
    reference front() { return at(0); }
    const_reference front() const { return at(0); }

    /**
     * @brief 访问末元素
     * @throws std::out_of_range 容器为空
     */
    reference back() {
        if (empty()) throw std::out_of_range("DynamicArray::back on empty array");
        return data_[size_ - 1];
    }
    const_reference back() const {
        if (empty()) throw std::out_of_range("DynamicArray::back on empty array");
        return data_[size_ - 1];
    }

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
     * @brief 当前元素类型在字节乘法不溢出时可表示的最大元素数
     */
    static constexpr size_type max_size() noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

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
     * @brief 在容量足够时原地构造；扩容时先构造临时值以保护自引用参数
     */
    template<typename... Args>
    reference emplace_back(Args&&... args);

    /**
     * @brief 移除末尾元素；空容器上调用为无操作
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
    iterator end() noexcept { return size_ == 0 ? data_ : data_ + size_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator end() const noexcept { return size_ == 0 ? data_ : data_ + size_; }
    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return size_ == 0 ? data_ : data_ + size_; }

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
     * @brief 检查字节乘法后分配原始存储
     */
    static pointer allocate(size_type count);

    /**
     * @brief 使用与 allocate 匹配的方式释放原始存储
     */
    static void deallocate(pointer ptr) noexcept;

    /**
     * @brief 计算下一次增长容量，达到上限时抛出 length_error
     */
    size_type next_capacity() const;

};

// ========== 非成员函数 ==========

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template<typename T>
bool operator!=(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template<typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept;

} // namespace cpp_learning

// 模板定义必须对实例化点可见。将实现放在 .tpp 中既保持文件可读性，
// 又能让 DynamicArray<Point> 等用户自定义类型正确实例化。
#include "dynamic_array.tpp"

#endif // DYNAMIC_ARRAY_H
