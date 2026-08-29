#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "solution.h"

int main() {
    using day35::codec::Codec;
    using day35::codec::TreeNode;

    auto root = std::make_unique<TreeNode>(1);
    root->add_left(2);
    TreeNode* three = root->add_right(3);
    three->add_left(4);
    three->add_right(5);

    const Codec codec;
    const std::string serialized = codec.serialize(root.get());
    const std::unique_ptr<TreeNode> restored = codec.deserialize(serialized);
    const bool round_trip = codec.serialize(restored.get()) == serialized;
    const bool empty_round_trip = codec.serialize(codec.deserialize("#").get()) == "#";

    bool rejects_malformed = false;
    try {
        static_cast<void>(codec.deserialize("1,#"));
    } catch (const std::invalid_argument&) {
        rejects_malformed = true;
    }

    std::cout << "序列化: " << serialized << '\n';
    std::cout << "接口契约: 非法或截断输入抛出 invalid_argument\n";
    return round_trip && empty_round_trip && rejects_malformed ? 0 : 1;
}
