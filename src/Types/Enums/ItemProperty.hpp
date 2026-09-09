#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class ItemProperty : std::uint8_t {
  BERRY = 1U << 0U,
  CHOICE = 1U << 1U,
  GEM = 1U << 2U,
  IGNORES_KLUTZ = 1U << 3U,
  POKEBALL = 1U << 4U,
};

constexpr ItemProperty operator|(ItemProperty propertyA, ItemProperty propertyB) {
  return static_cast<ItemProperty>(static_cast<std::uint8_t>(propertyA) | static_cast<std::uint8_t>(propertyB));
}

constexpr bool operator&(ItemProperty propertyA, ItemProperty propertyB) {
  return (static_cast<std::uint8_t>(propertyA) & static_cast<std::uint8_t>(propertyB)) != 0U;
}
}  // namespace pokesim::dex
