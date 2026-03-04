/**
 * @file decltype_auto.cpp
 * @brief decltype(auto) 详解 (C++14)
 * 
 * 本文件详细讲解：
 * 1. decltype(auto) 的语法和语义
 * 2. 与 auto 的区别
 * 3. 典型应用场景
 * 4. 使用注意事项
 */

#include <iostream>
#include <vector>
#include <type_traits>
#include <iomanip>

// 辅助宏
#define PRINT_SEPARATOR() std::cout << "\n" << std::string(60, '=') << "\n"
#define PRINT_TITLE(title) std::cout << "\n【" << title << "】\n"

/**
 * @brief 演示 auto 和 decltype(auto) 的基本区别
 */
void demonstrateBasicDifference() {
    PRINT_TITLE("1. auto vs decltype(auto) 基本区别");
    
    int x = 10;
    int& ref = x;
    
    std::cout << "int x = 10;\n";
    std::cout << "int& ref = x;\n\n";
    
    // auto 忽略引用，decltype(auto) 保留引用
    auto a = ref;                    // int
    decltype(auto) da = ref;         // int&
    
    std::cout << "auto a = ref;           // a 是 int\n";
    std::cout << "decltype(auto) da = ref; // da 是 int&\n\n";
    
    // 验证
    a = 100;
    std::cout << "a = 100 后，x = " << x << " (auto复制了值)\n";
    da = 200;
    std::cout << "da = 200 后，x = " << x << " (decltype(auto)保留了引用)\n";
    
    // 双层括号的差异
    PRINT_SEPARATOR();
    std::cout << "\n双层括号的差异:\n";
    
    auto a2 = (x);                   // int
    decltype(auto) da2 = (x);        // int&
    
    std::cout << "auto a2 = (x);           // a2 是 int\n";
    std::cout << "decltype(auto) da2 = (x); // da2 是 int&\n\n";
    
    std::cout << "原因分析:\n";
    std::cout << "  - auto 总是使用模板参数推导规则\n";
    std::cout << "  - decltype(auto) 使用 decltype 语义\n";
    std::cout << "  - (x) 是左值表达式，decltype((x)) → int&\n";
}

/**
 * @brief 演示 decltype(auto) 在函数返回类型中的应用
 */
decltype(auto) getValueByAuto(bool returnRef) {
    static int value = 100;
    if (returnRef) {
        return (value);   // 返回 int&（因为 (value) 是左值表达式）
    }
    return value;         // 返回 int（value 是标识符）
}

// 对比：使用 auto 的版本
auto getValueByPlainAuto(bool returnRef) {
    static int value = 100;
    return value;  // 总是返回 int
}

void demonstrateFunctionReturn() {
    PRINT_TITLE("2. 函数返回类型应用");
    
    std::cout << "问题场景：需要根据条件返回引用或值\n\n";
    
    std::cout << "使用 decltype(auto):\n";
    std::cout << R"(
  decltype(auto) getValueByAuto(bool returnRef) {
      static int value = 100;
      if (returnRef) {
          return (value);   // 返回 int&
      }
      return value;         // 返回 int
  }
)";
    
    // 调用演示
    auto v1 = getValueByAuto(false);
    auto& v2 = getValueByAuto(true);
    
    std::cout << "\n调用:\n";
    std::cout << "  auto v1 = getValueByAuto(false);  // v1 是 int\n";
    std::cout << "  auto& v2 = getValueByAuto(true);  // v2 是 int&\n";
    std::cout << "  v1 = " << v1 << ", v2 = " << v2 << "\n";
    
    // 实际应用：转发函数
    PRINT_SEPARATOR();
    std::cout << "\n实际应用：完美转发包装器\n";
    
    std::cout << R"(
  template<typename Container>
  decltype(auto) getElement(Container& c, size_t index) {
      return c[index];  // 精确返回元素类型（可能是引用）
  }
)";
    
    // 使用示例
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    auto getRef = [](std::vector<int>& c, size_t i) -> decltype(auto) {
        return c[i];
    };
    
    decltype(auto) elem = getRef(vec, 0);
    elem = 100;
    
    std::cout << "\n示例:\n";
    std::cout << "  std::vector<int> vec = {1, 2, 3, 4, 5};\n";
    std::cout << "  decltype(auto) elem = getRef(vec, 0);\n";
    std::cout << "  elem = 100;  // 直接修改 vec[0]\n";
    std::cout << "  vec[0] = " << vec[0] << "\n";
}

/**
 * @brief 演示 decltype(auto) 的危险用法
 */
int dangerousReturn() {
    int x = 42;
    return x;  // 返回 int
}

decltype(auto) dangerousReturnRef() {
    int x = 42;
    return (x);  // 危险！返回局部变量的引用！
}

void demonstrateDangers() {
    PRINT_TITLE("3. 危险用法警示");
    
    std::cout << "【危险案例】返回局部变量的引用\n";
    std::cout << R"(
  decltype(auto) dangerousReturnRef() {
      int x = 42;
      return (x);  // 危险！返回 int&，但 x 是局部变量！
  }
)";
    
    std::cout << "\n问题分析:\n";
    std::cout << "  1. (x) 是左值表达式，decltype((x)) = int&\n";
    std::cout << "  2. 所以返回类型是 int&\n";
    std::cout << "  3. 但 x 是局部变量，函数返回后销毁\n";
    std::cout << "  4. 返回了悬空引用！\n";
    
    std::cout << "\n【安全做法】\n";
    std::cout << "  - 返回局部变量时，使用 auto 而非 decltype(auto)\n";
    std::cout << "  - 或者确保返回的是静态变量/成员变量\n";
    std::cout << "  - 使用 static 确保生命周期\n";
    
    PRINT_SEPARATOR();
    std::cout << "\n正确的用法示例:\n";
    
    std::cout << R"(
  // 安全：返回静态变量的引用
  decltype(auto) safeReturnRef() {
      static int x = 42;
      return (x);  // OK: x 是静态变量
  }
  
  // 安全：返回值
  auto safeReturnValue() {
      int x = 42;
      return (x);  // 使用 auto，总是返回值
  }
)";
}

/**
 * @brief 演示 decltype(auto) 在容器访问中的应用
 */
void demonstrateContainerAccess() {
    PRINT_TITLE("4. 容器元素访问");
    
    std::vector<int> vec = {10, 20, 30, 40, 50};
    
    // 使用 auto - 无法修改容器元素
    auto getWithAuto = [](std::vector<int>& v, size_t i) {
        return v[i];  // 返回 int（复制）
    };
    
    // 使用 decltype(auto) - 可以修改容器元素
    auto getWithDecltypeAuto = [](std::vector<int>& v, size_t i) -> decltype(auto) {
        return v[i];  // 返回 int&
    };
    
    std::cout << "使用 auto:\n";
    std::cout << "  auto getWithAuto = [](std::vector<int>& v, size_t i) {\n";
    std::cout << "      return v[i];  // 返回 int\n";
    std::cout << "  };\n";
    auto a = getWithAuto(vec, 0);
    a = 100;
    std::cout << "  赋值 a = 100 后，vec[0] = " << vec[0] << " (未改变)\n\n";
    
    std::cout << "使用 decltype(auto):\n";
    std::cout << "  auto getWithDecltypeAuto = [...]() -> decltype(auto) {\n";
    std::cout << "      return v[i];  // 返回 int&\n";
    std::cout << "  };\n";
    decltype(auto) d = getWithDecltypeAuto(vec, 0);
    d = 100;
    std::cout << "  赋值 d = 100 后，vec[0] = " << vec[0] << " (已改变)\n";
    
    PRINT_SEPARATOR();
    std::cout << "\n通用访问函数模板:\n";
    
    std::cout << R"(
  template<typename Container>
  decltype(auto) safeGet(Container& c, size_t i) {
      if (i >= c.size()) {
          throw std::out_of_range("Index out of bounds");
      }
      return c[i];  // 精确返回元素类型
  }
)";
}

/**
 * @brief 演示 decltype(auto) 与复杂类型
 */
void demonstrateComplexTypes() {
    PRINT_TITLE("5. 复杂类型处理");
    
    // map 的迭代器解引用
    std::vector<std::pair<int, std::string>> data = {
        {1, "one"}, {2, "two"}, {3, "three"}
    };
    
    auto getPair = [&data](size_t i) -> decltype(auto) {
        return data[i];
    };
    
    std::cout << "处理 std::vector<std::pair<int, std::string>>:\n\n";
    
    decltype(auto) pair = getPair(0);
    std::cout << "decltype(auto) pair = getPair(0);\n";
    std::cout << "  pair.first = " << pair.first << "\n";
    std::cout << "  pair.second = \"" << pair.second << "\"\n";
    
    // 修改
    pair.second = "ONE";
    std::cout << "\n  pair.second = \"ONE\";\n";
    std::cout << "  data[0].second = \"" << data[0].second << "\"\n";
    
    // 指针情况
    PRINT_SEPARATOR();
    std::cout << "\n指针情况:\n";
    
    int x = 10;
    int* ptr = &x;
    
    auto getPtr = [&ptr]() -> decltype(auto) {
        return *ptr;  // 返回 int&
    };
    
    decltype(auto) ref = getPtr();
    ref = 100;
    std::cout << "  通过返回的引用修改: x = " << x << "\n";
}

/**
 * @brief 最佳实践总结
 */
void printBestPractices() {
    PRINT_TITLE("6. 最佳实践");
    
    std::cout << R"(
┌─────────────────────────────────────────────────────────────┐
│                decltype(auto) 使用指南                       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  【推荐使用场景】                                            │
│  1. 转发函数包装器                                           │
│     decltype(auto) wrap(Args&&... args) {                   │
│         return func(std::forward<Args>(args)...);           │
│     }                                                       │
│                                                             │
│  2. 泛型元素访问函数                                         │
│     template<typename C>                                    │
│     decltype(auto) getElement(C& c, size_t i) {             │
│         return c[i];                                        │
│     }                                                       │
│                                                             │
│  3. 需要精确保留返回类型时                                    │
│     decltype(auto) getMember() { return (obj.member); }     │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  【避免使用场景】                                            │
│  1. 返回局部变量                                             │
│     decltype(auto) bad() {                                  │
│         int x = 0;                                          │
│         return (x);  // 危险！返回悬空引用                   │
│     }                                                       │
│                                                             │
│  2. 不确定返回类型时                                         │
│     // 使用 auto 更安全                                      │
│                                                             │
│  3. 简单场景                                                 │
│     // auto 通常足够                                         │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  【检查清单】                                                │
│  □ 返回的是引用还是值？                                      │
│  □ 如果是引用，引用的对象是否还在？                          │
│  □ 表达式是否需要额外括号？                                  │
│  □ 是否可以用普通 auto 替代？                                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
)";
}

/**
 * @brief main 函数
 */
int main() {
    PRINT_SEPARATOR();
    std::cout << "      decltype(auto) 详解 - Day 2";
    PRINT_SEPARATOR();
    
    demonstrateBasicDifference();
    demonstrateFunctionReturn();
    demonstrateDangers();
    demonstrateContainerAccess();
    demonstrateComplexTypes();
    printBestPractices();
    
    PRINT_SEPARATOR();
    std::cout << "讲解结束！\n";
    PRINT_SEPARATOR();
    
    return 0;
}
