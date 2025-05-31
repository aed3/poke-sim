#pragma once

#include <Config/Require.hpp>
#include <cstdint>
#include <entt/core/fwd.hpp>
#include <initializer_list>
#include <limits>
#include <vector>

namespace pokesim::internal {
template <typename T, std::uint64_t N = std::numeric_limits<entt::id_type>::max()>
class maxSizedVector : public std::vector<T> {
  using base = std::vector<T>;

  void checkSize() const {
    POKESIM_REQUIRE(base::size() <= max_size(), "More than " + std::to_string(N) + " elements are in this vector.");
  }

 public:
  using size_type = std::conditional_t<
    N <= std::numeric_limits<std::uint32_t>::max(),
    std::conditional_t<
      N <= std::numeric_limits<std::uint16_t>::max(),
      std::conditional_t<N <= std::numeric_limits<std::uint8_t>::max(), std::uint8_t, std::uint16_t>, std::uint32_t>,
    std::uint64_t>;

  template <typename... Args>
  maxSizedVector(Args&&... args) : base(std::forward<Args>(args)...) {
    checkSize();
  }

  maxSizedVector(std::initializer_list<T> list) : maxSizedVector() {
    reserve(list.size());
    for (const T& item : list) {
      push_back(item);
    }
  }

  void push_back(const T& value) {
    base::push_back(value);
    checkSize();
  }

  static constexpr size_type max() { return N; }
  constexpr size_type max_size() const { return max(); }

  size_type size() const { return (size_type)base::size(); }

  void reserve(size_type size) {
    POKESIM_REQUIRE(
      size <= max_size(),
      "More than " + std::to_string(N) + " elements are being reserved in this vector.");
    base::reserve(size);
  }

  void push_back(T&& value) {
    base::push_back(std::move(value));
    checkSize();
  }

  template <typename... Args>
  auto insert(Args&&... args) {
    auto result = base::insert(std::forward<Args>(args)...);
    checkSize();
    return result;
  }

  template <typename... Args>
  auto& emplace(Args&&... args) {
    auto& result = base::emplace(std::forward<Args>(args)...);
    checkSize();
    return result;
  }

  template <typename... Args>
  auto& emplace_back(Args&&... args) {
    auto& result = base::emplace_back(std::forward<Args>(args)...);
    checkSize();
    return result;
  }
};
}  // namespace pokesim::internal