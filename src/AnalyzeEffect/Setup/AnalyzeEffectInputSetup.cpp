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
#include <Types/Enums/PseudoWeather.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Terrain.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Enums/Weather.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <variant>

namespace pokesim::analyze_effect {
void InputSetup::setAttacker(entt::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(entt::entity entity) {
  handle.emplace<Defender>(entity);
}

void InputSetup::setEffect(types::EffectEnum effect) {
  if (std::holds_alternative<dex::PseudoWeather>(effect)) {
    handle.emplace<PseudoWeatherName>(std::get<dex::PseudoWeather>(effect));
  }
  else if (std::holds_alternative<dex::SideCondition>(effect)) {
    handle.emplace<SideConditionName>(std::get<dex::SideCondition>(effect));
  }
  else if (std::holds_alternative<dex::Status>(effect)) {
    handle.emplace<StatusName>(std::get<dex::Status>(effect));
  }
  else if (std::holds_alternative<dex::Terrain>(effect)) {
    handle.emplace<TerrainName>(std::get<dex::Terrain>(effect));
  }
  else if (std::holds_alternative<dex::Volatile>(effect)) {
    handle.emplace<VolatileName>(std::get<dex::Volatile>(effect));
  }
  else if (std::holds_alternative<dex::Weather>(effect)) {
    handle.emplace<WeatherName>(std::get<dex::Weather>(effect));
  }
  else {
    ENTT_FAIL("Effect does not contain a valid enum");
  }
}

void InputSetup::setBattle(entt::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim::analyze_effect