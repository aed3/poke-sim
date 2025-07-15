#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <CalcDamage/Helpers.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEffectInputs.hpp>
#include <Components/Boosts.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/SimulationResults.hpp>
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/PseudoWeather.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Terrain.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Enums/Weather.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>
#include <cstddef>

namespace pokesim::analyze_effect::debug {
struct Checks : pokesim::debug::Checks {
  Options options;
  Checks(const Simulation& _simulation)
      : pokesim::debug::Checks(_simulation), options(_simulation.analyzeEffectOptions) {}

  void checkInputs() {
    pokesim::debug::check(options.damageRollOptions);

    auto view = registry->view<tags::Input>();
    types::entityVector inputs{view.begin(), view.end()};
    for (types::entity input : inputs) {
      originalToCopy[input] = pokesim::debug::createEntityCopy(input, *registry, registryOnInput);
    }
    pokesim::debug::check(Inputs{inputs}, *registry);

    for (types::entity battle : simulation->selectedBattleEntities()) {
      checkBattle(battle);
      checkSide(registry->get<Sides>(battle).p1());
      checkSide(registry->get<Sides>(battle).p2());
    }

    const types::entityVector attackers = getPokemonList(true);
    const types::entityVector defenders = getPokemonList(false);

    for (const types::entityVector& pokemonList : {attackers, defenders}) {
      for (types::entity pokemon : pokemonList) {
        checkPokemon(pokemon);
      }
    }

    copyRemainingEntities();
  }

  void checkOutputs() const {
    POKESIM_REQUIRE_NM(options == simulation->analyzeEffectOptions);

    types::cloneIndex finalEntityCount = getFinalEntityCount();
    POKESIM_REQUIRE_NM(initialEntityCount == finalEntityCount);
    checkInputOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkRemainingOutputs();
  }

 private:
  types::entityVector getPokemonList(bool forAttacker) const {
    if (forAttacker) {
      auto view = registry->view<tags::Attacker>();
      return {view.begin(), view.end()};
    }

    auto view = registry->view<tags::Defender>();
    return {view.begin(), view.end()};
  }

  DamageRollKind getDamageRollKind(types::entity input, DamageRollOptions damageRollOptions) const {
    const Defenders& defenders = registry->get<Defenders>(input);
    const Side& side = registry->get<Side>(defenders.only());
    PlayerSideId playerSide = registry->get<PlayerSide>(side.val).val;
    switch (playerSide) {
      case PlayerSideId::P1: {
        return damageRollOptions.p1;
        break;
      }
      case PlayerSideId::P2: {
        return damageRollOptions.p2;
        break;
      }
      default: break;
    }

    POKESIM_REQUIRE_FAIL("Player side wasn't set properly.");
    return DamageRollKind::NONE;
  }

  void checkInputOutputs() const {
    for (types::entity input : registry->view<tags::Input>()) {
      pokesim::debug::TypesToIgnore typesToIgnore{};
      typesToIgnore.add<MultipliedDamageRolls>();

      POKESIM_REQUIRE_NM(has<MultipliedDamageRolls>(input));

      if (has<tags::InfiniteMultiplier>(input)) {
        POKESIM_REQUIRE_NM(!has<EffectMultiplier>(input));
        typesToIgnore.add<tags::InfiniteMultiplier>();
      }

      bool zeroEffectMultiplier = false;
      if (has<EffectMultiplier>(input)) {
        POKESIM_REQUIRE_NM(!has<tags::InfiniteMultiplier>(input));
        typesToIgnore.add<EffectMultiplier>();

        const auto [effectMultiplier, multipliedDamageRolls] =
          registry->get<EffectMultiplier, MultipliedDamageRolls>(input);
        if (effectMultiplier.val == 0U) {
          zeroEffectMultiplier = true;
          for (const Damage& multipliedDamageRoll : multipliedDamageRolls.val) {
            POKESIM_REQUIRE_NM(multipliedDamageRoll.val == 0U);
          }
        }
      }

      auto damageRollOptions = simulation->analyzeEffectOptions.damageRollOptions;
      auto noKoChanceCalculation = simulation->analyzeEffectOptions.noKoChanceCalculation;
      if (noKoChanceCalculation || zeroEffectMultiplier) {
        POKESIM_REQUIRE_NM(!has<MultipliedUsesUntilKo>(input));
      }
      else if (calc_damage::damageKindsMatch(
                 DamageRollKind::ALL_DAMAGE_ROLLS,
                 getDamageRollKind(input, damageRollOptions))) {
        POKESIM_REQUIRE_NM(has<MultipliedUsesUntilKo>(input));
        typesToIgnore.add<MultipliedUsesUntilKo>();
      }

      pokesim::debug::areEntitiesEqual(*registry, input, registryOnInput, originalToCopy.at(input), typesToIgnore);
    }
  }

  void checkPokemonOutputs(bool forAttacker) const {
    const types::entityVector pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      pokesim::debug::areEntitiesEqual(*registry, pokemon, registryOnInput, originalToCopy.at(pokemon));
    }
  }
};
}  // namespace pokesim::analyze_effect::debug
#else
namespace pokesim {
class Simulation;
namespace analyze_effect::debug {
struct Checks {
  Checks(const Simulation&) {}
  void checkInputs() const {}
  void checkOutputs() const {}
};
}  // namespace analyze_effect::debug
}  // namespace pokesim
#endif