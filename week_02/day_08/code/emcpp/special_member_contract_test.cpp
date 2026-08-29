#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace {

struct MoveOnlyMember {
    MoveOnlyMember() = default;
    MoveOnlyMember(const MoveOnlyMember&) = delete;
    MoveOnlyMember& operator=(const MoveOnlyMember&) = delete;
    MoveOnlyMember(MoveOnlyMember&&) noexcept = default;
    MoveOnlyMember& operator=(MoveOnlyMember&&) noexcept = default;
};

struct OrdinaryConstructorStillMoves {
    explicit OrdinaryConstructorStillMoves(int) {}
    MoveOnlyMember member;
};

struct DefaultedDestructorSuppressesMove {
    ~DefaultedDestructorSuppressesMove() = default;
    MoveOnlyMember member;
};

struct UserMoveDeletesCopy {
    UserMoveDeletesCopy() = default;
    UserMoveDeletesCopy(UserMoveDeletesCopy&&) noexcept = default;
    UserMoveDeletesCopy& operator=(UserMoveDeletesCopy&&) noexcept = default;
};

struct CopyFallback {
    CopyFallback() = default;
    CopyFallback(const CopyFallback&) { ++copies; }
    CopyFallback& operator=(const CopyFallback&) = default;
    ~CopyFallback() = default;

    static int copies;
};

int CopyFallback::copies = 0;

struct RuleOfZeroMoveOnly {
    explicit RuleOfZeroMoveOnly(int value) : value_(std::make_unique<int>(value)) {}
    std::unique_ptr<int> value_;
};

struct RuleOfZeroValue {
    std::string value;
};

static_assert(std::is_nothrow_move_constructible_v<OrdinaryConstructorStillMoves>,
              "普通参数构造函数只抑制默认构造，不抑制隐式移动");
static_assert(!std::is_copy_constructible_v<OrdinaryConstructorStillMoves>);
static_assert(!std::is_move_constructible_v<DefaultedDestructorSuppressesMove>,
              "=default析构仍是用户声明，会抑制隐式移动");
static_assert(!std::is_copy_constructible_v<DefaultedDestructorSuppressesMove>);
static_assert(!std::is_copy_constructible_v<UserMoveDeletesCopy>,
              "用户声明移动操作会使隐式拷贝操作被定义为deleted");
static_assert(std::is_nothrow_move_constructible_v<UserMoveDeletesCopy>);
static_assert(!std::is_copy_constructible_v<RuleOfZeroMoveOnly>);
static_assert(std::is_nothrow_move_constructible_v<RuleOfZeroMoveOnly>);
static_assert(std::is_copy_constructible_v<RuleOfZeroValue>);
static_assert(std::is_move_constructible_v<RuleOfZeroValue>);

} // namespace

int main() {
    CopyFallback source;
    CopyFallback::copies = 0;
    CopyFallback target(std::move(source));
    (void)target;
    if (CopyFallback::copies != 1) {
        std::cerr << "std::move在没有移动构造时应回退到拷贝构造\n";
        return 1;
    }

    std::cout << "C++17特殊成员函数生成、抑制与回退规则验证通过\n";
    return 0;
}
