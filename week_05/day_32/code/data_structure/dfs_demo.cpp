#include <cstddef>
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

struct TreeNode {
    explicit TreeNode(int value_in) : value(value_in) {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

void preorder_recursive(const TreeNode* node, std::vector<int>& order) {
    if (node == nullptr) {
        return;
    }
    order.push_back(node->value);
    preorder_recursive(node->left.get(), order);
    preorder_recursive(node->right.get(), order);
}

std::vector<int> preorder_iterative(const TreeNode* root) {
    std::vector<int> order;
    if (root == nullptr) {
        return order;
    }

    std::stack<const TreeNode*> pending;
    pending.push(root);
    while (!pending.empty()) {
        const TreeNode* node = pending.top();
        pending.pop();
        order.push_back(node->value);
        if (node->right != nullptr) {
            pending.push(node->right.get());
        }
        if (node->left != nullptr) {
            pending.push(node->left.get());
        }
    }
    return order;
}

void graph_dfs(const std::vector<std::vector<std::size_t>>& graph,
               std::size_t vertex,
               std::vector<bool>& visited,
               std::vector<std::size_t>& order) {
    if (visited[vertex]) {
        return;
    }
    visited[vertex] = true;
    order.push_back(vertex);
    for (const std::size_t neighbor : graph[vertex]) {
        graph_dfs(graph, neighbor, visited, order);
    }
}

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);
    root->left->left = std::make_unique<TreeNode>(4);
    root->left->right = std::make_unique<TreeNode>(5);

    std::vector<int> recursive_order;
    preorder_recursive(root.get(), recursive_order);
    const std::vector<int> iterative_order = preorder_iterative(root.get());
    expect(recursive_order == std::vector<int>({1, 2, 4, 5, 3}),
           "递归前序 DFS 顺序应正确");
    expect(iterative_order == recursive_order, "迭代栈应复现递归前序顺序");

    const std::vector<std::vector<std::size_t>> cyclic_graph{
        {1U, 2U}, {2U}, {0U, 3U}, {3U},
    };
    std::vector<bool> visited(cyclic_graph.size(), false);
    std::vector<std::size_t> graph_order;
    graph_dfs(cyclic_graph, 0U, visited, graph_order);
    expect(graph_order == std::vector<std::size_t>({0U, 1U, 2U, 3U}),
           "visited 应阻止有环图无限递归并保证每点只访问一次");

    std::cout << "树的递归/迭代 DFS 与有环图 visited 测试完成。\n";
    return failures == 0 ? 0 : 1;
}
