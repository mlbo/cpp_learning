/**
 * std::bind演示
 * 绑定函数和参数
 */

#include "bind_demo.h"
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std::placeholders;  // _1, _2, ...

// 普通函数
int add(int a, int b, int c) {
    return a + b + c;
}

// 用于演示重载
void print(int x) {
    std::cout << x << " ";
}

void print(int x, int y) {
    std::cout << "(" << x << ", " << y << ") ";
}

// 成员函数演示
struct Person {
    std::string name;
    int age;
    
    void introduce(const std::string& greeting) const {
        std::cout << greeting << ", I'm " << name << ", " << age << " years old." << std::endl;
    }
    
    void setAge(int newAge) { age = newAge; }
};

void bindDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        std::bind 参数绑定            ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 基本用法：绑定部分参数 ==========
    std::cout << "\n--- 1. 绑定部分参数 ---" << std::endl;
    
    // 绑定第一个参数为10
    auto add10 = std::bind(add, 10, _1, _2);
    std::cout << "  add(10, a, b): add(10, 3, 4) = " << add10(3, 4) << std::endl;
    
    // 绑定前两个参数
    auto add10and20 = std::bind(add, 10, 20, _1);
    std::cout << "  add(10, 20, a): add(10, 20, 5) = " << add10and20(5) << std::endl;
    
    // 绑定所有参数
    auto fixed = std::bind(add, 1, 2, 3);
    std::cout << "  add(1, 2, 3) = " << fixed() << std::endl;
    
    // ========== 2. 参数重排 ==========
    std::cout << "\n--- 2. 参数重排 ---" << std::endl;
    
    // 交换第一个和第二个参数
    auto subtract = [](int a, int b) { return a - b; };
    auto subtractReversed = std::bind(subtract, _2, _1);
    
    std::cout << "  原始: 10 - 3 = " << subtract(10, 3) << std::endl;
    std::cout << "  重排: 3 - 10 = " << subtractReversed(10, 3) << std::endl;
    
    // ========== 3. 绑定成员函数 ==========
    std::cout << "\n--- 3. 绑定成员函数 ---" << std::endl;
    
    Person alice{"Alice", 25};
    
    // 绑定成员函数（需要对象指针或引用）
    auto introduce = std::bind(&Person::introduce, &alice, _1);
    introduce("Hello");
    
    // 绑定所有参数
    auto sayHi = std::bind(&Person::introduce, &alice, "Hi");
    sayHi();
    
    // 修改成员变量
    auto setAliceAge = std::bind(&Person::setAge, &alice, _1);
    setAliceAge(26);
    std::cout << "  Alice's new age: " << alice.age << std::endl;
    
    // ========== 4. 与STL算法配合 ==========
    std::cout << "\n--- 4. 与STL算法配合 ---" << std::endl;
    
    std::vector<int> nums = {5, 2, 8, 1, 9};
    
    // 使用bind创建谓词
    auto greaterThan5 = std::bind(std::greater<int>(), _1, 5);
    
    auto it = std::find_if(nums.begin(), nums.end(), greaterThan5);
    if (it != nums.end()) {
        std::cout << "  第一个大于5的数: " << *it << std::endl;
    }
    
    // 统计大于5的元素个数
    int count = std::count_if(nums.begin(), nums.end(), greaterThan5);
    std::cout << "  大于5的元素个数: " << count << std::endl;
    
    // ========== 5. bind vs Lambda对比 ==========
    std::cout << "\n--- 5. bind vs Lambda对比 ---" << std::endl;
    
    // 使用bind
    auto bindVersion = std::bind(add, 100, _1, _2);
    
    // 使用Lambda
    auto lambdaVersion = [](int a, int b) { return add(100, a, b); };
    
    std::cout << "  bind版本: " << bindVersion(10, 20) << std::endl;
    std::cout << "  Lambda版本: " << lambdaVersion(10, 20) << std::endl;
    
    std::cout << "\n  对比总结：" << std::endl;
    std::cout << "    bind: 可读性较差，但可以重排参数" << std::endl;
    std::cout << "    Lambda: 可读性好，更现代，推荐使用" << std::endl;
    
    // ========== 6. 注意事项 ==========
    std::cout << "\n--- 6. 注意事项 ---" << std::endl;
    std::cout << "  1. 绑定引用需要使用std::ref或std::cref" << std::endl;
    std::cout << "  2. 绑定成员函数需要对象指针或引用" << std::endl;
    std::cout << "  3. 对于重载函数，需要显式指定函数类型" << std::endl;
    
    int x = 10;
    auto printRef = std::bind(static_cast<void(*)(int)>(print), std::ref(x));
    x = 20;
    std::cout << "  使用std::ref绑定引用: ";
    printRef();
    std::cout << std::endl;
}
