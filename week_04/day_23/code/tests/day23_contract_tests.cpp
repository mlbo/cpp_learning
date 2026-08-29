#include <iostream>

bool verify_move_semantics_contract();
bool verify_buffer_contract();
bool verify_item23_move_observation_contract();
bool verify_item24_forwarding_report_contract();
bool verify_item25_lifetime_contract();
bool verify_lc0001_integer_boundary_contract();
bool verify_lc0454_wide_pair_sum_contract();

int main() {
    int failures = 0;

    const auto expect = [&failures](bool condition, const char* name) {
        std::cout << name << ": " << (condition ? "通过" : "失败") << '\n';
        if (!condition) {
            ++failures;
        }
    };

    expect(verify_move_semantics_contract(),
           "MyString复制/移动、moved-from与抛异常流隔离契约");
    expect(verify_buffer_contract(),
           "Buffer复制赋值、移动、复用与抛异常流隔离契约");
    expect(verify_item23_move_observation_contract(),
           "真实移动目标值与源对象可复用契约");
    expect(verify_item24_forwarding_report_contract(),
           "命名形参恢复调用点值类别后再分类");
    expect(verify_item25_lifetime_contract(),
           "临时所有者与转发参数按值返回避免悬空");
    expect(verify_lc0001_integer_boundary_contract(),
           "LC1补数使用宽整数且极值输入不溢出");
    expect(verify_lc0454_wide_pair_sum_contract(),
           "LC454两数组合和使用宽整数到达真实哈希路径");

    return failures == 0 ? 0 : 1;
}
