#pragma once

#include <Config/Require.hpp>
#include <Utilities/NumberToType.hpp>
#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace pokesim::types {
template <typename Type, std::uint64_t MaxSize>
class maxSizedVector : public std::vector<Type> {
  using base = std::vector<Type>;

  void checkSize(std::uint64_t newSize) const {
    POKESIM_REQUIRE(newSize <= max_size(), "More than " + std::to_string(MaxSize) + " elements are in this vector.");
  }

 public:
  using size_type = internal::unsignedIntType<MaxSize>;

  template <typename... Args>
  maxSizedVector(Args&&... args) : base(std::forward<Args>(args)...) {
    checkSize(base::size());
  }

  maxSizedVector(std::initializer_list<Type> list) : maxSizedVector() {
    checkSize(list.size());
    reserve((size_type)list.size());
    for (const Type& item : list) {
      push_back(item);
    }
  }

  constexpr typename base::const_reference at(std::uint64_t pos) const {
    POKESIM_REQUIRE(pos < size(), "Accessing out of bounds element.");
    return base::at(pos);
  }

  constexpr typename base::const_reference operator[](std::uint64_t pos) const {
    POKESIM_REQUIRE(pos < size(), "Accessing out of bounds element.");
    return base::operator[](pos);
  }

  typename base::reference at(std::uint64_t pos) {
    POKESIM_REQUIRE(pos < size(), "Accessing out of bounds element.");
    return base::at(pos);
  }

  typename base::reference operator[](std::uint64_t pos) {
    POKESIM_REQUIRE(pos < size(), "Accessing out of bounds element.");
    return base::operator[](pos);
  }

  void push_back(const Type& value) {
    checkSize(base::size() + 1U);
    base::push_back(value);
  }

  void unordered_remove(const Type& value) {
    POKESIM_REQUIRE(std::find(base::begin(), base::end(), value) != base::end(), "Value must be in vector to remove.");
    for (size_type i = 0U; i < size() - 1U; i++) {
      if (value == at(i)) {
        at(i) = base::back();
        break;
      }
    }
    base::pop_back();
  }

  void pop_count(size_type remove) {
    POKESIM_REQUIRE(remove <= size(), "Cannot remove more elements than contained.");
    base::resize(size() - remove);
  }

  static constexpr size_type max() { return MaxSize; }
  constexpr size_type max_size() const { return max(); }

  size_type size() const { return (size_type)base::size(); }

  void resize(std::uint64_t newSize) {
    checkSize(newSize);
    base::resize(newSize);
  }

  void reserve(std::uint64_t newSize) {
    checkSize(newSize);
    base::reserve(newSize);
  }

  void push_back(Type&& value) {
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
}  // namespace pokesim::types
