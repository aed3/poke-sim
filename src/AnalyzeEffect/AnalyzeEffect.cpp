#include "AnalyzeEffect.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEventInputs.hpp>
#include <Components/AnalyzeEffect/RemovedEffect.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <cstdint>
#include <entt/container/dense_map.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim::analyze_effect {
namespace internal {
void assignInputsToClones(
  Simulation& simulation, types::entity battleEntity, const types::ClonedEntityMap& clonedEntityMap) {
  const Inputs& inputs = simulation.registry.get<Inputs>(battleEntity);
  const auto& battleClones = clonedEntityMap.at(battleEntity);
  ENTT_ASSERT(
    inputs.val.size() == battleClones.size(),
    "Each input must have a clone and no more clones than inputs should be made.");

  for (std::size_t i = 0; i < battleClones.size(); i++) {
    auto [battle, attacker, defenders, effectTarget] =
      simulation.registry.get<Battle, Attacker, Defenders, EffectTarget>(inputs.val[i]);
    battle.val = battleClones[i];
    simulation.registry.emplace<tags::BattleCloneForCalculation>(battleClones[i]);

    const auto& clonedAttackers = clonedEntityMap.at(attacker.val);
    ENTT_ASSERT(
      battleClones.size() == clonedAttackers.size(),
      "Each attacker must have a clone and no more clones than inputs should be made.");
    attacker.val = clonedAttackers[i];

    const auto& clonedDefenders = clonedEntityMap.at(defenders.val[0]);
    ENTT_ASSERT(
      battleClones.size() == clonedDefenders.size(),
      "Each defender must have a clone and no more clones than inputs should be made.");
    defenders.val[0] = clonedDefenders[i];

    const auto& clonedEffectTarget = clonedEntityMap.at(effectTarget.val);
    ENTT_ASSERT(
      battleClones.size() == clonedEffectTarget.size(),
      "Each effect target must have a clone and no more clones than inputs should be made.");
    effectTarget.val = clonedEffectTarget[i];
  }
}

void ignoreBattlesWithEffectActive(Simulation& /*simulation*/) {}

void createAppliedEffectBattles(Simulation& simulation) {
  entt::dense_map<types::eventPossibilities, std::vector<types::entity>> battlesByCloneCount{};

  simulation.registry.view<Inputs>().each([&battlesByCloneCount](types::entity battleEntity, const Inputs& inputs) {
    battlesByCloneCount[inputs.val.size()].push_back(battleEntity);
  });

  types::ClonedEntityMap clonedBattleMap;
  for (const auto& [eventCount, battleEntities] : battlesByCloneCount) {
    simulation.registry.insert<pokesim::tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
    auto clonedEntityMap = clone(simulation.registry, eventCount);
    for (types::entity battleEntity : battleEntities) {
      assignInputsToClones(simulation, battleEntity, clonedEntityMap);
    }
  }
}

void applyPseudoWeatherEffect(types::handle /*inputHandle*/, Battle /*battle*/, PseudoWeatherName /*effect*/) {}

void applySideConditionEffect(
  types::handle /*inputHandle*/, Attacker /*attacker*/, const Defenders& /*defenders*/, EffectTarget /*effectTarget*/,
  SideConditionName /*effect*/) {}

void applyStatusEffect(
  types::handle inputHandle, Attacker attacker, const Defenders& defenders, EffectTarget effectTarget,
  StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  StatusName* currentStatus = registry.try_get<StatusName>(effectTarget.val);
  if (currentStatus && currentStatus->name == effect.name) {
    clearStatus({registry, effectTarget.val});
    inputHandle.emplace<tags::InvertFinalAnswer>();
  }
  else {
    setStatus({registry, effectTarget.val}, effect.name);
  }
}

void applyTerrainEffect(types::handle /*inputHandle*/, Battle /*battle*/, TerrainName /*effect*/) {}

void applyVolatileEffect(
  types::handle /*inputHandle*/, Battle /*battle*/, Attacker /*attacker*/, const Defenders& /*defenders*/,
  EffectTarget /*effectTarget*/, VolatileName /*effect*/) {}

void applyWeatherEffect(types::handle /*inputHandle*/, Battle /*battle*/, WeatherName /*effect*/) {}

template <typename BoostType>
void applyBoostEffect(
  types::handle inputHandle, Battle battle, Attacker attacker, const Defenders& defenders, EffectTarget effectTarget,
  BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  BoostType* currentBoost = registry.try_get<BoostType>(effectTarget.val);
  if (currentBoost && currentBoost->val == effect.val) {
    registry.remove<BoostType>(effectTarget.val);
    inputHandle.emplace<tags::InvertFinalAnswer>();
  }
  else {
    registry.emplace_or_replace<BoostType>(effectTarget.val, effect);
  }
}

void createMoves(
  types::handle inputHandle, Battle battle, ParentBattle parentBattle, Attacker attacker, const Defenders& defenders,
  const EffectMoves& moves, const Pokedex& pokedex) {
  types::registry& registry = *inputHandle.registry();
  ENTT_ASSERT(!moves.val.empty(), "The list of moves cannot be blank.");

  MovePairs& movePairs = inputHandle.emplace<MovePairs>();
  movePairs.val.reserve(moves.val.size());

  for (dex::Move move : moves.val) {
    auto createMove = [&](types::entity battleEntity) {
      types::handle moveHandle{registry, pokedex.buildActionMove(move, registry)};
      moveHandle.emplace<MoveName>(move);
      moveHandle.emplace<Attacker>(attacker);
      moveHandle.emplace<Defenders>(defenders);
      moveHandle.emplace<Battle>(battleEntity);
      moveHandle.emplace<pokesim::tags::AnalyzeEffect>();
      moveHandle.emplace<tags::Move>();
      return moveHandle.entity();
    };

    movePairs.val.emplace_back(createMove(parentBattle.val), createMove(battle.val));
  }
}

void createOutput(types::handle inputHandle, const MovePairs& movePairs) {
  bool invert = inputHandle.all_of<tags::InvertFinalAnswer>();
  types::registry& registry = *inputHandle.registry();

  for (auto [parentBattleMove, childBattleMove] : movePairs.val) {
    const auto [childDamage, childDamageRolls] = registry.get<Damage, DamageRolls>(childBattleMove);
    auto [parentDamage, parentDamageRolls] = registry.get<Damage, DamageRolls>(parentBattleMove);

    if (invert) {
      if (parentDamage.val == 0) {
        registry.emplace<tags::InfiniteMultiplier>(parentBattleMove);
      }
      else {
        registry.emplace<EffectMultiplier>(parentBattleMove, (float)childDamage.val / parentDamage.val);
      }
    }
    else {
      if (childDamage.val == 0) {
        registry.emplace<tags::InfiniteMultiplier>(parentBattleMove);
      }
      else {
        registry.emplace<EffectMultiplier>(parentBattleMove, (float)parentDamage.val / childDamage.val);
      }

      parentDamage = childDamage;
      parentDamageRolls = childDamageRolls;
    }
  }
}

void postRunCleanup(Simulation& simulation) {
  simulation.addToEntities<pokesim::tags::CloneToRemove, tags::BattleCloneForCalculation>();
  deleteClones(simulation.registry);
  simulation.removeFromEntities<tags::Move, MoveName>();
}
}  // namespace internal

void run(Simulation& simulation) {
  if (!simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    internal::ignoreBattlesWithEffectActive(simulation);
  }

  internal::createAppliedEffectBattles(simulation);

  simulation.view<internal::applyPseudoWeatherEffect, Tags<tags::Input>>();
  simulation.view<internal::applySideConditionEffect, Tags<tags::Input>>();
  simulation.view<internal::applyStatusEffect, Tags<tags::Input>>();
  simulation.view<internal::applyTerrainEffect, Tags<tags::Input>>();
  simulation.view<internal::applyVolatileEffect, Tags<tags::Input>>();
  simulation.view<internal::applyWeatherEffect, Tags<tags::Input>>();

  simulation.view<internal::applyBoostEffect<AtkBoost>, Tags<tags::Input>>();
  simulation.view<internal::applyBoostEffect<DefBoost>, Tags<tags::Input>>();
  simulation.view<internal::applyBoostEffect<SpaBoost>, Tags<tags::Input>>();
  simulation.view<internal::applyBoostEffect<SpdBoost>, Tags<tags::Input>>();
  simulation.view<internal::applyBoostEffect<SpeBoost>, Tags<tags::Input>>();

  simulation.view<internal::createMoves, Tags<tags::Input>>(simulation.pokedex);

  calc_damage::run(simulation);

  simulation.view<internal::createOutput, Tags<tags::Input>>();

  internal::postRunCleanup(simulation);
}
}  // namespace pokesim::analyze_effect