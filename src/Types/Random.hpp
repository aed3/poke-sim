#pragma once

#include <cstdint>

namespace pokesim::types {
using probability = float;
using rngState = std::uint64_t;
using rngResult = std::uint32_t;
using percentChance = std::uint8_t;
using eventPossibilities = std::uint8_t;
}  // namespace pokesim::types