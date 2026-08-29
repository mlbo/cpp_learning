#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>

int main() {
    constexpr std::array<std::string_view, 42> items{{
        "模板类型推导", "auto 类型推导", "decltype", "查看推导类型",
        "优先 auto", "显式类型初始化器惯用法", "区分括号与花括号初始化", "优先 nullptr",
        "优先 using", "优先 enum class", "deleted 函数", "override",
        "const_iterator", "审慎 noexcept", "尽可能 constexpr", "const 成员函数线程安全",
        "特殊成员函数生成", "unique_ptr", "shared_ptr", "weak_ptr",
        "优先 make 函数", "Pimpl 特殊成员函数位置", "理解 move 与 forward", "区分转发引用",
        "右值引用 move、转发引用 forward", "避免转发引用重载", "转发引用重载替代方案", "引用折叠",
        "假定移动可能不存在或不便宜", "完美转发失败情形", "避免默认捕获", "初始化捕获",
        "泛型 lambda 的 auto&&", "优先 lambda 而非 bind", "任务优先于线程", "必要异步指定 launch::async",
        "thread 离开每条路径前变为 non-joinable", "线程句柄析构差异", "void future 一次性事件", "atomic 与 volatile 分工",
        "按值传递的适用条件", "审慎使用 emplacement",
    }};

    for (std::size_t index = 0; index < items.size(); ++index) {
        std::cout << "Item " << index + 1U << ": " << items[index] << '\n';
    }

    bool all_items_named = true;
    bool names_are_unique = true;
    for (std::size_t index = 0; index < items.size(); ++index) {
        all_items_named = all_items_named && !items[index].empty();
        for (std::size_t other = index + 1U; other < items.size(); ++other) {
            names_are_unique = names_are_unique && items[index] != items[other];
        }
    }
    const bool concurrency_items_aligned =
        items[34] == "任务优先于线程" &&
        items[35] == "必要异步指定 launch::async" &&
        items[36] == "thread 离开每条路径前变为 non-joinable" &&
        items[37] == "线程句柄析构差异" &&
        items[38] == "void future 一次性事件" &&
        items[39] == "atomic 与 volatile 分工";
    const bool final_items_aligned = items[40] == "按值传递的适用条件" &&
                                     items[41] == "审慎使用 emplacement";
    return all_items_named && names_are_unique && concurrency_items_aligned &&
                   final_items_aligned
               ? 0
               : 1;
}
