#include "data_structure/simple_hash_table.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace {

int failures = 0;

void expect(bool condition, const char* caseName) {
    std::cout << caseName << ": " << (condition ? "通过" : "失败") << '\n';
    if (!condition) {
        ++failures;
    }
}

struct ThrowState {
    bool enabled = false;
    std::size_t calls = 0;
    std::size_t throwOnCall = 0;
};

struct ThrowingHash {
    std::shared_ptr<ThrowState> state;

    std::size_t operator()(int key) const {
        if (state->enabled && state->calls++ == state->throwOnCall) {
            throw std::runtime_error("injected hash failure");
        }
        return static_cast<std::size_t>(key);
    }
};

struct ThrowingValue {
    static bool throwOnCopy;
    int payload;

    explicit ThrowingValue(int value) : payload(value) {}

    ThrowingValue(const ThrowingValue& other) : payload(other.payload) {
        if (throwOnCopy) {
            throw std::runtime_error("injected value copy failure");
        }
    }

    ThrowingValue& operator=(const ThrowingValue& other) {
        payload = -999; // 模拟可能先改写一部分状态、随后失败的赋值实现。
        if (throwOnCopy) {
            throw std::runtime_error("injected value assignment failure");
        }
        payload = other.payload;
        return *this;
    }
};

bool ThrowingValue::throwOnCopy = false;

void testThrowingHashDuringRehash() {
    auto state = std::make_shared<ThrowState>();
    hash_table_review::SimpleHashTable<int, int, ThrowingHash> table(
        2, 1.0, ThrowingHash{state});
    table.insert(0, 0);
    table.insert(4, 400); // 与 key=0 位于同一条旧链。

    const std::size_t oldSize = table.size();
    const std::size_t oldBucketCount = table.bucketCount();
    state->enabled = true;
    state->calls = 0;
    state->throwOnCall = 2; // 新键一次、旧键一次成功后，在第二个旧键处抛出。

    bool threw = false;
    try {
        table.insert(8, 800);
    } catch (const std::runtime_error&) {
        threw = true;
    }
    state->enabled = false;

    const int* key0 = table.find(0);
    const int* key4 = table.find(4);
    const int* key8 = table.find(8);
    std::cout << "threw=" << threw
              << " key0=" << (key0 == nullptr ? -1 : *key0)
              << " key4=" << (key4 == nullptr ? -1 : *key4) << '\n';

    expect(threw, "自定义Hash在rehash中途抛出");
    expect(key0 != nullptr && *key0 == 0 && key4 != nullptr && *key4 == 400,
           "rehash失败后所有旧键值保持");
    expect(key8 == nullptr && table.size() == oldSize &&
               table.bucketCount() == oldBucketCount,
           "rehash失败后新键、size与桶拓扑均未提交");

    table.insert(8, 800);
    expect(table.find(0) != nullptr && table.find(4) != nullptr &&
               table.find(8) != nullptr && *table.find(8) == 800,
           "关闭故障后同一张表仍可扩容并继续使用");
}

void testThrowingValueDuringUpdate() {
    hash_table_review::SimpleHashTable<int, ThrowingValue> table(4, 1.0);
    table.insert(1, ThrowingValue{10});
    const std::size_t oldBucketCount = table.bucketCount();

    ThrowingValue::throwOnCopy = true;
    bool threw = false;
    try {
        table.insert(1, ThrowingValue{20});
    } catch (const std::runtime_error&) {
        threw = true;
    }
    ThrowingValue::throwOnCopy = false;

    const ThrowingValue* preserved = table.find(1);
    expect(threw, "更新已有键时值复制失败会传播");
    expect(preserved != nullptr && preserved->payload == 10 &&
               table.size() == 1 && table.bucketCount() == oldBucketCount,
           "更新失败后旧值与结构保持不变");

    table.insert(1, ThrowingValue{20});
    expect(table.find(1) != nullptr && table.find(1)->payload == 20 &&
               table.size() == 1 && table.bucketCount() == oldBucketCount,
           "成功更新不误触发扩容且不增加size");
}

} // namespace

int main() {
    using Table = hash_table_review::SimpleHashTable<int, int>;
    static_assert(!std::is_copy_constructible_v<Table>);
    static_assert(!std::is_move_constructible_v<Table>);

    testThrowingHashDuringRehash();
    testThrowingValueDuringUpdate();

    if (failures != 0) {
        std::cerr << failures << " Day 28 hash-table exception test(s) failed\n";
        return 1;
    }
    std::cout << "Day 28 hash-table exception contracts passed\n";
    return 0;
}
