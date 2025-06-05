#pragma once

#include <Config/Require.hpp>
#include <Types/NumberToType.hpp>
#include <cstdint>
#include <entt/core/fwd.hpp>
#include <initializer_list>
#include <limits>
#include <vector>

namespace pokesim::internal {
template <typename T, std::uint64_t N = std::numeric_limits<entt::id_type>::max()>
class maxSizedVector : public std::vector<T> {
  using base = std::vector<T>;

  void checkSize(std::uint64_t newSize) const {
    POKESIM_REQUIRE(newSize <= max_size(), "More than " + std::to_string(N) + " elements are in this vector.");
  }

 public:
  using size_type = unsignedIntType<N>;

  template <typename... Args>
  maxSizedVector(Args&&... args) : base(std::forward<Args>(args)...) {
    checkSize(base::size());
  }

  maxSizedVector(std::initializer_list<T> list) : maxSizedVector() {
    checkSize(list.size());
    reserve((size_type)list.size());
    for (const T& item : list) {
      push_back(item);
    }
  }

  void push_back(const T& value) {
    checkSize(base::size() + 1U);
    base::push_back(value);
  }

  static constexpr size_type max() { return N; }
  constexpr size_type max_size() const { return max(); }

  size_type size() const { return (size_type)base::size(); }

  void reserve(std::uint64_t newSize) {
    checkSize(newSize);
    base::reserve(newSize);
  }

  void push_back(T&& value) {
    checkSize(base::size() + 1U);
    base::push_back(std::move(value));
  }

  template <typename... Args>
  auto insert(Args&&... args) {
    auto result = base::insert(std::forward<Args>(args)...);
    checkSize(base::size());
    return result;
  }

  template <typename... Args>
  auto& emplace(Args&&... args) {
    checkSize(base::size() + 1U);
    auto& result = base::emplace(std::forward<Args>(args)...);
    return result;
  }

  template <typename... Args>
  auto& emplace_back(Args&&... args) {
    checkSize(base::size() + 1U);
    auto& result = base::emplace_back(std::forward<Args>(args)...);
    return result;
  }
};
}  // namespace pokesim::internal