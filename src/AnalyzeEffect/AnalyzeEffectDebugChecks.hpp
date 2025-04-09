#pragma once

#include <CalcDamage/Helpers.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEffectInputs.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/SimulationResults.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/Assert.hpp>
#include <Utilities/DebugChecks.hpp>

namespace pokesim::analyze_effect::debug {
struct Checks {
#ifdef NDEBUG
  Checks(const Simulation&) {}
#else
  Checks(const Simulation& _simulation) : simulation(_simulation), registry(simulation.registry) {
    for (types::entity input : registry.view<tags::Input>()) {
      originalToCopy[input] = pokesim::debug::createEntityCopy(input, registry, registryOnInput);
    }

    const std::vector<types::entity> attackers = getPokemonList(true);
    const std::vector<types::entity> defenders = getPokemonList(false);

    for (const std::vector<types::entity>& pokemonList : {attackers, defenders}) {
      for (types::entity pokemon : pokemonList) {
        originalToCopy[pokemon] = pokesim::debug::createEntityCopy(pokemon, registry, registryOnInput);
      }
    }

    for (types::entity entity : registry.view<types::entity>()) {
      if (!registry.orphan(entity)) {
        entityCount++;
        if (originalToCopy.contains(entity)) {
          specificallyChecked.emplace(entity);
        }
        else {
          originalToCopy[entity] = pokesim::debug::createEntityCopy(entity, registry, registryOnInput);
        }
      }
    }
  }
  ~Checks() { checkOutputs(); }

 private:
  const Simulation& simulation;
  const types::registry& registry;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;
  entt::dense_set<types::entity> specificallyChecked;
  std::size_t entityCount = 0;

  std::vector<types::entity> getPokemonList(bool forAttacker) const {
    if (forAttacker) {
      auto view = registry.view<tags::Attacker>();
      return {view.begin(), view.end()};
    }

    auto view = registry.view<tags::Defender>();
    return {view.begin(), view.end()};
  }

  DamageRollKind getDamageRollKind(types::entity input, DamageRollOptions damageRollOptions) const {
    const Defenders& defenders = registry.get<Defenders>(input);
    const Side& side = registry.get<Side>(defenders.only());
    PlayerSideId playerSide = registry.get<PlayerSide>(side.val).val;
    switch (playerSide) {
      default: {
        POKESIM_ASSERT_FAIL("Player side wasn't set properly.");
        break;
      }
      case PlayerSideId::P1: {
        return damageRollOptions.p1;
        break;
      }
      case PlayerSideId::P2: {
        return damageRollOptions.p2;
        break;
      }
    }
  }

  void checkInputOutputs() const {
    for (types::entity input : registry.view<tags::Input>()) {
      pokesim::debug::TypesToIgnore typesToIgnore{};
      typesToIgnore.add<MultipliedDamageRolls>();

      POKESIM_ASSERT_NM(registry.all_of<MultipliedDamageRolls>(input));

      if (registry.all_of<tags::InfiniteMultiplier>(input)) {
        POKESIM_ASSERT_NM(!registry.all_of<EffectMultiplier>(input));
        typesToIgnore.add<tags::InfiniteMultiplier>();
      }

      bool zeroEffectMultiplier = false;
      if (registry.all_of<EffectMultiplier>(input)) {
        POKESIM_ASSERT_NM(!registry.all_of<tags::InfiniteMultiplier>(input));
        typesToIgnore.add<EffectMultiplier>();

        const auto [effectMultiplier, multipliedDamageRolls] =
          registry.get<EffectMultiplier, MultipliedDamageRolls>(input);
        if (effectMultiplier.val == 0) {
          zeroEffectMultiplier = true;
          for (const Damage& multipliedDamageRoll : multipliedDamageRolls.val) {
            POKESIM_ASSERT_NM(multipliedDamageRoll.val == 0);
          }
        }
      }

      auto damageRollOptions = simulation.analyzeEffectOptions.damageRollOptions;
      auto noKoChanceCalculation = simulation.analyzeEffectOptions.noKoChanceCalculation;
      if (noKoChanceCalculation || zeroEffectMultiplier) {
        POKESIM_ASSERT_NM(!registry.all_of<MultipliedUsesUntilKo>(input));
      }
      else if (calc_damage::damageKindsMatch(
                 DamageRollKind::ALL_DAMAGE_ROLLS,
                 getDamageRollKind(input, damageRollOptions))) {
        POKESIM_ASSERT_NM(registry.all_of<MultipliedUsesUntilKo>(input));
        typesToIgnore.add<MultipliedUsesUntilKo>();
      }

      pokesim::debug::areEntitiesEqual(registry, input, registryOnInput, originalToCopy.at(input), typesToIgnore);
    }
  }

  void checkPokemonOutputs(bool forAttacker) const {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      pokesim::debug::areEntitiesEqual(registry, pokemon, registryOnInput, originalToCopy.at(pokemon));
    }
  }

  void checkRemainingOutputs() const {
    for (auto [original, copy] : originalToCopy) {
      if (!specificallyChecked.contains(original)) {
        pokesim::debug::areEntitiesEqual(registry, original, registryOnInput, copy);
      }
    }
  }

  void checkOutputs() const {
    std::size_t finalEntityCount = 0;
    for (types::entity entity : registry.view<types::entity>()) {
      if (!registry.orphan(entity)) {
        finalEntityCount++;
      }
    }
    POKESIM_ASSERT_NM(entityCount == finalEntityCount);
    checkInputOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkRemainingOutputs();
  }
#endif
};
}  // namespace pokesim::analyze_effect::debug