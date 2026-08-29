/**
 * @file solution.cpp
 * @brief LeetCode 460: LFU缓存
 */

#include "solution.h"

#include <algorithm>
#include <limits>
#include <memory>
#include <stdexcept>

struct LFUCache::Node {
    int key;
    int value;
    Frequency freq;
    Node* prev;
    Node* next;

    Node(int nodeKey = 0, int nodeValue = 0)
        : key(nodeKey), value(nodeValue), freq(1), prev(nullptr), next(nullptr) {}
};

class LFUCache::DList {
private:
    Node* head_;
    Node* tail_;
    std::size_t size_;

public:
    DList() : head_(nullptr), tail_(nullptr), size_(0) {
        auto head = std::make_unique<Node>();
        auto tail = std::make_unique<Node>();
        head->next = tail.get();
        tail->prev = head.get();
        head_ = head.release();
        tail_ = tail.release();
    }

    ~DList() {
        delete head_;
        delete tail_;
    }

    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;
    DList(DList&&) = delete;
    DList& operator=(DList&&) = delete;

    bool isEmpty() const {
        return size_ == 0;
    }

    void addToHead(Node* node) {
        node->prev = head_;
        node->next = head_->next;
        head_->next->prev = node;
        head_->next = node;
        ++size_;
    }

    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev = nullptr;
        node->next = nullptr;
        --size_;
    }

    Node* removeTail() {
        if (isEmpty()) {
            return nullptr;
        }
        Node* node = tail_->prev;
        removeNode(node);
        return node;
    }
};

LFUCache::LFUCache(int capacity)
    : LFUCache(capacity, std::numeric_limits<Frequency>::max()) {}

LFUCache::LFUCache(int capacity, Frequency frequencyCeiling)
    : capacity_(std::max(0, capacity)),
      minFreq_(0),
      frequencyCeiling_(frequencyCeiling == 0 ? 1 : frequencyCeiling) {
    if (capacity_ > 0) {
        const auto capacitySize = static_cast<std::size_t>(capacity_);
        // 新键 put 先取得全部可能失败的资源，再淘汰旧节点；瞬时可有 capacity + 1 项。
        keyMap_.reserve(capacitySize + 1);
        // 升频时会先建新桶再删空旧桶，瞬时最多 capacity + 1 个映射项。
        freqMap_.reserve(capacitySize + 1);
    }
}

LFUCache::~LFUCache() {
    for (auto& entry : keyMap_) {
        delete entry.second;
    }
    for (auto& bucket : freqMap_) {
        delete bucket.second;
    }
}

LFUCache::DList* LFUCache::getOrCreateBucket(Frequency frequency) {
    const auto existing = freqMap_.find(frequency);
    if (existing != freqMap_.end()) {
        return existing->second;
    }

    auto bucket = std::make_unique<DList>();
    DList* result = bucket.get();
    freqMap_.emplace(frequency, result);
    bucket.release();
    return result;
}

void LFUCache::eraseBucketIfEmpty(Frequency frequency) {
    const auto it = freqMap_.find(frequency);
    if (it == freqMap_.end() || !it->second->isEmpty()) {
        return;
    }
    delete it->second;
    freqMap_.erase(it);
}

void LFUCache::increaseFreq(Node* node) {
    const Frequency oldFrequency = node->freq;
    DList* oldBucket = freqMap_.at(oldFrequency);

    // 达到可表示上界后不再递增，只更新同频桶内的 LRU 顺序。
    // 默认上界是 uint64_t 最大值；第二个构造参数用于有限步测试边界策略。
    if (oldFrequency >= frequencyCeiling_) {
        oldBucket->removeNode(node);
        oldBucket->addToHead(node);
        return;
    }

    const Frequency newFrequency = oldFrequency + 1;
    DList* newBucket = getOrCreateBucket(newFrequency);

    oldBucket->removeNode(node);
    node->freq = newFrequency;
    newBucket->addToHead(node);

    if (oldFrequency == minFreq_ && oldBucket->isEmpty()) {
        minFreq_ = newFrequency;
    }
    eraseBucketIfEmpty(oldFrequency);
}

void LFUCache::addNode(int key, int value) {
    auto node = std::make_unique<Node>(key, value);
    const bool createdFirstBucket = freqMap_.find(1) == freqMap_.end();
    DList* firstBucket = getOrCreateBucket(1);
    Node* rawNode = node.get();
    try {
        const auto insertion = keyMap_.emplace(key, rawNode);
        if (!insertion.second) {
            throw std::logic_error("LFU 新键插入前置条件被破坏");
        }
    } catch (...) {
        if (createdFirstBucket) {
            eraseBucketIfEmpty(1);
        }
        throw;
    }
    firstBucket->addToHead(rawNode);
    node.release();
    minFreq_ = 1;
}

void LFUCache::evict(Frequency frequency) {
    const auto bucketIt = freqMap_.find(frequency);
    if (bucketIt == freqMap_.end()) {
        return;
    }

    Node* removed = bucketIt->second->removeTail();
    if (removed == nullptr) {
        return;
    }

    keyMap_.erase(removed->key);
    delete removed;
    eraseBucketIfEmpty(frequency);
    if (keyMap_.empty()) {
        minFreq_ = 0;
    }
}

int LFUCache::get(int key) {
    const auto it = keyMap_.find(key);
    if (it == keyMap_.end()) {
        return -1;
    }

    Node* node = it->second;
    increaseFreq(node);
    return node->value;
}

void LFUCache::put(int key, int value) {
    if (capacity_ == 0) {
        return;
    }

    const auto existing = keyMap_.find(key);
    if (existing != keyMap_.end()) {
        increaseFreq(existing->second);
        existing->second->value = value;
        return;
    }

    if (keyMap_.size() == static_cast<std::size_t>(capacity_)) {
        const Frequency evictFrequency = minFreq_;
        // addNode 的分配或哈希插入若失败，旧缓存尚未淘汰；成功后再走不抛的摘链路径。
        addNode(key, value);
        evict(evictFrequency);
        return;
    }
    addNode(key, value);
}

std::size_t LFUCache::size() const {
    return keyMap_.size();
}

int LFUCache::capacity() const {
    return capacity_;
}

LFUCache::Frequency LFUCache::minFreq() const {
    return minFreq_;
}

std::size_t LFUCache::frequencyBucketCount() const {
    return freqMap_.size();
}
