/**
 * 字符串匹配算法演示
 * 展示暴力匹配、KMP算法、Rabin-Karp算法
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

namespace {

long long byteValue(char character) {
    return static_cast<long long>(static_cast<unsigned char>(character));
}

} // namespace

// 打印标题
void printTitle(const string& title) {
    cout << "\n========== " << title << " ==========" << endl;
}

// 打印分隔线
void printLine() {
    cout << "----------------------------------------" << endl;
}

// ==================== 1. 暴力匹配算法 ====================

/**
 * 暴力匹配算法
 * 时间复杂度：O(n*m)，其中n是文本长度，m是模式长度
 * 空间复杂度：O(1)
 */
string::size_type bruteForceMatch(const string& text, const string& pattern) {
    const string::size_type n = text.size();
    const string::size_type m = pattern.size();
    if (m == 0) return 0;
    if (m > n) return string::npos;

    for (string::size_type i = 0; i + m <= n; ++i) {
        string::size_type j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            return i;  // 找到匹配，返回起始位置
        }
    }
    
    return string::npos;  // 未找到；与 std::string::find 的约定一致
}

/**
 * 暴力匹配算法 - 详细演示版
 */
void demonstrateBruteForce() {
    printTitle("暴力匹配算法");
    
    cout << R"(
算法思想：
从文本串的每个位置开始，逐字符与模式串比较。
如果全部匹配成功，则找到答案；否则移动到下一个位置继续尝试。

时间复杂度：O(n*m) - 最坏情况下
空间复杂度：O(1)
)" << endl;
    
    string text = "ABABABCABABCABAB";
    string pattern = "ABABCABAB";
    
    cout << "文本串: \"" << text << "\"" << endl;
    cout << "模式串: \"" << pattern << "\"" << endl;
    cout << "\n匹配过程演示:" << endl;
    
    const string::size_type n = text.size();
    const string::size_type m = pattern.size();
    size_t comparisons = 0;
    
    for (string::size_type i = 0; i + m <= n; ++i) {
        cout << "\n位置 " << i << ": ";
        cout << text.substr(0, i) << "[";
        
        string::size_type j = 0;
        bool match = true;
        string matchStr = "";
        
        while (j < m) {
            ++comparisons;
            cout << text[i + j];
            matchStr += text[i + j];
            
            if (text[i + j] != pattern[j]) {
                cout << "]≠\"" << pattern << "\" (在位置" << j << "处失配)";
                match = false;
                break;
            }
            ++j;
        }
        
        if (match) {
            cout << "]=\"" << pattern << "\" ✓ 找到匹配！";
            cout << "\n\n匹配位置: " << i << endl;
            cout << "总比较次数: " << comparisons << endl;
            return;
        }
    }
    
    cout << "\n\n未找到匹配" << endl;
    cout << "总比较次数: " << comparisons << endl;
}

// ==================== 2. KMP算法 ====================

/**
 * 计算next数组（部分匹配表）
 * next[i]表示pattern[0...i]的最长相等前后缀长度
 */
vector<string::size_type> computeNext(const string& pattern) {
    const string::size_type m = pattern.size();
    vector<string::size_type> next(m, 0);
    
    string::size_type j = 0;  // 当前最长相等前后缀长度
    for (string::size_type i = 1; i < m; ++i) {
        // 不匹配时回退
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        
        // 匹配时扩展
        if (pattern[i] == pattern[j]) {
            ++j;
        }
        
        next[i] = j;
    }
    
    return next;
}

/**
 * KMP匹配算法
 * 时间复杂度：O(n+m)
 * 空间复杂度：O(m)
 */
string::size_type kmpMatch(const string& text, const string& pattern) {
    const string::size_type n = text.size();
    const string::size_type m = pattern.size();
    
    if (m == 0) return 0;
    if (n < m) return string::npos;
    
    // 计算next数组
    const vector<string::size_type> next = computeNext(pattern);
    
    string::size_type j = 0;  // 模式串指针
    for (string::size_type i = 0; i < n; ++i) {
        // 失配时根据next数组跳转
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        
        // 匹配时前进
        if (text[i] == pattern[j]) {
            ++j;
        }
        
        // 完全匹配
        if (j == m) {
            return i + 1U - m;
        }
    }
    
    return string::npos;
}

/**
 * KMP算法 - 详细演示版
 */
void demonstrateKMP() {
    printTitle("KMP算法");
    
    cout << R"(
算法思想：
当匹配失败时，利用已匹配信息，避免回溯文本串指针。
通过预处理模式串，构建next数组，指示失配时模式串应跳转的位置。

核心概念：
- 前缀：不包含最后一个字符的所有以第一个字符开头的连续子串
- 后缀：不包含第一个字符的所有以最后一个字符结尾的连续子串
- next[i]：pattern[0...i]的最长相等前后缀长度

时间复杂度：O(n+m) - 线性时间
空间复杂度：O(m) - 存储next数组
)" << endl;
    
    string pattern = "ABABCABAB";
    cout << "模式串: \"" << pattern << "\"" << endl;
    
    // 计算并展示next数组
    const vector<string::size_type> next = computeNext(pattern);
    
    cout << "\n构建next数组（部分匹配表）:" << endl;
    cout << "索引: ";
    for (string::size_type i = 0; i < pattern.size(); ++i) {
        cout << i << " ";
    }
    cout << "\n字符: ";
    for (char c : pattern) {
        cout << c << " ";
    }
    cout << "\nnext: ";
    for (string::size_type value : next) {
        cout << value << " ";
    }
    cout << endl;
    
    cout << "\nnext数组含义解释:" << endl;
    for (string::size_type i = 0; i < pattern.size(); ++i) {
        cout << "  next[" << i << "] = " << next[i];
        cout << " (\"" << pattern.substr(0, i + 1) << "\"的最长相等前后缀长度)" << endl;
    }
    
    // 匹配演示
    string text = "ABABABCABABCABAB";
    cout << "\n文本串: \"" << text << "\"" << endl;
    cout << "模式串: \"" << pattern << "\"" << endl;
    
    cout << "\nKMP匹配过程:" << endl;
    const string::size_type n = text.size();
    const string::size_type m = pattern.size();
    string::size_type j = 0;
    size_t comparisons = 0;
    
    for (string::size_type i = 0; i < n; ++i) {
        cout << "i=" << i << " ('" << text[i] << "'), j=" << j;
        
        while (j > 0 && text[i] != pattern[j]) {
            const string::size_type oldJ = j;
            j = next[j - 1];
            cout << " -> 失配，j从" << oldJ << "跳到" << j;
            ++comparisons;
        }
        
        if (text[i] == pattern[j]) {
            ++j;
            ++comparisons;
            cout << " -> 匹配，j=" << j;
        } else {
            ++comparisons;
            cout << " -> 失配";
        }
        
        cout << endl;
        
        if (j == m) {
            cout << "\n找到匹配！起始位置: " << i + 1U - m << endl;
            cout << "总比较次数: " << comparisons << endl;
            return;
        }
    }
}

// ==================== 3. Rabin-Karp算法 ====================

/**
 * Rabin-Karp算法
 * 使用滚动哈希进行字符串匹配
 */
string::size_type rabinKarpMatch(const string& text, const string& pattern) {
    const string::size_type n = text.size();
    const string::size_type m = pattern.size();
    
    if (m == 0) return 0;
    if (n < m) return string::npos;
    
    constexpr long long BASE = 256;       // 字符集基数
    constexpr long long MOD = 1'000'000'007; // 大质数取模
    
    // 计算BASE^(m-1) % MOD
    long long h = 1;
    for (string::size_type i = 1; i < m; ++i) {
        h = (h * BASE) % MOD;
    }
    
    // 计算模式串和文本串第一个窗口的哈希值
    long long patternHash = 0;
    long long textHash = 0;
    
    for (string::size_type i = 0; i < m; ++i) {
        patternHash = (patternHash * BASE + byteValue(pattern[i])) % MOD;
        textHash = (textHash * BASE + byteValue(text[i])) % MOD;
    }
    
    // 滑动窗口
    for (string::size_type i = 0; i + m <= n; ++i) {
        // 哈希值相等，进行精确比较
        if (patternHash == textHash) {
            bool match = true;
            for (string::size_type j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
        
        // 计算下一个窗口的哈希值（滚动哈希）
        if (i + m < n) {
            textHash = ((textHash - byteValue(text[i]) * h) * BASE
                      + byteValue(text[i + m])) % MOD;
            if (textHash < 0) textHash += MOD;
        }
    }
    
    return string::npos;
}

/**
 * Rabin-Karp算法 - 详细演示版
 */
void demonstrateRabinKarp() {
    printTitle("Rabin-Karp算法");
    
    cout << R"(
算法思想：
将字符串比较转化为数值比较。使用哈希函数计算模式串和文本串
中各子串的哈希值，通过比较哈希值来判断是否可能匹配。

核心概念：
- 滚动哈希：窗口滑动时，增量更新哈希值，O(1)时间
- 多项式哈希：H(s) = s[0]*d^(m-1) + s[1]*d^(m-2) + ... + s[m-1]

时间复杂度：平均O(n+m)，最坏O(n*m)
空间复杂度：O(1)
)" << endl;
    
    string text = "ABABABCABABCABAB";
    string pattern = "ABABCABAB";
    
    cout << "文本串: \"" << text << "\"" << endl;
    cout << "模式串: \"" << pattern << "\"" << endl;
    
    const string::size_type n = text.size();
    const string::size_type m = pattern.size();
    
    constexpr long long BASE = 256;
    constexpr long long MOD = 1'000'000'007;
    
    // 计算模式串哈希
    long long patternHash = 0;
    for (char c : pattern) {
        patternHash = (patternHash * BASE + byteValue(c)) % MOD;
    }
    cout << "\n模式串哈希值: " << patternHash << endl;
    
    // 计算h = BASE^(m-1)
    long long h = 1;
    for (string::size_type i = 1; i < m; ++i) {
        h = (h * BASE) % MOD;
    }
    
    // 滑动窗口演示
    cout << "\n滑动窗口计算哈希值:" << endl;
    long long textHash = 0;
    
    // 初始窗口哈希
    for (string::size_type i = 0; i < m; ++i) {
        textHash = (textHash * BASE + byteValue(text[i])) % MOD;
    }
    
    for (string::size_type i = 0; i + m <= n; ++i) {
        string window = text.substr(i, m);
        cout << "窗口[" << i << "]: \"" << window << "\"";
        cout << " -> 哈希值: " << textHash;
        
        if (textHash == patternHash) {
            cout << " (匹配！)";
            // 精确比较
            bool match = true;
            for (string::size_type j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                cout << " 精确匹配成功！";
                cout << "\n\n找到匹配！起始位置: " << i << endl;
                return;
            } else {
                cout << " (哈希冲突，精确匹配失败)";
            }
        }
        
        cout << endl;
        
        // 滚动更新哈希
        if (i + m < n) {
            textHash = ((textHash - byteValue(text[i]) * h) * BASE
                      + byteValue(text[i + m])) % MOD;
            if (textHash < 0) textHash += MOD;
        }
    }
    
    cout << "\n未找到匹配" << endl;
}

// ==================== 4. 算法对比 ====================

void compareAlgorithms() {
    printTitle("字符串匹配算法对比");
    
    cout << R"(
┌─────────────┬──────────────┬──────────────┬─────────────────────────┐
│   算法      │ 时间复杂度    │ 空间复杂度    │ 特点                    │
├─────────────┼──────────────┼──────────────┼─────────────────────────┤
│ 暴力匹配    │ O(n*m)       │ O(1)         │ 简单直观，适合短字符串   │
│             │              │              │ 无额外空间需求          │
├─────────────┼──────────────┼──────────────┼─────────────────────────┤
│ KMP         │ O(n+m)       │ O(m)         │ 线性时间，预处理模式串   │
│             │              │              │ 适合多次匹配同一模式     │
├─────────────┼──────────────┼──────────────┼─────────────────────────┤
│ Rabin-Karp  │ O(n+m)平均   │ O(1)         │ 哈希思想，适合多模式匹配 │
│             │ O(n*m)最坏   │              │ 可能发生哈希冲突        │
└─────────────┴──────────────┴──────────────┴─────────────────────────┘

实际选择建议：
1. 简单场景、短字符串：暴力匹配足够
2. 单模式多次匹配：KMP效率高
3. 多模式匹配：Rabin-Karp或AC自动机
4. 工程实践：标准库不承诺内部查找算法；先用库接口，再按测量证据选择专用算法
)" << endl;
}

// ==================== 5. 实际应用示例 ====================

void demonstrateApplications() {
    printTitle("实际应用示例");
    
    cout << "1. 在文本中查找所有出现位置：" << endl;
    string text = "ABABAABAABABAABA";
    string pattern = "ABA";
    
    cout << "   文本: \"" << text << "\"" << endl;
    cout << "   模式: \"" << pattern << "\"" << endl;
    
    vector<string::size_type> positions;
    size_t pos = 0;
    while ((pos = text.find(pattern, pos)) != string::npos) {
        positions.push_back(pos);
        pos++;
    }
    
    cout << "   找到位置: ";
    for (size_t i = 0; i < positions.size(); i++) {
        cout << positions[i];
        if (i < positions.size() - 1) cout << ", ";
    }
    cout << endl;
    
    cout << "\n2. 判断字符串是否是另一个的子串：" << endl;
    string s1 = "world";
    string s2 = "hello world";
    bool isSubstring = (s2.find(s1) != string::npos);
    cout << "   \"" << s1 << "\" 是 \"" << s2 << "\" 的子串: " 
         << (isSubstring ? "是" : "否") << endl;
    
    cout << "\n3. 统计模式串出现次数：" << endl;
    size_t count = 0;
    pos = 0;
    while ((pos = text.find(pattern, pos)) != string::npos) {
        ++count;
        ++pos;
    }
    cout << "   \"" << pattern << "\" 在 \"" << text << "\" 中出现 " 
         << count << " 次" << endl;
}

int main() {
    cout << R"(
╔════════════════════════════════════════╗
║       字符串匹配算法演示               ║
╚════════════════════════════════════════╝
)" << endl;

    demonstrateBruteForce();
    demonstrateKMP();
    demonstrateRabinKarp();
    compareAlgorithms();
    demonstrateApplications();

    cout << "\n========== 演示完成 ==========" << endl;
    return 0;
}
