#include "CalcDamage.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Pokemon/PokemonDataChecks.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CalcDamage/DamageFormulaVariables.hpp>
#include <Components/CalcDamage/DamageRollSides.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
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
#include <Types/Event.hpp>
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
  simulation.registry.clear<
    tags::Crit,
    AttackingLevel,
    AttackingStat,
    DefendingStat,
    DamageRollModifiers,
    tags::UsesAtk,
    tags::UsesDef,
    tags::UsesSpa,
    tags::UsesSpd,
    tags::UsesDefAsOffense,
    tags::IgnoresAttackingBoost,
    tags::IgnoresDefendingBoost>();
  simulation.removeFromEntities<Damage, pokesim::tags::CalculateDamage>();
}

void checkForAndApplyStab(
  types::handle moveHandle, const Attacker& attacker, TypeName type, DamageRollModifiers& modifier) {
  const SpeciesTypes& attackerTypes = moveHandle.registry()->get<SpeciesTypes>(attacker.val);

  if (attackerTypes.has(type.name)) {
    modifier.stab = StabBoostKind::STANDARD;
  }
}

void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, Defender defender, TypeName type, DamageRollModifiers& modifier, const Pokedex& pokedex) {
  const SpeciesTypes& defenderTypes = moveHandle.registry()->get<SpeciesTypes>(defender.val);

  modifier.typeEffectiveness = getAttackEffectiveness(defenderTypes, type.name, pokedex.typeChart());
}

void applyBurnModifier(types::registry& registry, const CurrentActionMovesAsSource& moves) {
  for (types::entity move : moves.val) {
    if (registry.all_of<move::tags::Physical>(move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      registry.get<DamageRollModifiers>(move).burn = true;
    }
  }
}

void applyCritDamageIncrease(Damage& damage) {
  damage.val = (types::damage)(damage.val * MechanicConstants::CRIT_MULTIPLIER);
}

void setDamageToMinimumPossible(Damage& damage) {
  damage.val = std::max(damage.val, MechanicConstants::Damage::MIN);
}

void setDefendingSide(types::handle moveHandle, Defender defender) {
  types::registry& registry = *moveHandle.registry();
  PlayerSideId playerSide = registry.get<PlayerSide>(registry.get<Side>(defender.val).val).val;
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
  types::effectMultiplier stab = ((std::underlying_type_t<StabBoostKind>)modifiers.stab) / 100.0F;
  damage.val = (types::damage)fixedPointMultiply(damage.val, stab);

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

  setDamageToMinimumPossible(damage);
}

void calculateAllDamageRolls(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  damageRolls.val.reserve(MechanicConstants::DamageRollCount::MAX);
  for (types::damageRollIndex i = 0U; i < MechanicConstants::DamageRollCount::MAX; i++) {
    Damage& damageRoll = damageRolls.val.emplace_back(damage);
    applyDamageRoll(damageRoll.val, i);
    modifyDamage(damageRoll, modifier);
  }
}

void applyAverageDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyAverageDamageRoll(damage.val);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyMinDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyMinDamageRoll(damage.val);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void reduceDamageRollsToDefenderHp(
  types::handle moveHandle, DamageRolls& damageRolls, Damage& damage, Defender defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.val);
  for (auto& damageRoll : damageRolls.val) {
    damageRoll.val = std::min(defenderHp.val, damageRoll.val);
  }
  damage.val = std::min(defenderHp.val, damage.val);
}

void assignCritChanceDivisor(types::handle moveHandle, CritBoost critBoost) {
  std::size_t index =
    std::min((std::size_t)critBoost.val, pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS.size() - 1U);
  moveHandle.emplace<CritChanceDivisor>(pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS[index]);
}

void setSourceLevel(types::handle moveHandle, Attacker attacker) {
  moveHandle.emplace<AttackingLevel>(moveHandle.registry()->get<Level>(attacker.val).val);
}

template <typename Category>
void setUsedAttackStat(types::handle moveHandle, Attacker attacker) {
  types::stat attackingStat = MechanicConstants::PokemonEffectiveStat::MIN;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    attackingStat = moveHandle.registry()->get<stat::EffectiveAtk>(attacker.val).val;
    moveHandle.emplace<tags::UsesAtk>();
  }
  else {
    attackingStat = moveHandle.registry()->get<stat::EffectiveSpa>(attacker.val).val;
    moveHandle.emplace<tags::UsesSpa>();
  }
  moveHandle.emplace<AttackingStat>(attackingStat);
}

template <typename Category>
void setUsedDefenseStat(types::handle moveHandle, Defender defender) {
  types::stat defendingStat = MechanicConstants::PokemonEffectiveStat::MIN;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    defendingStat = moveHandle.registry()->get<stat::EffectiveDef>(defender.val).val;
    moveHandle.emplace<tags::UsesDef>();
  }
  else {
    defendingStat = moveHandle.registry()->get<stat::EffectiveSpd>(defender.val).val;
    moveHandle.emplace<tags::UsesSpd>();
  }
  moveHandle.emplace<DefendingStat>(defendingStat);
}

template <typename BoostType>
void setIgnoreAttackingBoostIfNegative(types::handle moveHandle, Attacker attacker) {
  BoostType* boost = moveHandle.registry()->try_get<BoostType>(attacker.val);
  if (boost && boost->val < MechanicConstants::PokemonStatBoost::BASE) {
    moveHandle.emplace<tags::IgnoresAttackingBoost>();
  }
}

template <typename BoostType>
void setIgnoreDefendingBoostIfPositive(types::handle moveHandle, Defender defender) {
  BoostType* boost = moveHandle.registry()->try_get<BoostType>(defender.val);
  if (boost && boost->val > MechanicConstants::PokemonStatBoost::BASE) {
    moveHandle.emplace<tags::IgnoresDefendingBoost>();
  }
}

void calculateBaseDamage(
  types::handle moveHandle, BasePower basePower, AttackingLevel level, AttackingStat attack, DefendingStat defense) {
  // NOLINTNEXTLINE(readability-magic-numbers)
  types::damage damage = ((((2U * level.val / 5U + 2U) * basePower.val * attack.val) / defense.val) / 50U) + 2U;
  moveHandle.emplace<Damage>(damage);
}

void applyUsesUntilKo(types::handle moveHandle, const DamageRolls& damageRolls, Defender defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.val);
  UsesUntilKo usesUntilKo;
  POKESIM_REQUIRE(
    damageRolls.val.size() == MechanicConstants::DamageRollCount::MAX,
    "All the damage rolls are needed to calculate this correctly.");

  for (const Damage& damageRoll : damageRolls.val) {
    types::moveHits uses = (types::moveHits)std::ceil(defenderHp.val / (types::probability)damageRoll.val);
    if (usesUntilKo.val.empty() || usesUntilKo.val.back().uses != uses) {
      usesUntilKo.val.push_back({uses, MechanicConstants::Probability::MIN});
    }

    usesUntilKo.val.back().probability +=
      (MechanicConstants::Probability::MAX / MechanicConstants::DamageRollCount::MAX);
  }
  moveHandle.emplace<UsesUntilKo>(usesUntilKo);
}

template <typename SimulationTag, auto ApplyDamageRollKind>
void applySideDamageRollOptions(Simulation& simulation) {
  static constexpr bool isSimulateTurn = std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>;
  static constexpr bool isCalculateDamage = std::is_same_v<pokesim::tags::CalculateDamage, SimulationTag>;
  static constexpr bool isAnalyzeEffect = std::is_same_v<pokesim::tags::AnalyzeEffect, SimulationTag>;

  static_assert(
    isSimulateTurn || isCalculateDamage || isAnalyzeEffect,
    "Using a type that isn't a valid simulation tag.");

  pokesim::internal::SelectForCurrentActionMoveView<SimulationTag> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  static constexpr bool onlyPassDamageRoll = pokesim::internal::getArgumentCount(ApplyDamageRollKind) == 2U;

  DamageRollOptions damageRollOptions;
  bool noKoChanceCalculation = false;
  bool calculateUpToFoeHp = false;
  if constexpr (isSimulateTurn) {
    damageRollOptions = simulation.simulateTurnOptions.damageRollsConsidered;
    calculateUpToFoeHp = true;
  }
  else if constexpr (isCalculateDamage) {
    damageRollOptions = simulation.calculateDamageOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.calculateDamageOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }
  else if constexpr (isAnalyzeEffect) {
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

template <typename EffectiveStat>
void saveRealEffectiveAttackerStat(types::registry& registry, Attacker attacker) {
  EffectiveStat effectiveStat = registry.get<EffectiveStat>(attacker.val);
  registry.emplace<RealEffectiveStat>(attacker.val, effectiveStat.val);
}

template <typename EffectiveStat>
void saveRealEffectiveDefenderStat(types::registry& registry, Defender defender) {
  EffectiveStat effectiveStat = registry.get<EffectiveStat>(defender.val);
  registry.emplace<RealEffectiveStat>(defender.val, effectiveStat.val);
}

template <typename EffectiveStat>
void resetEffectiveAndAttackingStat(types::registry& registry, Attacker attacker, AttackingStat& attackingStat) {
  auto [effectiveStat, realEffectiveStat] = registry.get<EffectiveStat, RealEffectiveStat>(attacker.val);
  attackingStat.val = effectiveStat.val;
  effectiveStat.val = realEffectiveStat.val;
}

template <typename EffectiveStat>
void resetEffectiveAndDefendingStat(types::registry& registry, Defender defender, DefendingStat& defendingStat) {
  auto [effectiveStat, realEffectiveStat] = registry.get<EffectiveStat, RealEffectiveStat>(defender.val);
  defendingStat.val = effectiveStat.val;
  effectiveStat.val = realEffectiveStat.val;
}

template <typename EffectiveStat, typename IgnoresBoostTag, typename UsesStatTag>
void setUnboostedStat(Simulation& simulation) {
  static constexpr bool forAttacker = std::is_same_v<IgnoresBoostTag, tags::IgnoresAttackingBoost>;
  if constexpr (forAttacker) {
    simulation.viewForSelectedMoves<saveRealEffectiveAttackerStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }
  else {
    simulation.viewForSelectedMoves<saveRealEffectiveDefenderStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }

  internal::SelectForPokemonView<RealEffectiveStat> selectedPokemon{simulation};
  if (selectedPokemon.hasNoneSelected()) {
    return;
  }

  if constexpr (std::is_same_v<UsesStatTag, tags::UsesAtk>) {
    simulation.viewForSelectedPokemon<resetEffectiveAtk>();
    runModifyAtk(simulation);
  }
  else if constexpr (
    std::is_same_v<UsesStatTag, tags::UsesDef> || std::is_same_v<UsesStatTag, tags::UsesDefAsOffense>) {
    simulation.viewForSelectedPokemon<resetEffectiveDef>();
    runModifyDef(simulation);
  }
  else if constexpr (std::is_same_v<UsesStatTag, tags::UsesSpa>) {
    simulation.viewForSelectedPokemon<resetEffectiveSpa>();
    runModifySpa(simulation);
  }
  else if constexpr (std::is_same_v<UsesStatTag, tags::UsesSpd>) {
    simulation.viewForSelectedPokemon<resetEffectiveSpd>();
    runModifySpd(simulation);
  }
  else {
    static_assert("No other stat is used as the attacking or defending stat.");
  }

  if constexpr (forAttacker) {
    simulation
      .viewForSelectedMoves<resetEffectiveAndAttackingStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }
  else {
    simulation
      .viewForSelectedMoves<resetEffectiveAndDefendingStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }

  simulation.registry.clear<RealEffectiveStat>();
}

void setDamageFormulaVariables(Simulation& simulation) {
  simulation.viewForSelectedMoves<setSourceLevel>();

  simulation.viewForSelectedMoves<setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  simulation.viewForSelectedMoves<setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  simulation.viewForSelectedMoves<setIgnoreAttackingBoostIfNegative<AtkBoost>, Tags<tags::Crit, tags::UsesAtk>>();
  simulation.viewForSelectedMoves<setIgnoreAttackingBoostIfNegative<SpaBoost>, Tags<tags::Crit, tags::UsesSpa>>();
  simulation.viewForSelectedMoves<setIgnoreDefendingBoostIfPositive<DefBoost>, Tags<tags::Crit, tags::UsesDef>>();
  simulation.viewForSelectedMoves<setIgnoreDefendingBoostIfPositive<SpdBoost>, Tags<tags::Crit, tags::UsesSpd>>();
  // simulation.viewForSelectedMoves<setIgnoreAttackingBoostIfNegative<DefBoost>, Tags<tags::Crit,
  // tags::UsesDefAsOffense>>();

  // simulation.viewForSelectedPokemon<dex::latest::Unaware::onUsesBoost, Tags<ability::tags::Unaware,
  // tags::Attacker>>();

  setUnboostedStat<stat::EffectiveAtk, tags::IgnoresAttackingBoost, tags::UsesAtk>(simulation);
  setUnboostedStat<stat::EffectiveSpa, tags::IgnoresAttackingBoost, tags::UsesSpa>(simulation);
  setUnboostedStat<stat::EffectiveDef, tags::IgnoresDefendingBoost, tags::UsesDef>(simulation);
  setUnboostedStat<stat::EffectiveSpd, tags::IgnoresDefendingBoost, tags::UsesSpd>(simulation);
  // setUnboostedStat<stat::EffectiveDef, tags::IgnoresAttackingBoost, tags::UsesDefAsOffense>(simulation);
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

  runBasePowerEvent(simulation);
  setDamageFormulaVariables(simulation);

  simulation.viewForSelectedMoves<calculateBaseDamage>();
  simulation.viewForSelectedMoves<applyCritDamageIncrease, Tags<tags::Crit>>();

  simulation.addToEntities<DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  setDamageRollModifiers(simulation);

  applySideDamageRollOptions<SimulateTurn, applyDamageRollsAndModifiers<SimulateTurn>>(simulation);
  applySideDamageRollOptions<CalculateDamage, applyDamageRollsAndModifiers<CalculateDamage>>(simulation);
  applySideDamageRollOptions<AnalyzeEffect, applyDamageRollsAndModifiers<AnalyzeEffect>>(simulation);

  clearRunVariables(simulation);
}
}  // namespace

void applyDamageRoll(types::damage& damage, types::damageRollIndex damageRoll) {
  damage = (types::damage)(damage * ((100U - damageRoll) / 100.0F));
}

void applyAverageDamageRoll(types::damage& damage) {
  damage = (types::damage)(damage * (100U - (MechanicConstants::DamageRollCount::MAX - 1U) / 2.0F) / 100.0F);
}

void applyMinDamageRoll(types::damage& damage) {
  applyDamageRoll(damage, MechanicConstants::DamageRollCount::MAX - 1U);
}

void setDamageRollModifiers(Simulation& simulation) {
  simulation.viewForSelectedMoves<checkForAndApplyStab>();
  simulation.viewForSelectedMoves<checkForAndApplyTypeEffectiveness>(simulation.pokedex());
  simulation
    .viewForSelectedPokemon<applyBurnModifier, Tags<status::tags::Burn> /*, entt::exclude<ability::tags::Guts> */>();
}

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  calcDamage(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::calc_damage