/**
 * Day 22: 哈希表演示
 * 头文件
 */

#ifndef HASH_TABLE_DEMO_H
#define HASH_TABLE_DEMO_H

#include <array>
#include <cstddef>
#include <optional>
#include <string>

/**
 * 哈希表演示函数
 * 展示unordered_map和unordered_set的基本用法
 */
void hashTableDemo();

/**
 * 简单哈希表实现演示
 * 使用链地址法解决冲突
 */
class SimpleHashTable {
public:
    static constexpr std::size_t BUCKET_COUNT = 10;
    
    struct Node {
        std::string key;
        int value;
        Node* next;
        Node(const std::string& k, int v) : key(k), value(v), next(nullptr) {}
    };
    
    SimpleHashTable();
    ~SimpleHashTable();

    // 每个节点只能由一个哈希表释放。浅复制桶指针会造成重复释放，
    // 所以在尚未实现深拷贝前明确禁止复制和移动。
    SimpleHashTable(const SimpleHashTable&) = delete;
    SimpleHashTable& operator=(const SimpleHashTable&) = delete;
    SimpleHashTable(SimpleHashTable&&) = delete;
    SimpleHashTable& operator=(SimpleHashTable&&) = delete;
    
    void insert(const std::string& key, int value);
    std::optional<int> get(const std::string& key) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);
    std::size_t size() const noexcept { return size_; }
    double loadFactor() const noexcept;
    void print() const;
    
private:
    std::array<Node*, BUCKET_COUNT> table_{};
    std::size_t size_ = 0;

    std::size_t hash(const std::string& key) const noexcept;
    void clear() noexcept;
};

/**
 * 演示简单哈希表实现
 */
void simpleHashTableDemo();

/**
 * 演示unordered_map的常用操作
 */
void unorderedMapDemo();

/**
 * 演示unordered_set的常用操作
 */
void unorderedSetDemo();

#endif // HASH_TABLE_DEMO_H
