#pragma once

#include <Types/Variant.hpp>

#include "Enums/Ability.hpp"
#include "Enums/Item.hpp"
#include "Enums/Move.hpp"
#include "Enums/PseudoWeather.hpp"
#include "Enums/SideCondition.hpp"
#include "Enums/Status.hpp"
#include "Enums/Terrain.hpp"
#include "Enums/Volatile.hpp"
#include "Enums/Weather.hpp"

namespace pokesim::types {
using effectEnum = variant<
  std::monostate, dex::PseudoWeather, dex::SideCondition, dex::Status, dex::Terrain, dex::Volatile, dex::Weather>;
using effectSource = types::variant<dex::Item, dex::Move, dex::Ability>;

}  // namespace pokesim::types
