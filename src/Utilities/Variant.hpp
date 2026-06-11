#pragma once

#include <tuple>
#include <variant>

namespace pokesim::internal {
template <typename... Types>
class variant : public std::variant<Types...> {
 public:
  using std::variant<Types...>::variant;

  template <typename T>
  variant& operator=(const T& rhs) {
    std::variant<Types...>::operator=(rhs);
    return *this;
  }

  constexpr bool empty() const { return holds<std::monostate>(); }

  template <typename Type>
  constexpr bool holds() const {
    return std::holds_alternative<Type>(*this);
  }

  template <typename Type>
  constexpr auto& get() const {
    return std::get<Type>(*this);
  }

  template <typename Type>
  constexpr auto& get() {
    return std::get<Type>(*this);
  }

  template <typename... Type>
  constexpr auto get_if() const {
    return std::forward_as_tuple(std::get_if<Type>(this)...);
  }

  template <typename... Type>
  constexpr auto get_if() {
    return std::forward_as_tuple(std::get_if<Type>(this)...);
  }
};
}  // namespace pokesim::internal
