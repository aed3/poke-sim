#pragma once

#include <Utilities/NumberToType.hpp>
#include <cstdint>

#include "Constants.hpp"

namespace pokesim::types {
using probability = float;
using rngState = std::uint64_t;
using rngResult = std::uint32_t;
using percentChance = pokesim::internal::unsignedIntType<Constants::PercentChance::MAX>;
using eventPossibilities = std::uint8_t;
}  // namespace pokesim::types
