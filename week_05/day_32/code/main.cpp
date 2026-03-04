/**
 * Day 32: DFS深度优先搜索 - 主程序
 */

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

// 二叉树节点
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 前向声明
void dfsDemo();
void atomicDemo();
void item39_40Demo();

// DFS遍历
void dfs(TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) return;
    result.push_back(root->val);
    dfs(root->left, result);
    dfs(root->right, result);
}

// 计算树深度
int maxDepth(TreeNode* root) {
    if (root == nullptr) return 0;
    return 1 + std::max(maxDepth(root->left), maxDepth(root->right));
}

// 删除树
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::cout << "=== Day 32: DFS深度优先搜索 ===" << std::endl;
    
    // 1. DFS演示
    std::cout << "\n--- 1. DFS基础 ---" << std::endl;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    std::vector<int> result;
    dfs(root, result);
    
    std::cout << "DFS遍历结果: ";
    for (int v : result) std::cout << v << " ";
    std::cout << std::endl;
    std::cout << "树深度: " << maxDepth(root) << std::endl;
    deleteTree(root);
    
    // 2. atomic演示
    std::cout << "\n--- 2. atomic原子操作 ---" << std::endl;
    atomicDemo();
    
    // 3. EMC++ Item 39-40
    std::cout << "\n--- 3. EMC++ Item 39-40 ---" << std::endl;
    item39_40Demo();
    
    std::cout << "\n=== Day 32 学习完成 ===" << std::endl;
    return 0;
}

void atomicDemo() {
    std::atomic<int> counter(0);
    
    // 多线程原子递增
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    
    for (auto& t : threads) t.join();
    std::cout << "原子计数器结果: " << counter.load() << std::endl;
    
    // CAS操作演示
    int expected = 5000;
    bool success = counter.compare_exchange_strong(expected, 0);
    std::cout << "CAS操作: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "重置后counter: " << counter.load() << std::endl;
}

void item39_40Demo() {
    std::cout << "Item 39: atomic用于并发，volatile用于特殊内存" << std::endl;
    std::cout << "  - atomic保证原子性和内存序" << std::endl;
    std::cout << "  - volatile只告诉编译器不要优化访问" << std::endl;
    std::cout << "  - volatile不保证线程安全！" << std::endl;
    
    std::cout << "\nItem 40: void future用于一次性事件通信" << std::endl;
    std::cout << "  - promise<void>/future<void>比条件变量更简洁" << std::endl;
}
