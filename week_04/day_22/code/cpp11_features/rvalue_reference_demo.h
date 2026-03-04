/**
 * Day 22: 右值引用演示
 * 头文件
 */

#ifndef RVALUE_REFERENCE_DEMO_H
#define RVALUE_REFERENCE_DEMO_H

#include <iostream>
#include <string>
#include <utility>

/**
 * 右值引用演示函数
 */
void rvalueReferenceDemo();

/**
 * 演示左值和右值的区别
 */
void lvalueRvalueDemo();

/**
 * 演示std::move的使用
 */
void moveDemo();

/**
 * 简单字符串类，展示移动语义
 */
class MyString {
public:
    // 默认构造函数
    MyString(const char* str = "");
    
    // 析构函数
    ~MyString();
    
    // 拷贝构造函数
    MyString(const MyString& other);
    
    // 移动构造函数
    MyString(MyString&& other) noexcept;
    
    // 拷贝赋值运算符
    MyString& operator=(const MyString& other);
    
    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept;
    
    // 获取字符串
    const char* c_str() const { return data_; }
    
    // 获取大小
    size_t size() const { return size_; }

private:
    char* data_;
    size_t size_;
};

/**
 * 演示移动语义
 */
void moveSemanticsDemo();

#endif // RVALUE_REFERENCE_DEMO_H
