/**
 * @file solution.cpp
 * @brief LeetCode 27题：移除元素
 * 
 * 题目描述：
 * 给你一个数组 nums 和一个值 val，你需要原地移除所有数值等于 val 的元素，
 * 并返回移除后数组的新长度。
 * 
 * 算法：双指针
 * - 左指针：指向待填充位置
 * - 右指针：遍历数组
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */

#include <iostream>
#include <vector>

namespace leetcode {
namespace p0027 {

/**
 * @brief 移除元素（双指针法）
 * @param nums 数组
 * @param val 要移除的值
 * @return 移除后的长度
 * 
 * 算法思路：
 * 1. 左指针left指向待填充位置
 * 2. 右指针遍历整个数组
 * 3. 当nums[right] != val时，保留该元素
 * 4. 将保留的元素复制到left位置，left++
 */
int removeElement(std::vector<int>& nums, int val) {
    int left = 0;  // 左指针：指向待填充位置
    
    // 右指针：遍历数组
    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        if (nums[right] != val) {
            nums[left] = nums[right];
            ++left;
        }
    }
    
    return left;
}

/**
 * @brief 移除元素优化版（适用于val出现次数少的情况）
 * 
 * 当要移除的元素很少时，可以减少赋值操作
 * 将匹配的元素与末尾元素交换，然后缩小数组大小
 */
int removeElementOptimized(std::vector<int>& nums, int val) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    
    while (left <= right) {
        if (nums[left] == val) {
            nums[left] = nums[right];
            --right;
        } else {
            ++left;
        }
    }
    
    return left;
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
    std::vector<int> nums1 = {3, 2, 2, 3};
    int val1 = 3;
    std::cout << "测试用例: {3,2,2,3}, val=" << val1 << std::endl;
    int len1 = removeElement(nums1, val1);
    std::cout << "移除后长度: " << len1 << std::endl;
    printVector(nums1, len1);
    
    // 测试用例2
    std::vector<int> nums2 = {0, 1, 2, 2, 3, 0, 4, 2};
    int val2 = 2;
    std::cout << "\n测试用例: {0,1,2,2,3,0,4,2}, val=" << val2 << std::endl;
    int len2 = removeElement(nums2, val2);
    std::cout << "移除后长度: " << len2 << std::endl;
    printVector(nums2, len2);
    
    // 测试用例3：空数组
    std::vector<int> nums3 = {};
    int val3 = 1;
    std::cout << "\n测试用例: {}, val=" << val3 << std::endl;
    int len3 = removeElement(nums3, val3);
    std::cout << "移除后长度: " << len3 << std::endl;
    printVector(nums3, len3);
    
    // 测试用例4：全部要移除
    std::vector<int> nums4 = {1, 1, 1, 1};
    int val4 = 1;
    std::cout << "\n测试用例: {1,1,1,1}, val=" << val4 << std::endl;
    int len4 = removeElement(nums4, val4);
    std::cout << "移除后长度: " << len4 << std::endl;
    printVector(nums4, len4);
    
    // 测试用例5：不需要移除任何元素
    std::vector<int> nums5 = {1, 2, 3, 4};
    int val5 = 5;
    std::cout << "\n测试用例: {1,2,3,4}, val=" << val5 << std::endl;
    int len5 = removeElement(nums5, val5);
    std::cout << "移除后长度: " << len5 << std::endl;
    printVector(nums5, len5);
    
    // 测试优化版本
    std::cout << "\n--- 测试优化版本 ---" << std::endl;
    std::vector<int> nums6 = {3, 2, 2, 3};
    int val6 = 3;
    std::cout << "测试用例: {3,2,2,3}, val=" << val6 << std::endl;
    int len6 = removeElementOptimized(nums6, val6);
    std::cout << "移除后长度: " << len6 << std::endl;
    printVector(nums6, len6);
}

/**
 * @brief 图解双指针算法
 */
void explain_algorithm() {
    std::cout << "\n=== 双指针算法图解 ===" << std::endl;
    std::cout << R"(
初始状态: nums = [3, 2, 2, 3], val = 3
          left = 0, right = 0

Step 1: nums[0]=3 == val, 跳过
        [3, 2, 2, 3]
         l
         r

Step 2: nums[1]=2 != val, nums[0]=nums[1], left++
        [2, 2, 2, 3]
            l
            r

Step 3: nums[2]=2 != val, nums[1]=nums[2], left++
        [2, 2, 2, 3]
               l
               r

Step 4: nums[3]=3 == val, 跳过
        [2, 2, 2, 3]
               l
                  r

结果: 返回 left = 2
      前两个元素 [2, 2] 为结果
)" << std::endl;
}

/**
 * @brief 对比两种算法
 */
void compare_algorithms() {
    std::cout << "\n=== 两种算法对比 ===" << std::endl;
    std::cout << "┌─────────────────┬─────────────────┬─────────────────┐" << std::endl;
    std::cout << "│     特性        │    标准双指针   │   优化版本      │" << std::endl;
    std::cout << "├─────────────────┼─────────────────┼─────────────────┤" << std::endl;
    std::cout << "│ 时间复杂度      │     O(n)        │     O(n)        │" << std::endl;
    std::cout << "│ 空间复杂度      │     O(1)        │     O(1)        │" << std::endl;
    std::cout << "│ 赋值次数        │    n次          │   ≤n次          │" << std::endl;
    std::cout << "│ 元素顺序        │    保持         │   不保持        │" << std::endl;
    std::cout << "│ 适用场景        │    通用         │  val出现少时    │" << std::endl;
    std::cout << "└─────────────────┴─────────────────┴─────────────────┘" << std::endl;
}

} // namespace p0027

// 封装测试函数
void test_remove_element() {
    p0027::test();
}

} // namespace leetcode
