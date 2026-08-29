#ifndef WEEK_02_COMMON_NOEXCEPT_OUTPUT_H
#define WEEK_02_COMMON_NOEXCEPT_OUTPUT_H

#include <utility>

namespace week2_support {

// 析构函数和智能指针删除器通常处在noexcept清理边界内。教学日志只是附加
// 观察手段，不能因为输出流开启了异常就破坏真正的资源释放。
template<typename Writer>
void write_noexcept(Writer&& writer) noexcept {
    try {
        std::forward<Writer>(writer)();
    } catch (...) {
        // 清理路径保留资源不变量；日志失败不再向外传播。
    }
}

} // namespace week2_support

#endif // WEEK_02_COMMON_NOEXCEPT_OUTPUT_H
