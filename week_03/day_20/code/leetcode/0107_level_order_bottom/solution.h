#ifndef SOLUTION_107_H
#define SOLUTION_107_H

#include "data_structure/tree_node.h"

#include <vector>

namespace leetcode_0107 {

class Solution {
public:
    std::vector<std::vector<int>> levelOrderBottom(TreeNode* root);
};

} // namespace leetcode_0107

void testLevelOrderBottom();

#endif // SOLUTION_107_H
