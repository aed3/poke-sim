#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon gender name
enum class Gender : std::uint8_t { NO_GENDER = 0, FEMALE, MALE, GENDER_TOTAL };
}  // namespace pokesim::dex
