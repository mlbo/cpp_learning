#ifndef WEEK04_DAY25_PERFECT_FORWARDING_DEMO_H
#define WEEK04_DAY25_PERFECT_FORWARDING_DEMO_H

#include <string>
#include <utility>

namespace cpp11_features {

enum class ForwardingRoute {
    Lvalue,
    Rvalue
};

namespace detail {

ForwardingRoute forwardingTarget(const std::string& value);
ForwardingRoute forwardingTarget(std::string&& value);

} // namespace detail

template<typename T>
ForwardingRoute forwardingRoute(T&& value) {
    return detail::forwardingTarget(std::forward<T>(value));
}

void perfectForwardingDemo();

} // namespace cpp11_features

#endif
