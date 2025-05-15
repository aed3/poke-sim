#pragma once

#include <Config/Require.hpp>
#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>

namespace pokesim::internal {
template <typename T, std::uint8_t N>
class maxSizedVector : private std::array<T, N> {
  using base = std::array<T, N>;
  std::uint8_t used = 0;

 public:
  using base::begin;
  using base::cbegin;
  using base::crbegin;
  using base::max_size;

  maxSizedVector() : base() {}
  maxSizedVector(std::initializer_list<T> list) : base() {
    for (const T& item : list) {
      push_back(item);
    }
  }

  constexpr std::uint8_t size() const noexcept { return used; }
  constexpr bool empty() const noexcept { return used == 0; }
  constexpr typename base::const_reference front() const noexcept { return *base::begin(); }
  constexpr typename base::const_reference back() const noexcept { return N ? *(end() - 1) : *end(); }

  constexpr typename base::const_reference at(std::uint8_t pos) const {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::at(pos);
  }

  constexpr typename base::const_reference operator[](std::uint8_t pos) const {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::operator[](pos);
  }

  typename base::reference at(std::uint8_t pos) {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::at(pos);
  }

  typename base::reference operator[](std::uint8_t pos) {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::operator[](pos);
  }

  void push_back(const T& value) {
    base::at(used) = value;
    used++;
  }

  void pop_back() {
    if (empty()) return;
    used--;
  }

  void pop_count(std::uint8_t remove) {
    POKESIM_REQUIRE(remove <= used, "Removing more values than are used.");
    used -= remove;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    base::at(used) = {args...};
    used++;
  }

  bool operator==(const maxSizedVector<T, N>& other) const noexcept {
    return used == other.used && std::equal(begin(), end(), other.begin());
  }

  typename base::const_iterator end() const noexcept { return base::begin() + used; }
  typename base::const_iterator cend() const noexcept { return end(); }
  typename base::const_reverse_iterator rend() const noexcept { return const_reverse_iterator(end()); }
  typename base::const_reverse_iterator crend() const noexcept { return rend(); }
};
}  // namespace pokesim::internal