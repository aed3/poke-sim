#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class AddedTargetOptions : std::uint8_t {
  NONE = 0U,
  TARGET_ALLY = 1U << 0U,
  USER_ALLY = 1U << 1U,
  TARGET_SIDE = 1U << 2U,
  USER_SIDE = 1U << 3U,
  FIELD = 1U << 4U,
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
