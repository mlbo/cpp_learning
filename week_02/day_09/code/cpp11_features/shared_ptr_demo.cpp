/**
 * @file shared_ptr_demo.cpp
 * @brief shared_ptr 基础用法演示
 * 
 * 涵盖内容：
 * 1. 创建 shared_ptr
 * 2. 引用计数机制
 * 3. 拷贝与移动语义
 * 4. 常用操作
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 资源类，用于跟踪生命周期
class Resource {
public:
    Resource(const std::string& name) : name_(name) {
        std::cout << "  [构造] Resource: " << name_ << "\n";
    }
    
    ~Resource() {
        std::cout << "  [析构] Resource: " << name_ << "\n";
    }
    
    void use() const {
        std::cout << "  [使用] Resource: " << name_ << "\n";
    }
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
};

// ============================================================
// 1. 创建 shared_ptr 的几种方式
// ============================================================

void demoCreation() {
    std::cout << "【创建 shared_ptr 的几种方式】\n\n";
    
    // 方式1: make_shared（推荐）
    std::cout << "1. 使用 make_shared:\n";
    {
        auto sp1 = std::make_shared<Resource>("Resource_A");
        // 优点：一次内存分配，异常安全
    }
    
    // 方式2: 从裸指针构造
    std::cout << "\n2. 从裸指针构造:\n";
    {
        Resource* raw = new Resource("Resource_B");
        std::shared_ptr<Resource> sp2(raw);
        // 注意：不要用同一个裸指针创建多个 shared_ptr！
    }
    
    // 方式3: 使用 new 表达式
    std::cout << "\n3. 使用 new 表达式:\n";
    {
        std::shared_ptr<Resource> sp3(new Resource("Resource_C"));
        // 可行，但不如 make_shared
    }
    
    // 方式4: 共享空指针
    std::cout << "\n4. 空指针:\n";
    {
        std::shared_ptr<Resource> sp4;  // 空指针
        std::cout << "  空指针 use_count: " << sp4.use_count() << "\n";
        
        sp4 = nullptr;  // 重置为空
        std::cout << "  赋值为 nullptr 后 use_count: " << sp4.use_count() << "\n";
    }
}

// ============================================================
// 2. 引用计数机制
// ============================================================

void demoRefCount() {
    std::cout << "\n【引用计数机制】\n\n";
    
    auto sp1 = std::make_shared<Resource>("CountDemo");
    std::cout << "  创建 sp1, use_count = " << sp1.use_count() << "\n";
    
    {
        auto sp2 = sp1;  // 拷贝构造，引用计数+1
        std::cout << "  拷贝构造 sp2, use_count = " << sp1.use_count() << "\n";
        
        auto sp3 = sp1;  // 再次拷贝
        std::cout << "  拷贝构造 sp3, use_count = " << sp1.use_count() << "\n";
        
        // sp2, sp3 离开作用域
    }
    std::cout << "  sp2, sp3 离开作用域, use_count = " << sp1.use_count() << "\n";
    
    std::cout << "  sp1 离开作用域...\n";
}

// ============================================================
// 3. 拷贝与移动语义
// ============================================================

void demoCopyAndMove() {
    std::cout << "\n【拷贝与移动语义】\n\n";
    
    // 拷贝：增加引用计数
    std::cout << "--- 拷贝语义 ---\n";
    auto sp1 = std::make_shared<Resource>("CopyDemo");
    auto sp2 = sp1;  // 拷贝
    std::cout << "  拷贝后: sp1.use_count = " << sp1.use_count() 
              << ", sp2.use_count = " << sp2.use_count() << "\n";
    
    // 移动：不增加引用计数，转移所有权
    std::cout << "\n--- 移动语义 ---\n";
    auto sp3 = std::make_shared<Resource>("MoveDemo");
    std::cout << "  移动前: sp3.use_count = " << sp3.use_count() << "\n";
    
    auto sp4 = std::move(sp3);  // 移动
    std::cout << "  移动后: sp3 = " << (sp3 ? "非空" : "空") 
              << ", sp4.use_count = " << sp4.use_count() << "\n";
    
    // 重置
    std::cout << "\n--- reset 操作 ---\n";
    sp4.reset();
    std::cout << "  reset 后: sp4 = " << (sp4 ? "非空" : "空") << "\n";
    
    // 重置并管理新资源
    sp4.reset(new Resource("NewResource"));
    std::cout << "  reset 新资源后: sp4.use_count = " << sp4.use_count() << "\n";
}

// ============================================================
// 4. 常用操作
// ============================================================

void demoOperations() {
    std::cout << "\n【常用操作】\n\n";
    
    auto sp = std::make_shared<Resource>("OpsDemo");
    
    // 检查是否有效
    std::cout << "--- 检查有效性 ---\n";
    if (sp) {
        std::cout << "  sp 有效，可以访问\n";
        sp->use();
    }
    
    // 获取裸指针（谨慎使用）
    std::cout << "\n--- 获取裸指针 ---\n";
    Resource* raw = sp.get();
    std::cout << "  get() 返回裸指针: " << raw << "\n";
    std::cout << "  注意：不要删除这个指针！\n";
    
    // 解引用
    std::cout << "\n--- 解引用 ---\n";
    std::cout << "  *sp 访问对象: name = " << sp->name() << "\n";
    std::cout << "  sp-> 访问成员: name = " << sp->name() << "\n";
    
    // 交换
    std::cout << "\n--- 交换 ---\n";
    auto sp2 = std::make_shared<Resource>("SwapDemo");
    std::cout << "  交换前: sp=" << sp->name() << ", sp2=" << sp2->name() << "\n";
    sp.swap(sp2);
    std::cout << "  交换后: sp=" << sp->name() << ", sp2=" << sp2->name() << "\n";
    
    // 唯一性检查
    std::cout << "\n--- 唯一性检查 ---\n";
    std::cout << "  sp.unique() = " << sp.unique() << " (C++17已弃用)\n";
    std::cout << "  替代方案: sp.use_count() == 1 -> " << (sp.use_count() == 1) << "\n";
}

// ============================================================
// 5. 容器中的 shared_ptr
// ============================================================

void demoInContainer() {
    std::cout << "\n【容器中的 shared_ptr】\n\n";
    
    std::vector<std::shared_ptr<Resource>> resources;
    
    for (int i = 0; i < 3; ++i) {
        resources.push_back(std::make_shared<Resource>("Container_" + std::to_string(i)));
    }
    std::cout << "  容器中有 " << resources.size() << " 个资源\n";
    
    // 拷贝到另一个容器
    auto copy = resources;
    std::cout << "  拷贝后，第一个资源的 use_count = " << copy[0].use_count() << "\n";
    
    // 清空原容器
    resources.clear();
    std::cout << "  清空原容器后，copy中资源的 use_count = " << copy[0].use_count() << "\n";
    
    std::cout << "  copy 离开作用域...\n";
}

// ============================================================
// 演示函数
// ============================================================

void demoSharedPtrBasic() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║           shared_ptr 基础用法                             ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    demoCreation();
    demoRefCount();
    demoCopyAndMove();
    demoOperations();
    demoInContainer();
    
    std::cout << "\n要点总结：\n";
    std::cout << "1. 优先使用 make_shared 创建\n";
    std::cout << "2. 拷贝增加引用计数，移动转移所有权\n";
    std::cout << "3. 引用计数归零时自动释放资源\n";
    std::cout << "4. 容器中安全存储\n";
}
