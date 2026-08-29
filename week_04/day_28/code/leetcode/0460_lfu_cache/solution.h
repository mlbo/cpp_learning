#ifndef WEEK04_DAY28_LC0460_SOLUTION_H
#define WEEK04_DAY28_LC0460_SOLUTION_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>

class LFUCache {
public:
    using Frequency = std::uint64_t;

    explicit LFUCache(int capacity);
    LFUCache(int capacity, Frequency frequencyCeiling);
    ~LFUCache();

    LFUCache(const LFUCache&) = delete;
    LFUCache& operator=(const LFUCache&) = delete;
    LFUCache(LFUCache&&) = delete;
    LFUCache& operator=(LFUCache&&) = delete;

    int get(int key);
    void put(int key, int value);

    std::size_t size() const;
    int capacity() const;
    Frequency minFreq() const;
    std::size_t frequencyBucketCount() const;

private:
    struct Node;
    class DList;

    int capacity_;
    Frequency minFreq_;
    Frequency frequencyCeiling_;
    std::unordered_map<int, Node*> keyMap_;
    std::unordered_map<Frequency, DList*> freqMap_;

    DList* getOrCreateBucket(Frequency frequency);
    void eraseBucketIfEmpty(Frequency frequency);
    void increaseFreq(Node* node);
    void addNode(int key, int value);
    void evict(Frequency frequency);
};

#endif
