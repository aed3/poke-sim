#pragma once

#include <Utilities/Variant.hpp>

#include "Enums/PseudoWeather.hpp"
#include "Enums/SideCondition.hpp"
#include "Enums/Status.hpp"
#include "Enums/Terrain.hpp"
#include "Enums/Volatile.hpp"
#include "Enums/Weather.hpp"

namespace pokesim::types {
using effectEnum = pokesim::internal::variant<
  std::monostate, dex::PseudoWeather, dex::SideCondition, dex::Status, dex::Terrain, dex::Volatile, dex::Weather>;
}  // namespace pokesim::types