#include "AnalyzeEffectInputSetup.hpp"

#include <Components/AnalyzeEffect/AttackerDefender.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Types/Effect.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/PseudoWeather.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Terrain.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Enums/Weather.hpp>
#include <Types/Utilities/variant.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::analyze_effect {
void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  handle.emplace<Defender>(entity);
}

void InputSetup::setEffect(types::EffectEnum effect) {
  if (effect.holds<dex::PseudoWeather>()) {
    handle.emplace<PseudoWeatherName>(effect.get<dex::PseudoWeather>());
  }
  else if (effect.holds<dex::SideCondition>()) {
    handle.emplace<SideConditionName>(effect.get<dex::SideCondition>());
  }
  else if (effect.holds<dex::Status>()) {
    handle.emplace<StatusName>(effect.get<dex::Status>());
  }
  else if (effect.holds<dex::Terrain>()) {
    handle.emplace<TerrainName>(effect.get<dex::Terrain>());
  }
  else if (effect.holds<dex::Volatile>()) {
    handle.emplace<VolatileName>(effect.get<dex::Volatile>());
  }
  else if (effect.holds<dex::Weather>()) {
    handle.emplace<WeatherName>(effect.get<dex::Weather>());
  }
  else {
    ENTT_FAIL("Effect does not contain a valid enum");
  }
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim::analyze_effect