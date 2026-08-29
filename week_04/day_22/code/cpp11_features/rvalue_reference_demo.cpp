/**
 * Day 22: 右值引用演示
 * 实现文件
 */

#include "rvalue_reference_demo.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>

// ==================== MyString 实现 ====================

MyString::MyString(const char* str) {
    const char* source = str ? str : "";
    size_ = std::strlen(source);
    data_ = new char[size_ + 1];
    std::memcpy(data_, source, size_ + 1);
}

MyString::~MyString() {
    delete[] data_;
}

MyString::MyString(const MyString& other) : size_(other.size_) {
    data_ = new char[size_ + 1];
    std::memcpy(data_, other.c_str(), size_ + 1);
}

MyString::MyString(MyString&& other) noexcept {
    // 直接"窃取"资源
    data_ = other.data_;
    size_ = other.size_;
    
    // 置空源对象，防止重复释放
    other.data_ = nullptr;
    other.size_ = 0;
    
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        // 先构造临时副本；若分配失败，当前对象保持不变（强异常保证）。
        MyString copy(other);
        swap(copy);
    }
    return *this;
}

void MyString::swap(MyString& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        
        // 直接"窃取"资源
        data_ = other.data_;
        size_ = other.size_;
        
        // 置空源对象
        other.data_ = nullptr;
        other.size_ = 0;
        
    }
    return *this;
}

// ==================== 演示函数 ====================

void lvalueRvalueDemo() {
    std::cout << "\n--- 左值与右值 ---" << std::endl;
    
    int x = 10;  // 表达式 x 是左值，字面量表达式 10 是纯右值
    
    // 左值引用
    int& lr = x;
    std::cout << "左值引用 int& lr = x; -> " << lr << std::endl;
    
    // 右值引用
    int&& rr1 = 10;
    std::cout << "右值引用 int&& rr1 = 10; -> " << rr1 << std::endl;
    std::cout << "  注意：rr1 的声明类型是 int&&，但有名字的表达式 rr1 是左值" << std::endl;
    std::cout << "  std::move(rr1) 才产生将亡值(xvalue)，std::move 本身不搬资源" << std::endl;
    
    int&& rr2 = x + 5;  // x+5是临时对象（右值）
    std::cout << "右值引用 int&& rr2 = x + 5; -> " << rr2 << std::endl;
    
    // const左值引用可以绑定右值
    const int& clr = 20;
    std::cout << "const左值引用 const int& clr = 20; -> " << clr << std::endl;
    
    std::cout << "\n规则总结：" << std::endl;
    std::cout << "  - 左值引用(&) 只能绑定左值" << std::endl;
    std::cout << "  - 右值引用(&&) 只能绑定右值" << std::endl;
    std::cout << "  - const左值引用(const&) 可以绑定任意值" << std::endl;
    std::cout << "  - 值类别属于表达式，不属于对象；同一个对象可由左值或xvalue表达式指代" << std::endl;
    std::cout << "  - std::move把表达式转换为xvalue，不保证随后一定发生移动" << std::endl;
}

void moveDemo() {
    std::cout << "\n--- std::move 演示 ---" << std::endl;
    
    std::string str1 = "Hello";
    std::cout << "原始字符串 str1 = \"" << str1 << "\"" << std::endl;
    
    // std::move 将表达式无条件转换为 xvalue，本身不执行移动。
    std::string str2 = std::move(str1);
    std::cout << "使用 std::move 后:" << std::endl;
    std::cout << "  str1 = \"" << str1 << "\" (有效但状态未指定，不能假定为空)" << std::endl;
    std::cout << "  str2 = \"" << str2 << "\"" << std::endl;
    str1 = "可重新赋值";
    std::cout << "  str1 重新赋值后 = \"" << str1 << "\"" << std::endl;
    
    // vector的移动
    std::cout << "\n--- vector 移动演示 ---" << std::endl;
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "v1 大小: " << v1.size() << std::endl;
    
    std::vector<int> v2 = std::move(v1);
    std::cout << "std::move 后:" << std::endl;
    std::cout << "  v1 大小: " << v1.size() << "（仅展示本次实现结果，不作可移植保证）" << std::endl;
    std::cout << "  v2 大小: " << v2.size() << std::endl;
    
    std::cout << "  v2 元素: ";
    for (int n : v2) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void moveSemanticsDemo() {
    std::cout << "\n--- 移动语义演示 ---" << std::endl;
    std::cout << "  特殊成员只管理资源；观察日志放在调用点，流异常不会改变资源操作契约。"
              << std::endl;
    
    std::cout << "创建 s1:" << std::endl;
    MyString s1("Hello");
    std::cout << "  s1 构造完成: \"" << s1.c_str() << "\"" << std::endl;
    
    std::cout << "\n拷贝构造 s2 = s1:" << std::endl;
    MyString s2 = s1;
    std::cout << "  s2 拷贝完成: \"" << s2.c_str() << "\"" << std::endl;
    std::cout << "  s1.c_str() = \"" << s1.c_str() << "\"" << std::endl;
    std::cout << "  s2.c_str() = \"" << s2.c_str() << "\"" << std::endl;
    
    std::cout << "\n移动构造 s3 = std::move(s2):" << std::endl;
    MyString s3 = std::move(s2);
    std::cout << "  s2.c_str() = \"" << s2.c_str()
              << "\"（MyString 自己把 moved-from 状态定义为空串）" << std::endl;
    std::cout << "  s3.c_str() = \"" << s3.c_str() << "\"" << std::endl;

    std::cout << "\n从移动后的 s2 再拷贝，验证空状态仍满足类不变量:" << std::endl;
    MyString s4 = s2;
    std::cout << "  s4.c_str() = \"" << s4.c_str() << "\"" << std::endl;
    
    std::cout << "\n析构顺序（离开作用域时）：" << std::endl;
}

void rvalueReferenceDemo() {
    std::cout << "========== 右值引用演示 ==========" << std::endl;
    
    // 1. 左值和右值
    lvalueRvalueDemo();
    
    // 2. std::move
    moveDemo();
    
    // 3. 移动语义
    moveSemanticsDemo();
    
    // 4. 使用建议
    std::cout << "\n--- 右值引用使用建议 ---" << std::endl;
    std::cout << "  1. 优先让string、vector、unique_ptr等成员自动管理资源（Rule of Zero）" << std::endl;
    std::cout << "  2. 只有直接拥有裸资源时，才系统设计复制、移动与析构契约" << std::endl;
    std::cout << "  3. std::move只表达“允许按右值处理”，最终可能移动，也可能拷贝" << std::endl;
    std::cout << "  4. 标准库对象移动后通常有效但状态未指定；只析构、赋值或调用有前置条件保证的操作" << std::endl;
    std::cout << "  5. 确实不抛异常的移动操作应标记为noexcept" << std::endl;
    
    std::cout << "\n========== 右值引用演示结束 ==========" << std::endl;
}
