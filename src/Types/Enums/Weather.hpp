#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class Weather : std::uint8_t {
  // clang-format off
  NO_WEATHER = 0U, WEATHER_TOTAL
  // clang-format on
};

static constexpr std::uint8_t TOTAL_WEATHER_COUNT = (std::uint8_t)Weather::WEATHER_TOTAL - 1U;
}  // namespace pokesim::dex
