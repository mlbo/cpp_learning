/**
 * EMC++ Item 31: 避免默认捕获模式
 */

#include "item31_avoid_default_capture.h"
#include <iostream>
#include <functional>
#include <memory>

namespace {

std::function<void()> makeSafeCallback() {
    int local = 42;
    return [local]() {
        std::cout << "  逃逸回调保存的值: " << local << std::endl;
    };
}

class Counter {
public:
    explicit Counter(int value) : value_(value) {}

    std::function<void()> makeSnapshotCallback() const {
        // C++14 初始化捕获：复制成员值，而不是隐式捕获 this。
        return [value = value_]() {
            std::cout << "  对象成员快照: " << value << std::endl;
        };
    }

private:
    int value_;
};

}  // namespace

void item31Demo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║  EMC++ Item 31: 避免默认捕获模式     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    std::cout << "\n--- 1. 捕获发生在闭包创建时 ---" << std::endl;
    int value = 10;
    auto snapshot = [value]() { return value; };
    value = 20;
    std::cout << "  原变量现在是 " << value
              << "，值捕获快照仍是 " << snapshot() << std::endl;

    std::cout << "\n--- 2. 引用捕获只在原对象仍存活时安全 ---" << std::endl;
    int total = 5;
    auto addWithinScope = [&total](int delta) {
        total += delta;
    };
    addWithinScope(3);
    std::cout << "  同一作用域内修改 total: " << total << std::endl;
    std::cout << "  若把该闭包返回并在 total 销毁后调用，就会产生悬空引用。" << std::endl;

    std::cout << "\n--- 3. 逃逸回调显式拥有所需状态 ---" << std::endl;
    auto safeCallback = makeSafeCallback();
    safeCallback();

    std::cout << "\n--- 4. 成员变量应避免隐藏的 this 依赖 ---" << std::endl;
    std::function<void()> memberCallback;
    {
        Counter counter(99);
        memberCallback = counter.makeSnapshotCallback();
    }
    memberCallback();

    std::cout << "\n--- 总结 ---" << std::endl;
    std::cout << "  1. 优先显式写出捕获，便于审查依赖" << std::endl;
    std::cout << "  2. 值捕获是创建时快照，不是调用时读取" << std::endl;
    std::cout << "  3. 引用捕获要求被引用对象覆盖闭包的全部调用期" << std::endl;
    std::cout << "  4. 长期回调应按需要捕获值或拥有资源的智能指针" << std::endl;
}
