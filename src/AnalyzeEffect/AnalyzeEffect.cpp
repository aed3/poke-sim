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
#include "Helpers.hpp"

namespace pokesim::analyze_effect {
namespace {
enum class EffectPresentCheck : std::uint8_t {
  NOT_PRESENT,
  PRESENT_AND_NOT_APPLIED,
  PRESENT_AND_APPLIED,
};

void restoreInputs(
  types::registry& registry, MovePair& movePair, const OriginalInputEntities& originalEntities, Battle& battle,
  Attacker& attacker, Defenders& defenders, EffectTarget& effectTarget) {
  for (types::entity pokemon : {attacker.val, defenders.only(), originalEntities.attacker, originalEntities.defender}) {
    CurrentActionMoves* moves = registry.try_get<CurrentActionMoves>(pokemon);

    if (moves == nullptr) {
      continue;
    }

    auto end = moves->val.end();
    end = std::remove_if(moves->val.begin(), end, [&movePair](types::entity entity) {
      return entity == movePair.original || entity == movePair.copy;
    });

    moves->val.resize(end - moves->val.begin());
    if (moves->val.empty()) {
      registry.remove<CurrentActionMoves>(pokemon);
    }
  }

  registry.destroy(movePair.original);
  if (movePair.original != movePair.copy) {
    registry.destroy(movePair.copy);
  }

  movePair.original = entt::null;
  movePair.copy = entt::null;

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
    (cloneCount == 0U) == clonedEntityMap.empty(),
    "There should be no cloned entities if no clones are needed.");

  POKESIM_REQUIRE(
    cloneCount <= inputs.val.size(),
    "Not all inputs need clones, so there shouldn't be more clones than inputs.");

  const auto& battleClones = cloneCount == 0U ? types::entityVector{} : clonedEntityMap.at(originalBattleEntity);

  POKESIM_REQUIRE(
    cloneCount == battleClones.size(),
    "Each input must have a clone and no more clones than inputs should be made.");

  types::entityIndex cloneIndex = 0U;
  for (types::entity input : inputs.val) {
    bool usesClone = !registry.all_of<tags::RunOneCalculation>(input);

    if (!registry.all_of<GroupedInputs>(input)) {
      continue;
    }

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

    const GroupedInputs& groupedInputs = registry.get<GroupedInputs>(input);
    for (types::entity originalInput : groupedInputs.val) {
      const auto& [battle, move, attacker, defenders, effectTarget] =
        registry.get<Battle, EffectMove, Attacker, Defenders, EffectTarget>(originalInput);

      entt::entity moveEntity = createMove(move.val, battle.val, attacker.val, defenders.only());
      MovePair& movePair = registry.emplace<MovePair>(originalInput, moveEntity, moveEntity);

      registry.emplace<OriginalInputEntities>(
        originalInput,
        OriginalInputEntities{battle.val, attacker.val, defenders.only(), effectTarget.val});

      if (usesClone) {
        POKESIM_REQUIRE(battleClones.size() > cloneIndex, "More inputs want clones than clones made.");
        registry.emplace_or_replace<tags::BattleCloneForCalculation>(battleClones[cloneIndex]);

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

        movePair.copy =
          createMove(move.val, battleClones[cloneIndex], clonedAttackers[cloneIndex], clonedDefenders[cloneIndex]);

        battle.val = battleClones[cloneIndex];
        attacker.val = clonedAttackers[cloneIndex];
        defenders.val[0] = clonedDefenders[cloneIndex];
        effectTarget.val = clonedEffectTarget[cloneIndex];
      }
    }

    if (usesClone) {
      cloneIndex++;
    }
  }
}

template <typename BattleEffectType>
EffectPresentCheck hasBattleEffect(types::registry& registry, Battle battle, BattleEffectType battleEffect) {
  BattleEffectType* currentEffect = registry.try_get<BattleEffectType>(battle.val);
  if (!currentEffect) {
    return EffectPresentCheck::NOT_PRESENT;
  }

  if (currentEffect->name == battleEffect.name) {
    return EffectPresentCheck::PRESENT_AND_APPLIED;
  }

  return EffectPresentCheck::PRESENT_AND_NOT_APPLIED;
}

EffectPresentCheck hasSideEffect(types::registry&, EffectTarget, dex::SideCondition) {
  return EffectPresentCheck::NOT_PRESENT;
}

EffectPresentCheck hasVolatileEffect(types::registry&, EffectTarget, dex::Volatile) {
  return EffectPresentCheck::NOT_PRESENT;
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

template <typename T, typename EffectTuple>
bool namedEffectPointerMatch(const EffectTuple& current, const EffectTuple& other) {
  const T* currentProperty = std::get<const T*>(current);
  const T* otherProperty = std::get<const T*>(other);

  if ((currentProperty == nullptr) != (otherProperty == nullptr)) return false;
  if (currentProperty == nullptr) return true;
  return currentProperty->name == otherProperty->name;
}

template <typename T, typename EffectTuple>
bool valuedEffectPointerMatch(const EffectTuple& current, const EffectTuple& other) {
  const T* currentProperty = std::get<const T*>(current);
  const T* otherProperty = std::get<const T*>(other);

  if ((currentProperty == nullptr) != (otherProperty == nullptr)) return false;
  if (currentProperty == nullptr) return true;
  return currentProperty->val == otherProperty->val;
}

template <typename EffectTuple>
bool canInputsShareABattle(
  const EffectTuple& currentEffects, const EffectTarget& currentEffectTarget, types::entity otherInput,
  const types::registry& registry) {
  const auto otherEffects = tryGetAllInputEffects(otherInput, registry);

  if (!namedEffectPointerMatch<PseudoWeatherName>(currentEffects, otherEffects)) return false;
  if (!namedEffectPointerMatch<TerrainName>(currentEffects, otherEffects)) return false;
  if (!namedEffectPointerMatch<WeatherName>(currentEffects, otherEffects)) return false;

  EffectTarget otherEffectTarget = registry.get<EffectTarget>(otherInput);
  if (registry.get<Side>(currentEffectTarget.val).val != registry.get<Side>(otherEffectTarget.val).val) return false;
  if (!namedEffectPointerMatch<SideConditionName>(currentEffects, otherEffects)) return false;

  if (currentEffectTarget.val != otherEffectTarget.val) return false;
  if (!namedEffectPointerMatch<StatusName>(currentEffects, otherEffects)) return false;
  if (!namedEffectPointerMatch<VolatileName>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<AtkBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<DefBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<SpaBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<SpdBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<SpeBoost>(currentEffects, otherEffects)) return false;

  return true;
}

void groupSimilarInputs(types::handle battleHandle, const Inputs& inputs) {
  types::registry& registry = *battleHandle.registry();
  types::entityIndex inputCount = inputs.val.size();
  std::vector<bool> checkedIndexes(inputCount, false);

  for (types::entityIndex currentIndex = 0U; currentIndex < inputCount; currentIndex++) {
    if (checkedIndexes[currentIndex]) continue;

    types::entity currentInput = inputs.val[currentIndex];
    GroupedInputs& groupedInputs = registry.emplace<GroupedInputs>(currentInput);

    const auto currentEffects = tryGetAllInputEffects(currentInput, registry);
    EffectTarget currentEffectTarget = registry.get<EffectTarget>(currentInput);

    groupedInputs.val.push_back(currentInput);

    for (types::entityIndex otherIndex = currentIndex + 1U; otherIndex < inputCount; otherIndex++) {
      if (checkedIndexes[otherIndex]) continue;

      types::entity otherInput = inputs.val[otherIndex];
      if (!canInputsShareABattle(currentEffects, currentEffectTarget, otherInput, registry)) continue;

      groupedInputs.val.push_back(otherInput);
      registry.emplace<tags::GroupedWithOtherInput>(otherInput);
      battleHandle.get_or_emplace<SkippedInputCount>().val++;

      checkedIndexes[otherIndex] = true;
    }

    checkedIndexes[currentIndex] = true;
  }
}

void setRunOneCalculation(types::handle inputHandle, Battle battle) {
  inputHandle.emplace<tags::RunOneCalculation>();
  inputHandle.registry()->get_or_emplace<SkippedInputCount>(battle.val).val++;
}

void setInvertFinalAnswer(types::handle inputHandle) {
  for (types::entity input : inputHandle.get<GroupedInputs>().val) {
    inputHandle.registry()->emplace<tags::InvertFinalAnswer>(input);
  }
}

template <typename BattleEffectType>
void ignoreWithBattleEffect(types::handle inputHandle, Battle battle, BattleEffectType battleEffect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasBattleEffect(registry, battle, battleEffect);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithSideConditionEffect(
  types::handle inputHandle, Battle battle, EffectTarget effectTarget, SideConditionName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasSideEffect(registry, effectTarget, effect.name);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithStatusEffect(types::handle inputHandle, Battle battle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasStatusEffect(registry, effectTarget, effect);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithVolatileEffect(
  types::handle inputHandle, Battle battle, EffectTarget effectTarget, VolatileName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasVolatileEffect(registry, effectTarget, effect.name);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

template <typename BoostType>
void ignoreWithBoostEffect(types::handle inputHandle, Battle battle, EffectTarget effectTarget, BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck boostCheck = hasBoostEffect(registry, effectTarget, effect);
  if (boostCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreBattlesWithEffectActive(Simulation& simulation) {
  simulation
    .view<ignoreWithBattleEffect<PseudoWeatherName>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation
    .view<ignoreWithBattleEffect<TerrainName>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation
    .view<ignoreWithBattleEffect<WeatherName>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithSideConditionEffect, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithStatusEffect, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithVolatileEffect, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();

  simulation.view<ignoreWithBoostEffect<AtkBoost>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithBoostEffect<DefBoost>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithBoostEffect<SpaBoost>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithBoostEffect<SpdBoost>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
  simulation.view<ignoreWithBoostEffect<SpeBoost>, Tags<tags::Input>, entt::exclude_t<tags::GroupedWithOtherInput>>();
}

void createAppliedEffectBattles(Simulation& simulation) {
  entt::dense_map<types::eventPossibilities, types::entityVector> battlesByCloneCount{};

  simulation.registry.view<Inputs>().each([&](types::entity battleEntity, const Inputs& inputs) {
    POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
    POKESIM_REQUIRE(
      inputs.val.size() <= std::numeric_limits<types::eventPossibilities>().max(),
      "More clones are being made than possibilities.");
    types::eventPossibilities cloneCount = (types::eventPossibilities)inputs.val.size();

    const SkippedInputCount* skippedInputCount = simulation.registry.try_get<SkippedInputCount>(battleEntity);
    types::eventPossibilities ignoredCount = skippedInputCount == nullptr ? 0U : skippedInputCount->val;

    POKESIM_REQUIRE(cloneCount >= ignoredCount, "Must have more inputs than inputs ignored.");
    battlesByCloneCount[cloneCount - ignoredCount].push_back(battleEntity);
  });

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

void applyPseudoWeatherEffect(types::handle, Battle, PseudoWeatherName) {}

void applyTerrainEffect(types::handle, Battle, TerrainName) {}

void applyWeatherEffect(types::handle, Battle, WeatherName) {}

void applySideConditionEffect(types::handle, EffectTarget, SideConditionName) {}

void applyStatusEffect(types::handle inputHandle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck statusCheck = hasStatusEffect(registry, effectTarget, effect);
  if (statusCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    clearStatus({registry, effectTarget.val});
    setInvertFinalAnswer(inputHandle);
  }
  else {
    setStatus({registry, effectTarget.val}, effect.name);
  }
}

void applyVolatileEffect(types::handle, EffectTarget, VolatileName) {}

template <typename BoostType>
void applyBoostEffect(types::handle inputHandle, EffectTarget effectTarget, BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck boostCheck = hasBoostEffect(registry, effectTarget, effect);
  if (boostCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    registry.remove<BoostType>(effectTarget.val);
    setInvertFinalAnswer(inputHandle);
  }
  else {
    registry.emplace_or_replace<BoostType>(effectTarget.val, effect);
  }
}

void createOutput(types::handle inputHandle, const MovePair& movePairs) {
  bool invert = inputHandle.all_of<tags::InvertFinalAnswer>();
  types::registry& registry = *inputHandle.registry();

  auto [parentBattleMove, childBattleMove] = movePairs;
  const auto [childDamage, childDamageRolls] = registry.get<Damage, DamageRolls>(childBattleMove);
  auto [parentDamage, parentDamageRolls] = registry.get<Damage, DamageRolls>(parentBattleMove);

  if (invert) {
    if (childDamage.val == 0U) {
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
    if (parentDamage.val == 0U) {
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

void clearRunVariables(Simulation& simulation) {
  simulation.view<restoreInputs>();
  simulation.addToEntities<pokesim::tags::CloneToRemove, tags::BattleCloneForCalculation>();
  deleteClones(simulation.registry);

  simulation.registry.clear<
    GroupedInputs,
    MovePair,
    OriginalInputEntities,
    tags::RunOneCalculation,
    tags::InvertFinalAnswer,
    tags::GroupedWithOtherInput,
    SkippedInputCount,
    Damage>();
}

void analyzeEffect(Simulation& simulation) {
  pokesim::internal::SelectForPokemonView<pokesim::tags::AnalyzeEffect> selectedPokemon(simulation);
  pokesim::internal::SelectForBattleView<pokesim::tags::AnalyzeEffect> selectedBattle(simulation);

  if (selectedPokemon.hasNoneSelected() || selectedBattle.hasNoneSelected()) {
    return;
  }

  simulation.viewForSelectedBattles<groupSimilarInputs>();

  if (!simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    ignoreBattlesWithEffectActive(simulation);
  }

  createAppliedEffectBattles(simulation);

  using Excluded = entt::exclude_t<tags::RunOneCalculation, tags::GroupedWithOtherInput>;
  simulation.view<applyPseudoWeatherEffect, Tags<tags::Input>, Excluded>();
  simulation.view<applyTerrainEffect, Tags<tags::Input>, Excluded>();
  simulation.view<applyWeatherEffect, Tags<tags::Input>, Excluded>();
  simulation.view<applySideConditionEffect, Tags<tags::Input>, Excluded>();
  simulation.view<applyStatusEffect, Tags<tags::Input>, Excluded>();
  simulation.view<applyVolatileEffect, Tags<tags::Input>, Excluded>();

  simulation.view<applyBoostEffect<AtkBoost>, Tags<tags::Input>, Excluded>();
  simulation.view<applyBoostEffect<DefBoost>, Tags<tags::Input>, Excluded>();
  simulation.view<applyBoostEffect<SpaBoost>, Tags<tags::Input>, Excluded>();
  simulation.view<applyBoostEffect<SpdBoost>, Tags<tags::Input>, Excluded>();
  simulation.view<applyBoostEffect<SpeBoost>, Tags<tags::Input>, Excluded>();

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