/**
 * LeetCode 1047. 删除字符串中的所有相邻重复项
 */

#ifndef LC1047_SOLUTION_H
#define LC1047_SOLUTION_H

#include <string>

namespace lc1047 {

class Solution {
public:
    std::string removeDuplicates(const std::string& s);
};

bool testRemoveDuplicates();

}  // namespace lc1047

#endif // LC1047_SOLUTION_H
