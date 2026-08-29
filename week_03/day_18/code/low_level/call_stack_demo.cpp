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
    
    // ========== 1. 先理解函数契约 ==========
    std::cout << "\n--- 1. 函数契约与调用链 ---" << std::endl;
    std::cout << "  调用者负责满足前置条件；被调函数承诺返回值和副作用。" << std::endl;
    std::cout << "  调用栈保存的是尚未完成的调用现场，不决定函数的业务含义。" << std::endl;
    std::cout << "  main() 调用 funcA()" << std::endl;
    funcA();
    std::cout << "  main() 继续" << std::endl;
    
    std::cout << "\n  未内联时的抽象调用栈变化：" << std::endl;
    std::cout << "    1. main() 调用上下文存在" << std::endl;
    std::cout << "    2. 进入 funcA()" << std::endl;
    std::cout << "    3. 进入 funcB()" << std::endl;
    std::cout << "    4. 进入 funcC()" << std::endl;
    std::cout << "    5. funcC() 返回" << std::endl;
    std::cout << "    6. funcB() 返回" << std::endl;
    std::cout << "    7. funcA() 返回" << std::endl;
    std::cout << "    8. main() 继续执行" << std::endl;
    
    // ========== 2. 递归演示 ==========
    std::cout << "\n--- 2. 递归含义与调用栈 ---" << std::endl;
    std::cout << "  factorial(n)的含义是返回n!；递归步把问题缩小为factorial(n-1)，" << std::endl;
    std::cout << "  基准情况终止递归，栈帧只是保存每层尚未完成的乘法。" << std::endl;
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
    
    // ========== 3. 调用上下文 ==========
    std::cout << "\n--- 3. 调用上下文（实现相关）---" << std::endl;
    std::cout << "  为恢复未完成调用，概念上可能需要保存：" << std::endl;
    std::cout << "    - 返回位置和部分调用者寄存器" << std::endl;
    std::cout << "    - 必须跨调用保留的参数或局部状态" << std::endl;
    std::cout << "    - 临时计算状态" << std::endl;
    std::cout << "  参数可能走寄存器，局部变量也可能被优化掉；" << std::endl;
    std::cout << "  C++不保证固定布局，也不保证一定存在帧指针。" << std::endl;
    
    // ========== 4. 栈溢出警告 ==========
    std::cout << "\n--- 4. 栈溢出警告 ---" << std::endl;
    std::cout << "  栈空间有限，常见溢出原因：" << std::endl;
    std::cout << "    1. 无限递归（无基准情况）" << std::endl;
    std::cout << "    2. 递归层数太深" << std::endl;
    std::cout << "    3. 局部变量太大" << std::endl;
    std::cout << "\n  解决方法：" << std::endl;
    std::cout << "    1. 确保递归有正确的基准情况" << std::endl;
    std::cout << "    2. 优先改为迭代；C++不保证尾递归一定被优化" << std::endl;
    std::cout << "    3. 大数组使用堆内存" << std::endl;
}
