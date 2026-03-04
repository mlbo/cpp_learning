/**
 * @file solution.cpp
 * @brief LeetCode 146: LRU缓存机制
 * 
 * 题目描述：
 * 运用你所掌握的数据结构，设计和实现一个LRU (最近最少使用) 缓存机制。
 * 实现 LRUCache 类：
 * - LRUCache(int capacity) 以正整数作为容量 capacity 初始化 LRU 缓存
 * - int get(int key) 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1
 * - void put(int key, int value) 如果关键字存在则更新值，不存在则插入
 * - get 和 put 操作必须以 O(1) 平均时间复杂度运行
 */

#include <unordered_map>

// 双向链表节点
struct DListNode {
    int key;
    int value;
    DListNode* prev;
    DListNode* next;
    
    DListNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DListNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

/**
 * @brief LRU缓存类
 * 
 * 使用哈希表 + 双向链表的组合实现O(1)时间复杂度：
 * - 哈希表：快速查找节点
 * - 双向链表：维护访问顺序
 */
class LRUCache {
private:
    int capacity_;                    // 缓存容量
    int size_;                        // 当前元素数量
    DListNode* head_;                 // 虚拟头节点
    DListNode* tail_;                 // 虚拟尾节点
    std::unordered_map<int, DListNode*> cache_;  // 哈希表：key -> Node*

    /**
     * @brief 将节点移动到链表头部（最近使用位置）
     * @param node 要移动的节点
     */
    void moveToHead(DListNode* node) {
        removeNode(node);
        addToHead(node);
    }

    /**
     * @brief 从链表中移除节点
     * @param node 要移除的节点
     */
    void removeNode(DListNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    /**
     * @brief 将节点添加到链表头部
     * @param node 要添加的节点
     */
    void addToHead(DListNode* node) {
        node->prev = head_;
        node->next = head_->next;
        head_->next->prev = node;
        head_->next = node;
    }

    /**
     * @brief 移除链表尾部节点（最久未使用）
     * @return 被移除的节点指针
     */
    DListNode* removeTail() {
        DListNode* node = tail_->prev;
        removeNode(node);
        return node;
    }

public:
    /**
     * @brief 构造函数
     * @param capacity 缓存容量
     */
    LRUCache(int capacity) 
        : capacity_(capacity), size_(0) {
        // 创建虚拟头尾节点，简化边界处理
        head_ = new DListNode();
        tail_ = new DListNode();
        head_->next = tail_;
        tail_->prev = head_;
    }

    /**
     * @brief 析构函数
     */
    ~LRUCache() {
        // 释放所有节点
        DListNode* curr = head_;
        while (curr) {
            DListNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    /**
     * @brief 获取缓存值
     * @param key 键
     * @return 值，不存在返回-1
     * 
     * 时间复杂度: O(1)
     */
    int get(int key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            return -1;  // 未找到
        }
        
        // 命中，将节点移到头部（最近使用）
        DListNode* node = it->second;
        moveToHead(node);
        return node->value;
    }

    /**
     * @brief 插入或更新缓存
     * @param key 键
     * @param value 值
     * 
     * 时间复杂度: O(1)
     */
    void put(int key, int value) {
        auto it = cache_.find(key);
        
        if (it != cache_.end()) {
            // 键已存在，更新值并移到头部
            DListNode* node = it->second;
            node->value = value;
            moveToHead(node);
        } else {
            // 键不存在，创建新节点
            DListNode* newNode = new DListNode(key, value);
            cache_[key] = newNode;
            addToHead(newNode);
            ++size_;
            
            // 检查是否超出容量
            if (size_ > capacity_) {
                // 淘汰尾部节点（最久未使用）
                DListNode* removed = removeTail();
                cache_.erase(removed->key);
                delete removed;
                --size_;
            }
        }
    }
    
    /**
     * @brief 获取当前缓存大小
     */
    int size() const { return size_; }
    
    /**
     * @brief 获取缓存容量
     */
    int capacity() const { return capacity_; }
};
