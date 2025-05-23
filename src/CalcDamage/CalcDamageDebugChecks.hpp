#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/CalcDamage/Aliases.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/Level.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationOptions.hpp>
#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>
#include <cstddef>

#include "Helpers.hpp"

namespace pokesim::calc_damage::debug {
struct Checks : pokesim::debug::Checks {
  Checks(const Simulation& _simulation) : pokesim::debug::Checks(_simulation) {}

  void checkInputs() {
    checkMoveInputs();
    checkPokemonInputs(true);
    checkPokemonInputs(false);
    checkBattleInputs();
    copyRemainingEntities();

    simulateTurnCount = registry->view<pokesim::tags::SimulateTurn>().size();
    calcDamageCount = registry->view<pokesim::tags::CalculateDamage>().size();
    analyzeEffectCount = registry->view<pokesim::tags::AnalyzeEffect>().size();
  }

  void checkOutputs() const {
    checkMoveOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkBattleOutputs();
    checkRemainingOutputs();

    std::size_t finalEntityCount = getFinalEntityCount();

    if (!simulation->simulateTurnOptions.makeBranchesOnRandomEvents) {
      std::size_t finalSimulationTurnCount = registry->view<pokesim::tags::SimulateTurn>().size();
      POKESIM_REQUIRE_NM(simulateTurnCount == finalSimulationTurnCount);
      POKESIM_REQUIRE_NM(initialEntityCount == finalEntityCount);
    }
    else if (simulateTurnCount == 0) {
      POKESIM_REQUIRE_NM(initialEntityCount == finalEntityCount);
    }

    std::size_t finalCalcDamageCount = registry->view<pokesim::tags::CalculateDamage>().size();
    POKESIM_REQUIRE_NM(calcDamageCount == finalCalcDamageCount);
    std::size_t finalAnalyzeEffectCount = registry->view<pokesim::tags::AnalyzeEffect>().size();
    POKESIM_REQUIRE_NM(analyzeEffectCount == finalAnalyzeEffectCount);
  }

 private:
  std::size_t simulateTurnCount = 0;
  std::size_t calcDamageCount = 0;
  std::size_t analyzeEffectCount = 0;

  void checkMoveInputs() {
    for (types::entity move : simulation->selectedMoveEntities()) {
      originalToCopy[move] = pokesim::debug::createEntityCopy(move, *registry, registryOnInput);

      POKESIM_REQUIRE_NM(registry->all_of<Attacker>(move));
      POKESIM_REQUIRE_NM(registry->all_of<tags::Attacker>(registry->get<Attacker>(move).val));
      POKESIM_REQUIRE_NM(registry->all_of<Defenders>(move));
      POKESIM_REQUIRE_NM(registry->all_of<tags::Defender>(registry->get<Defenders>(move).only()));
      POKESIM_REQUIRE_NM(registry->all_of<Battle>(move));
      POKESIM_REQUIRE_NM(registry->all_of<TypeName>(move));
      bool isPhysical = registry->all_of<move::tags::Physical>(move);
      bool isSpecial = registry->all_of<move::tags::Special>(move);
      bool isStatus = registry->all_of<move::tags::Status>(move);
      POKESIM_REQUIRE_NM(isPhysical || isSpecial || isStatus);

      bool hasSimulateTurn = registry->all_of<pokesim::tags::SimulateTurn>(move);
      bool hasCalculateDamage = registry->all_of<pokesim::tags::CalculateDamage>(move);
      bool hasAnalyzeEffect = registry->all_of<pokesim::tags::AnalyzeEffect>(move);
      POKESIM_REQUIRE_NM(hasSimulateTurn || hasCalculateDamage || hasAnalyzeEffect);
    }
  }

  std::vector<types::entity> getPokemonList(bool forAttacker) const {
    std::vector<types::entity> selectedPokemon = simulation->selectedPokemonEntities();
    auto end = std::remove_if(selectedPokemon.begin(), selectedPokemon.end(), [&](types::entity entity) {
      if (forAttacker) {
        return !this->registry->all_of<tags::Attacker>(entity);
      }

      return !this->registry->all_of<tags::Defender>(entity);
    });

    return {selectedPokemon.begin(), end};
  }

  void checkPokemonInputs(bool forAttacker) {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      originalToCopy[pokemon] = pokesim::debug::createEntityCopy(pokemon, *registry, registryOnInput);

      POKESIM_REQUIRE_NM(registry->all_of<UsedMoves>(pokemon));
      bool needsPhy = false;
      bool needsSpc = false;
      for (types::entity move : registry->get<UsedMoves>(pokemon).val) {
        POKESIM_REQUIRE_NM(registry->all_of<tags::UsedMove>(move));
        needsPhy |= registry->all_of<move::tags::Physical>(move);
        needsSpc |= registry->all_of<move::tags::Special>(move);
      }
      POKESIM_REQUIRE_NM(registry->all_of<Battle>(pokemon));
      POKESIM_REQUIRE_NM(registry->all_of<Side>(pokemon));
      POKESIM_REQUIRE_NM(registry->all_of<PlayerSide>(registry->get<Side>(pokemon).val));
      POKESIM_REQUIRE_NM(registry->all_of<Level>(pokemon));
      POKESIM_REQUIRE_NM(registry->all_of<SpeciesTypes>(pokemon));

      if (forAttacker) {
        if (needsPhy) {
          POKESIM_REQUIRE_NM(registry->all_of<stat::EffectiveAtk>(pokemon));
        }
        if (needsSpc) {
          POKESIM_REQUIRE_NM(registry->all_of<stat::EffectiveSpa>(pokemon));
        }
      }
      else {
        POKESIM_REQUIRE_NM(registry->all_of<stat::CurrentHp>(pokemon));
        if (needsPhy) {
          POKESIM_REQUIRE_NM(registry->all_of<stat::EffectiveDef>(pokemon));
        }
        if (needsSpc) {
          POKESIM_REQUIRE_NM(registry->all_of<stat::EffectiveSpd>(pokemon));
        }
      }

      bool hasSimulateTurn = registry->all_of<pokesim::tags::SimulateTurn>(pokemon);
      bool hasCalculateDamage = registry->all_of<pokesim::tags::CalculateDamage>(pokemon);
      bool hasAnalyzeEffect = registry->all_of<pokesim::tags::AnalyzeEffect>(pokemon);
      POKESIM_REQUIRE_NM(hasSimulateTurn || hasCalculateDamage || hasAnalyzeEffect);
    }
  }

  void checkBattleInputs() {
    for (types::entity battle : simulation->selectedBattleEntities()) {
      originalToCopy[battle] = pokesim::debug::createEntityCopy(battle, *registry, registryOnInput);
    }
  }

  DamageRollKind getDamageRollKind(types::entity move, DamageRollOptions damageRollOptions) const {
    const Defenders& defenders = registry->get<Defenders>(move);
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

  void checkCalcDamageResultOutputs(types::entity move) const {
    POKESIM_REQUIRE_NM(registry->all_of<DamageRolls>(move));

    DamageRollOptions damageRollOptions;
    bool noKoChanceCalculation;
    bool calculateUpToFoeHp;
    if (registry->all_of<pokesim::tags::CalculateDamage>(move)) {
      damageRollOptions = simulation->calculateDamageOptions.damageRollOptions;
      calculateUpToFoeHp = simulation->calculateDamageOptions.calculateUpToFoeHp;
      noKoChanceCalculation = simulation->calculateDamageOptions.noKoChanceCalculation;
    }
    else if (registry->all_of<pokesim::tags::AnalyzeEffect>(move)) {
      damageRollOptions = simulation->analyzeEffectOptions.damageRollOptions;
      calculateUpToFoeHp = simulation->analyzeEffectOptions.calculateUpToFoeHp;
      noKoChanceCalculation = simulation->analyzeEffectOptions.noKoChanceCalculation;
    }
    else {
      POKESIM_REQUIRE_FAIL("Where'd the simulation type go? It was checked in the inputs.");
    }

    types::damage lastDamage = std::numeric_limits<types::damage>::max();
    const auto& [damageRolls, defenders] = registry->get<DamageRolls, Defenders>(move);
    const stat::CurrentHp& defenderHp = registry->get<stat::CurrentHp>(defenders.only());

    POKESIM_REQUIRE_NM(!damageRolls.val.empty());

    for (const Damage& damageRoll : damageRolls.val) {
      POKESIM_REQUIRE_NM(lastDamage >= damageRoll.val);
      POKESIM_REQUIRE_NM(damageRoll.val > 0);
      if (calculateUpToFoeHp) {
        POKESIM_REQUIRE_NM(damageRoll.val <= defenderHp.val);
      }

      lastDamage = damageRoll.val;
    }

    DamageRollKind damageRollKind = getDamageRollKind(move, damageRollOptions);

    POKESIM_REQUIRE(
      damageRollKind != DamageRollKind::NONE,
      "Cannot calculate damage without knowing what rolls to consider.");

    std::size_t idealDamageRollCount = 0;
    if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      idealDamageRollCount = MechanicConstants::MAX_DAMAGE_ROLL_COUNT;
    }
    else {
      if (damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
        idealDamageRollCount++;
      }

      if (damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE)) {
        idealDamageRollCount++;
      }

      if (damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE)) {
        idealDamageRollCount++;
      }
    }

    POKESIM_REQUIRE_NM(idealDamageRollCount);
    POKESIM_REQUIRE_NM(damageRolls.val.size() == idealDamageRollCount);

    if (noKoChanceCalculation) {
      POKESIM_REQUIRE_NM(!registry->all_of<UsesUntilKo>(move));
    }
    else if (damageKindsMatch(DamageRollKind::ALL_DAMAGE_ROLLS, getDamageRollKind(move, damageRollOptions))) {
      POKESIM_REQUIRE_NM(registry->all_of<UsesUntilKo>(move));

      const UsesUntilKo& usesUntilKo = registry->get<UsesUntilKo>(move);
      POKESIM_REQUIRE_NM(!usesUntilKo.val.empty());
      POKESIM_REQUIRE_NM(usesUntilKo.val.front() == usesUntilKo.minHits());
      POKESIM_REQUIRE_NM(usesUntilKo.val.back() == usesUntilKo.maxHits());
      POKESIM_REQUIRE_NM(usesUntilKo.val.size() <= MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
      POKESIM_REQUIRE_NM(usesUntilKo.val.size() <= damageRolls.val.size());

      types::moveHits lastUses = 0;
      types::useUntilKoChance totalChance = 0;
      for (const auto& useUntilKo : usesUntilKo.val) {
        POKESIM_REQUIRE_NM(lastUses < useUntilKo.uses);
        totalChance += useUntilKo.chance;
        lastUses = useUntilKo.uses;
      }

      POKESIM_REQUIRE_NM(totalChance == 1.0F);
    }
  }

  void checkAttackHpResults(types::entity move) const {
    const auto [attackerHpRecovered, attackerHpLost] = registry->try_get<AttackerHpRecovered, AttackerHpLost>(move);
    const DamageRolls& damageRolls = registry->get<DamageRolls>(move);
    if (attackerHpRecovered != nullptr) {
      POKESIM_REQUIRE_NM(attackerHpRecovered->val.size() == damageRolls.val.size());

      for (std::size_t i = 0; i < damageRolls.val.size(); i++) {
        POKESIM_REQUIRE_NM(attackerHpRecovered->val[i].val <= damageRolls.val[i].val);
      }
    }

    if (attackerHpLost != nullptr) {
      POKESIM_REQUIRE_NM(attackerHpLost->val.size() == damageRolls.val.size());
    }
  }

  void checkMoveOutputs() const {
    for (types::entity move : simulation->selectedMoveEntities()) {
      pokesim::debug::TypesToIgnore typesToIgnore{};
      if (registry->all_of<pokesim::tags::AnalyzeEffect>(move)) {
        typesToIgnore.add<Damage>();
        POKESIM_REQUIRE_NM(registry->all_of<Damage>(move));
      }

      if (registry->all_of<pokesim::tags::SimulateTurn>(move)) {
        typesToIgnore.add<Damage>();
        POKESIM_REQUIRE_NM(registry->all_of<Damage>(move));
        POKESIM_REQUIRE_NM(registry->get<Damage>(move).val > 0);
        POKESIM_REQUIRE_NM(!registry->all_of<DamageRolls>(move));
        POKESIM_REQUIRE_NM(!registry->all_of<UsesUntilKo>(move));
      }
      else {
        typesToIgnore.add<DamageRolls, UsesUntilKo>();
        checkCalcDamageResultOutputs(move);

        if (registry->all_of<pokesim::tags::CalculateDamage>(move)) {
          typesToIgnore.add<AttackerHpRecovered, AttackerHpLost>();
          checkAttackHpResults(move);
        }
      }

      types::entity originalMove = pokesim::debug::findCopyParent(originalToCopy, *registry, move);
      pokesim::debug::areEntitiesEqual(
        *registry,
        move,
        registryOnInput,
        originalToCopy.at(originalMove),
        typesToIgnore);
    }
  }

  void checkPokemonOutputs(bool forAttacker) const {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      types::entity originalPokemon = pokesim::debug::findCopyParent(originalToCopy, *registry, pokemon);
      pokesim::debug::areEntitiesEqual(*registry, pokemon, registryOnInput, originalToCopy.at(originalPokemon));
    }
  }

  void checkBattleOutputs() const {
    for (types::entity battle : simulation->selectedBattleEntities()) {
      pokesim::debug::TypesToIgnore typesToIgnore;
      if (registry->all_of<pokesim::tags::SimulateTurn>(battle)) {
        typesToIgnore.add<Probability, RngSeed, ParentBattle>();
      }
      types::entity originalBattle = pokesim::debug::findCopyParent(originalToCopy, *registry, battle);
      pokesim::debug::areEntitiesEqual(
        *registry,
        battle,
        registryOnInput,
        originalToCopy.at(originalBattle),
        typesToIgnore);
    }
  }
};
}  // namespace pokesim::calc_damage::debug
#else
namespace pokesim {
class Simulation;
namespace calc_damage::debug {
struct Checks {
  Checks(const Simulation&) {}
  void checkInputs() const {}
  void checkOutputs() const {}
};
}  // namespace calc_damage::debug
}  // namespace pokesim
#endif