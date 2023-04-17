#pragma once
#include <cstdint>

namespace pokesim {
namespace dex {
/** @brief Pokemon gender name */
enum Gender : std::uint8_t { NO_GENDER = 0, FEMALE, MALE, GENDER_TOTAL };
}  // namespace dex

struct GenderName {
  dex::Gender name = dex::NO_GENDER;
};
}  // namespace pokesim