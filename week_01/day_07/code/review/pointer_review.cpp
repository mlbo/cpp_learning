/**
 * @file pointer_review.cpp
 * @brief 指针与智能指针复习
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

// ============================================================================
// 1. 原始指针基础
// ============================================================================

void raw_pointer_basics() {
    std::cout << "\n【1. 原始指针基础】\n";
    std::cout << "-------------------------------------------\n";

    // 指针声明与初始化
    int value = 42;
    int* ptr = &value;
    
    std::cout << "  int value = 42;\n";
    std::cout << "  int* ptr = &value;\n";
    std::cout << "  *ptr = " << *ptr << "\n";
    std::cout << "  ptr 地址 = " << ptr << "\n";

    // 空指针的表示方式
    std::cout << "\n  // 空指针表示方式:\n";
    int* p1 = nullptr;  // C++11 推荐
    int* p2 = NULL;     // C风格，不推荐
    int* p3 = 0;        // 不推荐
    
    std::cout << "  int* p1 = nullptr;  // C++11 推荐\n";
    std::cout << "  p1 == nullptr: " << (p1 == nullptr) << "\n";

    // 指针与数组
    std::cout << "\n  // 指针与数组:\n";
    int arr[] = {1, 2, 3, 4, 5};
    int* begin = arr;
    int* end = arr + 5;
    
    std::cout << "  int arr[] = {1, 2, 3, 4, 5};\n";
    std::cout << "  *begin = " << *begin << "\n";
    std::cout << "  *(begin + 2) = " << *(begin + 2) << "\n";
    
    // 指针算术
    for (int* p = begin; p != end; ++p) {
        std::cout << " " << *p;
    }
    std::cout << "\n";
}

// ============================================================================
// 2. 智能指针详解
// ============================================================================

// 自定义删除器示例
struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) {
            std::cout << "    [FileDeleter] 关闭文件\n";
            fclose(f);
        }
    }
};

void smart_pointers_demo() {
    std::cout << "\n【2. 智能指针】\n";
    std::cout << "-------------------------------------------\n";

    // unique_ptr - 独占所有权
    std::cout << "  // unique_ptr - 独占所有权\n";
    {
        auto uptr = std::make_unique<int>(42);
        std::cout << "  auto uptr = std::make_unique<int>(42);\n";
        std::cout << "  *uptr = " << *uptr << "\n";
        
        // 独占所有权，不能拷贝
        // auto uptr2 = uptr;  // 编译错误
        
        // 但可以移动
        auto uptr2 = std::move(uptr);
        std::cout << "  auto uptr2 = std::move(uptr);\n";
        std::cout << "  uptr 为空: " << (uptr == nullptr) << "\n";
        std::cout << "  *uptr2 = " << *uptr2 << "\n";
    }
    std::cout << "    [离开作用域，自动销毁]\n";

    // shared_ptr - 共享所有权
    std::cout << "\n  // shared_ptr - 共享所有权\n";
    {
        auto sptr1 = std::make_shared<int>(100);
        std::cout << "  auto sptr1 = std::make_shared<int>(100);\n";
        std::cout << "  引用计数: " << sptr1.use_count() << "\n";
        
        {
            auto sptr2 = sptr1;  // 拷贝，共享所有权
            std::cout << "  auto sptr2 = sptr1;\n";
            std::cout << "  引用计数: " << sptr1.use_count() << "\n";
        }
        
        std::cout << "  [sptr2 离开作用域]\n";
        std::cout << "  引用计数: " << sptr1.use_count() << "\n";
    }

    // weak_ptr - 打破循环引用
    std::cout << "\n  // weak_ptr - 打破循环引用\n";
    {
        auto sptr = std::make_shared<int>(200);
        std::weak_ptr<int> wptr = sptr;
        
        std::cout << "  auto sptr = std::make_shared<int>(200);\n";
        std::cout << "  std::weak_ptr<int> wptr = sptr;\n";
        std::cout << "  引用计数: " << sptr.use_count() << " (weak_ptr 不增加)\n";
        std::cout << "  wptr.expired(): " << wptr.expired() << "\n";
        
        if (auto locked = wptr.lock()) {
            std::cout << "  *locked = " << *locked << "\n";
        }
    }
}

// ============================================================================
// 3. 循环引用问题
// ============================================================================

struct Node {
    int value;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 使用 weak_ptr 打破循环
    
    Node(int v) : value(v) { 
        std::cout << "    Node(" << v << ") 构造\n"; 
    }
    ~Node() { 
        std::cout << "    Node(" << value << ") 析构\n"; 
    }
};

void circular_reference_demo() {
    std::cout << "\n【3. 循环引用问题】\n";
    std::cout << "-------------------------------------------\n";
    
    std::cout << "  // 使用 weak_ptr 打破循环引用:\n";
    {
        auto node1 = std::make_shared<Node>(1);
        auto node2 = std::make_shared<Node>(2);
        
        node1->next = node2;
        node2->prev = node1;  // weak_ptr，不增加引用计数
        
        std::cout << "  node1 引用计数: " << node1.use_count() << "\n";
        std::cout << "  node2 引用计数: " << node2.use_count() << "\n";
        std::cout << "  [离开作用域，正确销毁]\n";
    }
}

// ============================================================================
// 4. 自定义删除器
// ============================================================================

void custom_deleter_demo() {
    std::cout << "\n【4. 自定义删除器】\n";
    std::cout << "-------------------------------------------\n";
    
    // 使用函数作为删除器
    auto array_deleter = [](int* p) {
        std::cout << "    [ArrayDeleter] delete[] p\n";
        delete[] p;
    };
    
    std::unique_ptr<int[], decltype(array_deleter)> arr_ptr(
        new int[5]{1, 2, 3, 4, 5}, 
        array_deleter
    );
    
    std::cout << "  std::unique_ptr<int[], Deleter> arr_ptr(new int[5], ...);\n";
    std::cout << "  arr_ptr[2] = " << arr_ptr[2] << "\n";
    
    // shared_ptr 也可以使用自定义删除器
    std::cout << "\n  // shared_ptr 自定义删除器:\n";
    std::shared_ptr<int> sptr(
        new int(42),
        [](int* p) {
            std::cout << "    [Lambda Deleter] delete p\n";
            delete p;
        }
    );
    std::cout << "  *sptr = " << *sptr << "\n";
}

// ============================================================================
// 5. 性能对比
// ============================================================================

void performance_demo() {
    std::cout << "\n【5. 性能考虑】\n";
    std::cout << "-------------------------------------------\n";
    
    const int N = 1000000;
    
    // 原始指针
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<int*> ptrs;
        ptrs.reserve(N);
        for (int i = 0; i < N; ++i) {
            ptrs.push_back(new int(i));
        }
        for (auto p : ptrs) delete p;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto raw_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // unique_ptr
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::unique_ptr<int>> ptrs;
        ptrs.reserve(N);
        for (int i = 0; i < N; ++i) {
            ptrs.push_back(std::make_unique<int>(i));
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto unique_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // shared_ptr
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::shared_ptr<int>> ptrs;
        ptrs.reserve(N);
        for (int i = 0; i < N; ++i) {
            ptrs.push_back(std::make_shared<int>(i));
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto shared_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  创建并销毁 " << N << " 个指针:\n";
    std::cout << "  原始指针:    " << std::setw(6) << raw_duration.count() << " μs\n";
    std::cout << "  unique_ptr:  " << std::setw(6) << unique_duration.count() << " μs\n";
    std::cout << "  shared_ptr:  " << std::setw(6) << shared_duration.count() << " μs\n";
    
    std::cout << "\n  结论: unique_ptr 几乎零开销，shared_ptr 有引用计数开销\n";
}

// ============================================================================
// 6. 最佳实践总结
// ============================================================================

void best_practices_summary() {
    std::cout << "\n【6. 最佳实践总结】\n";
    std::cout << "-------------------------------------------\n";
    
    std::cout << "  ✅ 使用智能指针管理动态资源:\n";
    std::cout << "     • 独占所有权 → unique_ptr\n";
    std::cout << "     • 共享所有权 → shared_ptr\n";
    std::cout << "     • 打破循环   → weak_ptr\n";
    
    std::cout << "\n  ✅ 优先使用 make_unique/make_shared:\n";
    std::cout << "     • 异常安全\n";
    std::cout << "     • 性能优化（shared_ptr 一次分配）\n";
    
    std::cout << "\n  ✅ 避免原始指针管理所有权:\n";
    std::cout << "     • 原始指针用于\"观察\"，不用于\"管理\"\n";
    std::cout << "     • 不知道所有权时使用 observer_ptr 或裸指针\n";
    
    std::cout << "\n  ✅ 使用 nullptr 而非 NULL 或 0:\n";
    std::cout << "     • 类型安全\n";
    std::cout << "     • 避免重载歧义\n";
    
    std::cout << "\n  ⚠️  注意事项:\n";
    std::cout << "     • 不要从原始指针创建多个独立智能指针\n";
    std::cout << "     • 不要用 get() 返回的指针创建新智能指针\n";
    std::cout << "     • 注意 shared_ptr 的循环引用问题\n";
}

// ============================================================================

void pointer_review() {
    raw_pointer_basics();
    smart_pointers_demo();
    circular_reference_demo();
    custom_deleter_demo();
    performance_demo();
    best_practices_summary();
    
    std::cout << "\n✅ 指针复习完成!\n";
}
