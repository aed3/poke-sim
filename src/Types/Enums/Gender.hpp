#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon gender name
enum class Gender : std::uint8_t { NO_GENDER = 0, FEMALE, MALE };

static constexpr std::size_t TOTAL_GENDER_COUNT = 3U;
}  // namespace pokesim::dex
