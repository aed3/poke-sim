#pragma once

#include <cstdint>

namespace pokesim {
enum class AddedTargetOptions : std::uint8_t {
  NONE = 0b00000000,
  TARGET_ALLY = 0b00000001,
  USER_ALLY = 0b00000010,
  TARGET_SIDE = 0b00000100,
  USER_SIDE = 0b00001000,
  FIELD = 0b00010000,
};
}