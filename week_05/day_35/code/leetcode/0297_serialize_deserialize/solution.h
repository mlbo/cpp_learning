#pragma once

#include <charconv>
#include <cstddef>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace day35::codec {

struct TreeNode {
    explicit TreeNode(int node_value) : value{node_value} {}

    TreeNode* add_left(int node_value) {
        left = std::make_unique<TreeNode>(node_value);
        return left.get();
    }

    TreeNode* add_right(int node_value) {
        right = std::make_unique<TreeNode>(node_value);
        return right.get();
    }

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class Codec {
public:
    std::string serialize(const TreeNode* root) const {
        std::vector<std::string> tokens;
        serialize_node(root, tokens, 1U);
        std::ostringstream output;
        for (std::size_t index = 0; index < tokens.size(); ++index) {
            if (index != 0U) {
                output << ',';
            }
            output << tokens[index];
        }
        return output.str();
    }

    std::unique_ptr<TreeNode> deserialize(const std::string& data) const {
        const std::vector<std::string> tokens = tokenize(data);
        std::size_t index = 0U;
        std::unique_ptr<TreeNode> root = deserialize_node(tokens, index, 1U);
        if (index != tokens.size()) {
            throw std::invalid_argument("serialized tree has trailing tokens");
        }
        return root;
    }

private:
    static constexpr std::size_t max_tree_depth_{1024U};
    static constexpr std::size_t max_tokens_{100'000U};

    static std::vector<std::string> tokenize(const std::string& data) {
        if (data.empty()) {
            throw std::invalid_argument("serialized tree must not be empty");
        }
        if (data.front() == ',' || data.back() == ',') {
            throw std::invalid_argument("serialized tree contains an empty token");
        }

        std::vector<std::string> tokens;
        std::size_t begin = 0U;
        while (begin < data.size()) {
            const std::size_t end = data.find(',', begin);
            const std::size_t length =
                end == std::string::npos ? data.size() - begin : end - begin;
            if (length == 0U) {
                throw std::invalid_argument("serialized tree contains an empty token");
            }
            if (tokens.size() == max_tokens_) {
                throw std::length_error("serialized tree contains too many tokens");
            }
            tokens.push_back(data.substr(begin, length));
            if (end == std::string::npos) {
                break;
            }
            begin = end + 1U;
        }
        return tokens;
    }

    static void append_token(std::vector<std::string>& tokens, std::string token) {
        if (tokens.size() == max_tokens_) {
            throw std::length_error("serialized tree contains too many tokens");
        }
        tokens.push_back(std::move(token));
    }

    static void serialize_node(
        const TreeNode* node,
        std::vector<std::string>& tokens,
        std::size_t depth) {
        if (node == nullptr) {
            append_token(tokens, "#");
            return;
        }
        if (depth > max_tree_depth_) {
            throw std::length_error("tree depth exceeds codec contract");
        }
        append_token(tokens, std::to_string(node->value));
        serialize_node(node->left.get(), tokens, depth + 1U);
        serialize_node(node->right.get(), tokens, depth + 1U);
    }

    static std::unique_ptr<TreeNode> deserialize_node(
        const std::vector<std::string>& tokens,
        std::size_t& index,
        std::size_t depth) {
        if (index >= tokens.size()) {
            throw std::invalid_argument("serialized tree ended early");
        }
        const std::string& token = tokens[index];
        ++index;
        if (token == "#") {
            return nullptr;
        }
        if (depth > max_tree_depth_) {
            throw std::length_error("tree depth exceeds codec contract");
        }

        int value = 0;
        const char* const begin = token.data();
        const char* const end = begin + token.size();
        const auto [position, error] = std::from_chars(begin, end, value);
        if (error != std::errc{} || position != end) {
            throw std::invalid_argument("tree token is not an integer");
        }

        auto node = std::make_unique<TreeNode>(value);
        node->left = deserialize_node(tokens, index, depth + 1U);
        node->right = deserialize_node(tokens, index, depth + 1U);
        return node;
    }
};

}  // namespace day35::codec
