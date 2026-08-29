#include <iostream>

void demoItem19();
void demoItem20();  // 历史函数名：实际是 Item 19 的 shared_ptr 性能补充。

int main() {
    std::cout << "Day 9 EMC++ 扩展示例：Item 19 与 shared_ptr 成本机制\n\n";
    demoItem19();
    std::cout << "\n";
    demoItem20();
    return 0;
}
