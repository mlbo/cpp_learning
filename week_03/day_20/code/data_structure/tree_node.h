/**
 * @file tree_node.h
 * @brief 二叉树节点定义和基本操作声明
 * @author C++ Tutorial
 * @date Day 20
 * 
 * 本文件定义了二叉树节点结构，以及树操作函数的声明。
 * 支持多种创建方式、遍历方式和辅助操作。
 */

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <iostream>
#include <vector>
#include <queue>
#include <optional>
#include <functional>

// ============================================================
// 二叉树节点定义
// ============================================================

/**
 * @brief 二叉树节点结构
 * 
 * 最基础的树节点，包含数据域和左右孩子指针。
 */
struct TreeNode {
    int val;           ///< 数据域：存储节点值
    TreeNode* left;    ///< 左孩子指针
    TreeNode* right;   ///< 右孩子指针
    
    /**
     * @brief 构造函数
     * @param x 节点值，默认为0
     * @param l 左孩子指针，默认为nullptr
     * @param r 右孩子指针，默认为nullptr
     */
    explicit TreeNode(int x = 0, TreeNode* l = nullptr, TreeNode* r = nullptr) 
        : val(x), left(l), right(r) {}
    
    /**
     * @brief 析构函数
     * @note 不自动删除子节点，避免级联删除
     */
    ~TreeNode() = default;
};

// ============================================================
// 树操作命名空间
// ============================================================

namespace tree_ops {

// ==================== 创建与销毁 ====================

/**
 * @brief 从层序数组创建二叉树
 * @param values 层序遍历数组，使用std::nullopt表示空节点
 * @return 树根节点指针
 * 
 * @example
 *   // 创建树: [3,9,20,null,null,15,7]
 *   auto root = createTree({3, 9, 20, std::nullopt, std::nullopt, 15, 7});
 */
TreeNode* createTree(const std::vector<std::optional<int>>& values);

/**
 * @brief 从vector创建二叉树（简化版本，用INT_MIN表示null）
 * @param values 层序遍历数组
 * @param nullValue 表示空节点的值，默认为INT_MIN
 * @return 树根节点指针
 */
TreeNode* createTreeSimple(const std::vector<int>& values, int nullValue = INT_MIN);

/**
 * @brief 释放二叉树内存
 * @param root 树根节点
 * @note 使用后序遍历释放，确保先释放子节点
 */
void deleteTree(TreeNode* root);

/**
 * @brief 创建树的深拷贝
 * @param root 源树根节点
 * @return 新树根节点
 */
TreeNode* copyTree(const TreeNode* root);

// ==================== 遍历操作 ====================

/**
 * @brief 层序遍历（BFS）并打印
 * @param root 树根节点
 * @param os 输出流
 */
void printLevelOrder(const TreeNode* root, std::ostream& os = std::cout);

/**
 * @brief 前序遍历（DFS）
 * @param root 树根节点
 * @param visit 访问函数
 */
void preOrder(const TreeNode* root, std::function<void(const TreeNode*)> visit);

/**
 * @brief 中序遍历（DFS）
 * @param root 树根节点
 * @param visit 访问函数
 */
void inOrder(const TreeNode* root, std::function<void(const TreeNode*)> visit);

/**
 * @brief 后序遍历（DFS）
 * @param root 树根节点
 * @param visit 访问函数
 */
void postOrder(const TreeNode* root, std::function<void(const TreeNode*)> visit);

/**
 * @brief 前序遍历，返回节点值数组
 * @param root 树根节点
 * @return 节点值数组
 */
std::vector<int> preOrderValues(const TreeNode* root);

/**
 * @brief 中序遍历，返回节点值数组
 * @param root 树根节点
 * @return 节点值数组
 */
std::vector<int> inOrderValues(const TreeNode* root);

/**
 * @brief 层序遍历，返回二维数组
 * @param root 树根节点
 * @return 每层的节点值数组
 */
std::vector<std::vector<int>> levelOrderValues(const TreeNode* root);

// ==================== 属性查询 ====================

/**
 * @brief 获取树的节点数量
 * @param root 树根节点
 * @return 节点数量
 */
int getNodeCount(const TreeNode* root);

/**
 * @brief 获取树的高度
 * @param root 树根节点
 * @return 树的高度，空树返回0
 */
int getHeight(const TreeNode* root);

/**
 * @brief 获取树的最大宽度
 * @param root 树根节点
 * @return 树的最大宽度
 */
int getMaxWidth(const TreeNode* root);

/**
 * @brief 检查是否为空树
 * @param root 树根节点
 * @return 是否为空
 */
inline bool isEmpty(const TreeNode* root) {
    return root == nullptr;
}

// ==================== 查找操作 ====================

/**
 * @brief 查找值为val的节点
 * @param root 树根节点
 * @param val 目标值
 * @return 找到的节点指针，未找到返回nullptr
 */
TreeNode* findNode(TreeNode* root, int val);
const TreeNode* findNode(const TreeNode* root, int val);

/**
 * @brief 检查值是否存在于树中
 * @param root 树根节点
 * @param val 目标值
 * @return 是否存在
 */
bool contains(const TreeNode* root, int val);

// ==================== 可视化 ====================

/**
 * @brief 打印树结构（横向显示）
 * @param root 树根节点
 * @param os 输出流
 * 
 * @example
 *       3
 *     ┌─┴─┐
 *     9  20
 *       ┌─┴─┐
 *       15  7
 */
void printTree(const TreeNode* root, std::ostream& os = std::cout);

// ==================== 辅助函数 ====================

/**
 * @brief 将层序遍历结果转换为字符串
 * @param levels 层序遍历结果
 * @return 格式化字符串
 */
std::string levelOrderToString(const std::vector<std::vector<int>>& levels);

/**
 * @brief 比较两棵树是否相等
 * @param t1 第一棵树
 * @param t2 第二棵树
 * @return 是否相等
 */
bool isSameTree(const TreeNode* t1, const TreeNode* t2);

} // namespace tree_ops

#endif // TREE_NODE_H
