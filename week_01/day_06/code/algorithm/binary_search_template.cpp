/**
 * @file binary_search_template.cpp
 * @brief 二分查找通用模板类
 * 
 * 提供完整的二分查找工具集：
 * - 标准查找
 * - 左边界查找（lower_bound）
 * - 右边界查找
 * - 上界查找（upper_bound）
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */

#ifndef BINARY_SEARCH_TEMPLATE_HPP
#define BINARY_SEARCH_TEMPLATE_HPP

#include <vector>
#include <optional>
#include <functional>

/**
 * @brief 二分查找通用模板类
 * @tparam T 元素类型，必须支持比较操作
 * 
 * 使用示例：
 * std::vector<int> nums = {1, 2, 2, 2, 3};
 * BinarySearchTemplate<int> searcher(nums);
 * 
 * searcher.search(2);        // 返回任意一个 2 的索引
 * searcher.searchLeft(2);    // 返回第一个 2 的索引
 * searcher.searchRight(2);   // 返回最后一个 2 的索引
 * searcher.lowerBound(2);    // 返回第一个 >= 2 的索引
 * searcher.upperBound(2);    // 返回第一个 > 2 的索引
 */
template<typename T>
class BinarySearchTemplate {
public:
    /**
     * @brief 构造函数
     * @param nums 已排序的数组（升序）
     */
    explicit BinarySearchTemplate(const std::vector<T>& nums) : nums_(nums) {}
    
    /**
     * @brief 标准二分查找
     * @param target 目标值
     * @return 目标值的索引，如果不存在返回-1
     */
    int search(const T& target) const {
        if (nums_.empty()) {
            return -1;
        }
        
        int left = 0;
        int right = static_cast<int>(nums_.size()) - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (nums_[mid] == target) {
                return mid;
            } else if (nums_[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1;
    }
    
    /**
     * @brief 查找目标值的左边界（第一个出现位置）
     * @param target 目标值
     * @return 目标值第一次出现的索引，如果不存在返回-1
     */
    int searchLeft(const T& target) const {
        if (nums_.empty()) {
            return -1;
        }
        
        int left = 0;
        int right = static_cast<int>(nums_.size());
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (nums_[mid] >= target) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        if (left == static_cast<int>(nums_.size()) || nums_[left] != target) {
            return -1;
        }
        
        return left;
    }
    
    /**
     * @brief 查找目标值的右边界（最后一个出现位置）
     * @param target 目标值
     * @return 目标值最后一次出现的索引，如果不存在返回-1
     */
    int searchRight(const T& target) const {
        if (nums_.empty()) {
            return -1;
        }
        
        int left = 0;
        int right = static_cast<int>(nums_.size());
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (nums_[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        if (left == 0 || nums_[left - 1] != target) {
            return -1;
        }
        
        return left - 1;
    }
    
    /**
     * @brief 查找第一个大于等于目标值的位置（lower_bound）
     * @param target 目标值
     * @return 第一个 >= target 的索引
     */
    int lowerBound(const T& target) const {
        if (nums_.empty()) {
            return 0;
        }
        
        int left = 0;
        int right = static_cast<int>(nums_.size());
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (nums_[mid] >= target) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
    /**
     * @brief 查找第一个大于目标值的位置（upper_bound）
     * @param target 目标值
     * @return 第一个 > target 的索引
     */
    int upperBound(const T& target) const {
        if (nums_.empty()) {
            return 0;
        }
        
        int left = 0;
        int right = static_cast<int>(nums_.size());
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (nums_[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    /**
     * @brief 查找目标值的范围
     * @param target 目标值
     * @return {左边界, 右边界}，如果不存在返回{-1, -1}
     */
    std::pair<int, int> searchRange(const T& target) const {
        int left = searchLeft(target);
        if (left == -1) {
            return {-1, -1};
        }
        int right = searchRight(target);
        return {left, right};
    }
    
    /**
     * @brief 统计目标值出现的次数
     * @param target 目标值
     * @return 出现次数
     */
    int count(const T& target) const {
        auto [left, right] = searchRange(target);
        if (left == -1) {
            return 0;
        }
        return right - left + 1;
    }
    
    /**
     * @brief 检查目标值是否存在
     * @param target 目标值
     * @return 是否存在
     */
    bool contains(const T& target) const {
        return search(target) != -1;
    }

private:
    const std::vector<T>& nums_;
};

/**
 * @brief 通用二分查找函数模板
 * @tparam T 元素类型
 * @param left 搜索左边界
 * @param right 搜索右边界
 * @param check 检查函数，返回true表示在右半部分
 * @return 满足条件的最小值
 * 
 * 使用示例：查找满足条件的最小值
 */
template<typename T>
T binarySearchTemplate(T left, T right, std::function<bool(T)> check) {
    while (left < right) {
        T mid = left + (right - left) / 2;
        if (check(mid)) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

/**
 * @brief 实数二分查找
 * @param left 搜索左边界
 * @param right 搜索右边界
 * @param eps 精度要求
 * @param check 检查函数
 * @return 满足条件的值
 */
double binarySearchDouble(double left, double right, double eps, 
                          std::function<bool(double)> check) {
    while (right - left > eps) {
        double mid = left + (right - left) / 2;
        if (check(mid)) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return left;
}

#endif // BINARY_SEARCH_TEMPLATE_HPP
