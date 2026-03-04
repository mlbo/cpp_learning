/**
 * @file item30_perfect_forward_fail.cpp
 * @brief EMC++ Item 30: 完美转发失败案例
 * 
 * 本文件演示：
 * 1. 大括号初始化器导致的转发失败
 * 2. 0或NULL作为空指针的转发失败
 * 3. 重载函数名的转发失败
 * 4. 位域的转发失败
 * 5. 解决方案
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>

// ==================== 辅助输出 ====================

#define LOG(msg) std::cout << msg << std::endl
#define SECTION(title) std::cout << "\n===== " << title << " =====\n\n"

// ==================== 目标函数 ====================

void processVector(const std::vector<int>& v) {
    std::cout << "  处理vector，大小: " << v.size() << "\n";
}

void processPointer(void* ptr) {
    std::cout << "  处理指针: " << (ptr ? "非空" : "空") << "\n";
}

void processInt(int x) {
    std::cout << "  处理整数: " << x << "\n";
}

int processInt(int x, int y) {
    std::cout << "  处理两个整数: " << x << ", " << y << "\n";
    return x + y;
}

// ==================== 完美转发包装器 ====================

template<typename T>
void fwd(T&& param) {
    // 模拟转发到一个处理函数
    // 这里简化处理，只打印信息
    std::cout << "  转发成功，类型: " << typeid(T).name() << "\n";
}

// 用于演示的通用转发函数
template<typename T>
void fwdToProcess(T&& param) {
    process(std::forward<T>(param));
}

// 通用的process函数
void process(const std::vector<int>& v) {
    processVector(v);
}

void process(void* ptr) {
    processPointer(ptr);
}

void process(int x) {
    processInt(x);
}

// ==================== 案例1：大括号初始化器 ====================

void case1BracedInitList() {
    SECTION("案例1：大括号初始化器");
    
    std::cout << "问题描述：\n";
    std::cout << "  大括号初始化器 {1, 2, 3} 没有类型\n";
    std::cout << "  无法被模板参数推导识别\n\n";
    
    std::cout << "直接调用（成功）：\n";
    std::cout << "  processVector({1, 2, 3});\n";
    // processVector({1, 2, 3});  // 直接调用可以，因为形参类型已知
    
    std::cout << "\n通过完美转发（失败）：\n";
    std::cout << "  fwd({1, 2, 3});  // 编译错误！\n";
    // fwd({1, 2, 3});  // 错误：无法推导类型
    
    std::cout << "\n原因分析：\n";
    std::cout << "  1. {1, 2, 3} 是 std::initializer_list<int>\n";
    std::cout << "  2. 模板推导时，大括号初始化器没有类型\n";
    std::cout << "  3. T&& 无法匹配到无类型的表达式\n";
    
    std::cout << "\n解决方案：\n";
    std::cout << "  方法1：显式指定类型\n";
    auto init = {1, 2, 3};
    std::cout << "  auto init = {1, 2, 3};\n";
    std::cout << "  fwd(init);  // 成功\n";
    fwd(init);
    
    std::cout << "\n  方法2：构造临时vector\n";
    std::cout << "  fwd(std::vector<int>{1, 2, 3});  // 成功\n";
    fwd(std::vector<int>{1, 2, 3});
}

// ==================== 案例2：0和NULL作为空指针 ====================

void case2NullPointer() {
    SECTION("案例2：0和NULL作为空指针");
    
    std::cout << "问题描述：\n";
    std::cout << "  0 和 NULL 在模板推导时被视为整数\n";
    std::cout << "  而不是指针类型\n\n";
    
    void* nullPtr = nullptr;
    
    std::cout << "直接调用（成功）：\n";
    std::cout << "  process(0);  // 可能成功，但语义不清\n";
    std::cout << "  process(NULL);  // 可能成功，但语义不清\n";
    
    std::cout << "\n通过完美转发：\n";
    std::cout << "  fwd(0);  // T推导为int，不是指针！\n";
    fwd(0);
    
    std::cout << "\n  fwd(NULL);  // T推导为int或long，不是指针！\n";
    // fwd(NULL);  // 视实现而定
    
    std::cout << "\n原因分析：\n";
    std::cout << "  1. 0 是 int 类型字面量\n";
    std::cout << "  2. NULL 通常是 0 或 0L 的宏定义\n";
    std::cout << "  3. 模板推导不会将它们视为指针\n";
    
    std::cout << "\n解决方案：\n";
    std::cout << "  使用 nullptr\n";
    std::cout << "  fwd(nullptr);  // T推导为 std::nullptr_t\n";
    fwd(nullptr);
    
    std::cout << "\n  nullptr 的优势：\n";
    std::cout << "  1. 是指针类型，不是整数\n";
    std::cout << "  2. 可以隐式转换为任何指针类型\n";
    std::cout << "  3. 类型安全，不会与整数混淆\n";
}

// ==================== 案例3：重载函数名 ====================

// 重载函数
int overloadedFunc(int x) {
    return x * 2;
}

double overloadedFunc(double x) {
    return x * 2.0;
}

void processFunc(int (*func)(int)) {
    std::cout << "  处理函数指针: " << (func ? "有效" : "无效") << "\n";
}

void case3OverloadedFunction() {
    SECTION("案例3：重载函数名");
    
    std::cout << "问题描述：\n";
    std::cout << "  重载函数名本身没有确定的类型\n";
    std::cout << "  模板推导无法选择正确的重载\n\n";
    
    std::cout << "有多个重载版本的函数：\n";
    std::cout << "  int overloadedFunc(int x);\n";
    std::cout << "  double overloadedFunc(double x);\n\n";
    
    std::cout << "直接调用（成功）：\n";
    std::cout << "  processFunc(overloadedFunc);  // 可能成功\n";
    // processFunc(overloadedFunc);  // 可能仍有歧义
    
    std::cout << "\n通过完美转发（失败）：\n";
    std::cout << "  fwd(overloadedFunc);  // 编译错误！\n";
    // fwd(overloadedFunc);  // 错误：无法推导类型
    
    std::cout << "\n原因分析：\n";
    std::cout << "  1. overloadedFunc 是重载函数名\n";
    std::cout << "  2. 没有上下文时，编译器不知道选择哪个版本\n";
    std::cout << "  3. 模板推导无法确定 T 的类型\n";
    
    std::cout << "\n解决方案：\n";
    std::cout << "  方法1：显式转换\n";
    std::cout << "  fwd(static_cast<int(*)(int)>(overloadedFunc));\n";
    fwd(static_cast<int(*)(int)>(overloadedFunc));
    
    std::cout << "\n  方法2：使用函数指针变量\n";
    int (*funcPtr)(int) = overloadedFunc;
    std::cout << "  int (*funcPtr)(int) = overloadedFunc;\n";
    std::cout << "  fwd(funcPtr);\n";
    fwd(funcPtr);
    
    std::cout << "\n  方法3：使用lambda包装\n";
    auto lambdaWrapper = [](int x) { return overloadedFunc(x); };
    std::cout << "  fwd(lambdaWrapper);  // 但类型变了\n";
}

// ==================== 案例4：位域 ====================

struct BitFieldStruct {
    int a : 4;    // 4位有符号整数
    int b : 4;    // 4位有符号整数
    int c : 8;    // 8位有符号整数
};

void case4BitFields() {
    SECTION("案例4：位域");
    
    BitFieldStruct bfs{5, 3, 100};
    
    std::cout << "问题描述：\n";
    std::cout << "  位域不能被非const引用绑定\n";
    std::cout << "  因为位域可能不是字节对齐的\n\n";
    
    std::cout << "位域结构体：\n";
    std::cout << "  struct BitFieldStruct {\n";
    std::cout << "      int a : 4;  // 4位\n";
    std::cout << "      int b : 4;  // 4位\n";
    std::cout << "      int c : 8;  // 8位\n";
    std::cout << "  };\n\n";
    
    std::cout << "直接使用（部分成功）：\n";
    std::cout << "  int x = bfs.a;  // 成功，复制值\n";
    int x = bfs.a;
    std::cout << "  x = " << x << "\n";
    
    std::cout << "\n通过完美转发（可能失败）：\n";
    std::cout << "  fwd(bfs.a);  // 可能失败！\n";
    // fwd(bfs.a);  // 取决于T&&如何绑定
    
    std::cout << "\n原因分析：\n";
    std::cout << "  1. 位域成员可能跨越字节边界\n";
    std::cout << "  2. 无法获取位域成员的地址\n";
    std::cout << "  3. 非const引用需要可寻址的对象\n";
    
    std::cout << "\n解决方案：\n";
    std::cout << "  方法：先复制到临时变量\n";
    int temp = bfs.a;
    std::cout << "  int temp = bfs.a;\n";
    std::cout << "  fwd(temp);  // 成功\n";
    fwd(temp);
    
    std::cout << "\n  或使用auto：\n";
    std::cout << "  auto val = bfs.b;\n";
    std::cout << "  fwd(val);\n";
    auto val = bfs.b;
    fwd(val);
}

// ==================== 总结 ====================

void summary() {
    SECTION("完美转发失败案例总结");
    
    std::cout << "┌─────────────────────────┬─────────────────────────┐\n";
    std::cout << "│       失败情况          │        解决方案         │\n";
    std::cout << "├─────────────────────────┼─────────────────────────┤\n";
    std::cout << "│ 大括号初始化器 {1,2,3}  │ 使用auto声明或显式类型   │\n";
    std::cout << "│ 0 或 NULL 作为空指针    │ 使用 nullptr            │\n";
    std::cout << "│ 重载函数名              │ 显式转换或使用函数指针  │\n";
    std::cout << "│ 位域成员                │ 先复制到临时变量        │\n";
    std::cout << "└─────────────────────────┴─────────────────────────┘\n";
    
    std::cout << "\n核心原则：\n";
    std::cout << "  完美转发依赖于类型推导\n";
    std::cout << "  如果类型信息不明确或无法推导，转发就会失败\n";
    std::cout << "  解决方案都是为了让类型信息变得明确\n";
}

// ==================== 主演示函数 ====================

namespace emcpp {

void item30PerfectForwardFail() {
    std::cout << "【EMC++ Item 30: 完美转发失败案例】\n";
    
    case1BracedInitList();
    case2NullPointer();
    case3OverloadedFunction();
    case4BitFields();
    summary();
    
    std::cout << "\n===== Item 30 总结 =====\n";
    std::cout << "  完美转发虽然强大，但并非万能\n";
    std::cout << "  了解失败情况有助于编写更健壮的代码\n";
    std::cout << "  当遇到编译错误时，检查是否属于这些情况\n";
}

} // namespace emcpp

// 独立测试入口
#ifdef STANDALONE_TEST
int main() {
    emcpp::item30PerfectForwardFail();
    return 0;
}
#endif
