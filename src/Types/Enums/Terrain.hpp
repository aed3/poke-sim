#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class Terrain : std::uint8_t {
  // clang-format off
  NO_TERRAIN = 0, TERRAIN_TOTAL
  // clang-format on
};

static constexpr std::uint8_t TOTAL_TERRAIN_COUNT = (std::uint8_t)Terrain::TERRAIN_TOTAL - 1U;
}  // namespace pokesim::dex
