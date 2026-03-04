/**
 * Day 35: 35天学习总结 - 主程序
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <string>
#include <sstream>

void dataStructureSummary();
void cpp11Summary();
void emcppSummary();
void leetcodeDemo();

int main() {
    std::cout << "=== Day 35: 35天学习总结 ===" << std::endl;
    
    dataStructureSummary();
    cpp11Summary();
    emcppSummary();
    leetcodeDemo();
    
    std::cout << "\n恭喜完成35天C++学习之旅！" << std::endl;
    return 0;
}

void dataStructureSummary() {
    std::cout << "\n--- 数据结构知识体系 ---" << std::endl;
    std::cout << "数组: 访问O(1), 插入删除O(n)" << std::endl;
    std::cout << "链表: 访问O(n), 插入删除O(1)" << std::endl;
    std::cout << "栈/队列: LIFO/FIFO" << std::endl;
    std::cout << "哈希表: 平均O(1)查找插入删除" << std::endl;
    std::cout << "二叉搜索树: O(log n)查找插入删除" << std::endl;
}

void cpp11Summary() {
    std::cout << "\n--- C++11特性综合 ---" << std::endl;
    
    // auto
    auto x = 42;
    std::cout << "auto推导: " << x << std::endl;
    
    // 智能指针
    auto uptr = std::make_unique<int>(100);
    auto sptr = std::make_shared<int>(200);
    std::weak_ptr<int> wptr = sptr;
    std::cout << "unique_ptr: " << *uptr << std::endl;
    std::cout << "shared_ptr use_count: " << sptr.use_count() << std::endl;
    
    // Lambda
    std::vector<int> v = {10, 20, 30};
    int sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
    std::cout << "Lambda求和: " << sum << std::endl;
    
    // 并发
    std::atomic<int> counter{0};
    std::thread t([&counter]() {
        for (int i = 0; i < 1000; ++i) counter++;
    });
    t.join();
    std::cout << "线程并发: " << counter.load() << std::endl;
}

void emcppSummary() {
    std::cout << "\n--- EMC++条款复习 ---" << std::endl;
    std::cout << "条款5: auto优于显式类型声明" << std::endl;
    std::cout << "条款17: 理解特种成员函数生成" << std::endl;
    std::cout << "条款25: 右值引用用move，万能引用用forward" << std::endl;
    std::cout << "条款31: 避免默认捕获模式" << std::endl;
    std::cout << "条款35: 任务优先于线程" << std::endl;
}

void leetcodeDemo() {
    std::cout << "\n--- LeetCode示例 ---" << std::endl;
    std::cout << "LC 297: 二叉树序列化" << std::endl;
    std::cout << "LC 124: 最大路径和" << std::endl;
}
