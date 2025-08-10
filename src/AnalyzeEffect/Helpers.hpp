#pragma once

#include <Components/Boosts.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>

namespace pokesim::analyze_effect {
inline auto tryGetAllInputEffects(types::entity input, const types::registry& registry) {
  return registry.try_get<
    PseudoWeatherName,
    SideConditionName,
    StatusName,
    TerrainName,
    VolatileName,
    WeatherName,
    AtkBoost,
    DefBoost,
    SpaBoost,
    SpdBoost,
    SpeBoost>(input);
}
}  // namespace pokesim::analyze_effect