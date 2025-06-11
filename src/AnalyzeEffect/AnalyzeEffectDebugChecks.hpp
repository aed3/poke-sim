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
  Checks(const Simulation& _simulation) : pokesim::debug::Checks(_simulation) {}

  void checkInputs() {
    for (types::entity input : registry->view<tags::Input>()) {
      originalToCopy[input] = pokesim::debug::createEntityCopy(input, *registry, registryOnInput);
      checkInput(input);
    }

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
    types::cloneIndex finalEntityCount = getFinalEntityCount();
    POKESIM_REQUIRE_NM(initialEntityCount == finalEntityCount);
    checkInputOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkRemainingOutputs();
  }

 private:
  void checkInput(types::entity input) const {
    POKESIM_REQUIRE_NM(has<Attacker>(input));
    POKESIM_REQUIRE_NM(has<Defenders>(input));
    POKESIM_REQUIRE_NM(has<EffectTarget>(input));
    POKESIM_REQUIRE_NM(has<EffectMoves>(input));
    POKESIM_REQUIRE_NM(has<Battle>(input));
    POKESIM_REQUIRE_NM((registry->any_of<
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
                        SpeBoost>(input)));

    auto const& [attacker, defenders, target, moves] =
      registry->get<Attacker, Defenders, EffectTarget, EffectMoves>(input);
    POKESIM_REQUIRE_NM(has<tags::Attacker>(attacker.val));
    POKESIM_REQUIRE_NM(has<tags::Defender>(defenders.only()));
    checkPokemon(attacker.val);
    checkPokemon(defenders.only());
    if (attacker.val != target.val && defenders.only() != target.val) {
      checkPokemon(target.val);
    }

    POKESIM_REQUIRE_NM(std::find(moves.val.begin(), moves.val.end(), dex::Move::NO_MOVE) == moves.val.end());

    if (has<PseudoWeatherName>(input)) {
      POKESIM_REQUIRE_NM(registry->get<PseudoWeatherName>(input).name != dex::PseudoWeather::NO_PSEUDO_WEATHER);
    }

    if (has<SideConditionName>(input)) {
      POKESIM_REQUIRE_NM(registry->get<SideConditionName>(input).name != dex::SideCondition::NO_SIDE_CONDITION);
    }

    if (has<StatusName>(input)) {
      POKESIM_REQUIRE_NM(registry->get<StatusName>(input).name != dex::Status::NO_STATUS);
    }

    if (has<TerrainName>(input)) {
      POKESIM_REQUIRE_NM(registry->get<TerrainName>(input).name != dex::Terrain::NO_TERRAIN);
    }

    if (has<VolatileName>(input)) {
      POKESIM_REQUIRE_NM(registry->get<VolatileName>(input).name != dex::Volatile::NO_VOLATILE);
    }

    if (has<WeatherName>(input)) {
      POKESIM_REQUIRE_NM(registry->get<WeatherName>(input).name != dex::Weather::NO_WEATHER);
    }

    if (has<AtkBoost>(input)) {
      types::boost boost = registry->get<AtkBoost>(input).val;
      checkBoost(boost);
    }

    if (has<DefBoost>(input)) {
      types::boost boost = registry->get<DefBoost>(input).val;
      checkBoost(boost);
    }

    if (has<SpaBoost>(input)) {
      types::boost boost = registry->get<SpaBoost>(input).val;
      checkBoost(boost);
    }

    if (has<SpdBoost>(input)) {
      types::boost boost = registry->get<SpdBoost>(input).val;
      checkBoost(boost);
    }

    if (has<SpeBoost>(input)) {
      types::boost boost = registry->get<SpeBoost>(input).val;
      checkBoost(boost);
    }
  }

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
        if (effectMultiplier.val == 0) {
          zeroEffectMultiplier = true;
          for (const Damage& multipliedDamageRoll : multipliedDamageRolls.val) {
            POKESIM_REQUIRE_NM(multipliedDamageRoll.val == 0);
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