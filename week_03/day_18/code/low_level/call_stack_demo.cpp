/**
 * 函数调用栈演示
 * 展示函数调用、栈帧、递归的工作原理
 */

#include "call_stack_demo.h"
#include <iostream>

// 递归计算阶乘
int factorial(int n) {
    std::cout << "  factorial(" << n << ") 开始" << std::endl;
    
    if (n <= 1) {
        std::cout << "  factorial(1) = 1 [基准情况，开始返回]" << std::endl;
        return 1;
    }
    
    int result = n * factorial(n - 1);
    std::cout << "  factorial(" << n << ") = " << n << " * factorial(" 
              << (n-1) << ") = " << result << std::endl;
    
    return result;
}

// 演示函数调用链
void funcC() {
    std::cout << "  funcC() 执行 - 栈最深" << std::endl;
    std::cout << "  funcC() 返回" << std::endl;
}

void funcB() {
    std::cout << "  funcB() 调用 funcC()" << std::endl;
    funcC();
    std::cout << "  funcB() 返回" << std::endl;
}

void funcA() {
    std::cout << "  funcA() 调用 funcB()" << std::endl;
    funcB();
    std::cout << "  funcA() 返回" << std::endl;
}

// 演示栈溢出（危险！）
// void infiniteRecursion() {
//     infiniteRecursion();  // 无限递归
// }

void callStackDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        函数调用栈 Call Stack         ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 函数调用链演示 ==========
    std::cout << "\n--- 1. 函数调用链 ---" << std::endl;
    std::cout << "  main() 调用 funcA()" << std::endl;
    funcA();
    std::cout << "  main() 继续" << std::endl;
    
    std::cout << "\n  调用栈变化：" << std::endl;
    std::cout << "    1. main() 入栈" << std::endl;
    std::cout << "    2. funcA() 入栈" << std::endl;
    std::cout << "    3. funcB() 入栈" << std::endl;
    std::cout << "    4. funcC() 入栈" << std::endl;
    std::cout << "    5. funcC() 出栈" << std::endl;
    std::cout << "    6. funcB() 出栈" << std::endl;
    std::cout << "    7. funcA() 出栈" << std::endl;
    std::cout << "    8. main() 继续执行" << std::endl;
    
    // ========== 2. 递归演示 ==========
    std::cout << "\n--- 2. 递归调用 ---" << std::endl;
    int result = factorial(5);
    std::cout << "  5! = " << result << std::endl;
    
    std::cout << "\n  递归栈帧：" << std::endl;
    std::cout << "    factorial(5) 等待 factorial(4)" << std::endl;
    std::cout << "    factorial(4) 等待 factorial(3)" << std::endl;
    std::cout << "    factorial(3) 等待 factorial(2)" << std::endl;
    std::cout << "    factorial(2) 等待 factorial(1)" << std::endl;
    std::cout << "    factorial(1) 返回 1" << std::endl;
    std::cout << "    factorial(2) 返回 2" << std::endl;
    std::cout << "    factorial(3) 返回 6" << std::endl;
    std::cout << "    factorial(4) 返回 24" << std::endl;
    std::cout << "    factorial(5) 返回 120" << std::endl;
    
    // ========== 3. 栈帧结构 ==========
    std::cout << "\n--- 3. 栈帧结构 ---" << std::endl;
    std::cout << "  每个栈帧包含：" << std::endl;
    std::cout << "    - 函数参数" << std::endl;
    std::cout << "    - 返回地址" << std::endl;
    std::cout << "    - 上一个栈帧指针" << std::endl;
    std::cout << "    - 局部变量" << std::endl;
    std::cout << "    - 临时变量" << std::endl;
    
    // ========== 4. 栈溢出警告 ==========
    std::cout << "\n--- 4. 栈溢出警告 ---" << std::endl;
    std::cout << "  栈空间有限，常见溢出原因：" << std::endl;
    std::cout << "    1. 无限递归（无基准情况）" << std::endl;
    std::cout << "    2. 递归层数太深" << std::endl;
    std::cout << "    3. 局部变量太大" << std::endl;
    std::cout << "\n  解决方法：" << std::endl;
    std::cout << "    1. 确保递归有正确的基准情况" << std::endl;
    std::cout << "    2. 使用尾递归优化或迭代" << std::endl;
    std::cout << "    3. 大数组使用堆内存" << std::endl;
}
