#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class PseudoWeather : std::uint8_t {
  // clang-format off
  NO_PSEUDO_WEATHER = 0U, PSEUDO_WEATHER_TOTAL
  // clang-format on
};

static constexpr std::uint8_t TOTAL_PSEUDO_WEATHER_COUNT = (std::uint8_t)PseudoWeather::PSEUDO_WEATHER_TOTAL - 1U;
}  // namespace pokesim::dex
