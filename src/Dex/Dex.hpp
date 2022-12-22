#pragma once

#include "Names.hpp"

namespace pokesim {
class Dex {
 public:
  struct Names : internal::Names {};
  /** @brief Converts a string to only have lowercase alphanumeric characters */
  /*SHF_inline*/ static std::string toID(const std::string& name);
};
}  // namespace pokesim
