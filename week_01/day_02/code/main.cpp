/**
 * @file main.cpp
 * @brief Day 2: 数组数据结构与decltype详解 - 主程序入口
 */

#include <iostream>
#include <vector>

// 声明各模块的演示函数
namespace data_structure {
    void demonstrate_array();
}

namespace cpp11_features {
    void demonstrate_decltype();
}

namespace emcpp {
    void demonstrate_auto_traps();
}

namespace leetcode {
    void test_remove_duplicates();
    void test_remove_element();
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Day 2: 数组与decltype学习演示" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // 1. 数组数据结构演示
    std::cout << "[1] 数组数据结构演示" << std::endl;
    std::cout << "====================" << std::endl;
    data_structure::demonstrate_array();
    std::cout << std::endl;

    // 2. decltype演示
    std::cout << "[2] decltype演示" << std::endl;
    std::cout << "================" << std::endl;
    cpp11_features::demonstrate_decltype();
    std::cout << std::endl;

    // 3. EMC++条款6：auto推导陷阱
    std::cout << "[3] EMC++ 条款6：auto推导陷阱" << std::endl;
    std::cout << "=============================" << std::endl;
    emcpp::demonstrate_auto_traps();
    std::cout << std::endl;

    // 4. LeetCode 26题
    std::cout << "[4] LeetCode 26题：删除有序数组中的重复项" << std::endl;
    std::cout << "=========================================" << std::endl;
    leetcode::test_remove_duplicates();
    std::cout << std::endl;

    // 5. LeetCode 27题
    std::cout << "[5] LeetCode 27题：移除元素" << std::endl;
    std::cout << "===========================" << std::endl;
    leetcode::test_remove_element();
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Day 2 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
