#include "cpp11_features/rvalue_reference_demo.h"
#include "data_structure/hash_table_demo.h"
#include "leetcode/0242_valid_anagram/solution.h"
#include "leetcode/0383_ransom_note/solution.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <type_traits>

namespace {

void expect(bool condition, const std::string& name, int& failures) {
    std::cout << name << ": " << (condition ? "通过" : "失败") << '\n';
    if (!condition) {
        ++failures;
    }
}

template<typename Callable>
void expectInvalidArgument(Callable action, const std::string& name, int& failures) {
    try {
        action();
    } catch (const std::invalid_argument&) {
        expect(true, name, failures);
        return;
    } catch (...) {
        expect(false, name + "（异常类型错误）", failures);
        return;
    }
    expect(false, name + "（未抛出异常）", failures);
}

class ThrowingStreamBuffer : public std::streambuf {
protected:
    int_type overflow(int_type) override {
        throw std::runtime_error("injected output failure");
    }
};

bool specialMembersIgnoreOutputFailure() {
    ThrowingStreamBuffer throwingBuffer;
    std::streambuf* const originalBuffer = std::cout.rdbuf(&throwingBuffer);
    const std::ios::iostate originalExceptions = std::cout.exceptions();
    std::cout.exceptions(std::ios::badbit | std::ios::failbit);

    bool passed = false;
    try {
        MyString source("stream-independent");
        MyString copied(source);
        MyString assigned("old");
        assigned = source;
        MyString moved(std::move(source));
        passed = std::string(copied.c_str()) == "stream-independent" &&
                 std::string(assigned.c_str()) == "stream-independent" &&
                 std::string(moved.c_str()) == "stream-independent" &&
                 source.size() == 0;
    } catch (...) {
        passed = false;
    }

    std::cout.exceptions(std::ios::goodbit);
    std::cout.clear();
    std::cout.rdbuf(originalBuffer);
    std::cout.exceptions(originalExceptions);
    return passed;
}

} // namespace

int main() {
    int failures = 0;

    static_assert(!std::is_copy_constructible_v<SimpleHashTable>);
    static_assert(!std::is_move_constructible_v<SimpleHashTable>);

    SimpleHashTable table;
    table.insert("ab", 10);
    table.insert("ba", -1);  // 与 ab 的字节和相同，刻意制造冲突
    expect(table.size() == 2, "冲突键都能保留", failures);
    expect(std::abs(table.loadFactor() - 0.2) < 1e-12,
           "负载因子等于元素数除以桶数", failures);
    expect(table.get("ab") == 10, "冲突链查找第一个键", failures);
    expect(table.get("ba") == -1, "值为-1不会与缺失混淆", failures);
    expect(!table.get("missing").has_value(), "缺失键返回nullopt", failures);
    table.insert("ab", 20);
    expect(table.size() == 2 && table.get("ab") == 20, "更新不增加元素数", failures);
    expect(table.remove("ba") && !table.contains("ba"), "删除已有键", failures);
    expect(!table.remove("ba"), "删除缺失键返回false", failures);

    const std::string highByte(1, static_cast<char>(0xFF));
    table.insert(highByte, 255);
    expect(table.get(highByte) == 255, "高位字节不会产生负桶下标", failures);

    MyString original("resource");
    MyString moved(std::move(original));
    MyString copiedFromMoved(original);
    expect(std::string(moved.c_str()) == "resource", "移动目标取得资源", failures);
    expect(original.size() == 0 && std::string(original.c_str()).empty(),
           "自定义类型明确空的moved-from状态", failures);
    expect(copiedFromMoved.size() == 0 && std::string(copiedFromMoved.c_str()).empty(),
           "移动后的对象仍可安全复制", failures);
    expect(specialMembersIgnoreOutputFailure(),
           "资源特殊成员不依赖观察流且保持异常契约", failures);

    expect(isAnagram_hash("listen", "silent"), "LC242哈希法", failures);
    expect(isAnagram_array("aacc", "ccaa"), "LC242数组法", failures);
    expect(canConstruct_hash("hello", "olehxl"), "LC383哈希法", failures);
    expect(!canConstruct_array("aaaa", "aaa"), "LC383字符不足", failures);
    expectInvalidArgument(
        [] { static_cast<void>(isAnagram_hash("A", "a")); },
        "LC242哈希法拒绝非小写输入", failures);
    expectInvalidArgument(
        [] { static_cast<void>(isAnagram_array("A", "abc")); },
        "LC242数组法在长度早退前校验输入", failures);
    expectInvalidArgument(
        [] { static_cast<void>(isAnagram_sort("a", "A")); },
        "LC242排序法与其他解法保持同一输入域", failures);
    expectInvalidArgument(
        [] { static_cast<void>(canConstruct_hash("A", "abc")); },
        "LC383哈希法拒绝非小写需求", failures);
    expectInvalidArgument(
        [] { static_cast<void>(canConstruct_array("a", "aB")); },
        "LC383数组法拒绝非小写供应", failures);

    return failures == 0 ? 0 : 1;
}
