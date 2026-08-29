/**
 * @file item29_move_assumptions.cpp
 * @brief EMC++ Item 29: 假定移动操作不存在、成本高、或未被使用
 *
 * 本文件演示：
 * 1. 移动操作可能不存在、不便宜或未被实际选择
 * 2. const 与 noexcept 如何影响移动路径和容器迁移选择
 * 3. std::array、短字符串等类型为何不能预设移动是常数成本
 * 4. Item 25 的 std::forward 前置知识与真实转发使用模式回顾
 */

#include <iostream>
#include <array>
#include <string>
#include <utility>
#include <type_traits>
#include <memory>
#include <vector>

// ==================== 辅助输出 ====================

#define LOG(msg) std::cout << msg << std::endl

// ==================== Item 29 的三个假设 ====================

class CopyOnly {
public:
    inline static int copies = 0;

    CopyOnly() = default;
    CopyOnly(const CopyOnly&) {
        ++copies;
        std::cout << "  CopyOnly没有移动构造：std::move后的实参仍调用拷贝构造\n";
    }
};

class MoveProbe {
public:
    inline static int copies = 0;
    inline static int moves = 0;

    MoveProbe() = default;
    MoveProbe(const MoveProbe&) {
        ++copies;
        std::cout << "  从const对象构造：调用拷贝构造\n";
    }
    MoveProbe(MoveProbe&&) noexcept {
        ++moves;
    }
};

class ThrowingMoveProbe {
public:
    inline static int copies = 0;
    inline static int moves = 0;

    explicit ThrowingMoveProbe(std::string value = {}) : value_(std::move(value)) {}
    ThrowingMoveProbe(const ThrowingMoveProbe& other) : value_(other.value_) {
        ++copies;
    }
    ThrowingMoveProbe(ThrowingMoveProbe&& other) noexcept(false)
        : value_(std::move(other.value_)) {
        ++moves;
    }

private:
    std::string value_;
};

class NoexceptMoveProbe {
public:
    inline static int copies = 0;
    inline static int moves = 0;

    explicit NoexceptMoveProbe(std::string value = {}) : value_(std::move(value)) {}
    NoexceptMoveProbe(const NoexceptMoveProbe& other) : value_(other.value_) {
        ++copies;
    }
    NoexceptMoveProbe(NoexceptMoveProbe&& other) noexcept
        : value_(std::move(other.value_)) {
        ++moves;
    }

private:
    std::string value_;
};

template<typename T>
void triggerVectorRelocation() {
    std::vector<T> values;
    values.reserve(1);
    values.emplace_back("first");
    T::copies = 0;
    T::moves = 0;
    values.emplace_back("second");
}

void explainItem29Assumptions() {
    std::cout << "\n===== Item 29：不要预设移动一定存在、便宜且会发生 =====\n";

    std::cout << "\n1. 移动操作可能不存在：\n";
    CopyOnly source;
    CopyOnly destination(std::move(source));
    (void)destination;

    std::cout << "\n2. 移动操作可能不便宜：\n";
    std::array<int, 1024> values{};
    auto movedValues = std::move(values);
    std::cout << "  std::array<int,1024>没有可窃取的独立缓冲区，移动仍逐元素进行，复杂度O(n)\n";
    std::cout << "  首元素仍为 " << movedValues.front() << "\n";

    std::string shortText = "short";
    std::string movedShortText = std::move(shortText);
    std::cout << "  许多实现会对短字符串采用SSO，把字符放在对象内部；此时移动也要复制这段内联存储。\n";
    std::cout << "  SSO不是标准强制的布局，不能从本例推导所有实现的精确成本；结果字符串为 "
              << movedShortText << "\n";

    std::cout << "\n3. 移动操作可能未被使用：\n";
    const MoveProbe constSource;
    MoveProbe copied(std::move(constSource));
    (void)copied;
    std::cout << "  std::move保留const，得到const MoveProbe&&；普通MoveProbe&&不能绑定它\n";

    std::cout << "\n4. 容器迁移还会检查异常保证：\n";
    triggerVectorRelocation<ThrowingMoveProbe>();
    std::cout << "  move可能抛异常时，vector扩容迁移旧元素：拷贝 "
              << ThrowingMoveProbe::copies << " 次，移动 " << ThrowingMoveProbe::moves << " 次\n";
    triggerVectorRelocation<NoexceptMoveProbe>();
    std::cout << "  move声明noexcept时，vector扩容迁移旧元素：拷贝 "
              << NoexceptMoveProbe::copies << " 次，移动 " << NoexceptMoveProbe::moves << " 次\n";
    std::cout << "  这是本标准库在这两个可拷贝类型上的可观察选择；移动可能抛且拷贝可用时，容器可能复制旧元素以维护异常保证。\n";
    std::cout << "  若拷贝不可用，容器仍可能使用可抛移动，具体保证取决于操作和类型要求。\n";

    std::cout << "\n实践结论：先保证代码在拷贝语义下正确，再把移动视为经过验证的优化。\n";
    std::cout << "只有已知类型、已知操作和测量结果时，才能断言移动带来显著收益。\n";
}

// ==================== 核心概念演示 ====================

/**
 * std::forward的本质
 *
 * std::forward是一个条件转换工具：
 * - 如果T是左值引用类型(Type&)，则返回左值引用
 * - 如果T是非引用类型(Type)，则返回右值引用
 *
 * 简化的实现概念：
 * template<typename T>
 * T&& forward(typename std::remove_reference<T>::type& param) {
 *     return static_cast<T&&>(param);
 * }
 */

// 模拟std::forward的行为（仅用于理解）
template<typename T>
T&& myForward(typename std::remove_reference<T>::type& param) {
    return static_cast<T&&>(param);
}

// ==================== 类型推导演示 ====================

// 展示模板参数T如何保存值类别信息
template<typename T>
void showTypeDeduction(T&& param) {
    std::cout << "\n--- 类型推导分析 ---\n";

    // 显示T的类型
    if (std::is_lvalue_reference<T>::value) {
        std::cout << "  T = " << typeid(T).name() << " (左值引用)\n";
        std::cout << "  这意味着传入的是左值\n";
    } else {
        std::cout << "  T = " << typeid(T).name() << " (非引用类型)\n";
        std::cout << "  这意味着传入的是右值\n";
    }

    // T&&的实际类型
    typedef T&& ParamType;
    if (std::is_lvalue_reference<ParamType>::value) {
        std::cout << "  T&& = " << typeid(ParamType).name() << " (左值引用，因为引用折叠)\n";
    } else {
        std::cout << "  T&& = " << typeid(ParamType).name() << " (右值引用)\n";
    }

    // std::forward<T>的返回类型
    typedef decltype(std::forward<T>(param)) ForwardType;
    if (std::is_lvalue_reference<ForwardType>::value) {
        std::cout << "  std::forward<T>(param) 返回: 左值引用\n";
    } else {
        std::cout << "  std::forward<T>(param) 返回: 右值引用\n";
    }
}

// ==================== 固定右值引用与转发引用的意图区别 ====================

// 固定右值引用显式写出 std::forward 虽可工作，但没有需要恢复的推导信息。
void wrongForwardDemo() {
    std::cout << "\n【固定右值引用不需要用 forward 表意】\n\n";

    std::cout << "问题代码:\n";
    std::cout << "  void func(int&& param) {\n";
    std::cout << "      // 命名参数变量的表达式 param 在这里是左值！\n";
    std::cout << "      // std::forward<int>(param) 返回 int&&\n";
    std::cout << "      // 但这实际上是多余的转换\n";
    std::cout << "  }\n\n";

    std::cout << "解释：\n";
    std::cout << "  1. int&& param 只能接受右值\n";
    std::cout << "  2. 在函数内部，命名参数变量的表达式 param 是左值\n";
    std::cout << "  3. std::forward<int>(param) 会返回 int&&\n";
    std::cout << "  4. 但没有类型信息来区分原始值类别\n\n";

    std::cout << "正确做法：使用std::move\n";
    std::cout << "  void func(int&& param) {\n";
    std::cout << "      doSomething(std::move(param));  // 清晰表明意图\n";
    std::cout << "  }\n";
}

enum class ForwardRoute {
    Lvalue,
    Rvalue
};

ForwardRoute forwardTarget(int&) {
    return ForwardRoute::Lvalue;
}

ForwardRoute forwardTarget(int&&) {
    return ForwardRoute::Rvalue;
}

// 正确示例：在转发引用中使用 forward，并真实调用目标重载。
template<typename T>
ForwardRoute correctForwardDemo(T&& param) {
    std::cout << "\n【正确示例：在转发引用中使用forward】\n";
    const ForwardRoute route = forwardTarget(std::forward<T>(param));
    std::cout << "  目标重载结果: "
              << (route == ForwardRoute::Lvalue ? "int& 左值重载" : "int&& 右值重载")
              << "\n";
    return route;
}

// ==================== 典型使用模式 ====================

// 模式1：工厂函数
class Product {
public:
    Product(const std::string& name, int id)
        : name_(name), id_(id) {
        std::cout << "  [Product] 左值构造\n";
    }

    Product(std::string&& name, int id)
        : name_(std::move(name)), id_(id) {
        std::cout << "  [Product] 右值构造\n";
    }

private:
    std::string name_;
    int id_;
};

template<typename T, typename... Args>
std::unique_ptr<T> create(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 模式2：包装函数
template<typename Func, typename... Args>
auto call(Func&& func, Args&&... args)
    -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...))
{
    return std::forward<Func>(func)(std::forward<Args>(args)...);
}

// 模式3：类模板参数转发
template<typename T>
class Container {
public:
    template<typename U>
    void add(U&& item) {
        items_.push_back(std::forward<U>(item));
    }

private:
    std::vector<T> items_;
};

// ==================== std::forward的实现解析 ====================

void explainForwardImplementation() {
    std::cout << "\n【std::forward实现解析】\n\n";

    std::cout << "标准库中的实现（简化版）：\n\n";
    std::cout << "  template<typename T>\n";
    std::cout << "  constexpr T&& forward(\n";
    std::cout << "      typename std::remove_reference<T>::type& t) noexcept\n";
    std::cout << "  {\n";
    std::cout << "      return static_cast<T&&>(t);\n";
    std::cout << "  }\n\n";

    std::cout << "关键点分析：\n";
    std::cout << "  1. 参数类型：remove_reference<T>::type&\n";
    std::cout << "     - 无论T是什么，参数总是左值引用\n";
    std::cout << "     - 这允许接受任何类型的参数\n\n";

    std::cout << "  2. 返回类型：T&&\n";
    std::cout << "     - 如果T是 Type&，返回 Type& && → Type& (引用折叠)\n";
    std::cout << "     - 如果T是 Type，返回 Type&&\n\n";

    std::cout << "  3. static_cast<T&&>(t)\n";
    std::cout << "     - 根据 T 的类型进行正确的转换\n";
    std::cout << "     - 左值转左值引用，非引用转右值引用\n";
}

// ==================== 主演示函数 ====================

namespace emcpp {

void item29StdForwardDetail() {
    std::cout << "【EMC++ Item 29: 不要假定移动操作存在、便宜且会发生】\n";

    explainItem29Assumptions();

    std::cout << "\n===== 前置知识回顾：Item 25 的 std::forward =====\n";

    // ========== 类型推导演示 ==========
    std::cout << "\n===== 类型推导如何保存值类别信息 =====\n";

    int x = 42;
    std::cout << "\n传入左值 x:\n";
    showTypeDeduction(x);

    std::cout << "\n\n传入右值 42:\n";
    showTypeDeduction(42);

    // ========== 错误用法 ==========
    wrongForwardDemo();

    // ========== 正确用法 ==========
    std::cout << "\n===== 正确使用示例 =====\n";

    int y = 100;
    const ForwardRoute leftRoute = correctForwardDemo(y);
    const ForwardRoute rightRoute = correctForwardDemo(200);
    std::cout << "  转发验证: 左值="
              << (leftRoute == ForwardRoute::Lvalue ? "保持" : "失败")
              << ", 右值="
              << (rightRoute == ForwardRoute::Rvalue ? "保持" : "失败") << "\n";

    // ========== 使用模式 ==========
    std::cout << "\n===== 典型使用模式 =====\n\n";

    std::cout << "1. 工厂函数模式:\n";
    std::string name = "Widget";
    auto p1 = create<Product>(name, 1);        // 左值
    auto p2 = create<Product>("Gadget", 2);    // 右值

    std::cout << "\n2. 包装函数模式:\n";
    auto add = [](int a, int b) { return a + b; };
    int result = call(add, 3, 4);
    std::cout << "  call(add, 3, 4) = " << result << "\n";

    // ========== 实现解析 ==========
    explainForwardImplementation();

    std::cout << "\n===== Item 29 总结 =====\n";
    std::cout << "  1. 没有移动成员时，右值仍可能走拷贝\n";
    std::cout << "  2. std::array等类型的移动可能仍是线性成本\n";
    std::cout << "  3. const对象经std::move后通常仍只能拷贝\n";
    std::cout << "  4. move非noexcept时，容器迁移旧元素可能选择拷贝\n";
    std::cout << "  5. 模板代码尤其不能假定未知类型的移动一定廉价\n";
}

bool verifyItem29Contracts() {
    CopyOnly::copies = 0;
    CopyOnly source;
    CopyOnly copied(std::move(source));
    (void)copied;

    MoveProbe::copies = 0;
    MoveProbe::moves = 0;
    const MoveProbe constSource;
    MoveProbe copiedFromConst(std::move(constSource));
    (void)copiedFromConst;

    int value = 7;
    return CopyOnly::copies == 1 &&
           MoveProbe::copies == 1 &&
           MoveProbe::moves == 0 &&
           correctForwardDemo(value) == ForwardRoute::Lvalue &&
           correctForwardDemo(7) == ForwardRoute::Rvalue &&
           !std::is_nothrow_move_constructible_v<ThrowingMoveProbe> &&
           std::is_nothrow_move_constructible_v<NoexceptMoveProbe>;
}

} // namespace emcpp

bool verify_item29_move_assumptions_contract() {
    return emcpp::verifyItem29Contracts();
}

// 独立测试入口
#ifdef STANDALONE_TEST
int main() {
    emcpp::item29StdForwardDetail();
    return 0;
}
#endif
