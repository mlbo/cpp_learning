/**
 * Lambda捕获方式详解
 */

#include "lambda_capture_demo.h"
#include <iostream>
#include <memory>

void lambdaCaptureDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        Lambda 捕获方式详解           ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 值捕获 ==========
    std::cout << "\n--- 1. 值捕获 [x] ---" << std::endl;
    
    int x = 10;
    auto valueCapture = [x]() {
        std::cout << "  捕获的x值: " << x << std::endl;
    };
    
    x = 100;
    valueCapture();
    std::cout << "  原变量x: " << x << std::endl;
    
    // ========== 2. 引用捕获 ==========
    std::cout << "\n--- 2. 引用捕获 [&x] ---" << std::endl;
    
    int z = 10;
    auto refCapture = [&z]() {
        z *= 2;
        std::cout << "  Lambda内部z: " << z << std::endl;
    };
    
    refCapture();
    std::cout << "  原变量z: " << z << std::endl;
    
    // ========== 3. 混合捕获 ==========
    std::cout << "\n--- 3. 混合捕获 ---" << std::endl;
    
    int m = 5, n = 10;
    auto mixed = [=, &n]() {
        n = m * 2;
        std::cout << "  m=" << m << ", n=" << n << std::endl;
    };
    mixed();
    
    // ========== 4. 捕获总结 ==========
    std::cout << "\n--- 4. 捕获方式总结 ---" << std::endl;
    std::cout << "  []        不捕获任何变量" << std::endl;
    std::cout << "  [x]       值捕获x" << std::endl;
    std::cout << "  [&x]      引用捕获x" << std::endl;
    std::cout << "  [=]       值捕获所有变量" << std::endl;
    std::cout << "  [&]       引用捕获所有变量" << std::endl;
    std::cout << "  [=, &x]   值捕获所有，但x引用捕获" << std::endl;
}
