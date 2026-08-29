#ifndef WEEK04_DAY28_SIMPLE_HASH_TABLE_H
#define WEEK04_DAY28_SIMPLE_HASH_TABLE_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace hash_table_review {

/**
 * @brief 使用链地址法的教学哈希表
 *
 * 模板实现必须放在头文件中，调用方才能用自己的 K、V 和 Hash 实例化并测试。
 * insert 提供强异常保证：哈希、相等比较、分配或复制失败时，表中的键值、
 * size 和桶拓扑保持不变。用户提供的 Hash 或 K::operator== 在外部计数器、日志
 * 等对象上的副作用不在回滚范围内；Hash 还必须在键存于表中期间为等价键给出
 * 稳定且一致的哈希值，相等比较也不能改写已存键。
 */
template<typename K, typename V, typename Hash = std::hash<K>>
class SimpleHashTable {
private:
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& nodeKey, const V& nodeValue)
            : key(nodeKey), value(nodeValue), next(nullptr) {}
    };

    using size_type = std::size_t;
    using Relocation = std::pair<Node*, size_type>;

    std::vector<Node*> buckets_;
    size_type size_;
    double maxLoadFactor_;
    Hash hash_;

    static size_type bucketIndex(size_type hashCode, size_type bucketCount) noexcept {
        return hashCode % bucketCount;
    }

    Node** findLink(const K& key, size_type hashCode) {
        Node** link = &buckets_[bucketIndex(hashCode, buckets_.size())];
        while (*link != nullptr && !((*link)->key == key)) {
            link = &((*link)->next);
        }
        return link;
    }

    const Node* findNode(const K& key, size_type hashCode) const {
        const Node* current = buckets_[bucketIndex(hashCode, buckets_.size())];
        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool needsGrowth() const noexcept {
        const long double nextSize = static_cast<long double>(size_) + 1.0L;
        const long double threshold =
            static_cast<long double>(maxLoadFactor_) *
            static_cast<long double>(buckets_.size());
        return nextSize > threshold;
    }

    size_type checkedGrowthCapacity() const {
        if (buckets_.size() > buckets_.max_size() / 2) {
            throw std::length_error("SimpleHashTable 桶数量无法继续翻倍");
        }
        return buckets_.size() * 2;
    }

    void rehash(size_type newCapacity) {
        // prepare：分配新桶并计算完整迁移计划；任何异常都还没有改写旧 next。
        std::vector<Node*> newBuckets(newCapacity, nullptr);
        std::vector<Relocation> relocationPlan;
        relocationPlan.reserve(size_);

        for (Node* head : buckets_) {
            for (Node* current = head; current != nullptr; current = current->next) {
                const size_type hashCode = hash_(current->key);
                relocationPlan.emplace_back(
                    current, bucketIndex(hashCode, newCapacity));
            }
        }

        // commit：从这里开始只改写指针和交换 vector，不再调用用户代码或分配。
        for (const auto& relocation : relocationPlan) {
            Node* node = relocation.first;
            const size_type index = relocation.second;
            node->next = newBuckets[index];
            newBuckets[index] = node;
        }
        buckets_.swap(newBuckets);
    }

public:
    explicit SimpleHashTable(size_type initCapacity = 16,
                             double maxLoadFactor = 0.75,
                             Hash hash = Hash{})
        : buckets_(std::max<size_type>(1, initCapacity), nullptr),
          size_(0),
          maxLoadFactor_(
              maxLoadFactor > 0.0 && std::isfinite(maxLoadFactor)
                  ? maxLoadFactor
                  : 0.75),
          hash_(std::move(hash)) {}

    ~SimpleHashTable() {
        clear();
    }

    SimpleHashTable(const SimpleHashTable&) = delete;
    SimpleHashTable& operator=(const SimpleHashTable&) = delete;
    SimpleHashTable(SimpleHashTable&&) = delete;
    SimpleHashTable& operator=(SimpleHashTable&&) = delete;

    void clear() noexcept {
        for (Node* head : buckets_) {
            while (head != nullptr) {
                Node* next = head->next;
                delete head;
                head = next;
            }
        }
        std::fill(buckets_.begin(), buckets_.end(), nullptr);
        size_ = 0;
    }

    void insert(const K& key, const V& value) {
        // 先完成哈希与查找；相等比较抛异常时，表还没有变化。
        const size_type keyHash = hash_(key);
        Node** existingLink = findLink(key, keyHash);
        if (*existingLink != nullptr) {
            // 不直接调用可能只完成一半的 V::operator=；先构造完整替代节点再提交。
            auto replacement = std::make_unique<Node>(key, value);
            Node* oldNode = *existingLink;
            replacement->next = oldNode->next;
            *existingLink = replacement.release();
            delete oldNode;
            return;
        }

        if (size_ == std::numeric_limits<size_type>::max()) {
            throw std::length_error("SimpleHashTable 元素数量已达表示上限");
        }

        // 节点复制/分配也先于 rehash 和链表提交。
        auto newNode = std::make_unique<Node>(key, value);
        if (needsGrowth()) {
            rehash(checkedGrowthCapacity());
        }

        const size_type index = bucketIndex(keyHash, buckets_.size());
        newNode->next = buckets_[index];
        buckets_[index] = newNode.release();
        ++size_;
    }

    V* find(const K& key) {
        const size_type hashCode = hash_(key);
        Node* node = *findLink(key, hashCode);
        return node == nullptr ? nullptr : &node->value;
    }

    const V* find(const K& key) const {
        const size_type hashCode = hash_(key);
        const Node* node = findNode(key, hashCode);
        return node == nullptr ? nullptr : &node->value;
    }

    bool erase(const K& key) {
        const size_type hashCode = hash_(key);
        Node** link = findLink(key, hashCode);
        if (*link == nullptr) {
            return false;
        }

        Node* removed = *link;
        *link = removed->next;
        delete removed;
        --size_;
        return true;
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type bucketCount() const noexcept {
        return buckets_.size();
    }

    double loadFactor() const noexcept {
        return static_cast<double>(size_) /
               static_cast<double>(buckets_.size());
    }

    void printStats() const {
        std::cout << "哈希表统计信息:\n";
        std::cout << "  元素数量: " << size_ << "\n";
        std::cout << "  桶数量: " << buckets_.size() << "\n";
        std::cout << "  装载因子: " << loadFactor() << "\n";

        size_type maxLength = 0;
        size_type emptyBuckets = 0;
        for (Node* head : buckets_) {
            if (head == nullptr) {
                ++emptyBuckets;
                continue;
            }
            size_type length = 0;
            for (Node* current = head; current != nullptr; current = current->next) {
                ++length;
            }
            maxLength = std::max(maxLength, length);
        }
        std::cout << "  空桶数量: " << emptyBuckets << "\n";
        std::cout << "  最长链表: " << maxLength << "\n";
    }
};

} // namespace hash_table_review

#endif
