#pragma once

#include <cstdint>

namespace pokesim::dex {

enum class MoveCategory : std::uint8_t {
  PHYSICAL,
  SPECIAL,
  STATUS,
};
}  // namespace pokesim::dex