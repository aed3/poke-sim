#include "CalcDamage.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/PokemonDataChecks.hpp>
#include <Components/BasePower.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CalcDamage/DamageCalcVariables.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/Level.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <SimulateTurn/CalcDamageSpecifics.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationOptions.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>
#include <Utilities/SelectForView.hpp>
#include <cmath>
#include <type_traits>

#include "Helpers.hpp"

namespace pokesim::calc_damage {
namespace internal {
struct DebugChecks {
#ifdef NDEBUG
  DebugChecks(const Simulation&) {}
#else
  const Simulation& simulation;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;

  // Check that the number of calc_damage and analyze_effect entities are the same
  // Add calc_damage outputs to exception list

  void checkInputs() {
    const types::registry& registry = simulation.registry;
    for (types::entity move : simulation.selectedMoveEntities()) {
      originalToCopy[move] = debug::createEntityCopy(move, simulation.registry, registryOnInput);
      assert(registry.all_of<Attacker>(move));
      assert(registry.all_of<tags::Attacker>(registry.get<Attacker>(move).val));
      assert(registry.all_of<Defenders>(move));
      assert(registry.all_of<tags::Defender>(registry.get<Defenders>(move).only()));
      assert(registry.all_of<Battle>(move));
      assert(registry.all_of<TypeName>(move));
      assert((registry.any_of<move::tags::Physical, move::tags::Special, move::tags::Status>(move)));
      assert(
        (registry.any_of<pokesim::tags::SimulateTurn, pokesim::tags::CalculateDamage, pokesim::tags::AnalyzeEffect>(
          move)));
    }

    for (types::entity attacker : registry.view<tags::Attacker>()) {
      originalToCopy[attacker] = debug::createEntityCopy(attacker, simulation.registry, registryOnInput);
      assert(registry.all_of<UsedMoves>(attacker));
      for (types::entity move : registry.get<UsedMoves>(attacker).val) {
        assert(registry.all_of<tags::UsedMove>(move));
      }
      assert(registry.all_of<Battle>(attacker));
      assert(registry.all_of<Side>(attacker));
      assert(registry.all_of<Level>(attacker));
      assert(registry.all_of<SpeciesTypes>(attacker));
      assert((registry.any_of<stat::EffectiveAtk, stat::EffectiveSpa>(attacker)));
      assert(
        (registry.any_of<pokesim::tags::SimulateTurn, pokesim::tags::CalculateDamage, pokesim::tags::AnalyzeEffect>(
          attacker)));
    }

    for (types::entity defender : registry.view<tags::Defender>()) {
      originalToCopy[defender] = debug::createEntityCopy(defender, simulation.registry, registryOnInput);
      assert(registry.all_of<UsedMoves>(defender));
      for (types::entity move : registry.get<UsedMoves>(defender).val) {
        assert(registry.all_of<tags::UsedMove>(move));
      }
      assert(registry.all_of<Battle>(defender));
      assert(registry.all_of<Side>(defender));
      assert(registry.all_of<Level>(defender));
      assert(registry.all_of<SpeciesTypes>(defender));
      assert((registry.any_of<stat::EffectiveDef, stat::EffectiveSpd>(defender)));
      assert(
        (registry.any_of<pokesim::tags::SimulateTurn, pokesim::tags::CalculateDamage, pokesim::tags::AnalyzeEffect>(
          defender)));
    }
  }
  void checkOutputs() {
    const types::registry& registry = simulation.registry;

    for (types::entity move : simulation.selectedMoveEntities()) {
      debug::TypesToIgnore typesToIgnore;
      typesToIgnore.add<Damage>();

      assert(registry.all_of<Damage>(move));
      assert(registry.get<Damage>(move).val > 0);
      if (registry.all_of<pokesim::tags::SimulateTurn>(move)) {
        assert(!registry.all_of<DamageRolls>(move));
      }
      else {
        typesToIgnore.add<DamageRolls>();
        assert(registry.all_of<DamageRolls>(move));

        types::damage lastDamage = 0;
        const DamageRolls& damageRolls = registry.get<DamageRolls>(move);
        for (std::int64_t i = damageRolls.val.size() - 1; i >= 0; i--) {
          assert(lastDamage <= damageRolls.val[i].val);
          lastDamage = damageRolls.val[i].val;
        }
      }

      assert((!registry.any_of<tags::Crit, AttackingLevel, AttackingStat, DefendingStat>(move)));
      types::entity originalMove = debug::findCopyParent(originalToCopy, simulation.registry, move);
      debug::areEntitiesEqual(
        simulation.registry,
        move,
        registryOnInput,
        originalToCopy.at(originalMove),
        typesToIgnore);
    }

    for (types::entity attacker : registry.view<tags::Attacker>()) {
      types::entity originalAttacker = debug::findCopyParent(originalToCopy, simulation.registry, attacker);
      debug::areEntitiesEqual(simulation.registry, attacker, registryOnInput, originalToCopy.at(originalAttacker));
    }

    for (types::entity defender : registry.view<tags::Defender>()) {
      types::entity originalDefender = debug::findCopyParent(originalToCopy, simulation.registry, defender);
      debug::areEntitiesEqual(simulation.registry, defender, registryOnInput, originalToCopy.at(originalDefender));
    }
  }

  DebugChecks(const Simulation& _simulation) : simulation(_simulation) { checkInputs(); }
  ~DebugChecks() { checkOutputs(); }
#endif
};

void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<tags::Crit, AttackingLevel, AttackingStat, DefendingStat>();
}

void checkForAndApplyStab(
  types::handle moveHandle, const Attacker& attacker, const TypeName& type, DamageRollModifier& modifier) {
  const SpeciesTypes& attackerTypes = moveHandle.registry()->get<SpeciesTypes>(attacker.val);

  if (attackerTypes.has(type.name)) {
    chainToModifier(modifier.val, MechanicConstants::STAB_MULTIPLIER);
  }
}

void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, const Attacker& attacker, const Defenders& defenders, const TypeName& type,
  DamageRollModifier& modifier) {
  const SpeciesTypes& defenderTypes = moveHandle.registry()->get<SpeciesTypes>(defenders.only());

  types::boost effectiveness = getAttackEffectiveness(defenderTypes, type.name);
  if (effectiveness < 0) {
    modifier.val = modifier.val >> -effectiveness;
  }
  else {
    modifier.val = modifier.val << effectiveness;
  }
}

void applyBurnModifier(types::registry& registry, const CurrentActionMoves& moves) {
  for (types::entity move : moves.val) {
    if (!registry.all_of<move::tags::Physical>(move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      return;
    }

    chainToModifier(registry.get<DamageRollModifier>(move).val, 0.5F);
  }
}

void setDamageToAtLeastOne(Damage& damage) {
  damage.val = std::max(damage.val, (types::damage)1U);
}

void setDefendingSide(types::handle moveHandle, const Defenders& defenders) {
  types::registry& registry = *moveHandle.registry();
  PlayerSideId playerSide = registry.get<PlayerSide>(registry.get<Side>(defenders.only()).val).val;
  switch (playerSide) {
    case PlayerSideId::NONE: {
      ENTT_FAIL("Player side wasn't set properly.");
      break;
    }
    case PlayerSideId::P1: {
      moveHandle.emplace<tags::P1Defending>();
      break;
    }
    case PlayerSideId::P2: {
      moveHandle.emplace<tags::P2Defending>();
      break;
    }
  }
}

void calculateAllDamageRolls(DamageRolls& damageRolls, const Damage& damage, const DamageRollModifier& modifier) {
  damageRolls.val.reserve(MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
  for (std::uint8_t i = 0; i < MechanicConstants::MAX_DAMAGE_ROLL_COUNT; i++) {
    Damage& damageRoll = damageRolls.val.emplace_back(damage);
    applyDamageRoll(damageRoll, i);
    applyChainedModifier(damageRoll.val, modifier.val);
  }
}

void reduceDamageRollsToFoeHp(
  types::handle moveHandle, DamageRolls& damageRolls, const DamageRollModifier& modifier, const Defenders& defenders) {
  const auto& defenderHp = moveHandle.registry()->get<pokesim::stat::CurrentHp>(defenders.only());
  for (auto& damageRoll : damageRolls.val) {
    damageRoll.val = std::min(defenderHp.val, damageRoll.val);
  }
}

void applyAverageDamageRollAndModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifier& modifier) {
  applyAverageDamageRoll(damage);
  applyChainedModifier(damage.val, modifier.val);
  setDamageToAtLeastOne(damage);

  damageRolls.val.emplace_back(damage);
}

void applyMinDamageRollAndModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifier& modifier) {
  applyMinDamageRoll(damage);
  applyChainedModifier(damage.val, modifier.val);
  setDamageToAtLeastOne(damage);

  damageRolls.val.emplace_back(damage);
}

void applyDamageModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifier& modifier) {
  applyChainedModifier(damage.val, modifier.val);
  damageRolls.val.emplace_back(damage);
}

void setDamageToFirstRoll(const DamageRolls& damageRolls, Damage& damage) {
  damage = damageRolls.val[0];
}

void applyDamageRollsAndModifiers(Simulation& simulation, DamageRollKind damageRollKind, bool calculateUpToFoeHp) {
  ENTT_ASSERT(
    damageRollKind != DamageRollKind::NONE,
    "Cannot calculate damage without knowing what rolls to consider.");

  simulation.addToEntities<DamageRolls, DamageRollModifier>();
  if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
    simulation.viewForSelectedMoves<calculateAllDamageRolls>();
  }
  else {
    if (damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
      simulation.viewForSelectedMoves<applyDamageModifier>();
    }

    if (damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE)) {
      simulation.viewForSelectedMoves<applyAverageDamageRollAndModifier>();
    }

    if (damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE)) {
      simulation.viewForSelectedMoves<applyMinDamageRollAndModifier>();
    }
  }

  if (calculateUpToFoeHp) {
    simulation.viewForSelectedMoves<reduceDamageRollsToFoeHp>();
  }
}

void assignCritChanceDivisor(types::handle moveHandle, const CritBoost& critBoost) {
  std::size_t index = std::min((std::size_t)critBoost.val, pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS.size() - 1);
  moveHandle.emplace<CritChanceDivisor>(pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS[index]);
}

void setSourceLevel(types::handle moveHandle, const Attacker& attacker) {
  moveHandle.emplace<AttackingLevel>(moveHandle.registry()->get<Level>(attacker.val).val);
}

template <typename Category>
void setUsedAttackStat(types::handle moveHandle, const Attacker& attacker) {
  types::stat attackingStat = 1;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    attackingStat = moveHandle.registry()->get<stat::EffectiveAtk>(attacker.val).val;
  }
  else {
    attackingStat = moveHandle.registry()->get<stat::EffectiveSpa>(attacker.val).val;
  }
  moveHandle.emplace<AttackingStat>(attackingStat);
}

template <typename Category>
void setUsedDefenseStat(types::handle moveHandle, const Defenders& defenders) {
  types::stat defendingStat = 1;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    defendingStat = moveHandle.registry()->get<stat::EffectiveDef>(defenders.only()).val;
  }
  else {
    defendingStat = moveHandle.registry()->get<stat::EffectiveSpd>(defenders.only()).val;
  }
  moveHandle.emplace<DefendingStat>(defendingStat);
}

void calculateBaseDamage(
  types::handle moveHandle, const BasePower& basePower, const AttackingLevel& level, const AttackingStat& attack,
  const DefendingStat& defense) {
  // NOLINTNEXTLINE(readability-magic-numbers)
  types::damage damage = (((2U * level.val / 5U + 2U) * basePower.val * attack.val) / defense.val) / 50U + 2;
  moveHandle.emplace<Damage>(damage);
}

void applyCritDamageIncrease(Damage& damage) {
  damage.val = (types::damage)(damage.val * MechanicConstants::CRIT_MULTIPLIER);
}
}  // namespace internal

template <typename SimulationTag>
void applyDamageRollsAndModifiers(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<SimulationTag> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  DamageRollOptions damageRollOptions;
  bool calculateUpToFoeHp = false;
  if constexpr (std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>) {
    damageRollOptions = simulation.simulateTurnOptions.damageRollsConsidered;
    calculateUpToFoeHp = true;
  }
  else if constexpr (std::is_same_v<pokesim::tags::CalculateDamage, SimulationTag>) {
    damageRollOptions = simulation.calculateDamageOptions.damageRollsReturned;
    calculateUpToFoeHp = simulation.calculateDamageOptions.calculateUpToFoeHp;
  }
  else if constexpr (std::is_same_v<pokesim::tags::AnalyzeEffect, SimulationTag>) {
    damageRollOptions = simulation.analyzeEffectOptions.damageRollsReturned;
    calculateUpToFoeHp = simulation.analyzeEffectOptions.calculateUpToFoeHp;
  }

  auto applyDamageRolls = [&simulation, calculateUpToFoeHp](DamageRollKind damageRollKind) {
    internal::applyDamageRollsAndModifiers(simulation, damageRollKind, calculateUpToFoeHp);
    if constexpr (std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>) {
      simulate_turn::cloneFromDamageRolls(simulation, damageRollKind);
    }
    else {
      simulation.viewForSelectedMoves<internal::setDamageToFirstRoll>();
    }
  };

  if (damageRollOptions.sidesMatch()) {
    applyDamageRolls(damageRollOptions.p1);
  }
  else {
    simulation.viewForSelectedMoves<internal::setDefendingSide>();
    pokesim::internal::SelectForCurrentActionMoveView<tags::P1Defending> p1DefendingMoves{simulation};
    if (!p1DefendingMoves.hasNoneSelected()) {
      applyDamageRolls(damageRollOptions.p1);
    }
    p1DefendingMoves.deselect();

    pokesim::internal::SelectForCurrentActionMoveView<tags::P2Defending> p2DefendingMoves{simulation};
    if (!p2DefendingMoves.hasNoneSelected()) {
      applyDamageRolls(damageRollOptions.p2);
    }

    simulation.registry.clear<tags::P1Defending, tags::P2Defending>();
  }
}

void setDamageRollModifiers(Simulation& simulation) {
  simulation.viewForSelectedMoves<internal::checkForAndApplyStab>();
  simulation.viewForSelectedMoves<internal::checkForAndApplyTypeEffectiveness>();
  simulation.viewForSelectedPokemon<
    internal::applyBurnModifier,
    Tags<status::tags::Burn, tags::Attacker> /*, entt::exclude<ability::tags::Guts> */>();
}

void applyDamageRoll(Damage& damage, types::damageRoll damageRoll) {
  damage.val = (types::damage)(damage.val * ((100U - damageRoll) / 100.0F));
}

void applyAverageDamageRoll(Damage& damage) {
  damage.val = (types::damage)(damage.val * (100U - (MechanicConstants::MAX_DAMAGE_ROLL_COUNT - 1U) / 2.0F) / 100.0F);
}

void applyMinDamageRoll(Damage& damage) {
  applyDamageRoll(damage, MechanicConstants::MAX_DAMAGE_ROLL_COUNT - 1U);
}

void setIfMoveCrits(Simulation& simulation) {
  simulation.addToEntities<calc_damage::CritBoost, pokesim::tags::SelectedForViewMove, pokesim::tags::SimulateTurn>();
  runModifyCritBoostEvent(simulation);
  simulation.viewForSelectedMoves<internal::assignCritChanceDivisor>();
  simulation.registry.clear<CritBoost>();

  simulate_turn::setIfMoveCrits(simulation);
}

void getDamage(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<> selectedMoves{simulation, entt::exclude<move::tags::Status>};
  if (selectedMoves.hasNoneSelected()) return;

  setIfMoveCrits(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);
  simulation.viewForSelectedMoves<internal::setSourceLevel>();

  simulation.viewForSelectedMoves<internal::setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<internal::setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  simulation.viewForSelectedMoves<internal::setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<internal::setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  simulation.viewForSelectedMoves<internal::calculateBaseDamage>();

  simulation.viewForSelectedMoves<internal::applyCritDamageIncrease, Tags<tags::Crit>>();

  simulation.addToEntities<DamageRollModifier, pokesim::tags::SelectedForViewMove>();
  setDamageRollModifiers(simulation);
  applyDamageRollsAndModifiers<pokesim::tags::SimulateTurn>(simulation);
  applyDamageRollsAndModifiers<pokesim::tags::CalculateDamage>(simulation);
  applyDamageRollsAndModifiers<pokesim::tags::AnalyzeEffect>(simulation);
  simulation.registry.clear<DamageRollModifier>();
}

void run(Simulation& simulation) {
  internal::DebugChecks debugChecks(simulation);

  getDamage(simulation);
  internal::clearRunVariables(simulation);
}
}  // namespace pokesim::calc_damage