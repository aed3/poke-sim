#include "AnalyzeEffect.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEventInputs.hpp>
#include <Components/AnalyzeEffect/RemovedEffect.hpp>
#include <Components/Boosts.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>
#include <Utilities/SelectForView.hpp>
#include <cstdint>
#include <entt/container/dense_map.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim::analyze_effect {
namespace internal {
struct DebugChecks {
#ifdef NDEBUG
  DebugChecks(const Simulation&) {}
#else
  std::size_t battleCount = 0;
  const Simulation& simulation;

  // Check that the number of analyze_effect entities are the same
  // Check entities are equal
  // Add analyze_effect outputs to exception list

  void checkInputs() { battleCount = simulation.registry.view<pokesim::tags::Battle>().size(); }

  void checkOutputs() { assert(battleCount == simulation.registry.view<pokesim::tags::Battle>().size()); }

  DebugChecks(const Simulation& _simulation) : simulation(_simulation) { checkInputs(); }
  ~DebugChecks() { checkOutputs(); }
#endif
};

void assignInputsToClones(
  Simulation& simulation, types::entity originalBattleEntity, const types::ClonedEntityMap& clonedEntityMap) {
  types::registry& registry = simulation.registry;
  const Inputs& inputs = registry.get<Inputs>(originalBattleEntity);
  const auto& battleClones = clonedEntityMap.at(originalBattleEntity);
  ENTT_ASSERT(
    inputs.val.size() == battleClones.size(),
    "Each input must have a clone and no more clones than inputs should be made.");

  for (std::size_t i = 0; i < battleClones.size(); i++) {
    auto [battle, attacker, defenders, effectTarget, moves] =
      registry.get<Battle, Attacker, Defenders, EffectTarget, EffectMoves>(inputs.val[i]);
    registry.emplace<tags::BattleCloneForCalculation>(battleClones[i]);

    MovePairs& movePairs = registry.emplace<MovePairs>(inputs.val[i]);
    movePairs.val.reserve(moves.val.size());

    const auto& clonedAttackers = clonedEntityMap.at(attacker.val);
    ENTT_ASSERT(
      battleClones.size() == clonedAttackers.size(),
      "Each attacker must have a clone and no more clones than inputs should be made.");

    const auto& clonedDefenders = clonedEntityMap.at(defenders.only());
    ENTT_ASSERT(
      battleClones.size() == clonedDefenders.size(),
      "Each defender must have a clone and no more clones than inputs should be made.");

    const auto& clonedEffectTarget = clonedEntityMap.at(effectTarget.val);
    ENTT_ASSERT(
      battleClones.size() == clonedEffectTarget.size(),
      "Each effect target must have a clone and no more clones than inputs should be made.");

    for (dex::Move move : moves.val) {
      auto createMove = [&](types::entity battleEntity, types::entity attackerEntity, types::entity defenderEntity) {
        types::entity moveEntity = createActionMoveForTarget(
          {simulation.registry, defenderEntity},
          battleEntity,
          attackerEntity,
          move,
          simulation.pokedex);
        registry.emplace<MoveName>(moveEntity, move);
        registry.emplace<pokesim::tags::AnalyzeEffect>(moveEntity);

        return moveEntity;
      };

      movePairs.val.emplace_back(
        createMove(battle.val, attacker.val, defenders.only()),
        createMove(battleClones[i], clonedAttackers[i], clonedDefenders[i]));
    }

    battle.val = battleClones[i];
    attacker.val = clonedAttackers[i];
    defenders.val[0] = clonedDefenders[i];
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

void createOutput(types::handle inputHandle, const MovePairs& movePairs) {
  bool invert = inputHandle.all_of<tags::InvertFinalAnswer>();
  types::registry& registry = *inputHandle.registry();

  for (auto [parentBattleMove, childBattleMove] : movePairs.val) {
    const auto [childDamage, childDamageRolls] = registry.get<Damage, DamageRolls>(childBattleMove);
    auto [parentDamage, parentDamageRolls] = registry.get<Damage, DamageRolls>(parentBattleMove);

    if (invert) {
      if (childDamage.val == 0) {
        registry.emplace<tags::InfiniteMultiplier>(parentBattleMove);
      }
      else {
        registry.emplace<EffectMultiplier>(parentBattleMove, (float)parentDamage.val / childDamage.val);
      }
    }
    else {
      if (parentDamage.val == 0) {
        registry.emplace<tags::InfiniteMultiplier>(parentBattleMove);
      }
      else {
        registry.emplace<EffectMultiplier>(parentBattleMove, (float)childDamage.val / parentDamage.val);
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
  internal::DebugChecks debugChecks(simulation);

  pokesim::internal::SelectForPokemonView<pokesim::tags::AnalyzeEffect> selectedPokemon(simulation);
  pokesim::internal::SelectForBattleView<pokesim::tags::AnalyzeEffect> selectedBattle(simulation);

  if (selectedPokemon.hasNoneSelected() || selectedBattle.hasNoneSelected()) {
    return;
  }

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

  calc_damage::run(simulation);

  simulation.view<internal::createOutput, Tags<tags::Input>>();

  internal::postRunCleanup(simulation);
}
}  // namespace pokesim::analyze_effect