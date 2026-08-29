/**
 * LeetCode 567: 字符串的排列 - 实现文件
 */

#include "solution.h"
#include <array>
#include <iostream>

using namespace std;

namespace day27::lc0567 {

namespace {

size_t byteIndex(char character) {
    return static_cast<unsigned char>(character);
}

} // namespace

// ==================== 主函数实现 ====================

bool Solution::checkInclusion(string s1, string s2) {
    const size_t n1 = s1.size();
    const size_t n2 = s2.size();
    
    // 边界条件：s1 比 s2 长，不可能存在排列
    if (n1 > n2) {
        return false;
    }
    
    // 按无符号字节计数，避免 char 为有符号类型时产生负下标。
    // 算法处理的是字节序列；若题目要按 Unicode 字符处理，应先做 UTF-8 解码。
    std::array<int, 256> count1{};
    std::array<int, 256> count2{};
    
    // 统计 s1 的字符
    for (char character : s1) {
        ++count1[byteIndex(character)];
    }
    
    // 初始化窗口：统计前 n1 个字符
    for (size_t i = 0; i < n1; ++i) {
        ++count2[byteIndex(s2[i])];
    }
    
    // 检查初始窗口是否匹配
    if (count1 == count2) {
        return true;
    }
    
    // 滑动窗口
    for (size_t i = n1; i < n2; ++i) {
        // 加入新字符
        ++count2[byteIndex(s2[i])];
        // 移除旧字符（窗口左边界）
        --count2[byteIndex(s2[i - n1])];
        
        // 检查是否匹配
        if (count1 == count2) {
            return true;
        }
    }
    
    return false;
}

// ==================== 优化版本实现 ====================

bool Solution::checkInclusionOptimized(string s1, string s2) {
    const size_t n1 = s1.size();
    const size_t n2 = s2.size();
    
    if (n1 > n2) {
        return false;
    }
    
    // 使用数组统计字符计数差异
    // count[c] = 窗口中字符c的数量 - s1中字符c的数量
    std::array<int, 256> count{};
    
    // 初始化：窗口前 n1 个字符与 s1 的差异
    for (size_t i = 0; i < n1; ++i) {
        --count[byteIndex(s1[i])];
        ++count[byteIndex(s2[i])];
    }
    
    // 统计有多少种字符的数量不匹配
    size_t diff = 0;
    for (size_t i = 0; i < count.size(); ++i) {
        if (count[i] != 0) {
            ++diff;
        }
    }
    
    // 如果初始窗口就匹配
    if (diff == 0) {
        return true;
    }
    
    // 滑动窗口
    for (size_t i = n1; i < n2; ++i) {
        // 加入的新字符
        const size_t in = byteIndex(s2[i]);
        // 移除的旧字符
        const size_t out = byteIndex(s2[i - n1]);
        
        // 处理新字符的加入
        if (count[in] == 0) {
            ++diff;  // 原本匹配，现在不匹配了
        }
        ++count[in];
        if (count[in] == 0) {
            --diff;  // 现在匹配了
        }
        
        // 处理旧字符的移除
        if (count[out] == 0) {
            ++diff;  // 原本匹配，现在不匹配了
        }
        --count[out];
        if (count[out] == 0) {
            --diff;  // 现在匹配了
        }
        
        // 检查是否完全匹配
        if (diff == 0) {
            return true;
        }
    }
    
    return false;
}

// ==================== 算法演示函数 ====================

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作
 */
void demonstratePermutationAlgorithm(const string& s1, const string& s2) {
    cout << "\n演示: s1 = \"" << s1 << "\", s2 = \"" << s2 << "\"" << endl;
    cout << "----------------------------------------" << endl;
    
    const size_t n1 = s1.size();
    const size_t n2 = s2.size();
    
    if (n1 > n2) {
        cout << "s1 比 s2 长，不可能存在排列" << endl;
        return;
    }
    if (s1.empty()) {
        cout << "空模式按约定匹配位置0的空窗口" << endl;
        return;
    }
    
    // 统计 s1 的字符
    array<int, 256> count1{};
    for (char c : s1) {
        ++count1[byteIndex(c)];
    }
    
    cout << "s1 字符统计: ";
    for (size_t i = 0; i < count1.size(); ++i) {
        if (count1[i] > 0) {
            cout << "byte[" << i << "]:" << count1[i] << " ";
        }
    }
    cout << endl;
    
    // 初始化窗口
    array<int, 256> count2{};
    for (size_t i = 0; i < n1; ++i) {
        ++count2[byteIndex(s2[i])];
    }
    
    cout << "\n初始窗口 [0," << n1 - 1 << "]: \"" << s2.substr(0, n1) << "\"" << endl;
    cout << "窗口字符统计: ";
    for (size_t i = 0; i < count2.size(); ++i) {
        if (count2[i] > 0) {
            cout << "byte[" << i << "]:" << count2[i] << " ";
        }
    }
    cout << endl;
    
    if (count1 == count2) {
        cout << ">>> 匹配成功！返回 true" << endl;
        return;
    }
    cout << "不匹配，继续滑动..." << endl;
    
    // 滑动窗口
    for (size_t i = n1; i < n2; ++i) {
        // 加入新字符，移除旧字符
        ++count2[byteIndex(s2[i])];
        --count2[byteIndex(s2[i - n1])];
        
        cout << "\n窗口 [" << i - n1 + 1 << "," << i << "]: \"";
        cout << s2.substr(i - n1 + 1, n1) << "\"" << endl;
        
        cout << "窗口字符统计: ";
        for (size_t j = 0; j < count2.size(); ++j) {
            if (count2[j] > 0) {
                cout << "byte[" << j << "]:" << count2[j] << " ";
            }
        }
        cout << endl;
        
        if (count1 == count2) {
            cout << ">>> 匹配成功！返回 true" << endl;
            return;
        }
        cout << "不匹配，继续滑动..." << endl;
    }
    
    cout << "\n遍历完成，未找到匹配。返回 false" << endl;
}

} // namespace day27::lc0567
