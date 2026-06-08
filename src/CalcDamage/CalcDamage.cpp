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
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Effects/Burn.hpp>
#include <SimulateTurn/CalcDamageSpecifics.hpp>
#include <Simulation/Formulas.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationOptions.hpp>
#include <Types/Constants.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/StabBoostKind.hpp>
#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <Utilities/EntityFilter.hpp>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include "CalcDamageDebugChecks.hpp"

namespace pokesim::calc_damage {
namespace {
auto getMoveFilter(Simulation& simulation) {
  return pokesim::internal::EntityFilter<pokesim::tags::CurrentMoveHit>{simulation};
}

void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<
    tags::Crit,
    AttackingLevel,
    AttackingStat,
    DefendingStat,
    DamageRollModifiers,
    Power,
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

  if (attackerTypes.has(type.val)) {
    modifier.stab = StabBoostKind::STANDARD;
  }
}

void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, Defender defender, TypeName type, DamageRollModifiers& modifier, const Pokedex& pokedex) {
  const SpeciesTypes& defenderTypes = moveHandle.registry()->get<SpeciesTypes>(defender.val);

  modifier.typeEffectiveness = getAttackEffectiveness(defenderTypes, type.val, pokedex.typeChart());
}

void applyCritDamageIncrease(Damage& damage, types::effectMultiplier critMultiplier) {
  damage.val = (types::damage)(damage.val * critMultiplier);
}

void setDamageToMinimumPossible(Damage& damage) {
  damage.val = std::max(damage.val, Constants::Damage::MIN);
}

void setDefendingSide(types::handle moveHandle, Defender defender) {
  types::registry& registry = *moveHandle.registry();
  PlayerSideId playerSide = registry.get<PlayerSide>(registry.get<Side>(defender.val).val).val;
  switch (playerSide) {
    case PlayerSideId::P1: {
      moveHandle.emplace<tags::P1Defending>();
      break;
    }
    case PlayerSideId::P2: {
      moveHandle.emplace<tags::P2Defending>();
      break;
    }
    default: {
      POKESIM_REQUIRE_FAIL("Player side wasn't set properly.");
      break;
    }
  }
}

void modifyDamage(Damage& damage, const DamageRollModifiers& modifiers, const Pokedex& pokedex) {
  types::effectMultiplier stab = ((std::underlying_type_t<StabBoostKind>)modifiers.stab) / 100.0F;
  damage.val = (types::damage)fixedPointMultiply(damage.val, stab);

  types::eventModifier typeEffectivenessModifier = Constants::FIXED_POINT_SCALE;
  if (modifiers.typeEffectiveness < 0) {
    typeEffectivenessModifier = typeEffectivenessModifier >> -modifiers.typeEffectiveness;
  }
  else {
    typeEffectivenessModifier = typeEffectivenessModifier << modifiers.typeEffectiveness;
  }

  damage.val = applyChainedModifier(damage.val, typeEffectivenessModifier);
  damage.val = applyChainedModifier(damage.val, modifiers.modifyDamageEvent);

  if (modifiers.burn) {
    const auto multiplier = pokedex.getStaticValue<dex::Burn::physicalDamageMultiplier>();

    damage.val = (types::damage)fixedPointMultiply(damage.val, multiplier);
  }

  setDamageToMinimumPossible(damage);
}

void calculateAllDamageRolls(
  DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier, const Pokedex& pokedex) {
  damageRolls.val.reserve(Constants::DamageRollCount::MAX);
  for (types::damageRollIndex i = 0U; i < Constants::DamageRollCount::MAX; i++) {
    Damage& damageRoll = damageRolls.val.emplace_back(damage);
    damageRoll.val = computeDamageRoll(damageRoll.val, i);
    modifyDamage(damageRoll, modifier, pokedex);
  }
}

void applyAverageDamageRollModifier(
  DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier, const Pokedex& pokedex) {
  modifyDamage(damage, modifier, pokedex);
  damage.val = computeAverageDamageRoll(damage.val);
  damageRolls.val.emplace_back(damage);
}

void applyMinDamageRollModifier(
  DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier, const Pokedex& pokedex) {
  damage.val = computeMinDamageRoll(damage.val);
  modifyDamage(damage, modifier, pokedex);
  damageRolls.val.emplace_back(damage);
}

void applyDamageRollModifier(
  DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier, const Pokedex& pokedex) {
  modifyDamage(damage, modifier, pokedex);
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

void assignCritChanceDivisor(
  types::handle moveHandle, CritBoost critBoost, const std::array<types::percentChance, 4U>& critChanceDivisors) {
  std::size_t index = std::min((std::size_t)critBoost.val, critChanceDivisors.size() - 1U);
  moveHandle.emplace<CritChanceDivisor>(critChanceDivisors[index]);
}

void setSourceLevel(types::handle moveHandle, Attacker attacker) {
  moveHandle.emplace<AttackingLevel>(moveHandle.registry()->get<Level>(attacker.val).val);
}

template <typename Category>
void setUsedAttackStat(types::handle moveHandle, Attacker attacker) {
  types::stat attackingStat = Constants::PokemonEffectiveStat::DEFAULT;
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
  types::stat defendingStat = Constants::PokemonEffectiveStat::DEFAULT;
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
  if (boost && boost->val < Constants::PokemonStatBoost::DEFAULT) {
    moveHandle.emplace<tags::IgnoresAttackingBoost>();
  }
}

template <typename BoostType>
void setIgnoreDefendingBoostIfPositive(types::handle moveHandle, Defender defender) {
  BoostType* boost = moveHandle.registry()->try_get<BoostType>(defender.val);
  if (boost && boost->val > Constants::PokemonStatBoost::DEFAULT) {
    moveHandle.emplace<tags::IgnoresDefendingBoost>();
  }
}

void calculateBaseDamage(
  types::handle moveHandle, Power power, AttackingLevel level, AttackingStat attack, DefendingStat defense) {
  types::damage damage = computeBaseDamage(power.val, level.val, attack.val, defense.val);
  moveHandle.emplace<Damage>(damage);
}

void applyUsesUntilKo(types::handle moveHandle, const DamageRolls& damageRolls, Defender defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.val);
  UsesUntilKo usesUntilKo;
  POKESIM_REQUIRE(
    damageRolls.val.size() == Constants::DamageRollCount::MAX,
    "All the damage rolls are needed to calculate this correctly.");

  for (const Damage& damageRoll : damageRolls.val) {
    types::moveHits hits = (types::moveHits)std::ceil(defenderHp.val / (types::probability)damageRoll.val);
    if (usesUntilKo.val.empty() || usesUntilKo.val.back().hits != hits) {
      usesUntilKo.val.push_back({hits});
    }

    usesUntilKo.val.back().damageRollsIncluded++;
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

  pokesim::internal::EntityFilter<SimulationTag, pokesim::tags::CurrentMoveHit> moveFilter{simulation};
  if (moveFilter.hasNoneSelected()) {
    return;
  }

  static constexpr bool onlyPassDamageRoll = pokesim::internal::getArgumentCount(ApplyDamageRollKind) == 2U;

  DamageRollOptions damageRollOptions;
  bool noKoChanceCalculation = false;
  bool calculateUpToFoeHp = false;
  if constexpr (isSimulateTurn) {
    damageRollOptions = simulation.simulateTurnOptions.getDamageRollsConsidered();
    calculateUpToFoeHp = true;
  }
  else if constexpr (isCalculateDamage) {
    damageRollOptions = simulation.calculateDamageOptions.getDamageRollOptions();
    calculateUpToFoeHp = simulation.calculateDamageOptions.getCalculateUpToFoeHp();
    noKoChanceCalculation = simulation.calculateDamageOptions.getNoKoChanceCalculation();
  }
  else if constexpr (isAnalyzeEffect) {
    damageRollOptions = simulation.analyzeEffectOptions.getDamageRollOptions();
    calculateUpToFoeHp = simulation.analyzeEffectOptions.getCalculateUpToFoeHp();
    noKoChanceCalculation = simulation.calculateDamageOptions.getNoKoChanceCalculation();
  }

  if (damageRollOptions.sidesMatch()) {
    moveFilter.template addToSelected<internal::tags::ApplySideDamageRollOptions>();
    simulation.removeFromEntities<internal::tags::ApplySideDamageRollOptions, move::tags::Status>();
    if constexpr (onlyPassDamageRoll) {
      ApplyDamageRollKind(simulation, damageRollOptions.getP1());
    }
    else {
      ApplyDamageRollKind(simulation, damageRollOptions.getP1(), calculateUpToFoeHp, noKoChanceCalculation);
    }
    simulation.registry.clear<internal::tags::ApplySideDamageRollOptions>();
  }
  else {
    moveFilter.template view<setDefendingSide>();

    simulation.addToEntities<internal::tags::ApplySideDamageRollOptions, tags::P1Defending>();
    simulation.removeFromEntities<internal::tags::ApplySideDamageRollOptions, move::tags::Status>();
    if constexpr (onlyPassDamageRoll) {
      ApplyDamageRollKind(simulation, damageRollOptions.getP1());
    }
    else {
      ApplyDamageRollKind(simulation, damageRollOptions.getP1(), calculateUpToFoeHp, noKoChanceCalculation);
    }
    simulation.registry.clear<internal::tags::ApplySideDamageRollOptions, tags::P1Defending>();

    simulation.addToEntities<internal::tags::ApplySideDamageRollOptions, tags::P2Defending>();
    simulation.removeFromEntities<internal::tags::ApplySideDamageRollOptions, move::tags::Status>();
    if constexpr (onlyPassDamageRoll) {
      ApplyDamageRollKind(simulation, damageRollOptions.getP2());
    }
    else {
      ApplyDamageRollKind(simulation, damageRollOptions.getP2(), calculateUpToFoeHp, noKoChanceCalculation);
    }
    simulation.registry.clear<internal::tags::ApplySideDamageRollOptions, tags::P2Defending>();
  }
}

template <typename SimulationTag>
void setIfMoveCrits(Simulation& simulation, DamageRollKind damageRollKind) {
  if (damageRollKind & DamageRollKind::GUARANTEED_CRIT_CHANCE) {
    simulation.addToEntities<tags::Crit, internal::tags::ApplySideDamageRollOptions>();
    return;
  }

  if constexpr (std::is_same_v<SimulationTag, pokesim::tags::SimulateTurn>) {
    simulation.addToEntities<calc_damage::CritBoost, internal::tags::ApplySideDamageRollOptions>();
    runModifyCritBoostEvent(simulation);
    simulation.view<assignCritChanceDivisor>(
      simulation.pokedex().getStaticValue<MechanicConstants::CRIT_CHANCE_DIVISORS>());
    simulation.registry.clear<CritBoost>();

    simulate_turn::setIfMoveCrits(simulation);
    simulation.registry.clear<CritChanceDivisor>();
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

  pokesim::internal::EntityFilter<internal::tags::ApplySideDamageRollOptions> moveFilter{simulation};
  if (moveFilter.hasNoneSelected()) {
    return;
  }

  simulation.addToEntities<DamageRolls, DamageRollModifiers, internal::tags::ApplySideDamageRollOptions>();
  if (damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS) {
    moveFilter.view<calculateAllDamageRolls>(simulation.pokedex());
  }
  else {
    if (damageRollKind & DamageRollKind::MAX_DAMAGE) {
      moveFilter.view<applyDamageRollModifier>(simulation.pokedex());
    }

    if (damageRollKind & DamageRollKind::AVERAGE_DAMAGE) {
      moveFilter.view<applyAverageDamageRollModifier>(simulation.pokedex());
    }

    if (damageRollKind & DamageRollKind::MIN_DAMAGE) {
      moveFilter.view<applyMinDamageRollModifier>(simulation.pokedex());
    }
  }

  if constexpr (std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>) {
    if (calculateUpToFoeHp) {
      moveFilter.view<reduceDamageRollsToDefenderHp>();
    }
    simulate_turn::cloneFromDamageRolls(simulation, damageRollKind);
  }
  else {
    moveFilter.view<modifyDamage>(simulation.pokedex());
    if (calculateUpToFoeHp) {
      moveFilter.view<reduceDamageRollsToDefenderHp>();
    }
    if (!noKoChanceCalculation && damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS) {
      moveFilter.view<applyUsesUntilKo, Tags<SimulationTag>>();
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
  auto moveFilter = getMoveFilter(simulation);

  if constexpr (forAttacker) {
    moveFilter.view<saveRealEffectiveAttackerStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }
  else {
    moveFilter.view<saveRealEffectiveDefenderStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }

  if (simulation.registry.view<RealEffectiveStat>().empty()) {
    return;
  }

  if constexpr (std::is_same_v<UsesStatTag, tags::UsesAtk>) {
    simulation.addToEntities<pokesim::tags::AtkStatUpdateRequired, RealEffectiveStat>();
    updateAtk(simulation, true);
  }
  else if constexpr (
    std::is_same_v<UsesStatTag, tags::UsesDef> || std::is_same_v<UsesStatTag, tags::UsesDefAsOffense>) {
    simulation.addToEntities<pokesim::tags::DefStatUpdateRequired, RealEffectiveStat>();
    updateDef(simulation, true);
  }
  else if constexpr (std::is_same_v<UsesStatTag, tags::UsesSpa>) {
    simulation.addToEntities<pokesim::tags::SpaStatUpdateRequired, RealEffectiveStat>();
    updateSpa(simulation, true);
  }
  else if constexpr (std::is_same_v<UsesStatTag, tags::UsesSpd>) {
    simulation.addToEntities<pokesim::tags::SpdStatUpdateRequired, RealEffectiveStat>();
    updateSpd(simulation, true);
  }
  else {
    static_assert("No other stat is used as the attacking or defending stat.");
  }

  if constexpr (forAttacker) {
    moveFilter.view<resetEffectiveAndAttackingStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }
  else {
    moveFilter.view<resetEffectiveAndDefendingStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }

  simulation.registry.clear<RealEffectiveStat>();
}

void setDamageFormulaVariables(Simulation& simulation) {
  auto moveFilter = getMoveFilter(simulation);
  moveFilter.view<setSourceLevel>();

  moveFilter.view<setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  moveFilter.view<setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  moveFilter.view<setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  moveFilter.view<setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  moveFilter.view<setIgnoreAttackingBoostIfNegative<AtkBoost>, Tags<tags::Crit, tags::UsesAtk>>();
  moveFilter.view<setIgnoreAttackingBoostIfNegative<SpaBoost>, Tags<tags::Crit, tags::UsesSpa>>();
  moveFilter.view<setIgnoreDefendingBoostIfPositive<DefBoost>, Tags<tags::Crit, tags::UsesDef>>();
  moveFilter.view<setIgnoreDefendingBoostIfPositive<SpdBoost>, Tags<tags::Crit, tags::UsesSpd>>();
  // moveFilter.view<setIgnoreAttackingBoostIfNegative<DefBoost>, Tags<tags::Crit, tags::UsesDefAsOffense>>();

  // moveFilter.view<dex::latest::Unaware::onUsesBoost, Tags<ability::tags::Unaware, tags::Attacker>>();

  setUnboostedStat<stat::EffectiveAtk, tags::IgnoresAttackingBoost, tags::UsesAtk>(simulation);
  setUnboostedStat<stat::EffectiveSpa, tags::IgnoresAttackingBoost, tags::UsesSpa>(simulation);
  setUnboostedStat<stat::EffectiveDef, tags::IgnoresDefendingBoost, tags::UsesDef>(simulation);
  setUnboostedStat<stat::EffectiveSpd, tags::IgnoresDefendingBoost, tags::UsesSpd>(simulation);
  // setUnboostedStat<stat::EffectiveDef, tags::IgnoresAttackingBoost, tags::UsesDefAsOffense>(simulation);
}

void setDamageRollModifiers(Simulation& simulation) {
  auto moveFilter = getMoveFilter(simulation);
  moveFilter.view<checkForAndApplyStab>();
  moveFilter.view<checkForAndApplyTypeEffectiveness>(simulation.pokedex());
  dex::Burn::onSetDamageRollModifiers(simulation);
  runModifyDamageEvent(simulation);
}

void calcDamage(Simulation& simulation) {
  auto moveFilter = getMoveFilter(simulation);
  if (moveFilter.hasNoneSelected()) {
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

  moveFilter.view<calculateBaseDamage>();
  moveFilter.view<applyCritDamageIncrease, Tags<tags::Crit>>(
    simulation.pokedex().getStaticValue<MechanicConstants::CRIT_MULTIPLIER>());

  simulation.addToEntities<DamageRollModifiers, Damage, pokesim::tags::CurrentMoveHit>();
  setDamageRollModifiers(simulation);

  applySideDamageRollOptions<SimulateTurn, applyDamageRollsAndModifiers<SimulateTurn>>(simulation);
  applySideDamageRollOptions<CalculateDamage, applyDamageRollsAndModifiers<CalculateDamage>>(simulation);
  applySideDamageRollOptions<AnalyzeEffect, applyDamageRollsAndModifiers<AnalyzeEffect>>(simulation);

  runAfterModifyDamageEvent(simulation);

  clearRunVariables(simulation);
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  calcDamage(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::calc_damage
