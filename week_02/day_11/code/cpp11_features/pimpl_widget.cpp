/**
 * @file pimpl_widget.cpp
 * @brief Widget实现文件
 *
 * 关键点：
 * 1. 在此处定义Impl类（或包含impl.h）
 * 2. 所有特殊成员函数必须在此处定义
 * 3. unique_ptr的析构需要完整类型，所以不能在头文件中=default
 */

#include "pimpl_widget.h"
#include "pimpl_widget_impl.h"
#include <iostream>
#include <utility>

namespace cpp11_features {

// ========== 构造函数实现 ==========

Widget::Widget()
    : pImpl_(std::make_unique<WidgetImpl>()) {
    std::cout << "[Widget] 默认构造" << std::endl;
}

Widget::Widget(const std::string& name, int id)
    : pImpl_(std::make_unique<WidgetImpl>(name, id)) {
    std::cout << "[Widget] 参数构造" << std::endl;
}

// ========== 析构函数实现 ==========
// 必须定义！因为unique_ptr析构时需要完整的Impl类型
// 不能在头文件中使用 ~Widget() = default

Widget::~Widget() {
    std::cout << "[Widget] 析构" << std::endl;
    // unique_ptr自动管理内存，无需手动delete
    // 但必须在.cpp中定义，因为此处Impl是完整类型
}

// ========== 拷贝操作实现 ==========

Widget::Widget(const Widget& other)
    : pImpl_(std::make_unique<WidgetImpl>(*other.pImpl_)) {
    // 深拷贝：创建新的Impl，使用Impl的拷贝构造
    std::cout << "[Widget] 拷贝构造" << std::endl;
}

Widget& Widget::operator=(const Widget& other) {
    if (this != &other) {
        // 方式1：使用拷贝并交换惯用法
        // Widget temp(other);
        // swap(temp);

        // 方式2：直接赋值Impl
        *pImpl_ = *other.pImpl_;
    }
    std::cout << "[Widget] 拷贝赋值" << std::endl;
    return *this;
}

// ========== 移动操作实现 ==========

Widget::Widget(Widget&& other) noexcept
    : pImpl_(std::move(other.pImpl_)) {
    // unique_ptr的移动构造自动处理
    std::cout << "[Widget] 移动构造" << std::endl;
}

Widget& Widget::operator=(Widget&& other) noexcept {
    if (this != &other) {
        pImpl_ = std::move(other.pImpl_);
    }
    std::cout << "[Widget] 移动赋值" << std::endl;
    return *this;
}

// ========== 公共接口实现（转发给Impl） ==========

void Widget::setName(const std::string& name) {
    pImpl_->setName(name);
}

const std::string& Widget::getName() const {
    return pImpl_->getName();
}

void Widget::setId(int id) {
    pImpl_->setId(id);
}

int Widget::getId() const {
    return pImpl_->getId();
}

void Widget::addData(int value) {
    pImpl_->addData(value);
}

const std::vector<int>& Widget::getData() const {
    return pImpl_->getData();
}

void Widget::setEntry(int key, const std::string& value) {
    pImpl_->setEntry(key, value);
}

std::string Widget::getEntry(int key) const {
    return pImpl_->getEntry(key);
}

void Widget::doSomething() {
    pImpl_->doSomething();
}

void Widget::printInfo() const {
    pImpl_->printInfo();
}

// ========== 交换函数实现 ==========

void Widget::swap(Widget& other) noexcept {
    using std::swap;
    swap(pImpl_, other.pImpl_);
}

} // namespace cpp11_features
