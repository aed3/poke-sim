#include "AnalyzeEffectInputSetup.hpp"

#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEffectInputs.hpp>
#include <Components/Boosts.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Types/Effect.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/PseudoWeather.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Terrain.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Enums/Weather.hpp>
#include <Utilities/Variant.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::analyze_effect {
InputSetup::InputSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {
  handle.emplace<tags::Input>();
}

void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
  handle.registry()->emplace_or_replace<tags::Attacker>(entity);
}

void InputSetup::setEffectTarget(types::entity entity) {
  handle.emplace<EffectTarget>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  POKESIM_ASSERT(
    !handle.try_get<Defenders>(),
    "Calc damage only supports one defender per move. Make a new move instead.");
  handle.emplace<Defenders>().val.push_back(entity);
  handle.registry()->emplace_or_replace<tags::Defender>(entity);
}

void InputSetup::setEffectMoves(const std::vector<dex::Move>& moves) {
  handle.get_or_emplace<EffectMoves>().val = moves;
}

void InputSetup::setEffect(types::effectEnum effect) {
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
    POKESIM_ASSERT_FAIL("Effect does not contain a valid enum.");
  }
}

void InputSetup::setBoostEffect(dex::Stat stat, types::boost boost) {
  switch (stat) {
    case dex::Stat::ATK: {
      handle.emplace<AtkBoost>(boost);
      break;
    }
    case dex::Stat::DEF: {
      handle.emplace<DefBoost>(boost);
      break;
    }
    case dex::Stat::SPA: {
      handle.emplace<SpaBoost>(boost);
      break;
    }
    case dex::Stat::SPD: {
      handle.emplace<SpdBoost>(boost);
      break;
    }
    case dex::Stat::SPE: {
      handle.emplace<SpeBoost>(boost);
      break;
    }
    default: {
      POKESIM_ASSERT_FAIL("Using a stat enum that doesn't have boost.");
    }
  }
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
  handle.registry()->get_or_emplace<Inputs>(entity).val.push_back(handle.entity());
}
}  // namespace pokesim::analyze_effect