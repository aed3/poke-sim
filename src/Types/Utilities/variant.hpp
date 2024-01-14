#pragma once

#include <variant>

namespace pokesim::types::internal {
template <typename... Types>
class variant : public std::variant<Types...> {
 public:
  bool empty() const { return holds<std::monostate>(); }

  template <typename Type>
  bool holds() const {
    return std::holds_alternative<Type>(*this);
  }

  template <typename Type>
  auto get() const {
    return std::get<Type>(*this);
  }
};
}  // namespace pokesim::types::internal