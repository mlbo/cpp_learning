#include "cpp11_features/pimpl_widget.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <type_traits>
#include <utility>

namespace {

class ThrowingStreambuf final : public std::streambuf {
protected:
    int_type overflow(int_type) override {
        throw std::runtime_error("injected output failure");
    }

    std::streamsize xsputn(const char*, std::streamsize) override {
        throw std::runtime_error("injected output failure");
    }
};

class CoutFailureInjection {
public:
    CoutFailureInjection()
        : old_buffer_(std::cout.rdbuf()), old_exceptions_(std::cout.exceptions()) {
        std::cout.exceptions(std::ios::goodbit);
        std::cout.clear();
        std::cout.rdbuf(&throwing_buffer_);
        std::cout.exceptions(std::ios::badbit | std::ios::failbit);
    }

    ~CoutFailureInjection() noexcept {
        std::cout.exceptions(std::ios::goodbit);
        std::cout.rdbuf(old_buffer_);
        std::cout.clear();
        try {
            std::cout.exceptions(old_exceptions_);
        } catch (...) {
            std::terminate();
        }
    }

    CoutFailureInjection(const CoutFailureInjection&) = delete;
    CoutFailureInjection& operator=(const CoutFailureInjection&) = delete;

private:
    ThrowingStreambuf throwing_buffer_;
    std::streambuf* old_buffer_;
    std::ios::iostate old_exceptions_;
};

bool expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        return false;
    }
    return true;
}

} // namespace

int main() {
    using cpp11_features::Widget;

    static_assert(std::is_nothrow_move_constructible_v<Widget>,
                  "Widget公开的noexcept移动构造必须与实现一致");
    static_assert(std::is_nothrow_move_assignable_v<Widget>,
                  "Widget公开的noexcept移动赋值必须与实现一致");
    static_assert(std::is_nothrow_swappable_v<Widget>,
                  "Widget交换只能移动unique_ptr，必须为noexcept");

    bool ok = true;
    Widget source("source", 7);
    bool move_threw = false;
    {
        CoutFailureInjection inject_failure;
        try {
            Widget moved(std::move(source));
            ok = ok && moved.getName() == "source" && moved.getId() == 7;
        } catch (...) {
            move_threw = true;
        }
    }
    ok = expect(!move_threw,
                "noexcept移动路径不得执行可能抛出的输出") && ok;

    Widget assignment_source("new", 9);
    Widget assignment_target("old", 3);
    bool copy_threw = false;
    {
        CoutFailureInjection inject_failure;
        try {
            assignment_target = assignment_source;
        } catch (const std::exception&) {
            copy_threw = true;
        }
    }
    ok = expect(copy_threw, "输出异常注入应到达拷贝临时对象的构造日志") && ok;
    ok = expect(assignment_target.getName() == "old" &&
                    assignment_target.getId() == 3,
                "拷贝赋值在提交前失败时目标必须保持原值") && ok;
    ok = expect(assignment_source.getName() == "new" &&
                    assignment_source.getId() == 9,
                "拷贝赋值失败不得修改源对象") && ok;

    return ok ? 0 : 1;
}
