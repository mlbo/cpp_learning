/**
 * EMC++ Item 34: 优先使用Lambda而非std::bind
 * 
 * Lambda的优势：
 * 1. 更好的可读性
 * 2. 更好的性能
 * 3. 更强的表达能力
 * 4. 支持泛型（C++14）
 */

#include "item34_lambda_vs_bind.h"
#include <iostream>
#include <functional>
#include <chrono>
#include <string>
#include <memory>

using namespace std::placeholders;

// 示例函数
void setSound(std::chrono::seconds duration, const std::string& sound, int volume) {
    std::cout << "  设置声音: " << sound << ", 音量: " << volume 
              << ", 持续: " << duration.count() << "秒" << std::endl;
}

// 用于演示移动语义
class Widget {
public:
    std::string name;
    Widget(const std::string& n) : name(n) {}
    void doSomething(int value) {
        std::cout << "  Widget " << name << " 处理: " << value << std::endl;
    }
};

void item34Demo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║  Item 34: 优先Lambda而非std::bind    ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 可读性对比 ==========
    std::cout << "\n--- 1. 可读性对比 ---" << std::endl;
    
    // std::bind版本
    auto bindSetSound = std::bind(setSound, std::chrono::seconds(1), _1, 60);
    
    // Lambda版本
    auto lambdaSetSound = [](const std::string& sound) {
        setSound(std::chrono::seconds(1), sound, 60);
    };
    
    std::cout << "  std::bind调用:" << std::endl;
    bindSetSound("Beep");
    
    std::cout << "  Lambda调用:" << std::endl;
    lambdaSetSound("Beep");
    
    std::cout << "\n  Lambda更清晰：一眼就知道参数是什么" << std::endl;
    
    // ========== 2. 参数传递方式 ==========
    std::cout << "\n--- 2. 参数传递方式 ---" << std::endl;
    
    std::string sound = "Alert";
    
    // std::bind: 按值捕获（拷贝）
    auto bindCopy = std::bind(setSound, std::chrono::seconds(1), sound, 50);
    sound = "Changed";
    std::cout << "  bind拷贝后修改原变量:" << std::endl;
    bindCopy(sound);  // 使用的是拷贝的"Alert"
    
    // Lambda: 可以选择值捕获或引用捕获
    auto lambdaRef = [&sound]() {
        setSound(std::chrono::seconds(1), sound, 50);
    };
    sound = "New Alert";
    std::cout << "  Lambda引用捕获:" << std::endl;
    lambdaRef();  // 使用的是"New Alert"
    
    // ========== 3. 移动语义 ==========
    std::cout << "\n--- 3. 移动语义支持 ---" << std::endl;
    
    auto ptr = std::make_unique<Widget>("MyWidget");
    
    // Lambda: 支持移动捕获（C++14）
    auto lambdaMove = [w = std::move(ptr)](int value) {
        w->doSomething(value);
    };
    std::cout << "  Lambda移动捕获:" << std::endl;
    lambdaMove(42);
    
    // std::bind: C++11也可以通过bind实现类似效果
    auto bindMove = std::bind(
        [](std::unique_ptr<Widget>& w, int value) {
            w->doSomething(value);
        },
        std::move(ptr),
        _1
    );
    // 但ptr已经被移动，bindMove无法使用
    std::cout << "  std::bind移动后原指针为空" << std::endl;
    
    // ========== 4. 泛型支持（C++14） ==========
    std::cout << "\n--- 4. 泛型支持 ---" << std::endl;
    
    // Lambda: C++14支持泛型
    auto genericLambda = [](auto a, auto b) {
        return a + b;
    };
    
    std::cout << "  泛型Lambda:" << std::endl;
    std::cout << "    int: " << genericLambda(1, 2) << std::endl;
    std::cout << "    double: " << genericLambda(1.5, 2.5) << std::endl;
    std::cout << "    string: " << genericLambda(std::string("Hello"), std::string(" World")) << std::endl;
    
    // std::bind: 不支持泛型
    std::cout << "  std::bind不支持泛型Lambda" << std::endl;
    
    // ========== 5. 总结 ==========
    std::cout << "\n--- 5. 总结 ---" << std::endl;
    std::cout << "  Lambda优势：" << std::endl;
    std::cout << "    1. 可读性好：代码意图清晰" << std::endl;
    std::cout << "    2. 性能更好：可能内联，无间接调用" << std::endl;
    std::cout << "    3. 表达力强：支持移动捕获、泛型" << std::endl;
    std::cout << "    4. 灵活性高：可选择捕获方式" << std::endl;
    std::cout << "\n  std::bind使用场景：" << std::endl;
    std::cout << "    1. C++11中需要移动捕获时" << std::endl;
    std::cout << "    2. 需要参数重排时（但Lambda也可以做到）" << std::endl;
    std::cout << "    3. 与旧代码兼容时" << std::endl;
}
