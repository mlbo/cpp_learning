/**
 * 泛型Lambda演示（C++14）
 * 参数使用auto，实现通用Lambda
 */

#include "lambda_generic.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void lambdaGenericDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        泛型 Lambda (C++14)           ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 基本泛型Lambda ==========
    std::cout << "\n--- 1. 基本泛型Lambda ---" << std::endl;
    
    // C++14: 参数使用auto
    auto add = [](auto a, auto b) {
        return a + b;
    };
    
    std::cout << "  add(1, 2) = " << add(1, 2) << std::endl;
    std::cout << "  add(1.5, 2.5) = " << add(1.5, 2.5) << std::endl;
    std::cout << "  add(std::string(\"Hello\"), std::string(\" World\")) = " 
              << add(std::string("Hello"), std::string(" World")) << std::endl;
    
    // ========== 2. 泛型Lambda与STL算法 ==========
    std::cout << "\n--- 2. 泛型Lambda与STL算法 ---" << std::endl;
    
    // 打印任意类型的容器
    auto printContainer = [](const auto& container) {
        std::cout << "  [";
        bool first = true;
        for (const auto& item : container) {
            if (!first) std::cout << ", ";
            std::cout << item;
            first = false;
        }
        std::cout << "]" << std::endl;
    };
    
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::vector<std::string> strs = {"a", "b", "c"};
    
    std::cout << "  int vector: ";
    printContainer(nums);
    std::cout << "  string vector: ";
    printContainer(strs);
    
    // ========== 3. 泛型Lambda作为比较函数 ==========
    std::cout << "\n--- 3. 泛型Lambda作为比较函数 ---" << std::endl;
    
    // 通用的降序排序
    auto descending = [](const auto& a, const auto& b) {
        return a > b;
    };
    
    std::vector<int> arr1 = {5, 2, 8, 1, 9};
    std::sort(arr1.begin(), arr1.end(), descending);
    std::cout << "  int降序: ";
    printContainer(arr1);
    
    std::vector<double> arr2 = {1.1, 3.3, 2.2};
    std::sort(arr2.begin(), arr2.end(), descending);
    std::cout << "  double降序: ";
    printContainer(arr2);
    
    // ========== 4. 多参数泛型Lambda ==========
    std::cout << "\n--- 4. 多参数泛型Lambda ---" << std::endl;
    
    // 通用打印函数
    auto printTwo = [](auto a, auto b) {
        std::cout << "  a = " << a << ", b = " << b << std::endl;
    };
    
    printTwo(1, 2.5);
    printTwo(std::string("Hello"), 42);
    printTwo('A', 3.14);
    
    // ========== 5. 泛型Lambda与返回类型推导 ==========
    std::cout << "\n--- 5. 返回类型推导 ---" << std::endl;
    
    // 返回类型自动推导
    auto multiply = [](auto a, auto b) {
        return a * b;
    };
    
    auto r1 = multiply(3, 4);
    auto r2 = multiply(2.5, 4.0);
    
    std::cout << "  multiply(3, 4) = " << r1 << " (type: int)" << std::endl;
    std::cout << "  multiply(2.5, 4.0) = " << r2 << " (type: double)" << std::endl;
    
    // ========== 6. 泛型Lambda的优势 ==========
    std::cout << "\n--- 6. 泛型Lambda的优势 ---" << std::endl;
    std::cout << "  1. 减少代码重复，一个Lambda多种类型使用" << std::endl;
    std::cout << "  2. 与模板配合更灵活" << std::endl;
    std::cout << "  3. 可以接受任何支持所需操作的类型" << std::endl;
    
    // 示例：通用求和
    auto sum = [](auto... args) {  // C++17 折叠表达式
        return (args + ...);
    };
    
    // C++14 替代方案
    auto sumTwo = [](auto a, auto b) {
        return a + b;
    };
    
    std::cout << "  sumTwo(10, 20) = " << sumTwo(10, 20) << std::endl;
}
