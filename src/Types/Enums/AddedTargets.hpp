#pragma once

#include <array>
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

constexpr AddedTargetOptions operator|(AddedTargetOptions optionA, AddedTargetOptions optionB) {
  return static_cast<AddedTargetOptions>(static_cast<std::uint8_t>(optionA) | static_cast<std::uint8_t>(optionB));
}

constexpr bool operator&(AddedTargetOptions optionA, AddedTargetOptions optionB) {
  return (static_cast<std::uint8_t>(optionA) & static_cast<std::uint8_t>(optionB)) != 0U;
}

static constexpr inline std::array<AddedTargetOptions, 6U> VALID_ADDED_TARGET_OPTIONS = {
  AddedTargetOptions::NONE,
  AddedTargetOptions::TARGET_ALLY,
  AddedTargetOptions::USER_ALLY,
  AddedTargetOptions::TARGET_SIDE,
  AddedTargetOptions::USER_SIDE,
  AddedTargetOptions::FIELD,
};
}  // namespace pokesim