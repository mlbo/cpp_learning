/**
 * @file solution.cpp
 * @brief LeetCode 460: LFU缓存
 * 
 * 题目描述：
 * 请你为最不经常使用（LFU）缓存算法设计并实现数据结构。
 * 实现 LFUCache 类：
 * - LFUCache(int capacity) 用数据结构的容量 capacity 初始化对象
 * - int get(int key) 如果键存在返回值，否则返回 -1
 * - void put(int key, int value) 如果键存在更新值，否则插入。当达到容量时，
 *   应该在插入新项之前移除最不经常使用的项。如果多个项使用频率相同，移除最久未使用的
 */

#include <unordered_map>

/**
 * @brief 双向链表节点
 */
struct Node {
    int key;
    int value;
    int freq;       // 访问频率
    Node* prev;
    Node* next;
    
    Node(int k = 0, int v = 0) 
        : key(k), value(v), freq(1), prev(nullptr), next(nullptr) {}
};

/**
 * @brief 双向链表类
 * 管理同一频率的所有节点
 */
class DList {
private:
    Node* head_;    // 虚拟头节点
    Node* tail_;    // 虚拟尾节点
    int size_;      // 链表大小

public:
    DList() : size_(0) {
        head_ = new Node();
        tail_ = new Node();
        head_->next = tail_;
        tail_->prev = head_;
    }
    
    ~DList() {
        Node* curr = head_;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    
    /**
     * @brief 检查链表是否为空
     */
    bool isEmpty() const { return size_ == 0; }
    
    /**
     * @brief 获取链表大小
     */
    int size() const { return size_; }
    
    /**
     * @brief 将节点添加到链表头部（最近使用位置）
     */
    void addToHead(Node* node) {
        node->prev = head_;
        node->next = head_->next;
        head_->next->prev = node;
        head_->next = node;
        ++size_;
    }
    
    /**
     * @brief 从链表中移除节点
     */
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        --size_;
    }
    
    /**
     * @brief 移除链表尾部节点（最久未使用）
     * @return 被移除的节点指针，调用者负责删除
     */
    Node* removeTail() {
        if (isEmpty()) return nullptr;
        Node* node = tail_->prev;
        removeNode(node);
        return node;
    }
};

/**
 * @brief LFU缓存类
 * 
 * 数据结构设计：
 * 1. keyMap_: key -> Node*，快速查找节点
 * 2. freqMap_: freq -> DList*，每个频率对应一个双向链表
 * 3. minFreq_: 记录当前最小频率，用于淘汰
 */
class LFUCache {
private:
    int capacity_;                              // 缓存容量
    int minFreq_;                               // 当前最小频率
    std::unordered_map<int, Node*> keyMap_;     // key -> Node
    std::unordered_map<int, DList*> freqMap_;   // freq -> DList

    /**
     * @brief 增加节点的访问频率
     * @param node 要更新的节点
     * 
     * 步骤：
     * 1. 从旧频率链表中移除节点
     * 2. 更新最小频率（如果需要）
     * 3. 将节点添加到新频率链表头部
     */
    void increaseFreq(Node* node) {
        int oldFreq = node->freq;
        
        // 从旧频率链表中移除
        freqMap_[oldFreq]->removeNode(node);
        
        // 如果旧频率是最小频率，且链表变空，更新minFreq
        if (oldFreq == minFreq_ && freqMap_[oldFreq]->isEmpty()) {
            ++minFreq_;
        }
        
        // 增加频率并添加到新链表
        int newFreq = oldFreq + 1;
        node->freq = newFreq;
        
        if (freqMap_.find(newFreq) == freqMap_.end()) {
            freqMap_[newFreq] = new DList();
        }
        freqMap_[newFreq]->addToHead(node);
    }

    /**
     * @brief 添加新节点
     * @param key 键
     * @param value 值
     */
    void addNode(int key, int value) {
        Node* newNode = new Node(key, value);
        keyMap_[key] = newNode;
        
        // 新节点频率为1
        minFreq_ = 1;
        if (freqMap_.find(1) == freqMap_.end()) {
            freqMap_[1] = new DList();
        }
        freqMap_[1]->addToHead(newNode);
    }

    /**
     * @brief 淘汰一个节点
     */
    void evict() {
        // 从最小频率链表的尾部移除（最久未使用）
        DList* minList = freqMap_[minFreq_];
        Node* removed = minList->removeTail();
        
        if (removed) {
            keyMap_.erase(removed->key);
            delete removed;
        }
    }

public:
    /**
     * @brief 构造函数
     * @param capacity 缓存容量
     */
    LFUCache(int capacity) : capacity_(capacity), minFreq_(0) {}
    
    /**
     * @brief 析构函数
     */
    ~LFUCache() {
        // 删除所有节点
        for (auto& pair : keyMap_) {
            delete pair.second;
        }
        // 删除所有链表（不包含节点）
        for (auto& pair : freqMap_) {
            // 链表中的虚拟节点在DList析构函数中删除
            pair.second->~DList();
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
        auto it = keyMap_.find(key);
        if (it == keyMap_.end()) {
            return -1;
        }
        
        Node* node = it->second;
        increaseFreq(node);
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
        if (capacity_ == 0) return;
        
        auto it = keyMap_.find(key);
        
        if (it != keyMap_.end()) {
            // 键已存在，更新值并增加频率
            Node* node = it->second;
            node->value = value;
            increaseFreq(node);
        } else {
            // 键不存在
            if (keyMap_.size() == capacity_) {
                // 容量已满，淘汰
                evict();
            }
            // 添加新节点
            addNode(key, value);
        }
    }
    
    /**
     * @brief 获取当前缓存大小
     */
    int size() const { return keyMap_.size(); }
    
    /**
     * @brief 获取缓存容量
     */
    int capacity() const { return capacity_; }
    
    /**
     * @brief 获取当前最小频率
     */
    int minFreq() const { return minFreq_; }
};
