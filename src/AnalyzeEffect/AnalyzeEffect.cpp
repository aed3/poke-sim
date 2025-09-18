#include "AnalyzeEffect.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Setup/EmplaceTagFromEnum.hpp>
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
  using Included = Tags<tags::Input>;
  using Excluded = entt::exclude_t<tags::GroupedWithOtherInput>;
  simulation.view<ignoreWithBattleEffect<PseudoWeatherName>, Included, Excluded>();
  simulation.view<ignoreWithBattleEffect<TerrainName>, Included, Excluded>();
  simulation.view<ignoreWithBattleEffect<WeatherName>, Included, Excluded>();
  simulation.view<ignoreWithSideConditionEffect, Included, Excluded>();
  simulation.view<ignoreWithStatusEffect, Included, Excluded>();
  simulation.view<ignoreWithVolatileEffect, Included, Excluded>();

  simulation.view<ignoreWithBoostEffect<AtkBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<DefBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<SpaBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<SpdBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<SpeBoost>, Included, Excluded>();
}

types::entity createAnalyzeEffectMove(
  types::registry& registry, dex::Move move, types::entity battleEntity, types::entity attackerEntity,
  types::entity defenderEntity, const Pokedex& pokedex) {
  types::entity moveEntity =
    createActionMoveForTarget({registry, defenderEntity}, battleEntity, attackerEntity, move, pokedex);
  registry.emplace<MoveName>(moveEntity, move);
  registry.emplace_or_replace<tags::Attacker>(attackerEntity);
  registry.emplace_or_replace<tags::Defender>(defenderEntity);

  return moveEntity;
}

void createOneCalculationMovePair(
  types::handle inputHandle, Battle battle, EffectMove move, Attacker attacker, Defender defender,
  const Pokedex& pokedex) {
  entt::entity moveEntity =
    createAnalyzeEffectMove(*inputHandle.registry(), move.val, battle.val, attacker.val, defender.val, pokedex);
  inputHandle.emplace<MovePair>(moveEntity, moveEntity);
}

void createTwoCalculationsMovePair(
  types::handle inputHandle, Battle battle, EffectMove move, Attacker attacker, Defender defender,
  const OriginalInputEntities& originals, const Pokedex& pokedex) {
  types::registry& registry = *inputHandle.registry();

  entt::entity originalEntity =
    createAnalyzeEffectMove(registry, move.val, originals.battle, originals.attacker, originals.defender, pokedex);
  entt::entity copyEntity =
    createAnalyzeEffectMove(registry, move.val, battle.val, attacker.val, defender.val, pokedex);

  // All active pokemon in should have their stats refreshed in doubles for moves like Beat Up which rely on the stats
  // of Pokemon outside of the attacker and defender
  for (types::entity pokemon : {attacker.val, defender.val}) {
    registry.emplace_or_replace<pokesim::tags::AtkStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::DefStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::SpaStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::SpdStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::SpeStatUpdateRequired>(pokemon);
  }

  inputHandle.emplace<MovePair>(originalEntity, copyEntity);
}

void assignInputsToClones(
  Simulation& simulation, types::entityIndex cloneCount, types::entity originalBattleEntity,
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
    if (!registry.all_of<GroupedInputs>(input)) {
      continue;
    }

    if (registry.all_of<tags::RunOneCalculation>(input)) {
      continue;
    }

    POKESIM_REQUIRE(battleClones.size() > cloneIndex, "More inputs want clones than clones made.");
    registry.emplace<tags::BattleCloneForCalculation>(battleClones[cloneIndex]);

    const GroupedInputs& groupedInputs = registry.get<GroupedInputs>(input);
    for (types::entity originalInput : groupedInputs.val) {
      const auto& [battle, move, attacker, defender, effectTarget] =
        registry.get<Battle, EffectMove, Attacker, Defender, EffectTarget>(originalInput);

      registry.emplace<OriginalInputEntities>(
        originalInput,
        OriginalInputEntities{battle.val, attacker.val, defender.val, effectTarget.val});

      const auto& clonedAttackers = clonedEntityMap.at(attacker.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedAttackers.size(),
        "Each attacker must have a clone and no more clones than inputs should be made.");

      const auto& clonedDefenders = clonedEntityMap.at(defender.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedDefenders.size(),
        "Each defender must have a clone and no more clones than inputs should be made.");

      const auto& clonedEffectTarget = clonedEntityMap.at(effectTarget.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedEffectTarget.size(),
        "Each effect target must have a clone and no more clones than inputs should be made.");

      battle.val = battleClones[cloneIndex];
      attacker.val = clonedAttackers[cloneIndex];
      defender.val = clonedDefenders[cloneIndex];
      effectTarget.val = clonedEffectTarget[cloneIndex];
    }

    cloneIndex++;
  }
}

void createAppliedEffectBattles(Simulation& simulation) {
  entt::dense_map<types::entityIndex, types::entityVector> battlesByCloneCount{};

  simulation.registry.view<Inputs>(entt::exclude<SkippedInputCount>)
    .each([&](types::entity battleEntity, const Inputs& inputs) {
      POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
      types::entityIndex cloneCount = inputs.val.size();

      battlesByCloneCount[cloneCount].push_back(battleEntity);
    });

  simulation.registry.view<Inputs, SkippedInputCount>().each(
    [&](types::entity battleEntity, const Inputs& inputs, SkippedInputCount skippedInputCount) {
      POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
      types::entityIndex cloneCount = inputs.val.size();

      POKESIM_REQUIRE(cloneCount >= skippedInputCount.val, "Must have more inputs than inputs ignored.");
      battlesByCloneCount[cloneCount - skippedInputCount.val].push_back(battleEntity);
    });

  for (const auto& [cloneCount, battleEntities] : battlesByCloneCount) {
    if (cloneCount) {
      simulation.registry.insert<pokesim::tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
    }

    auto clonedEntityMap = clone(simulation.registry, cloneCount);

    for (types::entity battleEntity : battleEntities) {
      assignInputsToClones(simulation, cloneCount, battleEntity, clonedEntityMap);
    }
  }

  simulation.view<createOneCalculationMovePair, Tags<>, entt::exclude_t<OriginalInputEntities>>(simulation.pokedex());
  simulation.view<createTwoCalculationsMovePair>(simulation.pokedex());
}

void applyPseudoWeatherEffect(types::handle, Battle, PseudoWeatherName) {}

void applyTerrainEffect(types::handle, Battle, TerrainName) {}

void applyWeatherEffect(types::handle, Battle, WeatherName) {}

void applySideConditionEffect(types::handle, EffectTarget, SideConditionName) {}

void applyStatusEffect(types::handle inputHandle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck statusCheck = hasStatusEffect(registry, effectTarget, effect);
  types::handle pokemonHandle{registry, effectTarget.val};
  clearStatus(pokemonHandle);
  if (statusCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setInvertFinalAnswer(inputHandle);
  }
  else {
    pokemonHandle.emplace<StatusName>(effect.name);
    status::tags::emplaceTagFromEnum(effect.name, pokemonHandle);
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

void restoreInputs(
  const OriginalInputEntities& originalEntities, Battle& battle, Attacker& attacker, Defender& defender,
  EffectTarget& effectTarget) {
  battle.val = originalEntities.battle;
  attacker.val = originalEntities.attacker;
  defender.val = originalEntities.defender;
  effectTarget.val = originalEntities.effectTarget;
}

template <typename UsedMovesType>
void removeUsedMoves(types::registry& registry, const MovePair& movePair, types::entity pokemon) {
  UsedMovesType* moves = registry.try_get<UsedMovesType>(pokemon);

  if (moves == nullptr) {
    return;
  }

  auto end = moves->val.end();
  end = std::remove_if(moves->val.begin(), end, [&movePair](types::entity entity) {
    return entity == movePair.original || entity == movePair.copy;
  });

  moves->val.resize(end - moves->val.begin());
  if (moves->val.empty()) {
    registry.remove<UsedMovesType>(pokemon);
    if constexpr (std::is_same_v<UsedMovesType, UsedMovesAsAttacker>) {
      registry.remove<tags::Attacker>(pokemon);
    }
    else {
      registry.remove<tags::Defender>(pokemon);
    }
  }
}

void removeMovePairs(types::handle inputHandle, MovePair& movePair, Attacker& attacker, Defender& defender) {
  types::registry& registry = *inputHandle.registry();
  removeUsedMoves<UsedMovesAsAttacker>(registry, movePair, attacker.val);
  removeUsedMoves<UsedMovesAsDefender>(registry, movePair, defender.val);

  const OriginalInputEntities* original = inputHandle.try_get<OriginalInputEntities>();
  if (original) {
    removeUsedMoves<UsedMovesAsAttacker>(registry, movePair, original->attacker);
    removeUsedMoves<UsedMovesAsDefender>(registry, movePair, original->defender);
  }

  registry.destroy(movePair.original);
  if (movePair.original != movePair.copy) {
    registry.destroy(movePair.copy);
  }

  movePair.original = movePair.copy = entt::null;
}

void clearRunVariables(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  simulation.view<removeMovePairs>();
  simulation.view<restoreInputs>();

  simulation.addToEntities<pokesim::tags::CloneToRemove, tags::BattleCloneForCalculation>();
  deleteClones(registry);

  registry.clear<
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

  using Included = Tags<tags::Input>;
  using Excluded = entt::exclude_t<tags::RunOneCalculation, tags::GroupedWithOtherInput>;
  simulation.view<applyPseudoWeatherEffect, Included, Excluded>();
  simulation.view<applyTerrainEffect, Included, Excluded>();
  simulation.view<applyWeatherEffect, Included, Excluded>();
  simulation.view<applySideConditionEffect, Included, Excluded>();
  simulation.view<applyStatusEffect, Included, Excluded>();
  simulation.view<applyVolatileEffect, Included, Excluded>();

  simulation.view<applyBoostEffect<AtkBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<DefBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<SpaBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<SpdBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<SpeBoost>, Included, Excluded>();

  updateAllStats(simulation);

  calc_damage::run(simulation);

  simulation.view<createOutput, Included>();
  clearRunVariables(simulation);
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  analyzeEffect(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::analyze_effect