#include "algorithm/bfs_template.h"
#include "data_structure/tree_node.h"
#include "leetcode/0102_level_order/solution.h"
#include "leetcode/0107_level_order_bottom/solution.h"

#include <optional>
#include <vector>

int main() {
    bfsTemplateDemo();
    if (bfsMaze({{0, 0}, {0, 0}}, {0, 0}, {1, 1}) != 2) {
        return 1;
    }

    TreeNode* root = tree_ops::createTree(
        {3, 9, 20, std::nullopt, std::nullopt, 15, 7});
    leetcode_0102::Solution topDown;
    leetcode_0107::Solution bottomUp;
    const bool passed = topDown.levelOrder(root) ==
                            std::vector<std::vector<int>>({{3}, {9, 20}, {15, 7}}) &&
                        bottomUp.levelOrderBottom(root) ==
                            std::vector<std::vector<int>>({{15, 7}, {9, 20}, {3}});
    tree_ops::deleteTree(root);
    return passed ? 0 : 1;
}
