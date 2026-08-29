#include "leetcode/0155_min_stack/solution.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

static_assert(!std::is_copy_constructible_v<MinStack>);
static_assert(!std::is_copy_assignable_v<MinStack>);
static_assert(!std::is_move_constructible_v<MinStack>);
static_assert(!std::is_move_assignable_v<MinStack>);

namespace {

void expect(bool condition, const std::string& name, int& failures) {
    std::cout << name << ": " << (condition ? "通过" : "失败") << '\n';
    if (!condition) ++failures;
}

template<typename Operation>
void expectUnderflow(Operation operation, const std::string& name, int& failures) {
    bool threw = false;
    try {
        operation();
    } catch (const std::underflow_error&) {
        threw = true;
    }
    expect(threw, name, failures);
}

} // namespace

int main() {
    int failures = 0;
    MinStack stack;

    expect(stack.empty(), "新栈为空", failures);
    expectUnderflow([&stack]() { stack.pop(); }, "空栈pop有明确定义", failures);
    expectUnderflow([&stack]() { (void)stack.top(); }, "空栈top有明确定义", failures);
    expectUnderflow([&stack]() { (void)stack.getMin(); }, "空栈getMin有明确定义", failures);

    stack.push(-2);
    stack.push(0);
    stack.push(-3);
    expect(stack.getMin() == -3, "压入-3后最小值为-3", failures);
    stack.pop();
    expect(stack.top() == 0, "弹出后栈顶为0", failures);
    expect(stack.getMin() == -2, "弹出后最小值恢复为-2", failures);

    stack.push(-2);
    expect(stack.getMin() == -2, "重复最小值仍被记录", failures);
    stack.pop();
    expect(stack.getMin() == -2, "弹出一个重复最小值后仍为-2", failures);

    stack.pop();
    stack.pop();
    expect(stack.empty(), "全部弹出后为空", failures);

    return failures == 0 ? 0 : 1;
}
