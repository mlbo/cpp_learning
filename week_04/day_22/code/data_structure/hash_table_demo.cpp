/**
 * Day 22: 哈希表演示
 * 实现文件
 */

#include "hash_table_demo.h"
#include <algorithm>

// ==================== 简单哈希表实现 ====================

SimpleHashTable::SimpleHashTable() {
    // 初始化所有桶为空
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

SimpleHashTable::~SimpleHashTable() {
    // 释放所有节点
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
}

int SimpleHashTable::hash(const std::string& key) {
    // 简单的哈希函数：所有字符ASCII码之和取模
    int sum = 0;
    for (char c : key) {
        sum += static_cast<int>(c);
    }
    return sum % TABLE_SIZE;
}

void SimpleHashTable::insert(const std::string& key, int value) {
    int index = hash(key);
    
    // 检查是否已存在
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) {
            curr->value = value;  // 更新
            return;
        }
        curr = curr->next;
    }
    
    // 插入新节点（头插法）
    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    table[index] = newNode;
}

int SimpleHashTable::get(const std::string& key) {
    int index = hash(key);
    Node* curr = table[index];
    
    while (curr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    
    return -1;  // 未找到
}

void SimpleHashTable::remove(const std::string& key) {
    int index = hash(key);
    Node* curr = table[index];
    Node* prev = nullptr;
    
    while (curr) {
        if (curr->key == key) {
            if (prev) {
                prev->next = curr->next;
            } else {
                table[index] = curr->next;
            }
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void SimpleHashTable::print() {
    std::cout << "哈希表内容：" << std::endl;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        std::cout << "  Bucket[" << i << "]: ";
        Node* curr = table[i];
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
    std::cout << "  apple -> " << ht.get("apple") << std::endl;
    std::cout << "  banana -> " << ht.get("banana") << std::endl;
    std::cout << "  unknown -> " << ht.get("unknown") << std::endl;
    
    // 删除操作
    std::cout << "\n删除 banana 后：" << std::endl;
    ht.remove("banana");
    ht.print();
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
