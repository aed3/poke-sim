#pragma once

#include <cstdint>

namespace pokesim {
// The current turn of a battle
struct Turn {
  std::uint16_t turn = 0;
};
}  // namespace pokesim