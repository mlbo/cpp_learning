/**
 * @file shared_ptr_advanced.cpp
 * @brief shared_ptr 高级特性：控制块、自定义删除器、线程安全
 * 
 * 深入理解：
 * 1. 控制块结构
 * 2. make_shared 的内存布局优势
 * 3. 自定义删除器
 * 4. 别名构造函数（aliasing constructor）
 * 5. 线程安全性
 */

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <set>
#include <functional>

// ============================================================
// 1. 控制块结构演示
// ============================================================

/**
 * shared_ptr 的内部结构：
 * 
 * ┌─────────────────┐      ┌─────────────────────────────┐
 * │  shared_ptr     │      │      Control Block          │
 * ├─────────────────┤      ├─────────────────────────────┤
 * │  ptr ───────────┼──┐   │  strong_count (原子变量)     │
 * │  ctrl_block ────┼──┼──►│  weak_count (原子变量)       │
 * └─────────────────┘  │   │  deleter (删除器)            │
 *                      │   │  allocator (分配器)          │
 *                      │   │  managed_object (管理对象)   │
 *                      │   └─────────────────────────────┘
 *                      │
 *                      └──────────────────────────────────►实际对象
 */

void demoControlBlock() {
    std::cout << "【控制块结构演示】\n\n";
    
    // 方法1: 分开分配（两次内存分配）
    std::cout << "--- 分开分配 ---\n";
    {
        std::shared_ptr<int> sp1(new int(42));
        std::cout << "  new + shared_ptr: 可能两次内存分配\n";
        std::cout << "  对象地址: " << sp1.get() << "\n";
        // 控制块地址无法直接获取，但它独立于对象
    }
    
    // 方法2: 合并分配（一次内存分配）
    std::cout << "\n--- 合并分配 (make_shared) ---\n";
    {
        auto sp2 = std::make_shared<int>(42);
        std::cout << "  make_shared: 一次内存分配\n";
        std::cout << "  对象地址: " << sp2.get() << "\n";
        // 对象和控制块在同一块内存中
    }
    
    std::cout << "\n  控制块包含：\n";
    std::cout << "  • 强引用计数 (use_count)\n";
    std::cout << "  • 弱引用计数 (weak_count)\n";
    std::cout << "  • 删除器 (deleter)\n";
    std::cout << "  • 分配器 (allocator)\n";
}

// ============================================================
// 2. 自定义删除器
// ============================================================

// 自定义资源类型
struct FileHandle {
    FILE* fp;
    FileHandle(const char* filename) {
        fp = fopen(filename, "w");
        std::cout << "  打开文件\n";
    }
    ~FileHandle() {
        if (fp) {
            fclose(fp);
            std::cout << "  关闭文件\n";
        }
    }
};

void demoCustomDeleter() {
    std::cout << "\n【自定义删除器】\n\n";
    
    // 方式1: 使用 lambda 作为删除器
    std::cout << "--- 方式1: Lambda 删除器 ---\n";
    {
        auto fileDeleter = [](FILE* f) {
            if (f) {
                fclose(f);
                std::cout << "  通过 lambda 关闭文件\n";
            }
        };
        
        std::shared_ptr<FILE> file1(fopen("/tmp/test1.txt", "w"), fileDeleter);
        std::cout << "  文件已打开，use_count = " << file1.use_count() << "\n";
    }
    std::cout << "  离开作用域，文件自动关闭\n";
    
    // 方式2: 使用函数对象
    std::cout << "\n--- 方式2: 函数对象 ---\n";
    struct ArrayDeleter {
        void operator()(int* p) const {
            delete[] p;
            std::cout << "  通过自定义删除器释放数组\n";
        }
    };
    
    {
        std::shared_ptr<int> arr(new int[10]{1,2,3,4,5,6,7,8,9,10}, ArrayDeleter());
        std::cout << "  数组元素: " << arr.get()[0] << ", " << arr.get()[9] << "\n";
    }
    
    // 方式3: 使用 std::default_delete
    std::cout << "\n--- 方式3: default_delete ---\n";
    {
        std::shared_ptr<int[]> arr2(new int[5]{1,2,3,4,5}, std::default_delete<int[]>());
        std::cout << "  使用 default_delete<int[]> 管理数组\n";
    }
    
    // 删除器对控制块的影响
    std::cout << "\n--- 删除器的大小影响 ---\n";
    auto sp1 = std::make_shared<int>(42);
    auto sp2 = std::shared_ptr<int>(new int(42), [](int* p) {
        delete p;
        std::cout << "  自定义删除\n";
    });
    
    std::cout << "  shared_ptr 大小: " << sizeof(sp1) << " 字节\n";
    std::cout << "  （控制块大小因删除器类型而异，但 shared_ptr 大小不变）\n";
}

// ============================================================
// 3. 别名构造函数（Aliasing Constructor）
// ============================================================

struct Person {
    std::string name;
    int age;
    Person(const std::string& n, int a) : name(n), age(a) {
        std::cout << "  Person 构造: " << name << "\n";
    }
    ~Person() {
        std::cout << "  Person 析构: " << name << "\n";
    }
};

void demoAliasingConstructor() {
    std::cout << "\n【别名的构造函数】\n\n";
    std::cout << "  用途：访问对象的成员，同时保持整个对象的生命周期\n\n";
    
    auto person = std::make_shared<Person>("张三", 25);
    std::cout << "  person use_count = " << person.use_count() << "\n";
    
    // 创建指向成员的 shared_ptr
    // 构造函数: shared_ptr(const shared_ptr<Y>& r, element_type* ptr)
    // r 控制生命周期，ptr 是实际访问的地址
    std::shared_ptr<std::string> name(person, &person->name);
    std::shared_ptr<int> age(person, &person->age);
    
    std::cout << "  创建别名后，person use_count = " << person.use_count() << "\n";
    std::cout << "  name = " << *name << ", age = " << *age << "\n";
    
    // 重置 person，但 name 和 age 仍然有效
    person.reset();
    std::cout << "  person.reset() 后，use_count = " << name.use_count() << "\n";
    std::cout << "  仍然可以访问: name = " << *name << "\n";
    
    std::cout << "  name 和 age 离开作用域...\n";
}

// ============================================================
// 4. 线程安全性
// ============================================================

void demoThreadSafety() {
    std::cout << "\n【线程安全性】\n\n";
    
    auto sp = std::make_shared<int>(0);
    
    std::cout << "--- 引用计数的线程安全性 ---\n";
    std::cout << "  引用计数操作是原子的，多线程安全\n\n";
    
    std::vector<std::thread> threads;
    
    // 多个线程拷贝和销毁 shared_ptr
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([sp]() {  // 拷贝捕获
            auto local = sp;
            std::cout << "  线程中 use_count = " << local.use_count() << "\n";
            // local 离开作用域，减少引用计数
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "\n  所有线程结束后，use_count = " << sp.use_count() << "\n";
    
    std::cout << "\n--- 访问对象本身需要同步 ---\n";
    std::cout << "  虽然引用计数是线程安全的，\n";
    std::cout << "  但访问 *sp 需要额外同步机制！\n";
    
    // 使用 mutex 保护对象访问
    std::mutex mtx;
    auto counter = std::make_shared<int>(0);
    
    threads.clear();
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&mtx, counter]() {
            std::lock_guard<std::mutex> lock(mtx);
            ++(*counter);  // 需要同步
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "  使用 mutex 后，counter = " << *counter << "\n";
}

// ============================================================
// 5. owner_less 和作为关联容器键
// ============================================================

void demoOwnerLess() {
    std::cout << "\n【作为关联容器的键】\n\n";
    
    // 使用 owner_less 比较器
    std::set<std::shared_ptr<int>, std::owner_less<std::shared_ptr<int>>> s;
    
    auto sp1 = std::make_shared<int>(1);
    auto sp2 = std::make_shared<int>(2);
    auto sp3 = sp1;  // sp3 和 sp1 指向同一对象
    
    s.insert(sp1);
    s.insert(sp2);
    s.insert(sp3);  // 不会插入，因为 owner_less 认为它等于 sp1
    
    std::cout << "  set 大小: " << s.size() << " (sp3 与 sp1 被视为相同)\n";
    std::cout << "  owner_less 基于控制块地址比较，而非对象地址\n";
}

// ============================================================
// 演示函数
// ============================================================

void demoSharedPtrAdvanced() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║           shared_ptr 高级特性                             ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    demoControlBlock();
    demoCustomDeleter();
    demoAliasingConstructor();
    demoThreadSafety();
    
    std::cout << "\n要点总结：\n";
    std::cout << "1. make_shared 一次分配，效率更高\n";
    std::cout << "2. 自定义删除器支持特殊资源管理\n";
    std::cout << "3. 别名构造函数用于访问成员\n";
    std::cout << "4. 引用计数线程安全，但访问对象需同步\n";
    std::cout << "5. owner_less 用于关联容器键比较\n";
}
