#ifndef WEEK04_DAY28_LC0146_SOLUTION_H
#define WEEK04_DAY28_LC0146_SOLUTION_H

#include <unordered_map>

class LRUCache {
private:
    struct DListNode;

    int capacity_;
    int size_;
    DListNode* head_;
    DListNode* tail_;
    std::unordered_map<int, DListNode*> cache_;

    void moveToHead(DListNode* node);
    void removeNode(DListNode* node);
    void addToHead(DListNode* node);
    DListNode* removeTail();

public:
    explicit LRUCache(int capacity);
    ~LRUCache();

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = delete;
    LRUCache& operator=(LRUCache&&) = delete;

    int get(int key);
    void put(int key, int value);

    int size() const;
    int capacity() const;
};

#endif
