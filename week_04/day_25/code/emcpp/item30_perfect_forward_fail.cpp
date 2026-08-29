/**
 * @file item30_perfect_forward_fail.cpp
 * @brief EMC++ Item 30: 熟悉完美转发失败情形
 */

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

#define SECTION(title) std::cout << "\n===== " << title << " =====\n\n"

enum class TargetRoute {
    VectorLvalue,
    VectorRvalue,
    Integer,
    Pointer,
    Function,
    Size
};

TargetRoute lastRoute = TargetRoute::Integer;
int lastFunctionResult = 0;

void target(const std::vector<int>& values) {
    lastRoute = TargetRoute::VectorLvalue;
    std::cout << "  target(const vector&)，元素数 = " << values.size() << "\n";
}

void target(std::vector<int>&& values) {
    lastRoute = TargetRoute::VectorRvalue;
    std::cout << "  target(vector&&)，元素数 = " << values.size() << "\n";
}

void target(int value) {
    lastRoute = TargetRoute::Integer;
    std::cout << "  target(int)，值 = " << value << "\n";
}

void target(void* pointer) {
    lastRoute = TargetRoute::Pointer;
    std::cout << "  target(void*)，指针为 " << (pointer == nullptr ? "空" : "非空") << "\n";
}

void target(std::nullptr_t) {
    std::cout << "  target(nullptr_t)，随后可无歧义地转换为空对象指针\n";
    target(static_cast<void*>(nullptr));
}

void target(int (*function)(int)) {
    lastRoute = TargetRoute::Function;
    lastFunctionResult = function(21);
    std::cout << "  target(int(*)(int))，调用结果 = " << lastFunctionResult << "\n";
}

void target(std::size_t value) {
    lastRoute = TargetRoute::Size;
    std::cout << "  target(size_t)，值 = " << value << "\n";
}

template<typename T>
decltype(auto) fwd(T&& param) {
    // 真正把参数送到目标重载；测试输出来自 target，而不是只打印 T 的名字。
    return target(std::forward<T>(param));
}

struct StaticConfig {
    // 类内初始化足以把它当编译期值使用；一旦绑定引用，就会发生 ODR-use。
    static const std::size_t MinValues = 2;
};

// 为引用绑定提供存储定义。若删掉这一行，fwd(StaticConfig::MinValues) 通常会链接失败。
const std::size_t StaticConfig::MinValues;

int overloaded(int value) {
    return value * 2;
}

[[maybe_unused]] double overloaded(double value) {
    return value * 2.0;
}

template<typename T>
int functionTemplate(T value) {
    return static_cast<int>(value) + 1;
}

struct BitFields {
    unsigned mode : 3;
};

void caseBracedInitList() {
    SECTION("案例1：大括号初始化列表");

    std::cout << "  {1, 2, 3} 通常不是一个具有普通表达式类型的表达式，T&& 无从推导 T。\n";
    std::cout << "  auto values = {1, 2, 3} 能工作，是 auto 对列表初始化的特殊规则，结果为 initializer_list<int>。\n";
    std::cout << "  若目标语义是 vector，先明确构造 vector，避免依赖特殊推导规则：\n";

    std::vector<int> values{1, 2, 3};
    fwd(values);
    fwd(std::vector<int>{4, 5});

    auto list = {6, 7, 8};
    static_assert(std::is_same_v<decltype(list), std::initializer_list<int>>);
    std::cout << "  auto list 的类型经特殊规则成为 initializer_list<int>，元素数 = " << list.size() << "\n";
    std::cout << "  被注释的 fwd({1, 2, 3}) 会因模板参数无法推导而编译失败。\n";
}

void caseNullPointer() {
    SECTION("案例2：0、NULL 与 nullptr");

    std::cout << "  fwd(0) 保留 int 类型，因此实际到达整数重载：\n";
    fwd(0);

    std::cout << "  fwd(nullptr) 保留 std::nullptr_t，再转换到指针重载：\n";
    fwd(nullptr);

    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);
    static_assert(!std::is_pointer_v<std::nullptr_t>);
    std::cout << "  nullptr 的类型是 std::nullptr_t；它不是指针类型，但能安全转换为各类指针。\n";
    std::cout << "  NULL 常是整数宏，具体整数类型依实现而异，所以泛型代码应使用 nullptr。\n";
}

void caseStaticConstIntegral() {
    SECTION("案例3：仅类内声明的 static const 整型成员");

    std::cout << "  编译期按值使用 StaticConfig::MinValues 通常不需要额外存储。\n";
    std::cout << "  转发引用会绑定到该成员并造成 ODR-use，因此需要类外定义：\n";
    fwd(StaticConfig::MinValues);
    std::cout << "  C++17 也可把成员写成 inline static constexpr，从定义层面消除这个陷阱。\n";
}

void caseFunctionNames() {
    SECTION("案例4：重载函数名与函数模板名");

    std::cout << "  单独的 overloaded 代表重载集合，fwd(overloaded) 没有目标类型帮助选择版本。\n";
    std::cout << "  先固定函数指针类型，再让 fwd 真实转发到函数指针目标：\n";
    auto function = static_cast<int (*)(int)>(overloaded);
    fwd(function);

    std::cout << "  functionTemplate 也代表尚未选择实例的函数模板，fwd(functionTemplate) 同样无法推导 T。\n";
    std::cout << "  显式选择 functionTemplate<int> 并固定函数指针类型后，调用结果应为 22：\n";
    auto specialization = static_cast<int (*)(int)>(functionTemplate<int>);
    fwd(specialization);
}

void caseBitField() {
    SECTION("案例5：位域");

    BitFields bits{5};
    std::cout << "  位域不是可独立寻址的对象，不能直接绑定到转发引用。\n";
    const int copied = static_cast<int>(bits.mode);
    std::cout << "  先复制到普通对象，再转发：\n";
    fwd(copied);
}

void summary() {
    SECTION("Item 30 边界清单");
    std::cout << "  1. 大括号列表：给出明确对象类型。\n";
    std::cout << "  2. 0/NULL：用 nullptr 表达空指针。\n";
    std::cout << "  3. static const 整型成员：被引用绑定时提供定义，或使用 inline constexpr。\n";
    std::cout << "  4. 重载函数名或函数模板名：先用函数指针、显式模板实参或 lambda 固定目标。\n";
    std::cout << "  5. 位域：先复制到可寻址的普通对象。\n";
}

bool verifySuccessBoundaries() {
    std::vector<int> values{1, 2, 3};
    fwd(values);
    const bool vectorLvalue = lastRoute == TargetRoute::VectorLvalue;
    fwd(std::vector<int>{4, 5});
    const bool vectorRvalue = lastRoute == TargetRoute::VectorRvalue;
    fwd(0);
    const bool zeroStaysInteger = lastRoute == TargetRoute::Integer;
    fwd(nullptr);
    const bool nullptrReachesPointer = lastRoute == TargetRoute::Pointer;
    fwd(StaticConfig::MinValues);
    const bool staticMemberHasStorage = lastRoute == TargetRoute::Size;
    auto function = static_cast<int (*)(int)>(overloaded);
    fwd(function);
    const bool overloadWasResolved =
        lastRoute == TargetRoute::Function && lastFunctionResult == 42;
    auto specialization = static_cast<int (*)(int)>(functionTemplate<int>);
    fwd(specialization);
    const bool templateWasResolved =
        lastRoute == TargetRoute::Function && lastFunctionResult == 22;
    BitFields bits{5};
    const int copy = static_cast<int>(bits.mode);
    fwd(copy);
    const bool bitFieldCopyIsAddressable = lastRoute == TargetRoute::Integer;

    return vectorLvalue && vectorRvalue && zeroStaysInteger &&
           nullptrReachesPointer && staticMemberHasStorage &&
           overloadWasResolved && templateWasResolved && bitFieldCopyIsAddressable;
}

} // namespace

namespace emcpp {

void item30PerfectForwardFail() {
    std::cout << "【EMC++ Item 30: 熟悉完美转发失败情形】\n";
    caseBracedInitList();
    caseNullPointer();
    caseStaticConstIntegral();
    caseFunctionNames();
    caseBitField();
    summary();
}

bool verifyItem30Contracts() {
    return verifySuccessBoundaries();
}

} // namespace emcpp

bool verify_item30_forwarding_boundaries_contract() {
    return emcpp::verifyItem30Contracts();
}

#ifdef STANDALONE_TEST
int main() {
    emcpp::item30PerfectForwardFail();
    return 0;
}
#endif
