#include "CalcDamage.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/PokemonDataChecks.hpp>
#include <Components/BasePower.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CalcDamage/DamageCalcVariables.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/HitCount.hpp>
#include <Components/Level.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <SimulateTurn/CalcDamageSpecifics.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationOptions.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/StabBoostKind.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include "CalcDamageDebugChecks.hpp"
#include "Helpers.hpp"

namespace pokesim::calc_damage {
namespace {
void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<tags::Crit, AttackingLevel, AttackingStat, DefendingStat, DamageRollModifiers>();
  simulation.removeFromEntities<Damage, pokesim::tags::CalculateDamage>();
}

void checkForAndApplyStab(
  types::handle moveHandle, const Attacker& attacker, const TypeName& type, DamageRollModifiers& modifier) {
  const SpeciesTypes& attackerTypes = moveHandle.registry()->get<SpeciesTypes>(attacker.val);

  if (attackerTypes.has(type.name)) {
    modifier.stab = StabBoostKind::STANDARD;
  }
}

void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, const Defenders& defenders, const TypeName& type, DamageRollModifiers& modifier) {
  const SpeciesTypes& defenderTypes = moveHandle.registry()->get<SpeciesTypes>(defenders.only());

  modifier.typeEffectiveness = getAttackEffectiveness(defenderTypes, type.name);
}

void applyBurnModifier(types::registry& registry, const CurrentActionMoves& moves) {
  for (types::entity move : moves.val) {
    if (!registry.all_of<move::tags::Physical>(move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      return;
    }

    registry.get<DamageRollModifiers>(move).burn = true;
  }
}

void applyCritDamageIncrease(Damage& damage) {
  damage.val = (types::damage)(damage.val * MechanicConstants::CRIT_MULTIPLIER);
}

void setDamageToAtLeastOne(Damage& damage) {
  damage.val = std::max(damage.val, (types::damage)1U);
}

void setDefendingSide(types::handle moveHandle, const Defenders& defenders) {
  types::registry& registry = *moveHandle.registry();
  PlayerSideId playerSide = registry.get<PlayerSide>(registry.get<Side>(defenders.only()).val).val;
  switch (playerSide) {
    case PlayerSideId::NONE: {
      POKESIM_REQUIRE_FAIL("Player side wasn't set properly.");
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

void modifyDamage(Damage& damage, const DamageRollModifiers& modifiers) {
  damage.val = (types::damage)fixedPointMultiply(damage.val, ((std::uint8_t)modifiers.stab) / 100.0F);

  types::eventModifier typeEffectivenessModifier = MechanicConstants::FIXED_POINT_SCALE;
  if (modifiers.typeEffectiveness < 0) {
    typeEffectivenessModifier = typeEffectivenessModifier >> -modifiers.typeEffectiveness;
  }
  else {
    typeEffectivenessModifier = typeEffectivenessModifier << modifiers.typeEffectiveness;
  }

  applyChainedModifier(damage.val, typeEffectivenessModifier);
  applyChainedModifier(damage.val, modifiers.modifyDamageEvent);

  if (modifiers.burn) {
    damage.val = (types::damage)fixedPointMultiply(damage.val, 0.5F);
  }

  setDamageToAtLeastOne(damage);
}

void calculateAllDamageRolls(DamageRolls& damageRolls, const Damage& damage, const DamageRollModifiers& modifier) {
  damageRolls.val.reserve(MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
  for (std::uint8_t i = 0; i < MechanicConstants::MAX_DAMAGE_ROLL_COUNT; i++) {
    Damage& damageRoll = damageRolls.val.emplace_back(damage);
    applyDamageRoll(damageRoll, i);
    modifyDamage(damageRoll, modifier);
  }
}

void applyAverageDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyAverageDamageRoll(damage);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyMinDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyMinDamageRoll(damage);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void reduceDamageRollsToDefenderHp(
  types::handle moveHandle, DamageRolls& damageRolls, Damage& damage, const Defenders& defenders) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defenders.only());
  for (auto& damageRoll : damageRolls.val) {
    damageRoll.val = std::min(defenderHp.val, damageRoll.val);
  }
  damage.val = std::min(defenderHp.val, damage.val);
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
  types::damage damage = ((((2U * level.val / 5U + 2U) * basePower.val * attack.val) / defense.val) / 50U) + 2;
  moveHandle.emplace<Damage>(damage);
}

void applyUsesUntilKo(types::handle moveHandle, const DamageRolls& damageRolls, const Defenders& defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.only());
  UsesUntilKo usesUntilKo;
  POKESIM_REQUIRE(
    damageRolls.val.size() == MechanicConstants::MAX_DAMAGE_ROLL_COUNT,
    "All the damage rolls are needed to calculate this correctly.");

  for (const Damage& damageRoll : damageRolls.val) {
    types::moveHits uses = (types::moveHits)std::ceil(defenderHp.val / (types::useUntilKoChance)damageRoll.val);
    if (usesUntilKo.val.empty() || usesUntilKo.val.back().uses != uses) {
      usesUntilKo.val.push_back({uses, 0.0F});
    }

    usesUntilKo.val.back().chance += (1.0 / MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
  }
  moveHandle.emplace<UsesUntilKo>(usesUntilKo);
}

template <typename SimulationTag, auto ApplyDamageRollKind>
void applySideDamageRollOptions(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<SimulationTag> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  static constexpr bool isSimulateTurn = std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>;
  static constexpr bool onlyPassDamageRoll = pokesim::internal::getArgumentCount(ApplyDamageRollKind) == 2;

  DamageRollOptions damageRollOptions;
  bool noKoChanceCalculation = false;
  bool calculateUpToFoeHp = false;
  if constexpr (isSimulateTurn) {
    damageRollOptions = simulation.simulateTurnOptions.damageRollsConsidered;
    calculateUpToFoeHp = true;
  }
  else if constexpr (std::is_same_v<pokesim::tags::CalculateDamage, SimulationTag>) {
    damageRollOptions = simulation.calculateDamageOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.calculateDamageOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }
  else if constexpr (std::is_same_v<pokesim::tags::AnalyzeEffect, SimulationTag>) {
    damageRollOptions = simulation.analyzeEffectOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.analyzeEffectOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }

  if (damageRollOptions.sidesMatch()) {
    if constexpr (onlyPassDamageRoll) {
      ApplyDamageRollKind(simulation, damageRollOptions.p1);
    }
    else {
      ApplyDamageRollKind(simulation, damageRollOptions.p1, calculateUpToFoeHp, noKoChanceCalculation);
    }
  }
  else {
    simulation.viewForSelectedMoves<setDefendingSide>();
    pokesim::internal::SelectForCurrentActionMoveView<tags::P1Defending> p1DefendingMoves{simulation};
    if (!p1DefendingMoves.hasNoneSelected()) {
      if constexpr (onlyPassDamageRoll) {
        ApplyDamageRollKind(simulation, damageRollOptions.p1);
      }
      else {
        ApplyDamageRollKind(simulation, damageRollOptions.p1, calculateUpToFoeHp, noKoChanceCalculation);
      }
    }
    p1DefendingMoves.deselect();

    pokesim::internal::SelectForCurrentActionMoveView<tags::P2Defending> p2DefendingMoves{simulation};
    if (!p2DefendingMoves.hasNoneSelected()) {
      if constexpr (onlyPassDamageRoll) {
        ApplyDamageRollKind(simulation, damageRollOptions.p2);
      }
      else {
        ApplyDamageRollKind(simulation, damageRollOptions.p2, calculateUpToFoeHp, noKoChanceCalculation);
      }
    }

    simulation.registry.clear<tags::P1Defending, tags::P2Defending>();
  }
}

template <typename SimulationTag>
void setIfMoveCrits(Simulation& simulation, DamageRollKind damageRollKind) {
  if (damageKindsMatch(damageRollKind, DamageRollKind::GUARANTEED_CRIT_CHANCE)) {
    simulation.addToEntities<tags::Crit, pokesim::tags::SelectedForViewMove>();
    return;
  }

  if constexpr (std::is_same_v<SimulationTag, pokesim::tags::SimulateTurn>) {
    simulation.addToEntities<calc_damage::CritBoost, pokesim::tags::SelectedForViewMove, pokesim::tags::SimulateTurn>();
    runModifyCritBoostEvent(simulation);
    simulation.viewForSelectedMoves<assignCritChanceDivisor>();
    simulation.registry.clear<CritBoost>();

    simulate_turn::setIfMoveCrits(simulation);
  }
}

template <typename SimulationTag>
void applyDamageRollsAndModifiers(
  Simulation& simulation, DamageRollKind damageRollKind, bool calculateUpToFoeHp, bool noKoChanceCalculation) {
  POKESIM_REQUIRE(
    damageRollKind != DamageRollKind::NONE,
    "Cannot calculate damage without knowing what rolls to consider.");
  POKESIM_REQUIRE(
    damageRollKind != DamageRollKind::GUARANTEED_CRIT_CHANCE,
    "Must pick a damage roll kind to go along with crits.");

  simulation.addToEntities<DamageRolls, DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
    simulation.viewForSelectedMoves<calculateAllDamageRolls>();
  }
  else {
    if (damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
      simulation.viewForSelectedMoves<applyDamageRollModifier>();
    }

    if (damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE)) {
      simulation.viewForSelectedMoves<applyAverageDamageRollModifier>();
    }

    if (damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE)) {
      simulation.viewForSelectedMoves<applyMinDamageRollModifier>();
    }
  }

  if constexpr (std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>) {
    if (calculateUpToFoeHp) {
      simulation.viewForSelectedMoves<reduceDamageRollsToDefenderHp>();
    }
    simulate_turn::cloneFromDamageRolls(simulation, damageRollKind);
  }
  else {
    simulation.viewForSelectedMoves<modifyDamage>();
    if (calculateUpToFoeHp) {
      simulation.viewForSelectedMoves<reduceDamageRollsToDefenderHp>();
    }
    if (!noKoChanceCalculation && damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      simulation.viewForSelectedMoves<applyUsesUntilKo, Tags<SimulationTag>>();
    }
  }
}

void calcDamage(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<> selectedMoves{simulation, entt::exclude<move::tags::Status>};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  using SimulateTurn = pokesim::tags::SimulateTurn;
  using CalculateDamage = pokesim::tags::CalculateDamage;
  using AnalyzeEffect = pokesim::tags::AnalyzeEffect;

  applySideDamageRollOptions<SimulateTurn, setIfMoveCrits<SimulateTurn>>(simulation);
  applySideDamageRollOptions<CalculateDamage, setIfMoveCrits<CalculateDamage>>(simulation);
  applySideDamageRollOptions<AnalyzeEffect, setIfMoveCrits<AnalyzeEffect>>(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);
  simulation.viewForSelectedMoves<setSourceLevel>();

  simulation.viewForSelectedMoves<setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  simulation.viewForSelectedMoves<setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  simulation.viewForSelectedMoves<calculateBaseDamage>();

  simulation.viewForSelectedMoves<applyCritDamageIncrease, Tags<tags::Crit>>();

  simulation.addToEntities<DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  setDamageRollModifiers(simulation);

  applySideDamageRollOptions<SimulateTurn, applyDamageRollsAndModifiers<SimulateTurn>>(simulation);
  applySideDamageRollOptions<CalculateDamage, applyDamageRollsAndModifiers<CalculateDamage>>(simulation);
  applySideDamageRollOptions<AnalyzeEffect, applyDamageRollsAndModifiers<AnalyzeEffect>>(simulation);
}
}  // namespace

void setDamageRollModifiers(Simulation& simulation) {
  simulation.viewForSelectedMoves<checkForAndApplyStab>();
  simulation.viewForSelectedMoves<checkForAndApplyTypeEffectiveness>();
  simulation.viewForSelectedPokemon<
    applyBurnModifier,
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

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  calcDamage(simulation);

  clearRunVariables(simulation);
  debugChecks.checkOutputs();
}
}  // namespace pokesim::calc_damage