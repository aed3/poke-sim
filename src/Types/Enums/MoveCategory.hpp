#pragma once

#include <cstdint>

namespace pokesim::dex {

enum class MoveCategory : std::uint8_t {
  PHYSICAL,
  SPECIAL,
  STATUS,
};

static constexpr std::uint8_t TOTAL_MOVE_CATEGORY_COUNT = 3U;
}  // namespace pokesim::dex