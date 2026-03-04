/**
 * 字符串操作演示
 * 展示C++ std::string的各种常用操作
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// 打印标题
void printTitle(const string& title) {
    cout << "\n========== " << title << " ==========" << endl;
}

// 演示字符串初始化
void demoInitialization() {
    printTitle("字符串初始化");
    
    // 方式1：默认构造
    string s1;
    cout << "string s1; -> \"" << s1 << "\" (空字符串)" << endl;
    
    // 方式2：从C字符串构造
    string s2 = "Hello";
    cout << "string s2 = \"Hello\"; -> \"" << s2 << "\"" << endl;
    
    // 方式3：重复字符
    string s3(5, 'a');
    cout << "string s3(5, 'a'); -> \"" << s3 << "\"" << endl;
    
    // 方式4：拷贝构造
    string s4(s2);
    cout << "string s4(s2); -> \"" << s4 << "\"" << endl;
    
    // 方式5：从部分C字符串
    string s5("Hello World", 5);
    cout << "string s5(\"Hello World\", 5); -> \"" << s5 << "\"" << endl;
    
    // 方式6：从迭代器范围
    string s6(s2.begin(), s2.begin() + 3);
    cout << "string s6(s2.begin(), s2.begin()+3); -> \"" << s6 << "\"" << endl;
    
    // 方式7：移动构造（C++11）
    string s7 = string("Moved");
    cout << "string s7 = string(\"Moved\"); -> \"" << s7 << "\"" << endl;
}

// 演示容量操作
void demoCapacity() {
    printTitle("容量操作");
    
    string s = "Hello, C++ String!";
    
    cout << "字符串: \"" << s << "\"" << endl;
    cout << "size() = " << s.size() << " (字符数量)" << endl;
    cout << "length() = " << s.length() << " (同size)" << endl;
    cout << "capacity() = " << s.capacity() << " (已分配空间)" << endl;
    cout << "empty() = " << (s.empty() ? "true" : "false") << endl;
    
    // 预留空间
    cout << "\n预留空间 reserve(50):" << endl;
    s.reserve(50);
    cout << "capacity() = " << s.capacity() << endl;
    
    // 缩小到合适大小
    cout << "\n缩小空间 shrink_to_fit():" << endl;
    s.shrink_to_fit();
    cout << "capacity() = " << s.capacity() << endl;
    
    // 清空
    string temp = s;
    temp.clear();
    cout << "\nclear()后 empty() = " << (temp.empty() ? "true" : "false") << endl;
}

// 演示元素访问
void demoAccess() {
    printTitle("元素访问");
    
    string s = "Hello World";
    cout << "字符串: \"" << s << "\"" << endl;
    
    // 下标访问
    cout << "\n下标访问:" << endl;
    cout << "s[0] = '" << s[0] << "'" << endl;
    cout << "s[4] = '" << s[4] << "'" << endl;
    
    // at()访问（带边界检查）
    cout << "\nat()访问（带边界检查）:" << endl;
    cout << "s.at(0) = '" << s.at(0) << "'" << endl;
    
    // front()和back()
    cout << "\nfront()和back():" << endl;
    cout << "s.front() = '" << s.front() << "' (首字符)" << endl;
    cout << "s.back() = '" << s.back() << "' (尾字符)" << endl;
    
    // data()和c_str()
    cout << "\ndata()和c_str():" << endl;
    cout << "s.data() 返回字符数组指针" << endl;
    cout << "s.c_str() 返回C风格字符串（以\\0结尾）" << endl;
}

// 演示字符串拼接
void demoConcatenation() {
    printTitle("字符串拼接");
    
    string s1 = "Hello";
    string s2 = "World";
    
    // 使用 + 运算符
    string s3 = s1 + " " + s2;
    cout << "s1 + \" \" + s2 = \"" << s3 << "\"" << endl;
    
    // 使用 += 运算符
    s1 += " ";
    s1 += s2;
    cout << "s1 += \" \" + s2 后 s1 = \"" << s1 << "\"" << endl;
    
    // 使用 append()
    string s4 = "Hello";
    s4.append(" ");
    s4.append("World");
    cout << "append()结果: \"" << s4 << "\"" << endl;
    
    // append() 部分字符串
    string s5 = "Hello";
    s5.append(" World!!!", 6);  // 只追加前6个字符
    cout << "append(\" World!!!\", 6)结果: \"" << s5 << "\"" << endl;
    
    // push_back()
    string s6 = "Hello";
    s6.push_back('!');
    cout << "push_back('!')结果: \"" << s6 << "\"" << endl;
}

// 演示字符串查找
void demoSearch() {
    printTitle("字符串查找");
    
    string s = "Hello World, Hello C++";
    cout << "字符串: \"" << s << "\"" << endl;
    
    // find() - 从前往后找
    cout << "\nfind()搜索:" << endl;
    cout << "find(\"Hello\") = " << s.find("Hello") << endl;
    cout << "find(\"Hello\", 2) = " << s.find("Hello", 2) << " (从位置2开始)" << endl;
    cout << "find('o') = " << s.find('o') << endl;
    
    // rfind() - 从后往前找
    cout << "\nrfind()搜索:" << endl;
    cout << "rfind(\"Hello\") = " << s.rfind("Hello") << endl;
    cout << "rfind('o') = " << s.rfind('o') << endl;
    
    // find_first_of() - 找任意字符首次出现
    cout << "\nfind_first_of()搜索:" << endl;
    cout << "find_first_of(\"aeiou\") = " << s.find_first_of("aeiou") << " (第一个元音)" << endl;
    cout << "find_first_of(\"xyz\") = " << s.find_first_of("xyz") << " (npos=未找到)" << endl;
    
    // find_last_of() - 找任意字符最后出现
    cout << "\nfind_last_of()搜索:" << endl;
    cout << "find_last_of(\"aeiou\") = " << s.find_last_of("aeiou") << " (最后一个元音)" << endl;
    
    // find_first_not_of() - 找不在集合中的字符
    cout << "\nfind_first_not_of()搜索:" << endl;
    cout << "find_first_not_of(\"Hello\") = " << s.find_first_not_of("Hello") << " (第一个不在Helo中的字符)" << endl;
}

// 演示子串操作
void demoSubstring() {
    printTitle("子串操作");
    
    string s = "Hello World";
    cout << "字符串: \"" << s << "\"" << endl;
    
    // substr() - 获取子串
    cout << "\nsubstr()操作:" << endl;
    cout << "substr(0, 5) = \"" << s.substr(0, 5) << "\"" << endl;
    cout << "substr(6) = \"" << s.substr(6) << "\" (从位置6到结尾)" << endl;
    
    // 字符串比较
    cout << "\n字符串比较:" << endl;
    string s1 = "apple";
    string s2 = "banana";
    cout << "\"apple\" compare \"banana\" = " << s1.compare(s2) << " (负数=小于)" << endl;
    cout << "\"banana\" compare \"apple\" = " << s2.compare(s1) << " (正数=大于)" << endl;
    cout << "\"apple\" compare \"apple\" = " << s1.compare(s1) << " (零=相等)" << endl;
}

// 演示修改操作
void demoModification() {
    printTitle("修改操作");
    
    // insert() - 插入
    string s1 = "Hello World";
    cout << "原始: \"" << s1 << "\"" << endl;
    s1.insert(5, " Beautiful");
    cout << "insert(5, \" Beautiful\"): \"" << s1 << "\"" << endl;
    
    // erase() - 删除
    string s2 = "Hello Beautiful World";
    cout << "\n原始: \"" << s2 << "\"" << endl;
    s2.erase(5, 10);  // 从位置5开始删除10个字符
    cout << "erase(5, 10): \"" << s2 << "\"" << endl;
    
    // replace() - 替换
    string s3 = "Hello World";
    cout << "\n原始: \"" << s3 << "\"" << endl;
    s3.replace(6, 5, "C++");
    cout << "replace(6, 5, \"C++\"): \"" << s3 << "\"" << endl;
    
    // pop_back() - 删除末尾字符
    string s4 = "Hello!";
    cout << "\n原始: \"" << s4 << "\"" << endl;
    s4.pop_back();
    cout << "pop_back(): \"" << s4 << "\"" << endl;
    
    // swap() - 交换
    string a = "First";
    string b = "Second";
    cout << "\n交换前: a=\"" << a << "\", b=\"" << b << "\"" << endl;
    a.swap(b);
    cout << "交换后: a=\"" << a << "\", b=\"" << b << "\"" << endl;
}

// 演示字符串转换
void demoConversion() {
    printTitle("字符串转换");
    
    // 数字转字符串
    int num = 12345;
    double pi = 3.14159;
    
    cout << "数字转字符串:" << endl;
    cout << "to_string(12345) = \"" << to_string(num) << "\"" << endl;
    cout << "to_string(3.14159) = \"" << to_string(pi) << "\"" << endl;
    
    // 字符串转数字
    string sInt = "12345";
    string sDouble = "3.14159";
    
    cout << "\n字符串转数字:" << endl;
    cout << "stoi(\"12345\") = " << stoi(sInt) << endl;
    cout << "stod(\"3.14159\") = " << stod(sDouble) << endl;
    
    // 使用stringstream
    cout << "\n使用stringstream:" << endl;
    stringstream ss;
    ss << "Value: " << 42 << ", Pi: " << 3.14;
    cout << ss.str() << endl;
    
    // 大小写转换
    string s = "Hello World";
    cout << "\n大小写转换:" << endl;
    cout << "原始: \"" << s << "\"" << endl;
    
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    cout << "转大写: \"" << s << "\"" << endl;
    
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    cout << "转小写: \"" << s << "\"" << endl;
}

// 演示字符串分割
void demoSplit() {
    printTitle("字符串分割");
    
    string s = "apple,banana,orange,grape";
    cout << "字符串: \"" << s << "\"" << endl;
    
    // 手动分割
    cout << "\n按','分割:" << endl;
    vector<string> tokens;
    stringstream ss(s);
    string token;
    
    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "  [" << i << "]: \"" << tokens[i] << "\"" << endl;
    }
    
    // 按空格分割
    string sentence = "Hello World from C++";
    cout << "\n句子: \"" << sentence << "\"" << endl;
    cout << "按空格分割:" << endl;
    
    stringstream ss2(sentence);
    while (ss2 >> token) {
        cout << "  \"" << token << "\"" << endl;
    }
}

// 演示遍历方式
void demoIteration() {
    printTitle("字符串遍历");
    
    string s = "Hello";
    cout << "字符串: \"" << s << "\"" << endl;
    
    // 方式1：下标遍历
    cout << "\n方式1：下标遍历" << endl;
    for (size_t i = 0; i < s.size(); i++) {
        cout << "s[" << i << "] = '" << s[i] << "'" << endl;
    }
    
    // 方式2：迭代器遍历
    cout << "\n方式2：迭代器遍历" << endl;
    for (auto it = s.begin(); it != s.end(); ++it) {
        cout << "'" << *it << "'" << endl;
    }
    
    // 方式3：范围for循环（C++11）
    cout << "\n方式3：范围for循环" << endl;
    for (char c : s) {
        cout << "'" << c << "'" << endl;
    }
    
    // 方式4：范围for + 引用（可修改）
    cout << "\n方式4：范围for + 引用（可修改）" << endl;
    string s2 = s;
    for (char& c : s2) {
        c = toupper(c);  // 转大写
    }
    cout << "修改后: \"" << s2 << "\"" << endl;
}

int main() {
    cout << R"(
╔════════════════════════════════════════╗
║     C++ 字符串操作演示                 ║
╚════════════════════════════════════════╝
)" << endl;

    demoInitialization();
    demoCapacity();
    demoAccess();
    demoConcatenation();
    demoSearch();
    demoSubstring();
    demoModification();
    demoConversion();
    demoSplit();
    demoIteration();

    cout << "\n========== 演示完成 ==========" << endl;
    return 0;
}
