/**
 * EMC++全部条款复习
 */

#include <iostream>

void emcppReview() {
    std::cout << "=== EMC++条款复习 ===" << std::endl;
    
    std::cout << "\n第一章：类型推导 (Item 1-6)" << std::endl;
    std::cout << "  Item 1: 理解模板类型推导" << std::endl;
    std::cout << "  Item 2: 理解auto类型推导" << std::endl;
    std::cout << "  Item 3: 理解decltype" << std::endl;
    std::cout << "  Item 4: 学会查看推导类型" << std::endl;
    std::cout << "  Item 5: 优先使用auto" << std::endl;
    std::cout << "  Item 6: auto推导异常时显式初始化" << std::endl;
    
    std::cout << "\n第二章：转向现代C++ (Item 7-10)" << std::endl;
    std::cout << "  Item 7: 区别()和{}初始化" << std::endl;
    std::cout << "  Item 8: 优先nullptr" << std::endl;
    std::cout << "  Item 9: 优先using别名" << std::endl;
    std::cout << "  Item 10: 优先enum class" << std::endl;
    
    std::cout << "\n第三章：智能指针 (Item 17-22)" << std::endl;
    std::cout << "  Item 17: 理解特种成员函数生成" << std::endl;
    std::cout << "  Item 18: unique_ptr管理独占所有权" << std::endl;
    std::cout << "  Item 19: shared_ptr管理共享所有权" << std::endl;
    std::cout << "  Item 20: weak_ptr打破循环引用" << std::endl;
    std::cout << "  Item 21: 优先make_unique/make_shared" << std::endl;
    std::cout << "  Item 22: Pimpl模式在实现文件定义特殊函数" << std::endl;
    
    std::cout << "\n第四章：移动语义 (Item 23-30)" << std::endl;
    std::cout << "  Item 23: 理解move和forward" << std::endl;
    std::cout << "  Item 24: 区别万能引用和右值引用" << std::endl;
    std::cout << "  Item 25: 右值引用用move，万能引用用forward" << std::endl;
    std::cout << "  Item 26: 避免万能引用重载" << std::endl;
    std::cout << "  Item 27: 熟悉替代方案" << std::endl;
    std::cout << "  Item 28: 理解引用折叠" << std::endl;
    std::cout << "  Item 29: 假设移动不存在/不便宜" << std::endl;
    std::cout << "  Item 30: 熟悉完美转发失败情况" << std::endl;
    
    std::cout << "\n第五章：Lambda (Item 31-34)" << std::endl;
    std::cout << "  Item 31: 避免默认捕获" << std::endl;
    std::cout << "  Item 32: 使用初始化捕获移动对象" << std::endl;
    std::cout << "  Item 33: 对万能引用使用auto&&" << std::endl;
    std::cout << "  Item 34: 优先Lambda而非bind" << std::endl;
    
    std::cout << "\n第六章：并发API (Item 35-40)" << std::endl;
    std::cout << "  Item 35: 任务优先于线程" << std::endl;
    std::cout << "  Item 36: 需要异步指定launch::async" << std::endl;
    std::cout << "  Item 37: 确保thread不可join" << std::endl;
    std::cout << "  Item 38: 关注future析构行为" << std::endl;
    std::cout << "  Item 39: void future用于一次性事件" << std::endl;
    std::cout << "  Item 40: atomic用于并发，volatile用于特殊内存" << std::endl;
}

int main() {
    emcppReview();
    return 0;
}
