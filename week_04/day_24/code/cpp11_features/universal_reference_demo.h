#ifndef WEEK04_DAY24_UNIVERSAL_REFERENCE_DEMO_H
#define WEEK04_DAY24_UNIVERSAL_REFERENCE_DEMO_H

#include <utility>

namespace day24::universal_reference_lesson {

enum class OverloadRoute {
    Integer,
    ForwardingTemplate
};

OverloadRoute selectOverload(int) noexcept;

template<typename T>
OverloadRoute selectOverload(T&&) noexcept {
    return OverloadRoute::ForwardingTemplate;
}

enum class ValueCategoryRoute {
    Lvalue,
    Rvalue
};

ValueCategoryRoute forwardingTarget(int&) noexcept;
ValueCategoryRoute forwardingTarget(int&&) noexcept;

template<typename T>
ValueCategoryRoute forwardingCall(T&& value) noexcept {
    return forwardingTarget(std::forward<T>(value));
}

void run();

} // namespace day24::universal_reference_lesson

void runUniversalRefDemo();

#endif
