/**
 * @file binary_search_template.h
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

#include <cmath>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "../../../common/integer_contracts.h"

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
        (void)week01::checked_index(nums_.size());
        std::size_t left = 0;
        std::size_t right = nums_.size();
        
        while (left < right) {
            const std::size_t mid = left + (right - left) / 2;
            
            if (nums_[mid] == target) {
                return week01::checked_index(mid);
            } else if (nums_[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;
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
        
        (void)week01::checked_index(nums_.size());
        std::size_t left = 0;
        std::size_t right = nums_.size();
        
        while (left < right) {
            const std::size_t mid = left + (right - left) / 2;
            
            if (nums_[mid] >= target) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        if (left == nums_.size() || nums_[left] != target) {
            return -1;
        }
        
        return week01::checked_index(left);
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
        
        (void)week01::checked_index(nums_.size());
        std::size_t left = 0;
        std::size_t right = nums_.size();
        
        while (left < right) {
            const std::size_t mid = left + (right - left) / 2;
            
            if (nums_[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        if (left == 0 || nums_[left - 1] != target) {
            return -1;
        }
        
        return week01::checked_index(left - 1);
    }
    
    /**
     * @brief 查找第一个大于等于目标值的位置（lower_bound）
     * @param target 目标值
     * @return 第一个 >= target 的索引
     */
    int lowerBound(const T& target) const {
        (void)week01::checked_index(nums_.size());
        std::size_t left = 0;
        std::size_t right = nums_.size();
        
        while (left < right) {
            const std::size_t mid = left + (right - left) / 2;
            
            if (nums_[mid] >= target) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return week01::checked_index(left);
    }
    
    /**
     * @brief 查找第一个大于目标值的位置（upper_bound）
     * @param target 目标值
     * @return 第一个 > target 的索引
     */
    int upperBound(const T& target) const {
        (void)week01::checked_index(nums_.size());
        std::size_t left = 0;
        std::size_t right = nums_.size();
        
        while (left < right) {
            const std::size_t mid = left + (right - left) / 2;
            
            if (nums_[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return week01::checked_index(left);
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
 * @param left 闭区间左端点
 * @param right 闭区间右端点；必须满足 check(right) == true
 * @param check 纯谓词，在整数域上必须呈 false...false,true...true 的单调形态
 * @return 闭区间 [left, right] 中满足 check 的最小整数
 * 
 * 使用示例：查找满足条件的最小值
 */
template<typename T>
T binarySearchTemplate(T left, T right, std::function<bool(T)> check) {
    static_assert(std::is_integral<T>::value,
                  "binarySearchTemplate requires an integral boundary type");
    static_assert(!std::is_same<typename std::remove_cv<T>::type, bool>::value,
                  "binarySearchTemplate does not accept bool boundaries");
    if (right < left) {
        throw std::invalid_argument("binary search interval must satisfy left <= right");
    }
    if (!check(right)) {
        throw std::invalid_argument("binary search requires a feasible right endpoint");
    }
    if (check(left)) {
        return left;
    }

    // 端点检查后保持：left 不满足，right 满足，答案位于 (left, right]。
    while (left < right) {
        using Unsigned = typename std::make_unsigned<T>::type;
        const Unsigned distance = static_cast<Unsigned>(right) -
                                  static_cast<Unsigned>(left);
        const T half = static_cast<T>(distance / 2);
        const T mid = static_cast<T>(left + half);
        if (check(mid)) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

/**
 * @brief 在实数闭区间中逼近 false -> true 的单调分界
 * @param left 闭区间左端点；若已满足谓词则直接返回
 * @param right 闭区间右端点；必须满足 check(right) == true
 * @param eps 最终括区间的最大宽度
 * @param check 纯谓词，必须呈 false...false,true...true 的单调形态
 * @return 可行侧 right；除浮点数已无法继续细分外，与不可行侧距离不超过 eps
 */
inline double binarySearchDouble(double left, double right, double eps,
                                 std::function<bool(double)> check) {
    if (!std::isfinite(left) || !std::isfinite(right) || !std::isfinite(eps) ||
        eps <= 0.0 || right < left) {
        throw std::invalid_argument("binary search requires finite ordered bounds and eps > 0");
    }
    if (!check(right)) {
        throw std::invalid_argument("binary search requires a feasible right endpoint");
    }
    if (check(left)) {
        return left;
    }

    // 端点检查后保持：left 不满足，right 满足，分界位于 (left, right]。
    while (right - left > eps) {
        const double mid = left / 2.0 + right / 2.0;
        if (mid == left || mid == right) {
            break;  // 已达到 double 可表示精度，继续循环不会再缩小区间。
        }
        if (check(mid)) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return right;
}

#endif // BINARY_SEARCH_TEMPLATE_HPP
