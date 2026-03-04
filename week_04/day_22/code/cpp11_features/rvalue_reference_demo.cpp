/**
 * Day 22: 右值引用演示
 * 实现文件
 */

#include "rvalue_reference_demo.h"
#include <cstring>
#include <vector>
#include <utility>

// ==================== MyString 实现 ====================

MyString::MyString(const char* str) {
    size_ = strlen(str);
    data_ = new char[size_ + 1];
    strcpy(data_, str);
    std::cout << "  [构造] " << data_ << std::endl;
}

MyString::~MyString() {
    if (data_) {
        std::cout << "  [析构] " << data_ << std::endl;
        delete[] data_;
    }
}

MyString::MyString(const MyString& other) {
    size_ = other.size_;
    data_ = new char[size_ + 1];
    strcpy(data_, other.data_);
    std::cout << "  [拷贝构造] " << data_ << std::endl;
}

MyString::MyString(MyString&& other) noexcept {
    // 直接"窃取"资源
    data_ = other.data_;
    size_ = other.size_;
    
    // 置空源对象，防止重复释放
    other.data_ = nullptr;
    other.size_ = 0;
    
    std::cout << "  [移动构造] " << data_ << std::endl;
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "  [拷贝赋值] " << data_ << std::endl;
    }
    return *this;
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
        
        std::cout << "  [移动赋值] " << data_ << std::endl;
    }
    return *this;
}

// ==================== 演示函数 ====================

void lvalueRvalueDemo() {
    std::cout << "\n--- 左值与右值 ---" << std::endl;
    
    int x = 10;  // x是左值，10是右值
    
    // 左值引用
    int& lr = x;
    std::cout << "左值引用 int& lr = x; -> " << lr << std::endl;
    
    // 右值引用
    int&& rr1 = 10;
    std::cout << "右值引用 int&& rr1 = 10; -> " << rr1 << std::endl;
    
    int&& rr2 = x + 5;  // x+5是临时对象（右值）
    std::cout << "右值引用 int&& rr2 = x + 5; -> " << rr2 << std::endl;
    
    // const左值引用可以绑定右值
    const int& clr = 20;
    std::cout << "const左值引用 const int& clr = 20; -> " << clr << std::endl;
    
    std::cout << "\n规则总结：" << std::endl;
    std::cout << "  - 左值引用(&) 只能绑定左值" << std::endl;
    std::cout << "  - 右值引用(&&) 只能绑定右值" << std::endl;
    std::cout << "  - const左值引用(const&) 可以绑定任意值" << std::endl;
    std::cout << "  - std::move可以将左值转换为右值" << std::endl;
}

void moveDemo() {
    std::cout << "\n--- std::move 演示 ---" << std::endl;
    
    std::string str1 = "Hello";
    std::cout << "原始字符串 str1 = \"" << str1 << "\"" << std::endl;
    
    // std::move 将左值转换为右值
    std::string str2 = std::move(str1);
    std::cout << "使用 std::move 后:" << std::endl;
    std::cout << "  str1 = \"" << str1 << "\" (已为空)" << std::endl;
    std::cout << "  str2 = \"" << str2 << "\"" << std::endl;
    
    // vector的移动
    std::cout << "\n--- vector 移动演示 ---" << std::endl;
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "v1 大小: " << v1.size() << std::endl;
    
    std::vector<int> v2 = std::move(v1);
    std::cout << "std::move 后:" << std::endl;
    std::cout << "  v1 大小: " << v1.size() << std::endl;
    std::cout << "  v2 大小: " << v2.size() << std::endl;
    
    std::cout << "  v2 元素: ";
    for (int n : v2) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void moveSemanticsDemo() {
    std::cout << "\n--- 移动语义演示 ---" << std::endl;
    
    std::cout << "创建 s1:" << std::endl;
    MyString s1("Hello");
    
    std::cout << "\n拷贝构造 s2 = s1:" << std::endl;
    MyString s2 = s1;
    std::cout << "  s1.c_str() = \"" << s1.c_str() << "\"" << std::endl;
    std::cout << "  s2.c_str() = \"" << s2.c_str() << "\"" << std::endl;
    
    std::cout << "\n移动构造 s3 = std::move(s2):" << std::endl;
    MyString s3 = std::move(s2);
    std::cout << "  s2.c_str() = \"" << (s2.c_str() ? s2.c_str() : "(null)") << "\"" << std::endl;
    std::cout << "  s3.c_str() = \"" << s3.c_str() << "\"" << std::endl;
    
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
    std::cout << "  1. 资源管理类应实现移动构造和移动赋值" << std::endl;
    std::cout << "  2. 使用std::move显式触发移动语义" << std::endl;
    std::cout << "  3. 移动后的对象处于有效但未定义状态" << std::endl;
    std::cout << "  4. 移动操作应标记为noexcept" << std::endl;
    
    std::cout << "\n========== 右值引用演示结束 ==========" << std::endl;
}
