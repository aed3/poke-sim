#pragma once

#include <cstdint>

namespace pokesim::types {
using Pp = std::uint8_t;
using BasePower = std::uint8_t;
using BaseAccuracy = std::uint8_t;
using MoveHits = std::uint8_t;
using BaseEffectChance = std::uint8_t;

using Priority = std::int8_t;
using FractionalPriority = std::int8_t;
}  // namespace pokesim::types