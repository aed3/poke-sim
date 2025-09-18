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
  Options options;
  Checks(const Simulation& _simulation)
      : pokesim::debug::Checks(_simulation), options(_simulation.calculateDamageOptions) {}

  void checkInputs() {
    pokesim::debug::check(options.damageRollOptions);

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
    POKESIM_REQUIRE_NM(options == simulation->calculateDamageOptions);

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
    else if (simulateTurnCount == 0U) {
      POKESIM_REQUIRE_NM(initialEntityCount == finalEntityCount);
    }

    std::size_t finalCalcDamageCount = registry->view<pokesim::tags::CalculateDamage>().size();
    POKESIM_REQUIRE_NM(calcDamageCount == finalCalcDamageCount);
    std::size_t finalAnalyzeEffectCount = registry->view<pokesim::tags::AnalyzeEffect>().size();
    POKESIM_REQUIRE_NM(analyzeEffectCount == finalAnalyzeEffectCount);
  }

 private:
  std::size_t simulateTurnCount = 0U;
  std::size_t calcDamageCount = 0U;
  std::size_t analyzeEffectCount = 0U;

  void checkMoveInputs() {
    CurrentActionMovesAsSource moves{simulation->selectedMoveEntities()};
    for (types::entity move : moves.val) {
      if (has<pokesim::move::tags::Status>(move)) continue;

      originalToCopy[move] = pokesim::debug::createEntityCopy(move, *registry, registryOnInput);

      bool hasSimulateTurn = has<pokesim::tags::SimulateTurn>(move);
      bool hasCalculateDamage = has<pokesim::tags::CalculateDamage>(move);
      bool hasAnalyzeEffect = has<pokesim::tags::AnalyzeEffect>(move);
      POKESIM_REQUIRE_NM(hasSimulateTurn || hasCalculateDamage || hasAnalyzeEffect);
    }

    pokesim::debug::check(moves, *registry);
  }

  types::entityVector getPokemonList(bool forAttacker) const {
    types::entityVector selectedPokemon = simulation->selectedPokemonEntities();
    auto end = std::remove_if(selectedPokemon.begin(), selectedPokemon.end(), [&](types::entity entity) {
      if (forAttacker) {
        return !this->has<tags::Attacker>(entity);
      }

      return !this->has<tags::Defender>(entity);
    });

    return {selectedPokemon.begin(), end};
  }

  void checkPokemonInputs(bool forAttacker) {
    const types::entityVector pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      originalToCopy[pokemon] = pokesim::debug::createEntityCopy(pokemon, *registry, registryOnInput);
      checkPokemon(pokemon);

      types::entityVector moves;
      if (forAttacker) {
        POKESIM_REQUIRE_NM(has<UsedMovesAsAttacker>(pokemon));
        moves = registry->get<UsedMovesAsAttacker>(pokemon).val;
      }
      else {
        POKESIM_REQUIRE_NM(has<UsedMovesAsDefender>(pokemon));
        moves = registry->get<UsedMovesAsDefender>(pokemon).val;
      }

      bool needsPhy = false;
      bool needsSpc = false;
      for (types::entity move : moves) {
        POKESIM_REQUIRE_NM(has<tags::UsedMove>(move));
        needsPhy |= has<move::tags::Physical>(move);
        needsSpc |= has<move::tags::Special>(move);
      }

      if (forAttacker) {
        if (needsPhy) {
          POKESIM_REQUIRE_NM(has<stat::EffectiveAtk>(pokemon));
        }
        if (needsSpc) {
          POKESIM_REQUIRE_NM(has<stat::EffectiveSpa>(pokemon));
        }
      }
      else {
        POKESIM_REQUIRE_NM(has<stat::CurrentHp>(pokemon));
        if (needsPhy) {
          POKESIM_REQUIRE_NM(has<stat::EffectiveDef>(pokemon));
        }
        if (needsSpc) {
          POKESIM_REQUIRE_NM(has<stat::EffectiveSpd>(pokemon));
        }
      }

      bool hasSimulateTurn = has<pokesim::tags::SimulateTurn>(pokemon);
      bool hasCalculateDamage = has<pokesim::tags::CalculateDamage>(pokemon);
      bool hasAnalyzeEffect = has<pokesim::tags::AnalyzeEffect>(pokemon);
      POKESIM_REQUIRE_NM(hasSimulateTurn || hasCalculateDamage || hasAnalyzeEffect);
    }
  }

  void checkBattleInputs() {
    for (types::entity battle : simulation->selectedBattleEntities()) {
      originalToCopy[battle] = pokesim::debug::createEntityCopy(battle, *registry, registryOnInput);
      checkBattle(battle);
      checkSide(registry->get<Sides>(battle).p1());
      checkSide(registry->get<Sides>(battle).p2());
    }
  }

  DamageRollKind getDamageRollKind(types::entity move, DamageRollOptions damageRollOptions) const {
    const Defender& defender = registry->get<Defender>(move);
    const Side& side = registry->get<Side>(defender.val);
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
    POKESIM_REQUIRE_NM(has<DamageRolls>(move));

    DamageRollOptions damageRollOptions;
    bool noKoChanceCalculation;
    bool calculateUpToFoeHp;
    if (has<pokesim::tags::CalculateDamage>(move)) {
      damageRollOptions = simulation->calculateDamageOptions.damageRollOptions;
      calculateUpToFoeHp = simulation->calculateDamageOptions.calculateUpToFoeHp;
      noKoChanceCalculation = simulation->calculateDamageOptions.noKoChanceCalculation;
    }
    else if (has<pokesim::tags::AnalyzeEffect>(move)) {
      damageRollOptions = simulation->analyzeEffectOptions.damageRollOptions;
      calculateUpToFoeHp = simulation->analyzeEffectOptions.calculateUpToFoeHp;
      noKoChanceCalculation = simulation->analyzeEffectOptions.noKoChanceCalculation;
    }
    else {
      POKESIM_REQUIRE_FAIL("Where'd the simulation type go? It was checked in the inputs.");
    }

    types::damage lastDamage = std::numeric_limits<types::damage>::max();
    const auto& [damageRolls, defender] = registry->get<DamageRolls, Defender>(move);
    const stat::CurrentHp& defenderHp = registry->get<stat::CurrentHp>(defender.val);

    POKESIM_REQUIRE_NM(!damageRolls.val.empty());

    for (const Damage& damageRoll : damageRolls.val) {
      POKESIM_REQUIRE_NM(lastDamage >= damageRoll.val);
      POKESIM_REQUIRE_NM(damageRoll.val >= MechanicConstants::Damage::MIN);
      if (calculateUpToFoeHp) {
        POKESIM_REQUIRE_NM(damageRoll.val <= defenderHp.val);
      }

      lastDamage = damageRoll.val;
    }

    DamageRollKind damageRollKind = getDamageRollKind(move, damageRollOptions);

    POKESIM_REQUIRE(
      damageRollKind != DamageRollKind::NONE,
      "Cannot calculate damage without knowing what rolls to consider.");

    std::size_t idealDamageRollCount = 0U;
    if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      idealDamageRollCount = MechanicConstants::DamageRollCount::MAX;
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
      POKESIM_REQUIRE_NM(!has<UsesUntilKo>(move));
    }
    else if (damageKindsMatch(DamageRollKind::ALL_DAMAGE_ROLLS, getDamageRollKind(move, damageRollOptions))) {
      POKESIM_REQUIRE_NM(has<UsesUntilKo>(move));

      const UsesUntilKo& usesUntilKo = registry->get<UsesUntilKo>(move);
      pokesim::debug::check(usesUntilKo);
      POKESIM_REQUIRE_NM(usesUntilKo.val.size() <= damageRolls.val.size());
    }
  }

  void checkAttackHpResults(types::entity move) const {
    const auto [attackerHpRecovered, attackerHpLost] = registry->try_get<AttackerHpRecovered, AttackerHpLost>(move);
    const DamageRolls& damageRolls = registry->get<DamageRolls>(move);
    if (attackerHpRecovered != nullptr) {
      POKESIM_REQUIRE_NM(attackerHpRecovered->val.size() == damageRolls.val.size());

      for (types::damageRollIndex i = 0U; i < damageRolls.val.size(); i++) {
        POKESIM_REQUIRE_NM(attackerHpRecovered->val[i].val <= damageRolls.val[i].val);
      }
    }

    if (attackerHpLost != nullptr) {
      POKESIM_REQUIRE_NM(attackerHpLost->val.size() == damageRolls.val.size());
    }
  }

  void checkMoveOutputs() const {
    for (types::entity move : simulation->selectedMoveEntities()) {
      if (has<pokesim::move::tags::Status>(move)) continue;

      pokesim::debug::TypesToIgnore typesToIgnore{};
      if (has<pokesim::tags::AnalyzeEffect>(move)) {
        typesToIgnore.add<Damage>();
        POKESIM_REQUIRE_NM(has<Damage>(move));
      }

      if (has<pokesim::tags::SimulateTurn>(move)) {
        typesToIgnore.add<Damage>();
        POKESIM_REQUIRE_NM(has<Damage>(move));
        POKESIM_REQUIRE_NM(registry->get<Damage>(move).val >= MechanicConstants::Damage::MIN);
        POKESIM_REQUIRE_NM(!has<DamageRolls>(move));
        POKESIM_REQUIRE_NM(!has<UsesUntilKo>(move));
      }
      else {
        typesToIgnore.add<DamageRolls, UsesUntilKo>();
        checkCalcDamageResultOutputs(move);

        if (has<pokesim::tags::CalculateDamage>(move)) {
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
    const types::entityVector pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      types::entity originalPokemon = pokesim::debug::findCopyParent(originalToCopy, *registry, pokemon);
      pokesim::debug::areEntitiesEqual(*registry, pokemon, registryOnInput, originalToCopy.at(originalPokemon));
    }
  }

  void checkBattleOutputs() const {
    for (types::entity battle : simulation->selectedBattleEntities()) {
      pokesim::debug::TypesToIgnore typesToIgnore;
      if (has<pokesim::tags::SimulateTurn>(battle)) {
        typesToIgnore.add<Probability, RngSeed, ParentBattle>();
      }
      checkBattle(battle);
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