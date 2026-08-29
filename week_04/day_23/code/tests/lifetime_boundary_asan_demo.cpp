#include <iostream>
#include <string>
#include <utility>

namespace {

class UnsafeOwner {
public:
    explicit UnsafeOwner(std::string value) : value_(std::move(value)) {}

    std::string&& memberReference() && {
        return std::move(value_);
    }

private:
    std::string value_;
};

template<typename T>
T&& passThroughReference(T&& value) {
    return std::forward<T>(value);
}

int useDanglingMemberReference() {
    auto&& dangling = UnsafeOwner(std::string(4096, 'm')).memberReference();
    volatile char observed = dangling.front();
    return observed;
}

int useDanglingForwardedReference() {
    auto&& dangling = passThroughReference(std::string(4096, 'f'));
    volatile char observed = dangling.front();
    return observed;
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " member|forward\n";
        return 2;
    }
    const std::string mode = argv[1];
    if (mode == "member") {
        return useDanglingMemberReference();
    }
    if (mode == "forward") {
        return useDanglingForwardedReference();
    }
    std::cerr << "unknown mode: " << mode << '\n';
    return 2;
}
