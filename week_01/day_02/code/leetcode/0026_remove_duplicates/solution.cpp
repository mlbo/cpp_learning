/**
 * @file solution.cpp
 * @brief LeetCode 26题：删除有序数组中的重复项
 * 
 * 题目描述：
 * 给你一个升序排列的数组 nums，请你原地删除重复出现的元素，
 * 使每个元素只出现一次，返回删除后数组的新长度。
 * 
 * 算法：快慢指针
 * - 慢指针：指向待填充位置
 * - 快指针：遍历数组查找不同元素
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */

#include <iostream>
#include <vector>
#include <algorithm>

namespace leetcode {
namespace p0026 {

/**
 * @brief 删除有序数组中的重复项（快慢指针法）
 * @param nums 有序数组
 * @return 去重后的长度
 * 
 * 算法思路：
 * 1. 慢指针slow指向已处理区域的末尾
 * 2. 快指针fast遍历整个数组
 * 3. 当nums[fast] != nums[slow]时，说明发现新元素
 * 4. 将新元素放到++slow位置
 */
int removeDuplicates(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    int slow = 0;  // 慢指针：指向已处理区域末尾
    
    // 快指针：遍历数组
    for (int fast = 1; fast < static_cast<int>(nums.size()); ++fast) {
        if (nums[fast] != nums[slow]) {
            ++slow;
            nums[slow] = nums[fast];
        }
    }
    
    return slow + 1;
}

/**
 * @brief 打印vector内容
 */
void printVector(const std::vector<int>& nums, int length) {
    std::cout << "结果数组: ";
    for (int i = 0; i < length; ++i) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
}

/**
 * @brief 测试函数
 */
void test() {
    // 测试用例1
    std::vector<int> nums1 = {1, 1, 2};
    std::cout << "测试用例: {1,1,2}" << std::endl;
    int len1 = removeDuplicates(nums1);
    std::cout << "去重后长度: " << len1 << std::endl;
    printVector(nums1, len1);
    
    // 测试用例2
    std::vector<int> nums2 = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    std::cout << "\n测试用例: {0,0,1,1,1,2,2,3,3,4}" << std::endl;
    int len2 = removeDuplicates(nums2);
    std::cout << "去重后长度: " << len2 << std::endl;
    printVector(nums2, len2);
    
    // 测试用例3：空数组
    std::vector<int> nums3 = {};
    std::cout << "\n测试用例: {}" << std::endl;
    int len3 = removeDuplicates(nums3);
    std::cout << "去重后长度: " << len3 << std::endl;
    printVector(nums3, len3);
    
    // 测试用例4：单个元素
    std::vector<int> nums4 = {1};
    std::cout << "\n测试用例: {1}" << std::endl;
    int len4 = removeDuplicates(nums4);
    std::cout << "去重后长度: " << len4 << std::endl;
    printVector(nums4, len4);
    
    // 测试用例5：无重复元素
    std::vector<int> nums5 = {1, 2, 3, 4, 5};
    std::cout << "\n测试用例: {1,2,3,4,5}" << std::endl;
    int len5 = removeDuplicates(nums5);
    std::cout << "去重后长度: " << len5 << std::endl;
    printVector(nums5, len5);
}

/**
 * @brief 图解快慢指针算法
 */
void explain_algorithm() {
    std::cout << "\n=== 快慢指针算法图解 ===" << std::endl;
    std::cout << R"(
初始状态: nums = [1, 1, 2, 2, 3]
          slow = 0, fast = 0
          
Step 1: nums[1] == nums[0], 跳过
        [1, 1, 2, 2, 3]
         s     f

Step 2: nums[2] != nums[0], slow++, nums[1] = nums[2]
        [1, 2, 2, 2, 3]
            s     f

Step 3: nums[3] == nums[1], 跳过
        [1, 2, 2, 2, 3]
            s     f

Step 4: nums[4] != nums[1], slow++, nums[2] = nums[4]
        [1, 2, 3, 2, 3]
               s     f

结果: 返回 slow + 1 = 3
      前三个元素 [1, 2, 3] 为去重结果
)" << std::endl;
}

} // namespace p0026

// 封装测试函数
void test_remove_duplicates() {
    p0026::test();
}

} // namespace leetcode
