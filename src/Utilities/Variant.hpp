#pragma once

#include <variant>

namespace pokesim::internal {
template <typename... Types>
class variant : public std::variant<Types...> {
 public:
  using std::variant<Types...>::variant;

  template <typename T>
  variant& operator=(T&& rhs) {
    std::variant<Types...>::operator=(rhs);
    return *this;
  }

  bool empty() const { return holds<std::monostate>(); }

  template <typename Type>
  bool holds() const {
    return std::holds_alternative<Type>(*this);
  }

  template <typename Type>
  auto& get() const {
    return std::get<Type>(*this);
  }

  template <typename Type>
  auto& get() {
    return std::get<Type>(*this);
  }
};
}  // namespace pokesim::internal