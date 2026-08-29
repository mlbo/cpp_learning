#ifndef SOLUTION_102_H
#define SOLUTION_102_H

#include "data_structure/tree_node.h"

#include <vector>

namespace leetcode_0102 {

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root);
};

} // namespace leetcode_0102

void testLevelOrder();

#endif // SOLUTION_102_H
