#ifndef PIMPL_WIDGET_H
#define PIMPL_WIDGET_H

/**
 * @file pimpl_widget.h
 * @brief 使用Pimpl模式的Widget类 - 头文件
 *
 * Pimpl模式（Pointer to Implementation）优势：
 * 1. 编译防火墙：修改实现类无需重新编译使用者代码
 * 2. 减少头文件依赖：私有成员的依赖不会传播给使用者
 * 3. ABI稳定性：二进制接口更稳定
 * 4. 更快的编译速度：头文件更简洁
 */

#include <memory>  // 只需要包含<memory>，其他依赖在实现文件中
#include <string>
#include <vector>

namespace cpp11_features {
class WidgetImpl;

/**
 * @brief 使用Pimpl模式的Widget类
 *
 * 演示标准Pimpl模式的实现：
 * - 使用unique_ptr管理Impl
 * - 正确处理析构函数和移动语义
 * - 支持拷贝语义
 */
class Widget {
public:
    // ========== 构造函数与析构函数 ==========

    /**
     * @brief 默认构造函数
     */
    Widget();

    /**
     * @brief 参数构造函数
     */
    Widget(const std::string& name, int id);

    /**
     * @brief 析构函数
     * @note 必须在.cpp文件中定义，因为unique_ptr析构需要完整类型
     */
    ~Widget();

    // ========== 拷贝操作 ==========

    /**
     * @brief 拷贝构造函数
     * @note unique_ptr不可拷贝，需要手动实现深拷贝
     */
    Widget(const Widget& other);

    /**
     * @brief 拷贝赋值运算符
     */
    Widget& operator=(const Widget& other);

    // ========== 移动操作 ==========

    /**
     * @brief 移动构造函数
     * @note 必须在.cpp中定义
     */
    Widget(Widget&& other) noexcept;

    /**
     * @brief 移动赋值运算符
     */
    Widget& operator=(Widget&& other) noexcept;

    // ========== 公共接口 ==========

    void setName(const std::string& name);
    const std::string& getName() const;

    void setId(int id);
    int getId() const;

    void addData(int value);
    const std::vector<int>& getData() const;

    void setEntry(int key, const std::string& value);
    std::string getEntry(int key) const;

    void doSomething();
    void printInfo() const;

    // ========== 交换函数 ==========

    /**
     * @brief 交换两个Widget
     */
    void swap(Widget& other) noexcept;

private:
    // ========== Pimpl核心 ==========
    std::unique_ptr<WidgetImpl> pImpl_;
};

// 非成员swap函数
inline void swap(Widget& a, Widget& b) noexcept {
    a.swap(b);
}

} // namespace cpp11_features

#endif // PIMPL_WIDGET_H
