#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <CalcDamage/Helpers.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEffectInputs.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/SimulationResults.hpp>
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>
#include <cstddef>

namespace pokesim::analyze_effect::debug {
struct Checks : pokesim::debug::Checks {
  Checks(const Simulation& _simulation) : pokesim::debug::Checks(_simulation) {}

  void checkInputs() {
    for (types::entity input : registry->view<tags::Input>()) {
      originalToCopy[input] = pokesim::debug::createEntityCopy(input, *registry, registryOnInput);
    }

    const std::vector<types::entity> attackers = getPokemonList(true);
    const std::vector<types::entity> defenders = getPokemonList(false);

    for (const std::vector<types::entity>& pokemonList : {attackers, defenders}) {
      for (types::entity pokemon : pokemonList) {
        originalToCopy[pokemon] = pokesim::debug::createEntityCopy(pokemon, *registry, registryOnInput);
      }
    }

    copyRemainingEntities();
  }

  void checkOutputs() const {
    std::size_t finalEntityCount = getFinalEntityCount();
    POKESIM_REQUIRE_NM(initialEntityCount == finalEntityCount);
    checkInputOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkRemainingOutputs();
  }

 private:
  std::vector<types::entity> getPokemonList(bool forAttacker) const {
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

      POKESIM_REQUIRE_NM(registry->all_of<MultipliedDamageRolls>(input));

      if (registry->all_of<tags::InfiniteMultiplier>(input)) {
        POKESIM_REQUIRE_NM(!registry->all_of<EffectMultiplier>(input));
        typesToIgnore.add<tags::InfiniteMultiplier>();
      }

      bool zeroEffectMultiplier = false;
      if (registry->all_of<EffectMultiplier>(input)) {
        POKESIM_REQUIRE_NM(!registry->all_of<tags::InfiniteMultiplier>(input));
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
        POKESIM_REQUIRE_NM(!registry->all_of<MultipliedUsesUntilKo>(input));
      }
      else if (calc_damage::damageKindsMatch(
                 DamageRollKind::ALL_DAMAGE_ROLLS,
                 getDamageRollKind(input, damageRollOptions))) {
        POKESIM_REQUIRE_NM(registry->all_of<MultipliedUsesUntilKo>(input));
        typesToIgnore.add<MultipliedUsesUntilKo>();
      }

      pokesim::debug::areEntitiesEqual(*registry, input, registryOnInput, originalToCopy.at(input), typesToIgnore);
    }
  }

  void checkPokemonOutputs(bool forAttacker) const {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
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