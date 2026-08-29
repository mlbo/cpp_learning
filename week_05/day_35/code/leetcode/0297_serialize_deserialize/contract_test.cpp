#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "solution.h"

int main() {
    using day35::codec::Codec;

    const Codec codec;
    std::vector<std::string> malformed{
        "",
        "1,#,#,",
        ",1,#,#",
        "1,,#,#",
        "1abc,#,#",
        " 1,#,#",
        "1,#",
        "#,#",
    };
    malformed.push_back(std::string(1000U, '9') + ",#,#");

    bool all_rejected = true;
    for (const std::string& input : malformed) {
        try {
            static_cast<void>(codec.deserialize(input));
            all_rejected = false;
            std::cerr << "accepted malformed serialization: " << input << '\n';
        } catch (const std::invalid_argument&) {
        }
    }

    const std::string minimum = std::to_string(std::numeric_limits<int>::min()) + ",#,#";
    const std::string maximum = std::to_string(std::numeric_limits<int>::max()) + ",#,#";
    const bool exact_integer_bounds = codec.serialize(codec.deserialize(minimum).get()) == minimum &&
                                      codec.serialize(codec.deserialize(maximum).get()) == maximum;

    std::string too_deep;
    for (std::size_t depth = 0U; depth < 1100U; ++depth) {
        too_deep += "1,";
    }
    too_deep += '#';
    for (std::size_t depth = 0U; depth < 1100U; ++depth) {
        too_deep += ",#";
    }
    bool excessive_depth_rejected = false;
    try {
        static_cast<void>(codec.deserialize(too_deep));
    } catch (const std::length_error&) {
        excessive_depth_rejected = true;
    }

    std::string too_many_tokens{"#"};
    for (std::size_t count = 0U; count < 100'000U; ++count) {
        too_many_tokens += ",#";
    }
    bool excessive_token_count_rejected = false;
    try {
        static_cast<void>(codec.deserialize(too_many_tokens));
    } catch (const std::length_error&) {
        excessive_token_count_rejected = true;
    }

    if (!all_rejected || !exact_integer_bounds || !excessive_depth_rejected ||
        !excessive_token_count_rejected) {
        return 1;
    }

    std::cout << "Codec rejects malformed, over-token-budget and over-deep inputs\n";
    return 0;
}
