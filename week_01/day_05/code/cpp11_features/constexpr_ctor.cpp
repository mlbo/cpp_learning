/**
 * @file constexpr_ctor.cpp
 * @brief constexpr构造函数详解
 * 
 * constexpr构造函数使类成为"字面量类型"，
 * 允许在编译期创建和操作对象。
 */

#include <iostream>
#include <array>
#include <cmath>

using namespace std;

// ==================== 基本constexpr类 ====================

class Point {
private:
    int x_, y_;
    
public:
    // constexpr构造函数
    constexpr Point() : x_(0), y_(0) {}
    constexpr Point(int x, int y) : x_(x), y_(y) {}
    
    // constexpr成员函数
    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }
    
    constexpr void setX(int x) { x_ = x; }  // C++14起允许
    constexpr void setY(int y) { y_ = y; }
    
    // constexpr计算
    constexpr int distance_squared() const {
        return x_ * x_ + y_ * y_;
    }
    
    constexpr double distance() const {
        // 注意：sqrt在C++26前不是constexpr
        // 这里简化处理
        return x_ * x_ + y_ * y_;
    }
    
    // constexpr运算符重载
    constexpr Point operator+(const Point& other) const {
        return Point(x_ + other.x_, y_ + other.y_);
    }
    
    constexpr Point operator-(const Point& other) const {
        return Point(x_ - other.x_, y_ - other.y_);
    }
    
    constexpr bool operator==(const Point& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
};

// ==================== constexpr复数类 ====================

class Complex {
private:
    double real_, imag_;
    
public:
    constexpr Complex() : real_(0), imag_(0) {}
    constexpr Complex(double real, double imag) : real_(real), imag_(imag) {}
    
    constexpr double real() const { return real_; }
    constexpr double imag() const { return imag_; }
    
    constexpr Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }
    
    constexpr Complex operator*(const Complex& other) const {
        return Complex(
            real_ * other.real_ - imag_ * other.imag_,
            real_ * other.imag_ + imag_ * other.real_
        );
    }
    
    constexpr double magnitude_squared() const {
        return real_ * real_ + imag_ * imag_;
    }
    
    constexpr Complex conjugate() const {
        return Complex(real_, -imag_);
    }
};

// ==================== constexpr数组包装类 ====================

template <typename T, size_t N>
class ConstexprArray {
private:
    T data_[N];
    
public:
    // constexpr构造函数（初始化列表）
    constexpr ConstexprArray() : data_{} {}
    
    constexpr ConstexprArray(std::initializer_list<T> init) : data_{} {
        size_t i = 0;
        for (const auto& val : init) {
            if (i < N) data_[i++] = val;
        }
    }
    
    constexpr size_t size() const { return N; }
    
    constexpr T& operator[](size_t index) { return data_[index]; }
    constexpr const T& operator[](size_t index) const { return data_[index]; }
    
    constexpr const T* begin() const { return data_; }
    constexpr const T* end() const { return data_ + N; }
    
    constexpr T sum() const {
        T total = T{};
        for (size_t i = 0; i < N; ++i) {
            total += data_[i];
        }
        return total;
    }
    
    constexpr T max() const {
        T m = data_[0];
        for (size_t i = 1; i < N; ++i) {
            if (data_[i] > m) m = data_[i];
        }
        return m;
    }
};

// ==================== constexpr字符串类 ====================

class ConstexprString {
private:
    const char* data_;
    size_t size_;
    
public:
    template <size_t N>
    constexpr ConstexprString(const char (&str)[N]) 
        : data_(str), size_(N - 1) {}
    
    constexpr const char* data() const { return data_; }
    constexpr size_t size() const { return size_; }
    
    constexpr char operator[](size_t index) const {
        return index < size_ ? data_[index] : '\0';
    }
    
    constexpr bool operator==(const ConstexprString& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) return false;
        }
        return true;
    }
    
    constexpr bool starts_with(const ConstexprString& prefix) const {
        if (prefix.size_ > size_) return false;
        for (size_t i = 0; i < prefix.size_; ++i) {
            if (data_[i] != prefix.data_[i]) return false;
        }
        return true;
    }
};

// ==================== 演示函数 ====================

void demonstrate_point() {
    cout << "【constexpr Point类】\n\n";
    
    // 编译期创建Point对象
    constexpr Point origin;
    constexpr Point p1(3, 4);
    constexpr Point p2(1, 2);
    
    // 编译期计算
    constexpr int dist_sq = p1.distance_squared();
    constexpr Point sum = p1 + p2;
    constexpr bool eq = origin == Point(0, 0);
    
    cout << "编译期创建:\n";
    cout << "  constexpr Point origin;           // (" << origin.x() << ", " << origin.y() << ")\n";
    cout << "  constexpr Point p1(3, 4);         // (" << p1.x() << ", " << p1.y() << ")\n";
    cout << "  constexpr Point p2(1, 2);         // (" << p2.x() << ", " << p2.y() << ")\n\n";
    
    cout << "编译期计算:\n";
    cout << "  p1.distance_squared() = " << dist_sq << "\n";
    cout << "  p1 + p2 = (" << sum.x() << ", " << sum.y() << ")\n";
    cout << "  origin == Point(0,0) = " << (eq ? "true" : "false") << "\n";
    
    // 用作模板参数
    cout << "\n用作编译期常量:\n";
    array<Point, 3> points = {origin, p1, p2};
    cout << "  array<Point, 3> points = {origin, p1, p2};\n";
    cout << "  points[1].distance_squared() = " << points[1].distance_squared() << "\n";
}

void demonstrate_complex() {
    cout << "\n【constexpr Complex类】\n\n";
    
    // 编译期复数运算
    constexpr Complex c1(3, 4);
    constexpr Complex c2(1, 2);
    constexpr Complex sum = c1 + c2;
    constexpr Complex product = c1 * c2;
    constexpr Complex conj = c1.conjugate();
    
    cout << "编译期复数运算:\n";
    cout << "  c1 = " << c1.real() << " + " << c1.imag() << "i\n";
    cout << "  c2 = " << c2.real() << " + " << c2.imag() << "i\n";
    cout << "  c1 + c2 = " << sum.real() << " + " << sum.imag() << "i\n";
    cout << "  c1 * c2 = " << product.real() << " + " << product.imag() << "i\n";
    cout << "  c1.conjugate() = " << conj.real() << " + " << conj.imag() << "i\n";
    cout << "  c1.magnitude_squared() = " << c1.magnitude_squared() << "\n";
}

void demonstrate_array() {
    cout << "\n【constexpr ConstexprArray类】\n\n";
    
    // 编译期数组
    constexpr ConstexprArray<int, 5> arr{1, 2, 3, 4, 5};
    constexpr int total = arr.sum();
    constexpr int max_val = arr.max();
    
    cout << "编译期数组:\n";
    cout << "  constexpr ConstexprArray<int, 5> arr{1, 2, 3, 4, 5};\n";
    cout << "  arr[0] = " << arr[0] << "\n";
    cout << "  arr.size() = " << arr.size() << "\n";
    cout << "  arr.sum() = " << total << "\n";
    cout << "  arr.max() = " << max_val << "\n";
}

void demonstrate_string() {
    cout << "\n【constexpr ConstexprString类】\n\n";
    
    // 编译期字符串
    constexpr ConstexprString str1 = "Hello";
    constexpr ConstexprString str2 = "Hello";
    constexpr ConstexprString str3 = "Hello, World!";
    
    cout << "编译期字符串:\n";
    cout << "  constexpr ConstexprString str1 = \"Hello\";\n";
    cout << "  str1.size() = " << str1.size() << "\n";
    cout << "  str1[0] = '" << str1[0] << "'\n";
    cout << "  str1 == str2 = " << (str1 == str2 ? "true" : "false") << "\n";
    cout << "  str3.starts_with(str1) = " << (str3.starts_with(str1) ? "true" : "false") << "\n";
}

// ==================== 字面量类型要求 ====================

void explain_literal_type() {
    cout << "\n【字面量类型要求】\n\n";
    cout << "一个类要成为字面量类型（可用于constexpr），需要满足:\n";
    cout << "  1. 有constexpr构造函数\n";
    cout << "  2. 析构函数是平凡的（或constexpr，C++20起）\n";
    cout << "  3. 所有非静态数据成员都是字面量类型\n";
    cout << "  4. 如果有联合体，只能有一个成员有初始化\n";
    cout << "  5. 所有成员函数要么是constexpr，要么是虚函数（C++20起）\n";
}

// ==================== main函数 ====================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              constexpr构造函数详解                          ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    demonstrate_point();
    demonstrate_complex();
    demonstrate_array();
    demonstrate_string();
    explain_literal_type();
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     constexpr构造函数要点：                                 ║\n";
    cout << "║     1. 使类成为字面量类型                                   ║\n";
    cout << "║     2. 允许编译期创建对象                                   ║\n";
    cout << "║     3. 成员函数可以是constexpr                              ║\n";
    cout << "║     4. 支持运算符重载                                       ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}
