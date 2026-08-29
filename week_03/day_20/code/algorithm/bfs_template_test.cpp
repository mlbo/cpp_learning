#include "algorithm/bfs_template.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

void expectEqual(int actual, int expected, const std::string& name, int& failures) {
    const bool passed = actual == expected;
    std::cout << name << ": " << (passed ? "通过" : "失败") << '\n';
    if (!passed) {
        std::cout << "  实际: " << actual << "，期望: " << expected << '\n';
        ++failures;
    }
}

}  // namespace

int main() {
    int failures = 0;

    expectEqual(bfsMaze({}, {0, 0}, {0, 0}), -1, "空迷宫", failures);
    expectEqual(bfsMaze({{0}}, {0, 0}, {0, 0}), 0, "起点等于终点", failures);
    expectEqual(bfsMaze({{1}}, {0, 0}, {0, 0}), -1, "起点是墙", failures);
    expectEqual(bfsMaze({{0, 0}, {0}}, {0, 0}, {1, 0}), -1,
                "非矩形迷宫被拒绝", failures);

    const std::vector<std::vector<int>> openGrid = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    expectEqual(bfsMaze(openGrid, {0, 0}, {2, 2}), 4,
                "多条路径仍返回最少步数", failures);

    const std::vector<std::vector<int>> blocked = {
        {0, 1, 0},
        {1, 1, 0},
        {0, 0, 0}
    };
    expectEqual(bfsMaze(blocked, {0, 0}, {2, 2}), -1,
                "不可达终点", failures);

    return failures == 0 ? 0 : 1;
}
