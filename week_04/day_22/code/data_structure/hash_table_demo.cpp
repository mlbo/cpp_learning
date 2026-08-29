/**
 * Day 22: 哈希表演示
 * 实现文件
 */

#include "hash_table_demo.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

struct CaseInsensitiveHash {
    std::size_t operator()(const std::string& text) const noexcept {
        std::size_t result = 0;
        for (char character : text) {
            // string 按 char 存储；在进入 cctype 和哈希运算前显式解释为字节，
            // 避免由实现决定的 char 符号性悄悄改变高位字节的含义。
            const auto byte = static_cast<unsigned char>(character);
            const auto lower = static_cast<unsigned char>(std::tolower(byte));
            result = result * 131U + lower;
        }
        return result;
    }
};

struct CaseInsensitiveEqual {
    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            const auto left = static_cast<unsigned char>(lhs[i]);
            const auto right = static_cast<unsigned char>(rhs[i]);
            if (std::tolower(left) != std::tolower(right)) {
                return false;
            }
        }
        return true;
    }
};

} // namespace

// ==================== 简单哈希表实现 ====================

SimpleHashTable::SimpleHashTable() = default;

SimpleHashTable::~SimpleHashTable() {
    clear();
}

void SimpleHashTable::clear() noexcept {
    // 释放所有节点
    for (Node*& bucket : table_) {
        Node* curr = bucket;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
        bucket = nullptr;
    }
    size_ = 0;
}

std::size_t SimpleHashTable::hash(const std::string& key) const noexcept {
    // 教学用哈希：所有字节值之和取模。unsigned char 避免 char 为有符号时
    // 高位字节变成负数，进而产生负下标。
    std::size_t sum = 0;
    for (char character : key) {
        const auto byte = static_cast<unsigned char>(character);
        sum += byte;
    }
    return sum % BUCKET_COUNT;
}

void SimpleHashTable::insert(const std::string& key, int value) {
    const std::size_t index = hash(key);
    
    // 检查是否已存在
    Node* curr = table_[index];
    while (curr) {
        if (curr->key == key) {
            curr->value = value;  // 更新
            return;
        }
        curr = curr->next;
    }
    
    // 插入新节点（头插法）
    Node* newNode = new Node(key, value);
    newNode->next = table_[index];
    table_[index] = newNode;
    ++size_;
}

std::optional<int> SimpleHashTable::get(const std::string& key) const {
    const std::size_t index = hash(key);
    const Node* curr = table_[index];
    
    while (curr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    
    return std::nullopt;
}

bool SimpleHashTable::contains(const std::string& key) const {
    return get(key).has_value();
}

bool SimpleHashTable::remove(const std::string& key) {
    const std::size_t index = hash(key);
    Node* curr = table_[index];
    Node* prev = nullptr;
    
    while (curr) {
        if (curr->key == key) {
            if (prev) {
                prev->next = curr->next;
            } else {
                table_[index] = curr->next;
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

double SimpleHashTable::loadFactor() const noexcept {
    return static_cast<double>(size_) / static_cast<double>(BUCKET_COUNT);
}

void SimpleHashTable::print() const {
    std::cout << "哈希表内容：" << std::endl;
    for (std::size_t i = 0; i < BUCKET_COUNT; ++i) {
        std::cout << "  Bucket[" << i << "]: ";
        const Node* curr = table_[i];
        while (curr) {
            std::cout << "(" << curr->key << "," << curr->value << ") ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }
}

// ==================== 演示函数 ====================

void simpleHashTableDemo() {
    std::cout << "\n--- 简单哈希表实现演示 ---" << std::endl;
    
    SimpleHashTable ht;
    
    // 插入操作
    ht.insert("apple", 1);
    ht.insert("banana", 2);
    ht.insert("cherry", 3);
    ht.insert("date", 4);
    
    ht.print();
    
    // 查找操作
    std::cout << "\n查找操作：" << std::endl;
    const auto printLookup = [&ht](const std::string& key) {
        const auto value = ht.get(key);
        std::cout << "  " << key << " -> ";
        if (value) {
            std::cout << *value;
        } else {
            std::cout << "未找到";
        }
        std::cout << std::endl;
    };
    printLookup("apple");
    printLookup("banana");
    printLookup("unknown");
    
    // 删除操作
    std::cout << "\n删除 banana 后：" << std::endl;
    ht.remove("banana");
    ht.print();
    std::cout << "  当前元素数: " << ht.size()
              << "，负载因子: " << ht.loadFactor() << std::endl;
}

void unorderedMapDemo() {
    std::cout << "\n--- unordered_map 演示 ---" << std::endl;
    
    // 创建和插入
    std::unordered_map<std::string, int> scores;
    
    // 方式1：使用[]
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    
    // 方式2：使用insert
    scores.insert({"Charlie", 92});
    
    // 方式3：使用emplace（更高效）
    scores.emplace("David", 88);
    
    // 查找
    std::cout << "查找操作：" << std::endl;
    std::cout << "  Alice's score: " << scores["Alice"] << std::endl;
    std::cout << "  Bob's score: " << scores.at("Bob") << std::endl;
    
    // 安全查找（避免自动插入）
    auto it = scores.find("Eve");
    if (it != scores.end()) {
        std::cout << "  Eve's score: " << it->second << std::endl;
    } else {
        std::cout << "  Eve not found" << std::endl;
    }
    
    // 检查是否存在
    std::cout << "\n检查存在性：" << std::endl;
    std::cout << "  Alice exists: " << (scores.count("Alice") ? "Yes" : "No") << std::endl;
    std::cout << "  Eve exists: " << (scores.count("Eve") ? "Yes" : "No") << std::endl;
    
    // 遍历
    std::cout << "\n遍历所有元素：" << std::endl;
    for (const auto& [name, score] : scores) {
        std::cout << "  " << name << ": " << score << std::endl;
    }
    
    // 删除
    scores.erase("Bob");
    std::cout << "\n删除Bob后的大小: " << scores.size() << std::endl;
    
    // 统计信息
    std::cout << "\n哈希表统计：" << std::endl;
    std::cout << "  桶数量: " << scores.bucket_count() << std::endl;
    std::cout << "  负载因子: " << scores.load_factor() << std::endl;
    std::cout << "  最大负载因子: " << scores.max_load_factor() << std::endl;

    std::cout << "\n自定义哈希与相等谓词：" << std::endl;
    std::unordered_map<std::string, int, CaseInsensitiveHash,
                       CaseInsensitiveEqual> caseInsensitiveScores;
    caseInsensitiveScores.emplace("Alice", 95);
    caseInsensitiveScores["ALICE"] = 96;
    std::cout << "  Alice 与 ALICE 被判为同一个键，元素数: "
              << caseInsensitiveScores.size() << std::endl;
    std::cout << "  查询 alice: " << caseInsensitiveScores.at("alice") << std::endl;

    std::cout << "\n负载因子实验（观察值依赖具体标准库实现）：" << std::endl;
    std::unordered_map<int, int> loadExperiment;
    loadExperiment.max_load_factor(0.5F);
    loadExperiment.reserve(8);
    const auto bucketsBefore = loadExperiment.bucket_count();
    for (int i = 0; i < 8; ++i) {
        loadExperiment.emplace(i, i * i);
    }
    std::cout << "  reserve后桶数: " << bucketsBefore
              << "，插入后桶数: " << loadExperiment.bucket_count()
              << "，当前负载因子: " << loadExperiment.load_factor() << std::endl;
    std::cout << "  结论：负载因子影响冲突概率与扩容，但桶数不是可移植常量。" << std::endl;
}

void unorderedSetDemo() {
    std::cout << "\n--- unordered_set 演示 ---" << std::endl;
    
    std::unordered_set<int> nums;
    
    // 插入
    nums.insert(1);
    nums.insert(2);
    nums.insert(3);
    nums.insert(2);  // 重复元素不会插入
    
    std::cout << "插入 1,2,3,2 后的元素：" << std::endl;
    std::cout << "  ";
    for (int n : nums) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 查找
    std::cout << "\n查找操作：" << std::endl;
    std::cout << "  1 exists: " << (nums.count(1) ? "Yes" : "No") << std::endl;
    std::cout << "  5 exists: " << (nums.count(5) ? "Yes" : "No") << std::endl;
    
    // 删除
    nums.erase(2);
    std::cout << "\n删除2后的元素数量: " << nums.size() << std::endl;
    
    // 集合操作示例
    std::cout << "\n--- 集合操作示例 ---" << std::endl;
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {4, 5, 6, 7, 8};
    
    std::unordered_set<int> set1(vec1.begin(), vec1.end());
    std::unordered_set<int> set2(vec2.begin(), vec2.end());
    
    // 找交集
    std::cout << "交集元素: ";
    for (int n : set1) {
        if (set2.count(n)) {
            std::cout << n << " ";
        }
    }
    std::cout << std::endl;
    
    // 找差集
    std::cout << "vec1独有的元素: ";
    for (int n : set1) {
        if (!set2.count(n)) {
            std::cout << n << " ";
        }
    }
    std::cout << std::endl;
}

void hashTableDemo() {
    std::cout << "========== 哈希表演示 ==========" << std::endl;
    
    // 1. 简单哈希表实现
    simpleHashTableDemo();
    
    // 2. unordered_map
    unorderedMapDemo();
    
    // 3. unordered_set
    unorderedSetDemo();
    
    // 4. 哈希表性能说明
    std::cout << "\n--- 哈希表性能说明 ---" << std::endl;
    std::cout << "  平均时间复杂度：O(1)" << std::endl;
    std::cout << "  最坏时间复杂度：O(n)（所有键都冲突）" << std::endl;
    std::cout << "  空间复杂度：O(n)" << std::endl;
    std::cout << "  适用场景：快速查找、去重、计数" << std::endl;
}
