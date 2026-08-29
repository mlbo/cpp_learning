#ifndef PIMPL_WIDGET_H
#define PIMPL_WIDGET_H

/**
 * @file pimpl_widget.h
 * @brief 使用Pimpl模式的Widget类 - 头文件
 *
 * Pimpl模式（Pointer to Implementation）优势：
 * 1. 编译防火墙：只改实现细节时，通常无需重新编译使用者代码
 * 2. 减少头文件依赖：私有成员的依赖不会传播给使用者
 * 3. ABI稳定性：有助于保持对象布局稳定，但不自动保证ABI兼容
 * 4. 更快的编译速度：头文件更简洁
 */

#include <memory>
#include <string>  // 公开接口使用std::string，因此仍必须包含
#include <vector>  // 公开接口使用std::vector，因此仍必须包含

namespace cpp11_features {
class WidgetImpl;

/**
 * @brief 使用Pimpl模式的Widget类
 *
 * 演示标准Pimpl模式的实现：
 * - 使用unique_ptr管理Impl
 * - 正确处理析构函数和移动语义
 * - 支持拷贝语义
 *
 * @warning 本教学实现采用unique_ptr的自然移动状态。Widget被移动后pImpl_为空，
 *          只保证可以析构、作为赋值目标或参与swap；恢复有效状态前不得调用普通
 *          查询和修改接口。
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
     * @note 使用copy-and-swap，目标即使处于moved-from状态也可安全赋值
     */
    Widget& operator=(const Widget& other);

    // ========== 移动操作 ==========

    /**
     * @brief 移动构造函数
     * @note 必须在.cpp中定义
     * @note 被移动对象只保证可析构、可重新赋值或交换
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
    // 前向声明足以声明指针；真正删除WidgetImpl时必须看到完整类型。
    std::unique_ptr<WidgetImpl> pImpl_;
};

// 非成员swap函数
inline void swap(Widget& a, Widget& b) noexcept {
    a.swap(b);
}

} // namespace cpp11_features

#endif // PIMPL_WIDGET_H
