/**
 * LeetCode 297: 二叉树序列化与反序列化
 */

#include <iostream>
#include <string>
#include <sstream>
#include <queue>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Codec {
public:
    // 序列化：前序遍历
    std::string serialize(TreeNode* root) {
        std::string result;
        serializeHelper(root, result);
        return result;
    }
    
    void serializeHelper(TreeNode* root, std::string& result) {
        if (!root) {
            result += "null,";
            return;
        }
        result += std::to_string(root->val) + ",";
        serializeHelper(root->left, result);
        serializeHelper(root->right, result);
    }
    
    // 反序列化
    TreeNode* deserialize(std::string data) {
        std::queue<std::string> nodes;
        std::string node;
        std::stringstream ss(data);
        while (std::getline(ss, node, ',')) {
            nodes.push(node);
        }
        return deserializeHelper(nodes);
    }
    
    TreeNode* deserializeHelper(std::queue<std::string>& nodes) {
        std::string node = nodes.front();
        nodes.pop();
        if (node == "null") return nullptr;
        
        TreeNode* root = new TreeNode(std::stoi(node));
        root->left = deserializeHelper(nodes);
        root->right = deserializeHelper(nodes);
        return root;
    }
};

int main() {
    std::cout << "=== LeetCode 297: 序列化 ===" << std::endl;
    
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(5);
    
    Codec codec;
    std::string serialized = codec.serialize(root);
    std::cout << "序列化: " << serialized << std::endl;
    
    TreeNode* restored = codec.deserialize(serialized);
    std::cout << "反序列化完成，根节点: " << restored->val << std::endl;
    
    return 0;
}
