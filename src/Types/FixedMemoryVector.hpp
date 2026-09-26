#pragma once

#include <Config/Require.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace pokesim::types {
template <typename Type, std::uint8_t Size, std::uint8_t AverageSize = Size>
class fixedMemoryVector : private std::array<Type, Size> {
  using base = std::array<Type, Size>;
  std::uint8_t used = 0U;

 public:
  using base::begin;
  using base::cbegin;
  using base::crbegin;
  using base::max_size;

  using size_type = std::uint8_t;

  fixedMemoryVector() : base() {
    static_assert(
      sizeof(fixedMemoryVector<Type, Size, AverageSize>) <= sizeof(std::vector<Type>) + (sizeof(Type) * AverageSize),
      "A std::vector for this type and size would be smaller.");
  }

  fixedMemoryVector(size_type size, const Type& value) : fixedMemoryVector() {
    for (size_type i = 0U; i < size; i++) {
      push_back(value);
    }
  }

  fixedMemoryVector(std::initializer_list<Type> list) : fixedMemoryVector() {
    for (const Type& item : list) {
      push_back(item);
    }
  }

  constexpr size_type size() const noexcept { return used; }
  constexpr size_type max_size() const noexcept { return Size; }
  constexpr bool empty() const noexcept { return used == 0U; }

  constexpr typename base::const_reference front() const noexcept { return *base::begin(); }
  constexpr typename base::const_reference back() const noexcept { return Size ? *(end() - 1) : *end(); }

  constexpr typename base::reference front() noexcept { return *base::begin(); }
  constexpr typename base::reference back() noexcept { return Size ? *(end() - 1) : *end(); }

  constexpr typename base::const_reference at(size_type pos) const {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::at(pos);
  }

  constexpr typename base::const_reference operator[](size_type pos) const {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::operator[](pos);
  }

  typename base::reference at(size_type pos) {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::at(pos);
  }

  typename base::reference operator[](size_type pos) {
    POKESIM_REQUIRE(pos < used, "Accessing value that isn't used.");
    return base::operator[](pos);
  }

  void push_back(const Type& value) {
    base::at(used) = value;
    used++;
  }

  void pop_back() {
    if (empty()) return;
    used--;
  }

  void unordered_remove(const Type& value) {
    POKESIM_REQUIRE(std::find(begin(), end(), value) != end(), "Value must be in vector to remove.");
    for (uint8_t i = 0U; i < Size - 1U; i++) {
      if (value == at(i)) {
        at(i) = back();
        break;
      }
    }
    pop_back();
  }

  void pop_count(size_type remove) {
    POKESIM_REQUIRE(remove <= used, "Removing more values than are used.");
    used -= remove;
  }

  template <class... Args>
  Type& emplace_back(const Args&... args) {
    Type& newValue = base::at(used) = {args...};
    used++;
    return newValue;
  }

  constexpr bool operator==(const fixedMemoryVector<Type, Size, AverageSize>& other) const noexcept {
    return used == other.used && std::equal(begin(), end(), other.begin());
  }

  typename base::iterator end() noexcept { return base::begin() + used; }

  typename base::const_iterator end() const noexcept { return base::begin() + used; }
  typename base::const_iterator cend() const noexcept { return end(); }
  typename base::const_reverse_iterator rend() const noexcept { return const_reverse_iterator(end()); }
  typename base::const_reverse_iterator crend() const noexcept { return rend(); }
};
}  // namespace pokesim::types
