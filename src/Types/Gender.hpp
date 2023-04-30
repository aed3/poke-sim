#pragma once
#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon gender name */
enum Gender : std::uint8_t { NO_GENDER = 0, FEMALE, MALE, GENDER_TOTAL };
}  // namespace pokesim::dex
