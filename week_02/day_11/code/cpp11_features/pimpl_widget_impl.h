#ifndef PIMPL_WIDGET_IMPL_H
#define PIMPL_WIDGET_IMPL_H

/**
 * @file pimpl_widget_impl.h
 * @brief Widget::Impl 实现类定义
 *
 * 此文件通常不对外公开，仅被widget.cpp包含
 * 这里为了演示目的单独列出
 */

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace cpp11_features {

/**
 * @brief Widget的实现类
 *
 * 所有私有成员和实现细节都放在这里
 * 修改此文件不会导致包含widget.h的代码重新编译
 */
class WidgetImpl {
public:
    // 默认构造函数
    WidgetImpl() : name_("default"), id_(0) {
        std::cout << "[WidgetImpl] 默认构造" << std::endl;
    }

    // 参数构造函数
    WidgetImpl(const std::string& name, int id)
        : name_(name), id_(id) {
        std::cout << "[WidgetImpl] 构造: " << name << ", id=" << id << std::endl;
    }

    // 析构函数
    ~WidgetImpl() {
        std::cout << "[WidgetImpl] 析构: " << name_ << std::endl;
    }

    // 拷贝构造
    WidgetImpl(const WidgetImpl& other)
        : name_(other.name_)
        , id_(other.id_)
        , data_(other.data_)
        , lookup_(other.lookup_) {
        std::cout << "[WidgetImpl] 拷贝构造" << std::endl;
    }

    // 移动构造
    WidgetImpl(WidgetImpl&& other) noexcept
        : name_(std::move(other.name_))
        , id_(other.id_)
        , data_(std::move(other.data_))
        , lookup_(std::move(other.lookup_)) {
        other.id_ = 0;
        std::cout << "[WidgetImpl] 移动构造" << std::endl;
    }

    // 拷贝赋值
    WidgetImpl& operator=(const WidgetImpl& other) {
        if (this != &other) {
            name_ = other.name_;
            id_ = other.id_;
            data_ = other.data_;
            lookup_ = other.lookup_;
        }
        std::cout << "[WidgetImpl] 拷贝赋值" << std::endl;
        return *this;
    }

    // 移动赋值
    WidgetImpl& operator=(WidgetImpl&& other) noexcept {
        if (this != &other) {
            name_ = std::move(other.name_);
            id_ = other.id_;
            data_ = std::move(other.data_);
            lookup_ = std::move(other.lookup_);
            other.id_ = 0;
        }
        std::cout << "[WidgetImpl] 移动赋值" << std::endl;
        return *this;
    }

    // ========== 公共接口实现 ==========

    void setName(const std::string& name) {
        name_ = name;
    }

    const std::string& getName() const {
        return name_;
    }

    void setId(int id) {
        id_ = id;
    }

    int getId() const {
        return id_;
    }

    void addData(int value) {
        data_.push_back(value);
    }

    const std::vector<int>& getData() const {
        return data_;
    }

    void setEntry(int key, const std::string& value) {
        lookup_[key] = value;
    }

    std::string getEntry(int key) const {
        auto it = lookup_.find(key);
        return (it != lookup_.end()) ? it->second : "";
    }

    void doSomething() {
        // 复杂的实现逻辑
        std::cout << "[WidgetImpl] 执行操作，数据大小: "
                  << data_.size() << ", 查找表大小: "
                  << lookup_.size() << std::endl;
    }

    // 打印信息（用于演示）
    void printInfo() const {
        std::cout << "=== Widget信息 ===" << std::endl;
        std::cout << "名称: " << name_ << std::endl;
        std::cout << "ID: " << id_ << std::endl;
        std::cout << "数据: [";
        for (size_t i = 0; i < data_.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data_[i];
        }
        std::cout << "]" << std::endl;
        std::cout << "查找表: {";
        bool first = true;
        for (const auto& [key, value] : lookup_) {
            if (!first) std::cout << ", ";
            std::cout << key << ":" << value;
            first = false;
        }
        std::cout << "}" << std::endl;
    }

private:
    // ========== 私有数据成员 ==========
    std::string name_;                  // 名称
    int id_;                            // ID
    std::vector<int> data_;             // 数据容器
    std::map<int, std::string> lookup_; // 查找表
};

} // namespace cpp11_features

#endif // PIMPL_WIDGET_IMPL_H
