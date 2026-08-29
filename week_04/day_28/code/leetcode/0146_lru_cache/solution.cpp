/**
 * @file solution.cpp
 * @brief LeetCode 146: LRU缓存机制
 */

#include "solution.h"

#include <algorithm>
#include <memory>

struct LRUCache::DListNode {
    int key;
    int value;
    DListNode* prev;
    DListNode* next;

    DListNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DListNode(int nodeKey, int nodeValue)
        : key(nodeKey), value(nodeValue), prev(nullptr), next(nullptr) {}
};

LRUCache::LRUCache(int capacity)
    : capacity_(std::max(0, capacity)), size_(0), head_(nullptr), tail_(nullptr) {
    auto head = std::make_unique<DListNode>();
    auto tail = std::make_unique<DListNode>();
    head->next = tail.get();
    tail->prev = head.get();
    head_ = head.release();
    tail_ = tail.release();
}

LRUCache::~LRUCache() {
    DListNode* current = head_;
    while (current != nullptr) {
        DListNode* next = current->next;
        delete current;
        current = next;
    }
}

void LRUCache::removeNode(DListNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void LRUCache::addToHead(DListNode* node) {
    node->prev = head_;
    node->next = head_->next;
    head_->next->prev = node;
    head_->next = node;
}

void LRUCache::moveToHead(DListNode* node) {
    removeNode(node);
    addToHead(node);
}

LRUCache::DListNode* LRUCache::removeTail() {
    DListNode* node = tail_->prev;
    removeNode(node);
    return node;
}

int LRUCache::get(int key) {
    const auto it = cache_.find(key);
    if (it == cache_.end()) {
        return -1;
    }

    DListNode* node = it->second;
    moveToHead(node);
    return node->value;
}

void LRUCache::put(int key, int value) {
    if (capacity_ == 0) {
        return;
    }

    const auto existing = cache_.find(key);
    if (existing != cache_.end()) {
        existing->second->value = value;
        moveToHead(existing->second);
        return;
    }

    auto newNode = std::make_unique<DListNode>(key, value);
    DListNode* node = newNode.get();
    cache_.emplace(key, node);
    addToHead(node);
    newNode.release();
    ++size_;

    if (size_ > capacity_) {
        DListNode* removed = removeTail();
        cache_.erase(removed->key);
        delete removed;
        --size_;
    }
}

int LRUCache::size() const {
    return size_;
}

int LRUCache::capacity() const {
    return capacity_;
}
