#pragma once

#include <cstdint>

namespace pokesim::types {
using pp = std::uint8_t;
using basePower = std::uint8_t;
using baseAccuracy = std::uint8_t;
using moveHits = std::uint8_t;
using baseEffectChance = std::uint8_t;

using priority = std::int8_t;
using fractionalPriority = std::int8_t;
}  // namespace pokesim::types