/**
 * @file stack_frame.cpp
 * @brief 栈帧分析 - 深入理解栈帧的内存布局
 * 
 * 本文件演示：
 * 1. 栈帧的内存布局
 * 2. 参数传递机制
 * 3. 局部变量的存储
 * 4. 返回地址的作用
 * 
 * 注意：本文件使用一些底层技术来展示栈帧结构
 * 这些代码仅供学习理解，不建议在生产代码中使用
 */

#include <iostream>
#include <iomanip>
#include <cstdint>

// ============================================================================
// 栈帧结构演示
// ============================================================================

/**
 * @brief 打印内存内容（十六进制）
 * @param ptr 起始地址
 * @param size 要打印的字节数
 * @param label 标签
 */
void print_memory(void* ptr, size_t size, const char* label) {
    std::cout << "  " << label << " (地址: " << ptr << "):\n";
    
    auto bytes = static_cast<unsigned char*>(ptr);
    
    // 打印十六进制内容
    std::cout << "    十六进制: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::dec << "\n";
}

/**
 * @brief 分析函数栈帧的示例
 * @param a 第一个参数
 * @param b 第二个参数
 * @param c 第三个参数
 * @return 参数之和
 * 
 * 这个函数展示了：
 * 1. 参数在栈帧中的位置
 * 2. 局部变量在栈帧中的位置
 * 3. 栈帧的边界
 */
int analyzeStackFrame(int a, int b, int c) {
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║  函数 analyzeStackFrame 的栈帧分析                        ║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════════╝\n\n";
    
    // 局部变量
    int local1 = 100;
    int local2 = 200;
    int local3 = 300;
    
    // 打印参数地址
    std::cout << "  【参数在栈帧中的位置】\n";
    std::cout << "  ┌─────────────────────────────────────────────┐\n";
    std::cout << "  │ 参数 a = " << std::setw(5) << a 
              << "  地址: " << (void*)&a << " │\n";
    std::cout << "  │ 参数 b = " << std::setw(5) << b 
              << "  地址: " << (void*)&b << " │\n";
    std::cout << "  │ 参数 c = " << std::setw(5) << c 
              << "  地址: " << (void*)&c << " │\n";
    std::cout << "  └─────────────────────────────────────────────┘\n\n";
    
    // 打印局部变量地址
    std::cout << "  【局部变量在栈帧中的位置】\n";
    std::cout << "  ┌─────────────────────────────────────────────┐\n";
    std::cout << "  │ local1 = " << std::setw(5) << local1 
              << "  地址: " << (void*)&local1 << " │\n";
    std::cout << "  │ local2 = " << std::setw(5) << local2 
              << "  地址: " << (void*)&local2 << " │\n";
    std::cout << "  │ local3 = " << std::setw(5) << local3 
              << "  地址: " << (void*)&local3 << " │\n";
    std::cout << "  └─────────────────────────────────────────────┘\n\n";
    
    // 分析栈帧布局
    std::cout << "  【栈帧布局分析】\n";
    std::cout << "  高地址\n";
    std::cout << "  │\n";
    std::cout << "  │   ┌─────────────────┐\n";
    std::cout << "  │   │ 参数 a          │ ← 调用者压入\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ 参数 b          │\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ 参数 c          │\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ 返回地址        │ ← call 指令自动压入\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ 旧的 EBP/RBP    │ ← 函数入口保存\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ local1          │ ← 函数内分配\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ local2          │\n";
    std::cout << "  │   ├─────────────────┤\n";
    std::cout << "  │   │ local3          │\n";
    std::cout << "  │   └─────────────────┘\n";
    std::cout << "  │\n";
    std::cout << "  ↓ 低地址 (栈生长方向)\n\n";
    
    // 计算局部变量之间的地址差
    std::cout << "  【地址差分析】\n";
    long diff_a_b = (char*)&b - (char*)&a;
    long diff_b_c = (char*)&c - (char*)&b;
    long diff_local1_local2 = (char*)&local2 - (char*)&local1;
    long diff_local2_local3 = (char*)&local3 - (char*)&local2;
    
    std::cout << "  参数 a → b 地址差: " << diff_a_b << " 字节\n";
    std::cout << "  参数 b → c 地址差: " << diff_b_c << " 字节\n";
    std::cout << "  local1 → local2 地址差: " << diff_local1_local2 << " 字节\n";
    std::cout << "  local2 → local3 地址差: " << diff_local2_local3 << " 字节\n";
    std::cout << "  (注: 实际布局可能因编译器优化而不同)\n\n";
    
    return a + b + c + local1 + local2 + local3;
}

// ============================================================================
// 参数传递方式演示
// ============================================================================

/**
 * @brief 值传递演示
 * @param x 整数参数（值传递）
 */
void passByValue(int x) {
    std::cout << "\n  【值传递演示】\n";
    std::cout << "  函数内 x 的地址: " << (void*)&x << "\n";
    std::cout << "  函数内 x 的值: " << x << "\n";
    
    x = 999;  // 修改局部副本
    std::cout << "  修改后 x 的值: " << x << "\n";
    std::cout << "  (注意: 函数外的原始变量不受影响)\n";
}

/**
 * @brief 引用传递演示
 * @param x 整数参数（引用传递）
 */
void passByReference(int& x) {
    std::cout << "\n  【引用传递演示】\n";
    std::cout << "  引用 x 绑定的地址: " << (void*)&x << "\n";
    std::cout << "  引用 x 的值: " << x << "\n";
    
    x = 999;  // 修改原始变量
    std::cout << "  修改后 x 的值: " << x << "\n";
    std::cout << "  (注意: 函数外的原始变量也被修改)\n";
}

/**
 * @brief 指针传递演示
 * @param x 整数指针
 */
void passByPointer(int* x) {
    std::cout << "\n  【指针传递演示】\n";
    std::cout << "  指针 x 的地址: " << (void*)&x << "\n";
    std::cout << "  指针 x 存储的地址: " << (void*)x << "\n";
    std::cout << "  指针指向的值: " << *x << "\n";
    
    *x = 888;  // 修改指向的值
    std::cout << "  修改后指向的值: " << *x << "\n";
    std::cout << "  (注意: 函数外的原始变量也被修改)\n";
}

// ============================================================================
// 递归栈帧深度演示
// ============================================================================

/**
 * @brief 递归函数，展示栈帧的增长
 * @param depth 当前深度
 * @param maxDepth 最大深度
 */
void recursiveStackGrowth(int depth, int maxDepth) {
    int localVariable = depth * 10;  // 局部变量
    
    std::cout << "  深度 " << depth << "/" << maxDepth 
              << " | 局部变量 = " << std::setw(3) << localVariable
              << " | 地址: " << (void*)&localVariable << "\n";
    
    if (depth < maxDepth) {
        recursiveStackGrowth(depth + 1, maxDepth);
    }
    
    // 注意：这里可以看到每个递归调用的局部变量地址都不同
    // 因为每次调用都创建了新的栈帧
}

// ============================================================================
// 主演示函数
// ============================================================================

void run_stack_frame_demo() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                                      ║\n";
    std::cout << "║              栈帧分析演示 - Stack Frame Analysis                    ║\n";
    std::cout << "║                                                                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════╝\n";
    
    // 演示1：栈帧布局
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  演示 1：栈帧布局分析\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    int result = analyzeStackFrame(10, 20, 30);
    std::cout << "  返回值: " << result << "\n";
    
    // 演示2：参数传递方式
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  演示 2：参数传递方式对比\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    int value = 100;
    
    std::cout << "\n  原始值: " << value << "\n";
    std::cout << "  原始地址: " << (void*)&value << "\n";
    
    // 值传递
    passByValue(value);
    std::cout << "  值传递后原始值: " << value << " (未改变)\n";
    
    // 引用传递
    value = 100;  // 重置
    passByReference(value);
    std::cout << "  引用传递后原始值: " << value << " (已改变)\n";
    
    // 指针传递
    value = 100;  // 重置
    passByPointer(&value);
    std::cout << "  指针传递后原始值: " << value << " (已改变)\n";
    
    // 演示3：递归栈帧增长
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  演示 3：递归调用时栈帧的增长\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  观察每次递归调用的局部变量地址变化：\n\n";
    
    recursiveStackGrowth(1, 5);
    
    std::cout << "\n  注意：地址值递减，说明栈向下生长\n";
    
    // 总结
    std::cout << "\n\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  💡 栈帧关键知识点：\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  1. 栈帧包含：函数参数、返回地址、保存的寄存器、局部变量\n";
    std::cout << "  2. 参数从右向左入栈（C/C++ 调用约定）\n";
    std::cout << "  3. 栈向低地址方向生长\n";
    std::cout << "  4. 值传递创建副本，引用/指针传递操作原数据\n";
    std::cout << "  5. 每次递归调用都创建新的栈帧\n";
    std::cout << "  6. 局部变量在函数返回后自动销毁\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}
