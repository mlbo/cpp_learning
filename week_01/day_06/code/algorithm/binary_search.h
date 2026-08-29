#ifndef BINARY_SEARCH_ALGORITHMS_H
#define BINARY_SEARCH_ALGORITHMS_H

#include <vector>

// 所有返回 int 下标的接口在 nums.size() > INT_MAX 时抛出 std::length_error。

// 标准查找
int binarySearch(const std::vector<int>& nums, int target);
int binarySearchWithTrace(const std::vector<int>& nums, int target);

// 左边界与 lower_bound 语义
int binarySearchLeft(const std::vector<int>& nums, int target);
int lowerBound(const std::vector<int>& nums, int target);
int binarySearchLeftClosed(const std::vector<int>& nums, int target);

// 右边界与 upper_bound 语义
int binarySearchRight(const std::vector<int>& nums, int target);
int upperBound(const std::vector<int>& nums, int target);
int binarySearchRightClosed(const std::vector<int>& nums, int target);
int lastLessOrEqual(const std::vector<int>& nums, int target);

#endif // BINARY_SEARCH_ALGORITHMS_H
