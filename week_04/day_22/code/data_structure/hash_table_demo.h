/**
 * Day 22: 哈希表演示
 * 头文件
 */

#ifndef HASH_TABLE_DEMO_H
#define HASH_TABLE_DEMO_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

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
    static const int TABLE_SIZE = 10;
    
    struct Node {
        std::string key;
        int value;
        Node* next;
        Node(const std::string& k, int v) : key(k), value(v), next(nullptr) {}
    };
    
    SimpleHashTable();
    ~SimpleHashTable();
    
    void insert(const std::string& key, int value);
    int get(const std::string& key);
    void remove(const std::string& key);
    void print();
    
private:
    Node* table[TABLE_SIZE];
    int hash(const std::string& key);
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
