/**
 * @file bfs_demo.cpp
 * @brief BFS广度优先搜索演示代码
 * 
 * 通过形象化的例子展示BFS的工作原理：
 * 1. 水波扩散效果
 * 2. 层序遍历过程可视化
 * 3. BFS与DFS对比演示
 * 
 * @author C++ Tutorial
 * @date Day 20
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#include <string>

#include "data_structure/tree_node.h"

// ============================================================
// 演示1：水波扩散效果
// ============================================================

/**
 * @brief 模拟水波扩散的BFS
 * 
 * 形象化展示BFS的"逐层扩展"特性
 */
void waterWaveDemo() {
    std::cout << "\n======== 演示1：水波扩散效果 ========\n";
    std::cout << "\n想象往湖面扔一块石头，水波从落点开始一层层向外扩散...\n";
    
    // 创建一个简单的图（用邻接表表示）
    // 中心节点是0，周围是1-6，最外层是7-12
    std::vector<std::vector<int>> graph = {
        {1, 2, 3, 4, 5, 6},    // 0: 中心
        {0, 7, 8},             // 1
        {0, 8, 9},             // 2
        {0, 9, 10},            // 3
        {0, 10, 11},           // 4
        {0, 11, 12},           // 5
        {0, 12, 7},            // 6
        {1, 6},                // 7
        {1, 2},                // 8
        {2, 3},                // 9
        {3, 4},                // 10
        {4, 5},                // 11
        {5, 6}                 // 12
    };
    
    int n = static_cast<int>(graph.size());
    std::vector<int> distance(n, -1);
    std::queue<int> q;
    
    int start = 0;  // 从中心开始
    
    q.push(start);
    distance[start] = 0;
    
    std::cout << "\n石头落点: 节点" << start << " (中心)\n";
    std::cout << "\n水波扩散过程:\n";
    
    int currentLevel = 0;
    std::vector<std::vector<int>> levels;
    levels.push_back({start});
    
    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<int> thisLevel;
        
        for (int i = 0; i < levelSize; ++i) {
            int node = q.front();
            q.pop();
            thisLevel.push_back(node);
            
            for (int neighbor : graph[node]) {
                if (distance[neighbor] == -1) {
                    distance[neighbor] = distance[node] + 1;
                    q.push(neighbor);
                }
            }
        }
        
        if (!thisLevel.empty() && !(thisLevel.size() == 1 && thisLevel[0] == start)) {
            levels.push_back(thisLevel);
        }
    }
    
    // 打印水波扩散效果
    for (size_t i = 0; i < levels.size(); ++i) {
        std::cout << "第" << i + 1 << "圈水波 (距离=" << i << "): ";
        for (int node : levels[i]) {
            std::cout << node << " ";
        }
        std::cout << "\n";
        
        // 可视化水波
        std::cout << "   ";
        for (int j = 0; j < static_cast<int>(i); ++j) {
            std::cout << "~~~~";
        }
        std::cout << "🌊 ";
        for (size_t j = 0; j < levels[i].size(); ++j) {
            std::cout << "● ";
        }
        std::cout << "\n";
    }
}

// ============================================================
// 演示2：BFS逐层遍历过程可视化
// ============================================================

/**
 * @brief BFS遍历过程详细可视化
 */
void bfsVisualizationDemo() {
    std::cout << "\n======== 演示2：BFS逐层遍历可视化 ========\n";
    
    // 创建测试树
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    auto root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    
    std::cout << "\n【树结构】\n";
    std::cout << "        1        \n";
    std::cout << "       / \\       \n";
    std::cout << "      2   3      \n";
    std::cout << "     / \\   \\     \n";
    std::cout << "    4   5   6    \n";
    
    std::cout << "\n【BFS遍历过程】\n";
    
    std::queue<TreeNode*> q;
    q.push(root);
    int level = 0;
    
    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        
        std::cout << "\n┌─────────────────────────────────────┐\n";
        std::cout << "│ 第" << std::setw(2) << ++level << "层处理                              │\n";
        std::cout << "├─────────────────────────────────────┤\n";
        
        // 显示当前队列状态
        std::cout << "│ 队列中节点: ";
        std::queue<TreeNode*> tempQ = q;
        while (!tempQ.empty()) {
            std::cout << tempQ.front()->val << " ";
            tempQ.pop();
        }
        std::cout << "\n";
        
        std::cout << "│ 当前层节点数: " << levelSize << "\n";
        std::cout << "│ 处理顺序: ";
        
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();
            
            std::cout << node->val;
            if (i < levelSize - 1) std::cout << " -> ";
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        std::cout << "\n";
        
        std::cout << "└─────────────────────────────────────┘\n";
    }
    
    // 清理
    tree_ops::deleteTree(root);
}

// ============================================================
// 演示3：BFS与DFS对比
// ============================================================

/**
 * @brief 对比BFS和DFS的遍历顺序
 */
void bfsVsDfsDemo() {
    std::cout << "\n======== 演示3：BFS vs DFS 对比 ========\n";
    
    // 创建测试树
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    auto root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    
    std::cout << "\n【树结构】\n";
    std::cout << "        1        \n";
    std::cout << "       / \\       \n";
    std::cout << "      2   3      \n";
    std::cout << "     / \\   \\     \n";
    std::cout << "    4   5   6    \n";
    
    // BFS遍历
    std::cout << "\n【BFS遍历（逐层扩展）】\n";
    std::cout << "遍历顺序: ";
    std::vector<int> bfsOrder;
    std::queue<TreeNode*> bfsQ;
    bfsQ.push(root);
    
    while (!bfsQ.empty()) {
        TreeNode* node = bfsQ.front();
        bfsQ.pop();
        bfsOrder.push_back(node->val);
        
        if (node->left) bfsQ.push(node->left);
        if (node->right) bfsQ.push(node->right);
    }
    
    for (size_t i = 0; i < bfsOrder.size(); ++i) {
        if (i > 0) std::cout << " -> ";
        std::cout << bfsOrder[i];
    }
    std::cout << "\n";
    std::cout << "特点: 一层一层，从左到右，逐层处理\n";
    
    // DFS遍历（前序）
    std::cout << "\n【DFS遍历（深入到底）】\n";
    std::cout << "遍历顺序: ";
    std::vector<int> dfsOrder;
    std::stack<TreeNode*> dfsS;
    dfsS.push(root);
    
    while (!dfsS.empty()) {
        TreeNode* node = dfsS.top();
        dfsS.pop();
        dfsOrder.push_back(node->val);
        
        // 注意：栈是后进先出，所以先压右孩子
        if (node->right) dfsS.push(node->right);
        if (node->left) dfsS.push(node->left);
    }
    
    for (size_t i = 0; i < dfsOrder.size(); ++i) {
        if (i > 0) std::cout << " -> ";
        std::cout << dfsOrder[i];
    }
    std::cout << "\n";
    std::cout << "特点: 一条路走到底，回溯再继续\n";
    
    // 对比表格
    std::cout << "\n【对比总结】\n";
    std::cout << "+----------------+------------------+------------------+\n";
    std::cout << "|     特性       |       BFS        |       DFS        |\n";
    std::cout << "+----------------+------------------+------------------+\n";
    std::cout << "| 数据结构       |     队列 Queue   |     栈 Stack     |\n";
    std::cout << "| 遍历方式       |     逐层扩展     |     深入到底     |\n";
    std::cout << "| 空间复杂度     |     O(宽度)      |     O(深度)      |\n";
    std::cout << "| 最短路径       |       支持       |     不保证       |\n";
    std::cout << "+----------------+------------------+------------------+\n";
    
    // 清理
    tree_ops::deleteTree(root);
}

// ============================================================
// 演示4：队列工作原理
// ============================================================

/**
 * @brief 展示队列在BFS中的作用
 */
void queueDemo() {
    std::cout << "\n======== 演示4：队列工作原理 ========\n";
    
    std::cout << "\n队列特点：FIFO (First In First Out) 先进先出\n";
    std::cout << "就像排队买票，先来的人先买\n\n";
    
    // 创建简单树
    //     1
    //    / \
    //   2   3
    auto root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    
    std::queue<TreeNode*> q;
    q.push(root);
    
    std::cout << "【队列变化过程】\n\n";
    
    int step = 0;
    while (!q.empty()) {
        std::cout << "步骤 " << ++step << ":\n";
        
        // 显示队列状态
        std::cout << "  队列: [";
        std::queue<TreeNode*> temp = q;
        bool first = true;
        while (!temp.empty()) {
            if (!first) std::cout << ", ";
            std::cout << temp.front()->val;
            temp.pop();
            first = false;
        }
        std::cout << "] ← 尾部入队\n";
        std::cout << "         ↑\n";
        std::cout << "       头部出队\n\n";
        
        TreeNode* node = q.front();
        q.pop();
        
        std::cout << "  取出节点: " << node->val << "\n";
        
        if (node->left) {
            q.push(node->left);
            std::cout << "  节点" << node->val << "的左孩子" << node->left->val << "入队\n";
        }
        if (node->right) {
            q.push(node->right);
            std::cout << "  节点" << node->val << "的右孩子" << node->right->val << "入队\n";
        }
        
        std::cout << "\n";
    }
    
    std::cout << "队列空，遍历结束！\n";
    
    // 清理
    tree_ops::deleteTree(root);
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           BFS广度优先搜索 - 形象化演示                      ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n";
    
    waterWaveDemo();
    bfsVisualizationDemo();
    bfsVsDfsDemo();
    queueDemo();
    
    std::cout << "\n======== 所有演示完成 ========\n";
    std::cout << "\n关键记忆点：\n";
    std::cout << "  1. BFS使用队列，逐层扩展\n";
    std::cout << "  2. 先记录当前层大小，再处理\n";
    std::cout << "  3. 适合最短路径、层序遍历问题\n";
    std::cout << "  4. 队列的FIFO特性保证层级顺序\n";
    
    return 0;
}
