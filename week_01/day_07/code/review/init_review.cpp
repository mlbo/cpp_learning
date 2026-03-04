/**
 * @file init_review.cpp
 * @brief 初始化方式复习 - 统一初始化、初始化列表
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <initializer_list>

// ============================================================================
// 1. 各种初始化方式
// ============================================================================

struct Widget {
    int id;
    std::string name;
    
    Widget() : id(0), name("default") {
        std::cout << "    Widget() 默认构造\n";
    }
    
    Widget(int i, const std::string& n) : id(i), name(n) {
        std::cout << "    Widget(int, string) 构造\n";
    }
    
    Widget(std::initializer_list<int> init) : id(0), name("init_list") {
        std::cout << "    Widget(initializer_list<int>) 构造, size = " 
                  << init.size() << "\n";
    }
};

void initialization_forms_demo() {
    std::cout << "\n【1. 各种初始化形式】\n";
    std::cout << "-------------------------------------------\n";

    std::cout << "  // 默认初始化\n";
    int a;          // 未定义值（自动存储期）
    int b{};        // 值初始化为0
    std::cout << "  int a;       // 未定义值\n";
    std::cout << "  int b{};     // 值初始化为 0, b = " << b << "\n";

    std::cout << "\n  // 直接初始化\n";
    int c(42);
    int d = 42;
    int e{42};
    std::cout << "  int c(42);   // 直接初始化\n";
    std::cout << "  int d = 42;  // 拷贝初始化\n";
    std::cout << "  int e{42};   // 统一初始化\n";

    std::cout << "\n  // 指针初始化\n";
    int* p1 = nullptr;
    int* p2{};
    int* p3 = new int(42);
    delete p3;
    std::cout << "  int* p1 = nullptr;  // C++11空指针\n";
    std::cout << "  int* p2{};          // 值初始化为 nullptr\n";
}

// ============================================================================
// 2. 统一初始化的优势
// ============================================================================

void uniform_init_advantages_demo() {
    std::cout << "\n【2. 统一初始化的优势】\n";
    std::cout << "-------------------------------------------\n";

    // 优势1: 防止窄化转换
    std::cout << "  // 优势1: 防止窄化转换\n";
    double d = 3.14;
    // int i{d};  // 编译错误！窄化转换
    int j(d);     // 可能警告，但编译通过
    
    std::cout << "  double d = 3.14;\n";
    std::cout << "  // int i{d};  // 编译错误！窄化转换\n";
    std::cout << "  int j(d);     // 可能警告，但编译通过\n";
    std::cout << "  j = " << j << "\n";

    // 优势2: 免于"最令人恼火的解析"
    std::cout << "\n  // 优势2: 免于\"最令人恼火的解析\"\n";
    // Widget w();  // 函数声明，不是对象！
    Widget w1;     // 默认构造
    Widget w2{};   // 也是默认构造，更清晰
    
    std::cout << "  // Widget w();  // 函数声明，不是对象！\n";
    std::cout << "  Widget w1;      // 默认构造\n";
    std::cout << "  Widget w2{};    // 默认构造，更清晰\n";

    // 优势3: 可以初始化容器
    std::cout << "\n  // 优势3: 初始化容器\n";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };
    
    std::cout << "  std::vector<int> vec = {1, 2, 3, 4, 5};\n";
    std::cout << "  std::map<string, int> scores = {{\"Alice\", 95}, ...};\n";
    std::cout << "  vec.size() = " << vec.size() << "\n";
}

// ============================================================================
// 3. initializer_list 行为
// ============================================================================

void initializer_list_demo() {
    std::cout << "\n【3. initializer_list 行为】\n";
    std::cout << "-------------------------------------------\n";

    // 优先选择 initializer_list 构造函数
    std::cout << "  // initializer_list 构造函数优先级更高\n";
    std::cout << "  Widget w3(10, \"test\");  // 调用 (int, string) 构造\n";
    Widget w3(10, "test");
    
    std::cout << "\n  Widget w4{10, \"test\"};  // 也是 (int, string) 构造\n";
    std::cout << "  （因为没有 initializer_list<string> 构造函数）\n";
    Widget w4{10, "test"};
    
    std::cout << "\n  Widget w5{10, 20, 30};  // initializer_list<int> 构造\n";
    Widget w5{10, 20, 30};
    
    std::cout << "\n  Widget w6 = {10};  // initializer_list<int> 构造\n";
    Widget w6 = {10};

    // 空初始化列表
    std::cout << "\n  Widget w7{};  // 默认构造（空列表不匹配 initializer_list）\n";
    Widget w7{};
}

// ============================================================================
// 4. 类成员初始化
// ============================================================================

class ModernClass {
public:
    ModernClass() = default;
    ModernClass(int x) : extra_(x) {}
    
    void print() const {
        std::cout << "    data_ = " << data_ 
                  << ", name_ = \"" << name_ 
                  << "\", ptr_ = " << (ptr_ ? "valid" : "null")
                  << ", extra_ = " << extra_ << "\n";
    }
    
private:
    int data_ = 42;                    // 类内初始化
    std::string name_ = "default";     // 类内初始化
    std::unique_ptr<int> ptr_;         // 默认为 nullptr
    int extra_ = 0;
};

void class_member_init_demo() {
    std::cout << "\n【4. 类成员初始化】\n";
    std::cout << "-------------------------------------------\n";
    
    std::cout << "  class ModernClass {\n";
    std::cout << "    int data_ = 42;               // 类内初始化\n";
    std::cout << "    string name_ = \"default\";     // 类内初始化\n";
    std::cout << "    unique_ptr<int> ptr_;         // 默认 nullptr\n";
    std::cout << "  };\n";
    
    std::cout << "\n  ModernClass obj1;  // 使用类内初始值\n";
    ModernClass obj1;
    obj1.print();
    
    std::cout << "\n  ModernClass obj2{100};  // 构造函数覆盖\n";
    ModernClass obj2{100};
    obj2.print();
}

// ============================================================================
// 5. 最佳实践
// ============================================================================

void best_practices_demo() {
    std::cout << "\n【5. 最佳实践】\n";
    std::cout << "-------------------------------------------\n";
    
    std::cout << "  ✅ 推荐:\n";
    std::cout << "  • 使用 {} 初始化，避免窄化转换\n";
    std::cout << "  • 使用 = {} 拷贝初始化容器\n";
    std::cout << "  • 类成员使用类内初始值\n";
    std::cout << "  • 使用 nullptr 初始化空指针\n";
    
    std::cout << "\n  ⚠️ 注意:\n";
    std::cout << "  • 有 initializer_list 构造函数时，{} 可能不是预期行为\n";
    std::cout << "  • auto + {} 推导为 initializer_list\n";
    std::cout << "  • vector<string> v{10} 是10个空字符串，不是\"10\"\n";
    
    std::cout << "\n  // vector 陷阱示例:\n";
    std::vector<std::string> v1{10};     // 10个空字符串
    std::vector<int> v2{10};              // 1个元素，值为10
    std::vector<int> v3(10);              // 10个元素，值为0
    
    std::cout << "  vector<string> v1{10};  // 10个空字符串\n";
    std::cout << "  vector<int> v2{10};     // 1个元素: {10}\n";
    std::cout << "  vector<int> v3(10);     // 10个元素: {0,0,...}\n";
    std::cout << "  v1.size() = " << v1.size() << "\n";
    std::cout << "  v2.size() = " << v2.size() << "\n";
    std::cout << "  v3.size() = " << v3.size() << "\n";
}

// ============================================================================

void init_review() {
    initialization_forms_demo();
    uniform_init_advantages_demo();
    initializer_list_demo();
    class_member_init_demo();
    best_practices_demo();
    
    std::cout << "\n✅ 初始化复习完成!\n";
}
