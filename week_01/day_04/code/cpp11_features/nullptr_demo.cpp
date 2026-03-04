/**
 * @file nullptr_demo.cpp
 * @brief nullptr基本用法演示 - 10种使用场景
 */

#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>
#include <cstddef>  // for std::nullptr_t

// ============================================================================
// 场景1: 指针初始化
// ============================================================================
void demo_pointer_initialization() {
    std::cout << "【场景1: 指针初始化】\n";
    
    // 原始指针初始化
    int* intPtr = nullptr;
    double* doublePtr = nullptr;
    const char* charPtr = nullptr;
    
    std::cout << "  intPtr = " << (intPtr ? "非空" : "空") << "\n";
    std::cout << "  doublePtr = " << (doublePtr ? "非空" : "空") << "\n";
    std::cout << "  charPtr = " << (charPtr ? "非空" : "空") << "\n";
    
    // 智能指针初始化
    std::unique_ptr<int> uptr = nullptr;
    std::shared_ptr<double> sptr = nullptr;
    
    std::cout << "  unique_ptr: " << (uptr ? "非空" : "空") << "\n";
    std::cout << "  shared_ptr: " << (sptr ? "非空" : "空") << "\n";
    std::cout << "\n";
}

// ============================================================================
// 场景2: 成员指针初始化
// ============================================================================
class Widget {
public:
    Widget() : data_(nullptr), name_(nullptr) {}
    
    void setData(int* data) { data_ = data; }
    void setName(const char* name) { name_ = name; }
    
    void print() const {
        std::cout << "  Widget: data=" << (data_ ? "有值" : "空")
                  << ", name=" << (name_ ? name_ : "空") << "\n";
    }
    
private:
    int* data_;
    const char* name_;
};

void demo_member_initialization() {
    std::cout << "【场景2: 成员指针初始化】\n";
    
    Widget w;
    w.print();
    
    int value = 42;
    w.setData(&value);
    w.setName("TestWidget");
    w.print();
    std::cout << "\n";
}

// ============================================================================
// 场景3: 函数参数默认值
// ============================================================================
void process(const char* data = nullptr, int size = 0) {
    std::cout << "【场景3: 函数参数默认值】\n";
    std::cout << "  处理数据: " << (data ? data : "无数据") << "\n";
    std::cout << "  数据大小: " << size << "\n\n";
}

void demo_default_parameters() {
    process();                           // 使用默认nullptr
    process("Hello, nullptr!");          // 提供实际数据
    process(nullptr, 10);               // 显式传入nullptr
}

// ============================================================================
// 场景4: 函数返回空指针
// ============================================================================
int* findValue(int key, int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == key) {
            return &arr[i];  // 找到，返回指针
        }
    }
    return nullptr;  // 未找到，返回空指针
}

void demo_return_value() {
    std::cout << "【场景4: 函数返回空指针】\n";
    
    int arr[] = {1, 2, 3, 4, 5};
    
    int* found = findValue(3, arr, 5);
    std::cout << "  查找3: " << (found ? std::to_string(*found) : "未找到") << "\n";
    
    int* notFound = findValue(10, arr, 5);
    std::cout << "  查找10: " << (notFound ? std::to_string(*notFound) : "未找到") << "\n";
    std::cout << "\n";
}

// ============================================================================
// 场景5: 条件判断
// ============================================================================
void demo_conditional_check() {
    std::cout << "【场景5: 条件判断】\n";
    
    int* ptr = nullptr;
    
    // 方式1: 直接判断
    if (ptr) {
        std::cout << "  指针非空\n";
    } else {
        std::cout << "  指针为空\n";
    }
    
    // 方式2: 与nullptr比较
    if (ptr != nullptr) {
        std::cout << "  指针非空\n";
    } else {
        std::cout << "  指针为空 (显式比较)\n";
    }
    
    // 方式3: 三元运算符
    std::cout << "  状态: " << (ptr ? "有效" : "无效") << "\n\n";
}

// ============================================================================
// 场景6: 智能指针重置
// ============================================================================
void demo_smart_pointer_reset() {
    std::cout << "【场景6: 智能指针重置】\n";
    
    auto sptr = std::make_shared<int>(42);
    std::cout << "  创建shared_ptr: " << *sptr << " (use_count=" << sptr.use_count() << ")\n";
    
    // 方式1: 使用reset()
    sptr.reset();
    std::cout << "  reset()后: " << (sptr ? "非空" : "空") << "\n";
    
    // 方式2: 使用nullptr赋值
    sptr = std::make_shared<int>(100);
    std::cout << "  重新创建: " << *sptr << "\n";
    
    sptr = nullptr;
    std::cout << "  nullptr赋值后: " << (sptr ? "非空" : "空") << "\n\n";
}

// ============================================================================
// 场景7: 模板类型推导
// ============================================================================
template<typename T>
void printType(T arg) {
    std::cout << "【场景7: 模板类型推导】\n";
    std::cout << "  类型: " << typeid(T).name() << "\n";
    std::cout << "  是指针类型: " << (std::is_pointer_v<T> ? "是" : "否") << "\n";
    std::cout << "  是nullptr_t类型: " << (std::is_null_pointer_v<T> ? "是" : "否") << "\n\n";
}

void demo_template_deduction() {
    printType(0);          // T = int
    printType(nullptr);   // T = std::nullptr_t
    printType((int*)nullptr);  // T = int*
}

// ============================================================================
// 场景8: 容器初始化
// ============================================================================
void demo_container_initialization() {
    std::cout << "【场景8: 容器初始化】\n";
    
    // 使用nullptr初始化指针容器
    std::vector<int*> vec(5, nullptr);
    
    std::cout << "  容器大小: " << vec.size() << "\n";
    std::cout << "  所有元素: ";
    for (auto p : vec) {
        std::cout << (p ? "非空 " : "空 ");
    }
    std::cout << "\n\n";
}

// ============================================================================
// 场景9: 类型安全转换
// ============================================================================
void demo_type_safe_conversion() {
    std::cout << "【场景9: 类型安全转换】\n";
    
    // nullptr可以隐式转换为任何指针类型
    void* voidPtr = nullptr;
    int* intPtr = static_cast<int*>(voidPtr);
    double* doublePtr = nullptr;
    
    std::cout << "  voidPtr: " << (voidPtr ? "非空" : "空") << "\n";
    std::cout << "  intPtr: " << (intPtr ? "非空" : "空") << "\n";
    std::cout << "  doublePtr: " << (doublePtr ? "非空" : "空") << "\n";
    
    // 注意：nullptr不能转换为整数
    // int value = nullptr;  // 编译错误！
    std::cout << "  nullptr不能转换为int (编译时错误)\n\n";
}

// ============================================================================
// 场景10: 布尔上下文
// ============================================================================
void demo_boolean_context() {
    std::cout << "【场景10: 布尔上下文】\n";
    
    int* ptr = nullptr;
    
    // 隐式布尔转换
    bool isEmpty = !ptr;  // ptr隐式转换为bool
    std::cout << "  !ptr = " << (isEmpty ? "true" : "false") << "\n";
    
    // 在if语句中
    if (!ptr) {
        std::cout << "  指针为空 (通过!ptr判断)\n";
    }
    
    // 在while循环中
    ptr = new int(42);
    while (ptr) {
        std::cout << "  ptr有效: " << *ptr << "\n";
        delete ptr;
        ptr = nullptr;  // 这会终止循环
    }
    std::cout << "  循环结束\n\n";
}

// ============================================================================
// 主函数
// ============================================================================
void run_nullptr_demo() {
    demo_pointer_initialization();
    demo_member_initialization();
    demo_default_parameters();
    demo_return_value();
    demo_conditional_check();
    demo_smart_pointer_reset();
    demo_template_deduction();
    demo_container_initialization();
    demo_type_safe_conversion();
    demo_boolean_context();
    
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "nullptr基本用法演示完成 - 展示了10种常见使用场景\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
}
