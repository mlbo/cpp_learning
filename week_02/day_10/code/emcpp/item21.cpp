/**
 * @file item21.cpp
 * @brief EMC++ 条款21: shared_ptr 与 weak_ptr 配合使用
 * 
 * 条款要点：
 * 1. 当需要访问但不拥有资源时，使用 weak_ptr
 * 2. weak_ptr 不会增加引用计数
 * 3. 典型应用：缓存、观察者、打破循环
 */

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================
// 条款21 核心要点
// ============================================
/*
 * std::weak_ptr 的设计目的：
 * 1. 表达"非拥有性"引用
 * 2. 可以检测被引用对象是否存活
 * 3. 可以安全地尝试获取所有权
 * 
 * 典型使用场景：
 * 1. 缓存系统 - 不阻止缓存对象被释放
 * 2. 观察者模式 - Subject 不拥有 Observer
 * 3. 打破 shared_ptr 循环引用
 * 4. 对象池 - 允许对象被正常回收
 */

// ============================================
// 示例1：缓存系统
// ============================================

class ExpensiveResource {
public:
    std::string id;
    std::string data;
    
    ExpensiveResource(const std::string& i, const std::string& d)
        : id(i), data(d) {
        std::cout << "    🏗️  ExpensiveResource '" << id << "' 创建\n";
    }
    
    ~ExpensiveResource() {
        std::cout << "    💥 ExpensiveResource '" << id << "' 销毁\n";
    }
    
    void use() const {
        std::cout << "    📖 使用资源 '" << id << "': " << data << "\n";
    }
};

class ResourceCache {
public:
    // 缓存返回 weak_ptr，不阻止资源被释放
    std::weak_ptr<ExpensiveResource> get(const std::string& id) {
        auto it = cache_.find(id);
        if (it != cache_.end()) {
            if (!it->second.expired()) {
                std::cout << "    ✅ 缓存命中: " << id << "\n";
            }
            return it->second;
        }
        return std::weak_ptr<ExpensiveResource>();
    }
    
    void put(const std::string& id, std::shared_ptr<ExpensiveResource> resource) {
        cache_[id] = resource;
        std::cout << "    📦 缓存存储: " << id << "\n";
    }
    
    // 尝试获取，如果不存在则创建
    std::shared_ptr<ExpensiveResource> getOrCreate(
        const std::string& id, 
        const std::string& defaultData = "default") {
        
        if (auto cached = get(id).lock()) {
            return cached;
        }
        
        std::cout << "    ⚠️ 缓存未命中，创建新资源\n";
        auto resource = std::make_shared<ExpensiveResource>(id, defaultData);
        put(id, resource);
        return resource;
    }
    
    void showCacheStatus() {
        std::cout << "    📊 缓存状态:\n";
        for (const auto& [id, wp] : cache_) {
            std::cout << "      " << id << ": " 
                      << (wp.expired() ? "已过期" : "有效") << "\n";
        }
    }

private:
    std::unordered_map<std::string, std::weak_ptr<ExpensiveResource>> cache_;
};

void demo_cache() {
    std::cout << "\n【1. 缓存系统 - weak_ptr 不阻止资源释放】\n";
    
    ResourceCache cache;
    
    {
        auto res1 = std::make_shared<ExpensiveResource>("res1", "重要数据");
        cache.put("res1", res1);
        
        // 缓存可以访问
        if (auto cached = cache.get("res1").lock()) {
            cached->use();
        }
        
        std::cout << "\n  资源 res1 离开作用域...\n";
    }
    
    // 缓存自动检测到资源已释放
    std::cout << "\n  检查缓存状态:\n";
    cache.showCacheStatus();
    
    if (cache.get("res1").lock()) {
        std::cout << "    资源仍存在\n";
    } else {
        std::cout << "    ✅ 资源已正确释放，缓存自动失效\n";
    }
}

// ============================================
// 示例2：对象池
// ============================================

template<typename T>
class ObjectPool {
public:
    std::shared_ptr<T> acquire() {
        // 先尝试复用
        for (auto it = pool_.begin(); it != pool_.end(); ) {
            if (it->expired()) {
                it = pool_.erase(it);
            } else {
                ++it;
            }
        }
        
        // 创建新对象
        auto obj = std::make_shared<T>();
        pool_.push_back(obj);
        std::cout << "    🎯 对象池分配新对象\n";
        return obj;
    }
    
    size_t activeCount() const {
        size_t count = 0;
        for (const auto& wp : pool_) {
            if (!wp.expired()) count++;
        }
        return count;
    }

private:
    std::vector<std::weak_ptr<T>> pool_;
};

class Connection {
public:
    int id;
    Connection() : id(id_counter_++) {
        std::cout << "    🔌 Connection " << id << " 创建\n";
    }
    ~Connection() {
        std::cout << "    🔌 Connection " << id << " 关闭\n";
    }
    
    void query(const std::string& sql) {
        std::cout << "    Connection " << id << " 执行: " << sql << "\n";
    }
    
private:
    static int id_counter_;
};
int Connection::id_counter_ = 1;

void demo_object_pool() {
    std::cout << "\n【2. 对象池 - 允许对象正常回收】\n";
    
    ObjectPool<Connection> pool;
    
    {
        auto conn1 = pool.acquire();
        conn1->query("SELECT * FROM users");
        
        auto conn2 = pool.acquire();
        conn2->query("SELECT * FROM orders");
        
        std::cout << "  活跃连接数: " << pool.activeCount() << "\n";
    }
    
    std::cout << "  连接离开作用域后:\n";
    std::cout << "  活跃连接数: " << pool.activeCount() << "\n";
    std::cout << "  ✅ 对象池允许连接正常关闭\n";
}

// ============================================
// 示例3：enable_shared_from_this 与 weak_ptr
// ============================================

class Processor : public std::enable_shared_from_this<Processor> {
public:
    std::string name;
    
    Processor(const std::string& n) : name(n) {
        std::cout << "    Processor '" << name << "' 创建\n";
    }
    
    ~Processor() {
        std::cout << "    Processor '" << name << "' 销毁\n";
    }
    
    // 返回 weak_ptr，调用者不获取所有权
    std::weak_ptr<Processor> getWeakPtr() {
        return shared_from_this();
    }
    
    void process(const std::string& task) {
        std::cout << "    Processor '" << name << "' 处理: " << task << "\n";
    }
};

void demo_weak_this() {
    std::cout << "\n【3. enable_shared_from_this 与 weak_ptr】\n";
    
    std::weak_ptr<Processor> weakProc;
    
    {
        auto proc = std::make_shared<Processor>("MainProcessor");
        weakProc = proc->getWeakPtr();
        
        if (auto sp = weakProc.lock()) {
            sp->process("任务1");
        }
        
        std::cout << "  Processor 离开作用域...\n";
    }
    
    if (weakProc.expired()) {
        std::cout << "  ✅ weak_ptr 检测到 Processor 已销毁\n";
    }
}

// ============================================
// 条款21 最佳实践总结
// ============================================
void show_best_practices() {
    std::cout << "\n【条款21 最佳实践总结】\n";
    
    std::cout << "\n  使用 weak_ptr 的时机:\n";
    std::cout << "    ✅ 需要访问但不拥有资源\n";
    std::cout << "    ✅ 需要检测对象是否存活\n";
    std::cout << "    ✅ 需要打破 shared_ptr 循环\n";
    std::cout << "    ✅ 实现缓存系统\n";
    std::cout << "    ✅ 实现观察者模式\n";
    
    std::cout << "\n  不要使用 weak_ptr 的时机:\n";
    std::cout << "    ❌ 需要独占所有权 (用 unique_ptr)\n";
    std::cout << "    ❌ 需要共享所有权 (用 shared_ptr)\n";
    
    std::cout << "\n  使用模式:\n";
    std::cout << "    if (auto sp = wp.lock()) {\n";
    std::cout << "        // 安全使用 sp\n";
    std::cout << "    } else {\n";
    std::cout << "        // 对象已销毁\n";
    std::cout << "    }\n";
}

// ============================================
// 主演示函数
// ============================================
void demo_item21_weak_ptr() {
    demo_cache();
    demo_object_pool();
    demo_weak_this();
    show_best_practices();
    
    std::cout << "\n💡 EMC++ 条款21 核心结论:\n";
    std::cout << "   当你需要访问资源但不需要拥有它时，\n";
    std::cout << "   使用 weak_ptr。这允许资源在不再被\n";
    std::cout << "   需要时正确释放，同时提供安全的访问机制。\n";
}
