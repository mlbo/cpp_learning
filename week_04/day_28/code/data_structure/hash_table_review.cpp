/**
 * @file hash_table_review.cpp
 * @brief 哈希表综合复习示例
 * 
 * 涵盖内容：
 * 1. 哈希表基本操作
 * 2. 哈希函数设计
 * 3. 自定义类型的哈希支持
 * 4. 常见问题与最佳实践
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <chrono>

namespace hash_table_review {

// ========================================
// 自定义类型的哈希支持
// ========================================

/**
 * @brief 二维点结构体
 * 演示如何为自定义类型提供哈希支持
 */
struct Point {
    int x;
    int y;
    
    Point(int x_, int y_) : x(x_), y(y_) {}
    
    // 必须提供相等比较运算符
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    // 用于打印
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

/**
 * @brief Point的哈希函数
 * 使用标准的哈希组合技术
 */
struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        // 使用异或和移位组合两个坐标的哈希值
        // 这是一个简单但有效的哈希组合方法
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);  // 移位避免对称冲突
    }
};

// ========================================
// 简单哈希表实现演示
// ========================================

/**
 * @brief 简化的哈希表实现（教学目的）
 * 使用链地址法解决冲突
 */
template<typename K, typename V, typename Hash = std::hash<K>>
class SimpleHashTable {
private:
    // 桶节点
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    std::vector<Node*> buckets_;  // 桶数组
    size_t size_;                 // 元素数量
    float maxLoadFactor_;         // 最大装载因子
    Hash hash_;                   // 哈希函数
    
    // 计算桶索引
    size_t getBucketIndex(const K& key) const {
        return hash_(key) % buckets_.size();
    }
    
    // 重新哈希（扩容）
    void rehash(size_t newCapacity) {
        std::vector<Node*> newBuckets(newCapacity, nullptr);
        
        // 遍历所有旧桶
        for (Node* head : buckets_) {
            while (head) {
                Node* next = head->next;
                // 计算新位置
                size_t idx = hash_(head->key) % newCapacity;
                // 插入到新桶的头部
                head->next = newBuckets[idx];
                newBuckets[idx] = head;
                head = next;
            }
        }
        
        buckets_ = std::move(newBuckets);
    }
    
public:
    SimpleHashTable(size_t initCapacity = 16, float maxLF = 0.75)
        : buckets_(initCapacity, nullptr), size_(0), maxLoadFactor_(maxLF) {}
    
    ~SimpleHashTable() {
        clear();
    }
    
    void clear() {
        for (Node* head : buckets_) {
            while (head) {
                Node* next = head->next;
                delete head;
                head = next;
            }
        }
        std::fill(buckets_.begin(), buckets_.end(), nullptr);
        size_ = 0;
    }
    
    // 插入或更新
    void insert(const K& key, const V& value) {
        // 检查是否需要扩容
        if (static_cast<float>(size_ + 1) / buckets_.size() > maxLoadFactor_) {
            rehash(buckets_.size() * 2);
        }
        
        size_t idx = getBucketIndex(key);
        Node* curr = buckets_[idx];
        
        // 查找是否已存在
        while (curr) {
            if (curr->key == key) {
                curr->value = value;  // 更新
                return;
            }
            curr = curr->next;
        }
        
        // 插入新节点
        Node* newNode = new Node(key, value);
        newNode->next = buckets_[idx];
        buckets_[idx] = newNode;
        ++size_;
    }
    
    // 查找
    V* find(const K& key) {
        size_t idx = getBucketIndex(key);
        Node* curr = buckets_[idx];
        while (curr) {
            if (curr->key == key) {
                return &curr->value;
            }
            curr = curr->next;
        }
        return nullptr;
    }
    
    // 删除
    bool erase(const K& key) {
        size_t idx = getBucketIndex(key);
        Node* curr = buckets_[idx];
        Node* prev = nullptr;
        
        while (curr) {
            if (curr->key == key) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    buckets_[idx] = curr->next;
                }
                delete curr;
                --size_;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
    
    // 获取统计信息
    void printStats() const {
        std::cout << "哈希表统计信息:\n";
        std::cout << "  元素数量: " << size_ << "\n";
        std::cout << "  桶数量: " << buckets_.size() << "\n";
        std::cout << "  装载因子: " << static_cast<float>(size_) / buckets_.size() << "\n";
        
        // 计算链表长度分布
        size_t maxLen = 0;
        size_t emptyBuckets = 0;
        for (Node* head : buckets_) {
            if (!head) {
                ++emptyBuckets;
                continue;
            }
            size_t len = 0;
            while (head) {
                ++len;
                head = head->next;
            }
            maxLen = std::max(maxLen, len);
        }
        std::cout << "  空桶数量: " << emptyBuckets << "\n";
        std::cout << "  最长链表: " << maxLen << "\n";
    }
};

// ========================================
// 演示函数
// ========================================

void demonstrateBasicOperations() {
    std::cout << "=== 基本操作演示 ===\n\n";
    
    // 创建哈希表
    std::unordered_map<std::string, int> scores;
    
    // 插入元素
    std::cout << "插入元素:\n";
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    scores.emplace("David", 88);  // 使用emplace原地构造
    
    std::cout << "  当前元素数量: " << scores.size() << "\n";
    std::cout << "  桶数量: " << scores.bucket_count() << "\n";
    std::cout << "  装载因子: " << scores.load_factor() << "\n";
    
    // 查找元素
    std::cout << "\n查找元素:\n";
    std::string name = "Bob";
    auto it = scores.find(name);
    if (it != scores.end()) {
        std::cout << "  找到 " << name << ": " << it->second << "\n";
    }
    
    // 使用at()访问（会检查边界）
    try {
        std::cout << "  Alice的分数: " << scores.at("Alice") << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "  键不存在\n";
    }
    
    // 删除元素
    std::cout << "\n删除元素:\n";
    scores.erase("Bob");
    std::cout << "  删除Bob后，元素数量: " << scores.size() << "\n";
    
    // 遍历（注意：顺序不确定）
    std::cout << "\n遍历所有元素（顺序不确定）:\n";
    for (const auto& pair : scores) {
        std::cout << "  " << pair.first << ": " << pair.second << "\n";
    }
}

void demonstrateCustomTypeHash() {
    std::cout << "\n=== 自定义类型的哈希支持 ===\n\n";
    
    // 使用自定义哈希函数
    std::unordered_map<Point, std::string, PointHash> pointNames;
    
    pointNames[Point(0, 0)] = "原点";
    pointNames[Point(1, 0)] = "单位点X";
    pointNames[Point(0, 1)] = "单位点Y";
    pointNames[Point(3, 4)] = "特殊点";
    
    std::cout << "存储的点:\n";
    for (const auto& pair : pointNames) {
        std::cout << "  " << pair.first << " -> " << pair.second << "\n";
    }
    
    // 查找测试
    Point searchPoint(3, 4);
    auto it = pointNames.find(searchPoint);
    if (it != pointNames.end()) {
        std::cout << "\n找到 " << searchPoint << ": " << it->second << "\n";
    }
}

void demonstrateSimpleHashTable() {
    std::cout << "\n=== 简单哈希表实现演示 ===\n\n";
    
    SimpleHashTable<std::string, int> table(8);  // 初始容量8
    
    // 插入一些数据
    std::cout << "插入数据...\n";
    for (int i = 0; i < 20; ++i) {
        table.insert("key_" + std::to_string(i), i * 10);
    }
    
    table.printStats();
    
    // 查找测试
    std::cout << "\n查找测试:\n";
    int* val = table.find("key_5");
    if (val) {
        std::cout << "  找到 key_5: " << *val << "\n";
    }
    
    val = table.find("key_100");
    if (!val) {
        std::cout << "  key_100 不存在\n";
    }
    
    // 删除测试
    std::cout << "\n删除测试:\n";
    if (table.erase("key_10")) {
        std::cout << "  删除 key_10 成功\n";
    }
    
    table.printStats();
}

void demonstrateBestPractices() {
    std::cout << "\n=== 最佳实践演示 ===\n\n";
    
    // 1. 预分配容量
    std::cout << "1. 预分配容量避免频繁rehash:\n";
    std::unordered_map<int, std::string> map1;
    map1.reserve(10000);  // 预分配足够容量
    std::cout << "  预分配后桶数量: " << map1.bucket_count() << "\n";
    
    // 2. 自定义哈希函数的注意事项
    std::cout << "\n2. 好的哈希函数应该:\n";
    std::cout << "  - 计算效率高\n";
    std::cout << "  - 分布均匀\n";
    std::cout << "  - 确定性（相同输入=相同输出）\n";
    
    // 3. 使用emplace避免临时对象
    std::cout << "\n3. 使用emplace提高效率:\n";
    std::unordered_map<std::string, std::vector<int>> map2;
    map2.emplace("key1", std::vector<int>{1, 2, 3});  // 原地构造
    std::cout << "  emplace完成，vector大小: " << map2["key1"].size() << "\n";
    
    // 4. 使用unordered_set去重
    std::cout << "\n4. 使用unordered_set快速去重:\n";
    std::vector<int> data = {1, 2, 3, 2, 4, 3, 5, 1, 6};
    std::unordered_set<int> uniqueSet(data.begin(), data.end());
    std::cout << "  原始数据: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n  去重后: ";
    for (int x : uniqueSet) std::cout << x << " ";
    std::cout << "\n";
}

void demonstratePerformance() {
    std::cout << "\n=== 性能对比演示 ===\n\n";
    
    const int N = 100000;
    
    // 哈希表 vs 有序map
    std::unordered_map<int, int> hashMap;
    std::map<int, int> orderedMap;
    
    // 哈希表插入
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        hashMap[i] = i;
    }
    auto hashInsertTime = std::chrono::high_resolution_clock::now() - start;
    
    // 有序map插入
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        orderedMap[i] = i;
    }
    auto orderedInsertTime = std::chrono::high_resolution_clock::now() - start;
    
    // 哈希表查找
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        volatile int* p = &hashMap[i];
        (void)p;
    }
    auto hashFindTime = std::chrono::high_resolution_clock::now() - start;
    
    // 有序map查找
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        volatile int* p = &orderedMap[i];
        (void)p;
    }
    auto orderedFindTime = std::chrono::high_resolution_clock::now() - start;
    
    auto toMs = [](auto duration) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    };
    
    std::cout << "操作 " << N << " 个元素:\n";
    std::cout << "  哈希表插入: " << toMs(hashInsertTime) << " ms\n";
    std::cout << "  有序map插入: " << toMs(orderedInsertTime) << " ms\n";
    std::cout << "  哈希表查找: " << toMs(hashFindTime) << " ms\n";
    std::cout << "  有序map查找: " << toMs(orderedFindTime) << " ms\n";
}

void demonstrate() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║        哈希表综合复习                  ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    demonstrateBasicOperations();
    demonstrateCustomTypeHash();
    demonstrateSimpleHashTable();
    demonstrateBestPractices();
    demonstratePerformance();
    
    std::cout << "\n哈希表复习完成！\n";
}

} // namespace hash_table_review
