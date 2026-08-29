#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

struct BstNode {
    explicit BstNode(int value_in) : value(value_in) {}

    int value;
    std::unique_ptr<BstNode> left;
    std::unique_ptr<BstNode> right;
};

class BinarySearchTree {
public:
    bool insert(int value) {
        std::unique_ptr<BstNode>* link = &root_;
        while (*link != nullptr) {
            if (value < (*link)->value) {
                link = &((*link)->left);
            } else if (value > (*link)->value) {
                link = &((*link)->right);
            } else {
                return false;
            }
        }
        *link = std::make_unique<BstNode>(value);
        return true;
    }

    [[nodiscard]] bool contains(int value) const {
        const BstNode* node = root_.get();
        while (node != nullptr) {
            if (value == node->value) {
                return true;
            }
            node = value < node->value ? node->left.get() : node->right.get();
        }
        return false;
    }

    bool erase(int value) {
        return erase_node(root_, value);
    }

    [[nodiscard]] std::vector<int> inorder() const {
        std::vector<int> result;
        inorder_node(root_.get(), result);
        return result;
    }

    [[nodiscard]] int height() const {
        return height_of(root_.get());
    }

private:
    static bool erase_node(std::unique_ptr<BstNode>& node, int value) {
        if (node == nullptr) {
            return false;
        }
        if (value < node->value) {
            return erase_node(node->left, value);
        }
        if (value > node->value) {
            return erase_node(node->right, value);
        }

        if (node->left == nullptr) {
            node = std::move(node->right);
        } else if (node->right == nullptr) {
            node = std::move(node->left);
        } else {
            BstNode* successor = node->right.get();
            while (successor->left != nullptr) {
                successor = successor->left.get();
            }
            const int successor_value = successor->value;
            node->value = successor_value;
            static_cast<void>(erase_node(node->right, successor_value));
        }
        return true;
    }

    static void inorder_node(const BstNode* node, std::vector<int>& result) {
        if (node == nullptr) {
            return;
        }
        inorder_node(node->left.get(), result);
        result.push_back(node->value);
        inorder_node(node->right.get(), result);
    }

    static int height_of(const BstNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + std::max(height_of(node->left.get()), height_of(node->right.get()));
    }

    std::unique_ptr<BstNode> root_;
};

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    BinarySearchTree tree;
    for (const int value : {8, 3, 10, 1, 6, 14, 4, 7, 13}) {
        expect(tree.insert(value), "首次插入应成功");
    }
    expect(!tree.insert(6), "重复值契约是拒绝插入");
    expect(tree.contains(13), "应找到存在的值");
    expect(!tree.contains(5), "不存在的值不应被找到");
    expect(tree.inorder() == std::vector<int>({1, 3, 4, 6, 7, 8, 10, 13, 14}),
           "中序遍历应严格递增");

    expect(tree.erase(1), "应删除叶子节点");
    expect(tree.erase(14), "应删除单孩子节点");
    expect(tree.erase(3), "应删除双孩子节点");
    expect(!tree.erase(99), "删除不存在的值应报告失败");
    expect(tree.inorder() == std::vector<int>({4, 6, 7, 8, 10, 13}),
           "删除后仍应保持 BST 不变量");

    BinarySearchTree degenerate;
    for (int value = 1; value <= 7; ++value) {
        static_cast<void>(degenerate.insert(value));
    }
    expect(degenerate.height() == 7, "有序插入会使普通 BST 退化为链");

    std::cout << "BST 插入、查找、三类删除、重复值与退化测试完成。\n";
    return failures == 0 ? 0 : 1;
}
