#include "AnalyzeEffect.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Helpers.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEventInputs.hpp>
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
  DebugChecks(const Simulation& _simulation) : simulation(_simulation), registry(simulation.registry) { checkInputs(); }
  ~DebugChecks() { checkOutputs(); }

 private:
  const Simulation& simulation;
  const types::registry& registry;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;
  entt::dense_set<types::entity> specificallyChecked;
  std::size_t entityCount = 0;

  // Add analyze_effect outputs to exception list

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
        ENTT_FAIL("Player side wasn't set properly.");
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
      debug::TypesToIgnore typesToIgnore{};
      typesToIgnore.add<MultipliedDamage, MultipliedDamageRolls>();

      assert(registry.all_of<MultipliedDamage>(input));
      assert(registry.all_of<MultipliedDamageRolls>(input));

      if (registry.all_of<tags::InfiniteMultiplier>(input)) {
        assert(!registry.all_of<EffectMultiplier>(input));
        typesToIgnore.add<tags::InfiniteMultiplier>();
      }

      bool zeroEffectMultiplier = false;
      if (registry.all_of<EffectMultiplier>(input)) {
        assert(!registry.all_of<tags::InfiniteMultiplier>(input));
        typesToIgnore.add<EffectMultiplier>();

        const auto [effectMultiplier, multipliedDamage, multipliedDamageRolls] =
          registry.get<EffectMultiplier, MultipliedDamage, MultipliedDamageRolls>(input);
        if (effectMultiplier.val == 0) {
          zeroEffectMultiplier = true;
          assert(multipliedDamage.val == 0);
          for (const MultipliedDamage& multipliedDamageRoll : multipliedDamageRolls.val) {
            assert(multipliedDamageRoll.val == 0);
          }
        }
      }

      auto damageRollOptions = simulation.analyzeEffectOptions.damageRollOptions;
      auto noKoChanceCalculation = simulation.analyzeEffectOptions.noKoChanceCalculation;
      if (noKoChanceCalculation || zeroEffectMultiplier) {
        assert(!registry.all_of<MultipliedUsesUntilKo>(input));
      }
      else if (calc_damage::damageKindsMatch(
                 DamageRollKind::ALL_DAMAGE_ROLLS,
                 getDamageRollKind(input, damageRollOptions))) {
        assert(registry.all_of<MultipliedUsesUntilKo>(input));
        typesToIgnore.add<MultipliedUsesUntilKo>();
      }

      debug::areEntitiesEqual(registry, input, registryOnInput, originalToCopy.at(input), typesToIgnore);
    }
  }

  void checkPokemonOutputs(bool forAttacker) const {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      debug::areEntitiesEqual(registry, pokemon, registryOnInput, originalToCopy.at(pokemon));
    }
  }

  void checkRemainingOutputs() const {
    for (auto [original, copy] : originalToCopy) {
      if (!specificallyChecked.contains(original)) {
        debug::areEntitiesEqual(registry, original, registryOnInput, copy);
      }
    }
  }

  void checkInputs() {
    for (types::entity input : registry.view<tags::Input>()) {
      originalToCopy[input] = debug::createEntityCopy(input, registry, registryOnInput);
    }

    const std::vector<types::entity> attackers = getPokemonList(true);
    const std::vector<types::entity> defenders = getPokemonList(false);

    for (const std::vector<types::entity>& pokemonList : {attackers, defenders}) {
      for (types::entity pokemon : pokemonList) {
        originalToCopy[pokemon] = debug::createEntityCopy(pokemon, registry, registryOnInput);
      }
    }

    for (types::entity entity : registry.view<types::entity>()) {
      if (!registry.orphan(entity)) {
        entityCount++;
        if (originalToCopy.contains(entity)) {
          specificallyChecked.emplace(entity);
        }
        else {
          originalToCopy[entity] = debug::createEntityCopy(entity, registry, registryOnInput);
        }
      }
    }
  }

  void checkOutputs() const {
    std::size_t inputCount = registry.view<Inputs>().size();
    std::size_t finalEntityCount = 0;
    for (types::entity entity : registry.view<types::entity>()) {
      if (!registry.orphan(entity)) {
        finalEntityCount++;
      }
    }
    assert(entityCount + inputCount == finalEntityCount);
    checkInputOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkRemainingOutputs();
  }
#endif
};

void restoreInputs(
  types::registry& registry, const MovePairs& movePairs, const OriginalInputEntities& originalEntities, Battle& battle,
  Attacker& attacker, Defenders& defenders, EffectTarget& effectTarget) {
  for (types::entity pokemon : {attacker.val, defenders.only(), originalEntities.attacker, originalEntities.defender}) {
    CurrentActionMoves& moves = registry.get<CurrentActionMoves>(pokemon);

    auto end = moves.val.end();
    for (const auto& movePair : movePairs.val) {
      end = std::remove_if(moves.val.begin(), end, [&movePair](types::entity entity) {
        return entity == movePair.first || entity == movePair.second;
      });
    }

    moves.val.resize(end - moves.val.begin());
    if (moves.val.empty()) {
      registry.remove<CurrentActionMoves>(pokemon);
    }
  }

  for (auto [_, childBattleMove] : movePairs.val) {
    registry.destroy(childBattleMove);
  }

  battle.val = originalEntities.battle;
  attacker.val = originalEntities.attacker;
  defenders.val[0] = originalEntities.defender;
  effectTarget.val = originalEntities.effectTarget;
}

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

    registry.emplace<OriginalInputEntities>(
      inputs.val[0],
      OriginalInputEntities{battle.val, attacker.val, defenders.only(), effectTarget.val});

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
        inputHandle.emplace<tags::InfiniteMultiplier>();
      }
      else {
        inputHandle.emplace<EffectMultiplier>((float)parentDamage.val / childDamage.val);
      }

      inputHandle.emplace<MultipliedDamage>(parentDamage);
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
        inputHandle.emplace<EffectMultiplier>((float)childDamage.val / parentDamage.val);
      }

      inputHandle.emplace<MultipliedDamage>(childDamage);
      inputHandle.emplace<MultipliedDamageRolls>(childDamageRolls);
      auto* const childKoChances = registry.try_get<calc_damage::UsesUntilKo>(childBattleMove);
      if (childKoChances != nullptr) {
        inputHandle.emplace<MultipliedUsesUntilKo>(*childKoChances);
      }
    }
  }
}

void postRunCleanup(Simulation& simulation) {
  simulation.view<restoreInputs>();
  simulation.addToEntities<pokesim::tags::CloneToRemove, tags::BattleCloneForCalculation>();
  deleteClones(simulation.registry);
  simulation.registry.clear<MovePairs, OriginalInputEntities>();
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