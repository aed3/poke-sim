#pragma once

namespace pokesim::internal {
template <typename First, typename... T>
bool matchesFirst(const First& first, const T&... rest) {
  static_assert(std::conjunction_v<std::is_same<First, T>...>);
  return ((first == rest) || ...);
}
}  // namespace pokesim::internal
