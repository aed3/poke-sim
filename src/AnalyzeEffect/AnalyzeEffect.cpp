#include "AnalyzeEffect.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Helpers.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEffectInputs.hpp>
#include <Components/AnalyzeEffect/RemovedEffect.hpp>
#include <Components/Boosts.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Event.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <cstddef>
#include <cstdint>
#include <entt/container/dense_map.hpp>
#include <limits>
#include <vector>

#include "AnalyzeEffectDebugChecks.hpp"

namespace pokesim::analyze_effect {
namespace {
enum class EffectPresentCheck : std::uint8_t {
  NOT_PRESENT,
  PRESENT_AND_NOT_APPLIED,
  PRESENT_AND_APPLIED,
};

void restoreInputs(
  types::registry& registry, const MovePairs& movePairs, const OriginalInputEntities& originalEntities, Battle& battle,
  Attacker& attacker, Defenders& defenders, EffectTarget& effectTarget) {
  for (types::entity pokemon : {attacker.val, defenders.only(), originalEntities.attacker, originalEntities.defender}) {
    CurrentActionMoves* moves = registry.try_get<CurrentActionMoves>(pokemon);

    if (moves == nullptr) {
      continue;
    }

    auto end = moves->val.end();
    for (const auto& movePair : movePairs.val) {
      end = std::remove_if(moves->val.begin(), end, [&movePair](types::entity entity) {
        return entity == movePair.first || entity == movePair.second;
      });
    }

    moves->val.resize(end - moves->val.begin());
    if (moves->val.empty()) {
      registry.remove<CurrentActionMoves>(pokemon);
    }
  }

  for (auto [parentBattleMove, childBattleMove] : movePairs.val) {
    registry.destroy(parentBattleMove);
    if (parentBattleMove != childBattleMove) {
      registry.destroy(childBattleMove);
    }
  }

  battle.val = originalEntities.battle;
  attacker.val = originalEntities.attacker;
  defenders.val[0] = originalEntities.defender;
  effectTarget.val = originalEntities.effectTarget;
}

void assignInputsToClones(
  Simulation& simulation, types::eventPossibilities cloneCount, types::entity originalBattleEntity,
  const types::ClonedEntityMap& clonedEntityMap) {
  types::registry& registry = simulation.registry;
  const Inputs& inputs = registry.get<Inputs>(originalBattleEntity);

  POKESIM_REQUIRE(
    (cloneCount == 0) == clonedEntityMap.empty(),
    "There should be no cloned entities if no clones are needed.");

  POKESIM_REQUIRE(
    cloneCount <= inputs.val.size(),
    "Not all inputs need clones, so there shouldn't be more clones than inputs.");

  const auto& battleClones = cloneCount == 0 ? types::entityVector{} : clonedEntityMap.at(originalBattleEntity);

  POKESIM_REQUIRE(
    cloneCount == battleClones.size(),
    "Each input must have a clone and no more clones than inputs should be made.");

  types::cloneIndex cloneIndex = 0;
  for (types::entity input : inputs.val) {
    bool usesClone = !registry.all_of<tags::RunOneCalculation>(input);

    auto [battle, attacker, defenders, effectTarget, moves] =
      registry.get<Battle, Attacker, Defenders, EffectTarget, EffectMoves>(input);

    MovePairs& movePairs = registry.emplace<MovePairs>(input);
    movePairs.val.resize(moves.val.size());

    auto createMove =
      [&](dex::Move move, types::entity battleEntity, types::entity attackerEntity, types::entity defenderEntity) {
        types::entity moveEntity = createActionMoveForTarget(
          {simulation.registry, defenderEntity},
          battleEntity,
          attackerEntity,
          move,
          simulation.pokedex());
        registry.emplace<MoveName>(moveEntity, move);
        registry.emplace<pokesim::tags::AnalyzeEffect>(moveEntity);

        return moveEntity;
      };

    for (types::cloneIndex moveIndex = 0; moveIndex < moves.val.size(); moveIndex++) {
      entt::entity moveEntity = createMove(moves.val[moveIndex], battle.val, attacker.val, defenders.only());
      movePairs.val[moveIndex].first = movePairs.val[moveIndex].second = moveEntity;
    }

    registry.emplace<OriginalInputEntities>(
      input,
      OriginalInputEntities{battle.val, attacker.val, defenders.only(), effectTarget.val});

    if (usesClone) {
      POKESIM_REQUIRE(battleClones.size() > cloneIndex, "More inputs want clones than clones made.");

      registry.emplace<tags::BattleCloneForCalculation>(battleClones[cloneIndex]);

      const auto& clonedAttackers = clonedEntityMap.at(attacker.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedAttackers.size(),
        "Each attacker must have a clone and no more clones than inputs should be made.");

      const auto& clonedDefenders = clonedEntityMap.at(defenders.only());
      POKESIM_REQUIRE(
        battleClones.size() == clonedDefenders.size(),
        "Each defender must have a clone and no more clones than inputs should be made.");

      const auto& clonedEffectTarget = clonedEntityMap.at(effectTarget.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedEffectTarget.size(),
        "Each effect target must have a clone and no more clones than inputs should be made.");

      for (types::cloneIndex moveIndex = 0; moveIndex < moves.val.size(); moveIndex++) {
        movePairs.val[moveIndex].second = createMove(
          moves.val[moveIndex],
          battleClones[cloneIndex],
          clonedAttackers[cloneIndex],
          clonedDefenders[cloneIndex]);
      }

      battle.val = battleClones[cloneIndex];
      attacker.val = clonedAttackers[cloneIndex];
      defenders.val[0] = clonedDefenders[cloneIndex];
      effectTarget.val = clonedEffectTarget[cloneIndex];

      cloneIndex++;
    }
  }
}

EffectPresentCheck hasStatusEffect(types::registry& registry, EffectTarget effectTarget, StatusName status) {
  StatusName* currentStatus = registry.try_get<StatusName>(effectTarget.val);
  if (!currentStatus) {
    return EffectPresentCheck::NOT_PRESENT;
  }

  if (currentStatus->name == status.name) {
    return EffectPresentCheck::PRESENT_AND_APPLIED;
  }

  return EffectPresentCheck::PRESENT_AND_NOT_APPLIED;
}

template <typename BoostType>
EffectPresentCheck hasBoostEffect(types::registry& registry, EffectTarget effectTarget, BoostType boost) {
  BoostType* currentBoost = registry.try_get<BoostType>(effectTarget.val);
  if (!currentBoost) {
    return EffectPresentCheck::NOT_PRESENT;
  }

  if (currentBoost->val == boost.val) {
    return EffectPresentCheck::PRESENT_AND_APPLIED;
  }

  return EffectPresentCheck::PRESENT_AND_NOT_APPLIED;
}

void setRunOneCalculation(types::handle inputHandle, Battle battle) {
  inputHandle.emplace<tags::RunOneCalculation>();
  inputHandle.registry()->get_or_emplace<RunsOneCalculationCount>(battle.val).val++;
}

void ignoreWithPseudoWeatherEffect(types::handle /*inputHandle*/, Battle /*battle*/, PseudoWeatherName /*effect*/) {}

void ignoreWithTerrainEffect(types::handle /*inputHandle*/, Battle /*battle*/, TerrainName /*effect*/) {}

void ignoreWithWeatherEffect(types::handle /*inputHandle*/, Battle /*battle*/, WeatherName /*effect*/) {}

void ignoreWithSideConditionEffect(
  types::handle /*inputHandle*/, EffectTarget /*effectTarget*/, SideConditionName /*effect*/) {}

void ignoreWithStatusEffect(types::handle inputHandle, Battle battle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck statusCheck = hasStatusEffect(registry, effectTarget, effect);
  if (statusCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithVolatileEffect(types::handle /*inputHandle*/, EffectTarget /*effectTarget*/, VolatileName /*effect*/) {}

template <typename BoostType>
void ignoreWithBoostEffect(types::handle inputHandle, Battle battle, EffectTarget effectTarget, BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck boostCheck = hasBoostEffect(registry, effectTarget, effect);
  if (boostCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreBattlesWithEffectActive(Simulation& simulation) {
  simulation.view<ignoreWithPseudoWeatherEffect, Tags<tags::Input>>();
  simulation.view<ignoreWithTerrainEffect, Tags<tags::Input>>();
  simulation.view<ignoreWithWeatherEffect, Tags<tags::Input>>();
  simulation.view<ignoreWithSideConditionEffect, Tags<tags::Input>>();
  simulation.view<ignoreWithStatusEffect, Tags<tags::Input>>();
  simulation.view<ignoreWithVolatileEffect, Tags<tags::Input>>();

  simulation.view<ignoreWithBoostEffect<AtkBoost>, Tags<tags::Input>>();
  simulation.view<ignoreWithBoostEffect<DefBoost>, Tags<tags::Input>>();
  simulation.view<ignoreWithBoostEffect<SpaBoost>, Tags<tags::Input>>();
  simulation.view<ignoreWithBoostEffect<SpdBoost>, Tags<tags::Input>>();
  simulation.view<ignoreWithBoostEffect<SpeBoost>, Tags<tags::Input>>();
}

void createAppliedEffectBattles(Simulation& simulation) {
  entt::dense_map<types::eventPossibilities, types::entityVector> battlesByCloneCount{};

  if (simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    simulation.registry.view<Inputs>().each([&](types::entity battleEntity, const Inputs& inputs) {
      POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
      POKESIM_REQUIRE(
        inputs.val.size() <= std::numeric_limits<types::eventPossibilities>().max(),
        "More clones are being made than possibilities.");
      types::eventPossibilities cloneCount = (types::eventPossibilities)inputs.val.size();

      battlesByCloneCount[cloneCount].push_back(battleEntity);
    });
  }
  else {
    simulation.registry.view<Inputs>().each([&](types::entity battleEntity, const Inputs& inputs) {
      POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
      POKESIM_REQUIRE(
        inputs.val.size() <= std::numeric_limits<types::eventPossibilities>().max(),
        "More clones are being made than possibilities.");
      types::eventPossibilities cloneCount = (types::eventPossibilities)inputs.val.size();

      const RunsOneCalculationCount* ignoredInputCount =
        simulation.registry.try_get<RunsOneCalculationCount>(battleEntity);
      types::eventPossibilities ignoredCount = ignoredInputCount == nullptr ? 0U : ignoredInputCount->val;

      POKESIM_REQUIRE(cloneCount >= ignoredCount, "Must have more inputs than inputs ignored.");
      battlesByCloneCount[cloneCount - ignoredCount].push_back(battleEntity);
    });
  }

  types::ClonedEntityMap clonedBattleMap;
  for (const auto& [cloneCount, battleEntities] : battlesByCloneCount) {
    if (cloneCount) {
      simulation.registry.insert<pokesim::tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
    }

    auto clonedEntityMap = clone(simulation.registry, cloneCount);

    for (types::entity battleEntity : battleEntities) {
      assignInputsToClones(simulation, cloneCount, battleEntity, clonedEntityMap);
    }
  }
}

void applyPseudoWeatherEffect(types::handle /*inputHandle*/, Battle /*battle*/, PseudoWeatherName /*effect*/) {}

void applyTerrainEffect(types::handle /*inputHandle*/, Battle /*battle*/, TerrainName /*effect*/) {}

void applyWeatherEffect(types::handle /*inputHandle*/, Battle /*battle*/, WeatherName /*effect*/) {}

void applySideConditionEffect(
  types::handle /*inputHandle*/, EffectTarget /*effectTarget*/, SideConditionName /*effect*/) {}

void applyStatusEffect(types::handle inputHandle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck statusCheck = hasStatusEffect(registry, effectTarget, effect);
  if (statusCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    clearStatus({registry, effectTarget.val});
    inputHandle.emplace<tags::InvertFinalAnswer>();
  }
  else {
    setStatus({registry, effectTarget.val}, effect.name);
  }
}

void applyVolatileEffect(types::handle /*inputHandle*/, EffectTarget /*effectTarget*/, VolatileName /*effect*/) {}

template <typename BoostType>
void applyBoostEffect(types::handle inputHandle, EffectTarget effectTarget, BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck boostCheck = hasBoostEffect(registry, effectTarget, effect);
  if (boostCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    registry.remove<BoostType>(effectTarget.val);
    inputHandle.emplace<tags::InvertFinalAnswer>();
  }
  else {
    registry.emplace_or_replace<BoostType>(effectTarget.val, effect);
  }
}

void createOutput(types::handle inputHandle, const MovePairs& movePairs) {
  bool invert = inputHandle.all_of<tags::InvertFinalAnswer>();
  types::registry& registry = *inputHandle.registry();

  for (auto [parentBattleMove, childBattleMove] : movePairs.val) {
    const auto [childDamage, childDamageRolls] = registry.get<Damage, DamageRolls>(childBattleMove);
    auto [parentDamage, parentDamageRolls] = registry.get<Damage, DamageRolls>(parentBattleMove);

    if (invert) {
      if (childDamage.val == 0) {
        inputHandle.emplace<tags::InfiniteMultiplier>();
      }
      else {
        inputHandle.emplace<EffectMultiplier>((types::effectMultiplier)parentDamage.val / childDamage.val);
      }

      inputHandle.emplace<MultipliedDamageRolls>(parentDamageRolls);
      auto* const parentKoChances = registry.try_get<calc_damage::UsesUntilKo>(parentBattleMove);
      if (parentKoChances != nullptr) {
        inputHandle.emplace<MultipliedUsesUntilKo>(*parentKoChances);
      }
    }
    else {
      if (parentDamage.val == 0) {
        inputHandle.emplace<tags::InfiniteMultiplier>();
      }
      else {
        inputHandle.emplace<EffectMultiplier>((types::effectMultiplier)childDamage.val / parentDamage.val);
      }

      inputHandle.emplace<MultipliedDamageRolls>(childDamageRolls);
      auto* const childKoChances = registry.try_get<calc_damage::UsesUntilKo>(childBattleMove);
      if (childKoChances != nullptr) {
        inputHandle.emplace<MultipliedUsesUntilKo>(*childKoChances);
      }
    }
  }
}

void clearRunVariables(Simulation& simulation) {
  simulation.view<restoreInputs>();
  simulation.addToEntities<pokesim::tags::CloneToRemove, tags::BattleCloneForCalculation>();
  deleteClones(simulation.registry);

  simulation.registry.clear<
    MovePairs,
    OriginalInputEntities,
    tags::RunOneCalculation,
    tags::InvertFinalAnswer,
    RunsOneCalculationCount,
    Damage>();
}

void analyzeEffect(Simulation& simulation) {
  pokesim::internal::SelectForPokemonView<pokesim::tags::AnalyzeEffect> selectedPokemon(simulation);
  pokesim::internal::SelectForBattleView<pokesim::tags::AnalyzeEffect> selectedBattle(simulation);

  if (selectedPokemon.hasNoneSelected() || selectedBattle.hasNoneSelected()) {
    return;
  }

  if (!simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    ignoreBattlesWithEffectActive(simulation);
  }

  createAppliedEffectBattles(simulation);

  simulation.view<applyPseudoWeatherEffect, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyTerrainEffect, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyWeatherEffect, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applySideConditionEffect, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyStatusEffect, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyVolatileEffect, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();

  simulation.view<applyBoostEffect<AtkBoost>, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyBoostEffect<DefBoost>, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyBoostEffect<SpaBoost>, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyBoostEffect<SpdBoost>, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();
  simulation.view<applyBoostEffect<SpeBoost>, Tags<tags::Input>, entt::exclude_t<tags::RunOneCalculation>>();

  calc_damage::run(simulation);

  simulation.view<createOutput, Tags<tags::Input>>();
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  analyzeEffect(simulation);

  clearRunVariables(simulation);
  debugChecks.checkOutputs();
}
}  // namespace pokesim::analyze_effect