#pragma once

#include <Types/Constants.hpp>
#include <Utilities/NumberToType.hpp>
#include <cstdint>

namespace pokesim::types {
using eventModifier = std::uint32_t;
using effectMultiplier = float;
using effectDuration = pokesim::internal::unsignedIntType<Constants::EffectDuration::MAX>;
}  // namespace pokesim::types
