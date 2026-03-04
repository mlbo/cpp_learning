/**
 * EMC++ Item 11-16 演示
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>

// Item 11: 优先使用 = delete
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

// Item 12: 使用 override
class Base {
public:
    virtual void foo() { std::cout << "Base::foo" << std::endl; }
};

class Derived : public Base {
public:
    void foo() override { std::cout << "Derived::foo" << std::endl; }
};

// Item 14: noexcept
void safeSwap(int& a, int& b) noexcept {
    int temp = a;
    a = b;
    b = temp;
}

// Item 15: constexpr
constexpr int square(int x) { return x * x; }

// Item 16: const成员函数线程安全
class ThreadSafeValue {
public:
    int getValue() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return value_;
    }
    
    void setValue(int v) {
        std::lock_guard<std::mutex> lock(mtx_);
        value_ = v;
    }

private:
    mutable std::mutex mtx_;
    int value_ = 0;
};

void emcppDemo() {
    std::cout << "=== EMC++ Item 11-16 演示 ===" << std::endl;
    
    // Item 11
    std::cout << "\nItem 11: NonCopyable 使用 = delete" << std::endl;
    NonCopyable n1;
    // NonCopyable n2 = n1;  // 编译错误
    
    // Item 12
    std::cout << "\nItem 12: override 关键字" << std::endl;
    Base* b = new Derived();
    b->foo();
    delete b;
    
    // Item 13
    std::cout << "\nItem 13: const_iterator" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto it = std::find(v.cbegin(), v.cend(), 3);
    if (it != v.cend()) std::cout << "找到元素: " << *it << std::endl;
    
    // Item 14
    std::cout << "\nItem 14: noexcept 函数" << std::endl;
    int x = 1, y = 2;
    safeSwap(x, y);
    std::cout << "交换后: x=" << x << ", y=" << y << std::endl;
    
    // Item 15
    std::cout << "\nItem 15: constexpr" << std::endl;
    constexpr int result = square(5);
    std::cout << "square(5) = " << result << " (编译期计算)" << std::endl;
    
    // Item 16
    std::cout << "\nItem 16: const成员函数线程安全" << std::endl;
    ThreadSafeValue tsv;
    tsv.setValue(42);
    std::cout << "getValue(): " << tsv.getValue() << std::endl;
}

int main() {
    emcppDemo();
    return 0;
}
