/**
 * EMC++ Item 31: 避免默认捕获模式
 */

#include "item31_avoid_default_capture.h"
#include <iostream>
#include <functional>
#include <memory>

void item31Demo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║  EMC++ Item 31: 避免默认捕获模式     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 最佳实践 ==========
    std::cout << "\n--- 最佳实践：显式捕获 ---" << std::endl;
    
    int a = 10, b = 20;
    
    // ✅ 推荐：显式列出捕获的变量
    auto good = [a, &b]() {
        b = a + b;
    };
    good();
    std::cout << "  a + b = " << b << std::endl;
    
    // ========== 总结 ==========
    std::cout << "\n--- 总结 ---" << std::endl;
    std::cout << "  1. ❌ 避免使用 [=] 或 [&] 默认捕获" << std::endl;
    std::cout << "  2. ✅ 显式列出要捕获的变量" << std::endl;
    std::cout << "  3. ⚠️ 引用捕获要注意生命周期" << std::endl;
    std::cout << "  4. ✅ 使用智能指针更安全" << std::endl;
}
