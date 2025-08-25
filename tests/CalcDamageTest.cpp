#include "Tests.hpp"

namespace pokesim {
const DamageRollKind AVERAGE_DAMAGE = DamageRollKind::AVERAGE_DAMAGE;
const DamageRollKind MIN_DAMAGE = DamageRollKind::MIN_DAMAGE;
const DamageRollKind MAX_DAMAGE = DamageRollKind::MAX_DAMAGE;
const DamageRollKind GUARANTEED_CRIT_CHANCE = DamageRollKind::GUARANTEED_CRIT_CHANCE;
const DamageRollKind ALL_DAMAGE_ROLLS = DamageRollKind::ALL_DAMAGE_ROLLS;

const std::array<DamageRollKind, 16U> damageRollKindCombinations = {
  AVERAGE_DAMAGE,
  MIN_DAMAGE,
  MAX_DAMAGE,
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, MIN_DAMAGE),
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, MAX_DAMAGE),
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(MIN_DAMAGE, MAX_DAMAGE),
  calc_damage::combineDamageKinds(MIN_DAMAGE, GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(MAX_DAMAGE, GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, MIN_DAMAGE, MAX_DAMAGE),
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, MIN_DAMAGE, GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, MAX_DAMAGE, GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(MIN_DAMAGE, MAX_DAMAGE, GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(AVERAGE_DAMAGE, MIN_DAMAGE, MAX_DAMAGE, GUARANTEED_CRIT_CHANCE),
  ALL_DAMAGE_ROLLS,
  calc_damage::combineDamageKinds(ALL_DAMAGE_ROLLS, GUARANTEED_CRIT_CHANCE),
};

struct IdealDamageValues {
  DamageRolls rolls;
  Damage average;
  Damage minDamage;
  Damage maxDamage;
  calc_damage::UsesUntilKo koUses;

  IdealDamageValues(std::vector<types::damage> _rolls, const calc_damage::UsesUntilKo& _koUses, types::damage _average)
      : rolls(_rolls), average({_average}), minDamage({_rolls[15]}), maxDamage({_rolls[0]}), koUses(_koUses) {}
};

TEST_CASE("Calculate Damage: Vertical Slice 1", "[Simulation][CalculateDamage]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  battleCreationInfo.runWithCalculateDamage = true;
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);

  bool useSpecsInsteadOfBoost = GENERATE(false, true);
  if (useSpecsInsteadOfBoost) {
    battleCreationInfo.p2.team[0].item = dex::Item::CHOICE_SPECS;
    battleCreationInfo.p2.team[0].currentBoosts.spa = 0U;
  }
  else {
    battleCreationInfo.p2.team[0].item = dex::Item::NO_ITEM;
    battleCreationInfo.p2.team[0].currentBoosts.spa = 1U;
  }

  battleCreationInfo.damageCalculations = {
    {Slot::P1A, Slot::P2A, {dex::Move::FURY_ATTACK}},
    {Slot::P2A, Slot::P1A, {dex::Move::THUNDERBOLT}},
  };

  const IdealDamageValues furyAttackBaseDamage(
    {13U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 11U, 11U, 11U, 11U, 11U, 11U, 11U, 11U},
    {{{23U, 1 / 16.0F}, {25U, 7 / 16.0F}, {27U, 8 / 16.0F}}},
    12U);
  const IdealDamageValues thunderboltBaseDamage(
    {282U, 278U, 276U, 272U, 270U, 266U, 264U, 260U, 258U, 254U, 252U, 248U, 246U, 242U, 240U, 236U},
    {{{1U, 3 / 16.0F}, {2U, 13 / 16.0F}}},
    258U);

  const IdealDamageValues furyAttackCritDamage(
    {19U, 18U, 18U, 18U, 18U, 18U, 17U, 17U, 17U, 17U, 17U, 16U, 16U, 16U, 16U, 16U},
    {{{16U, 1 / 16.0F}, {17U, 5 / 16.0F}, {18U, 5 / 16.0F}, {19U, 5 / 16.0F}}},
    17U);
  const IdealDamageValues thunderboltCritDamage(
    {422U, 416U, 414U, 408U, 404U, 398U, 396U, 392U, 386U, 384U, 378U, 374U, 372U, 366U, 362U, 356U},
    {{{1U, 16 / 16.0F}}},
    390U);

  DamageRollOptions damageRollOptions;
  bool getKoUses = GENERATE(false, true);
  bool calculateUpToFoeHp = GENERATE(false, true);
  damageRollOptions.p1 = GENERATE(from_range(damageRollKindCombinations));
  damageRollOptions.p2 = GENERATE(from_range(damageRollKindCombinations));

  CAPTURE(useSpecsInsteadOfBoost, getKoUses, calculateUpToFoeHp, damageRollOptions.p1, damageRollOptions.p2);

  auto& options = simulation.calculateDamageOptions;
  options.calculateUpToFoeHp = calculateUpToFoeHp;
  options.noKoChanceCalculation = !getKoUses;
  options.damageRollOptions = damageRollOptions;

  simulation.createInitialStates({battleCreationInfo});
  auto result = simulation.calculateDamage();

  const auto damageRolls = result.damageRollResults();
  const auto hpLost = result.hpLostResults();
  const auto hpRecovered = result.hpRecoveredResults();
  const auto koUses = result.usesUntilKoResults();

  REQUIRE(damageRolls.size() == 2U);
  REQUIRE(hpLost.empty());
  REQUIRE(hpRecovered.empty());

  if (options.noKoChanceCalculation) {
    REQUIRE(koUses.empty());
  }
  else {
    types::damageRollIndex idealKoUsesSize = 0U;
    if (calc_damage::damageKindsMatch(damageRollOptions.p1, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      idealKoUsesSize++;
    }
    if (calc_damage::damageKindsMatch(damageRollOptions.p2, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      idealKoUsesSize++;
    }
    REQUIRE(koUses.size() == idealKoUsesSize);
  }

  auto pickIdealDamageValues = [&](dex::Move move, DamageRollKind damageRollKind) -> const IdealDamageValues& {
    REQUIRE((move == dex::Move::FURY_ATTACK || move == dex::Move::THUNDERBOLT));
    bool isCrit = calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::GUARANTEED_CRIT_CHANCE);

    if (move == dex::Move::FURY_ATTACK) {
      return isCrit ? furyAttackCritDamage : furyAttackBaseDamage;
    }

    return isCrit ? thunderboltCritDamage : thunderboltBaseDamage;
  };

  const types::registry& registry = simulation.registry;
  damageRolls.each([&](types::entity entity, const DamageRolls& trueDamageRolls) {
    types::entity target = registry.get<CurrentActionTargets>(entity).only();
    types::stat targetHp = registry.get<stat::CurrentHp>(target).val;

    PlayerSideId sideId = registry.get<PlayerSide>(registry.get<Side>(target).val).val;
    DamageRollKind damageRollKind = sideId == PlayerSideId::P1 ? damageRollOptions.p1 : damageRollOptions.p2;

    dex::Move move = registry.get<MoveName>(entity).name;
    const IdealDamageValues& idealDamageValues = pickIdealDamageValues(move, damageRollKind);

    if (calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      REQUIRE(trueDamageRolls.val.size() == idealDamageValues.rolls.val.size());
      for (types::damageRollIndex i = 0U; i < trueDamageRolls.val.size(); i++) {
        types::damage idealDamage = idealDamageValues.rolls.val[i].val;
        if (calculateUpToFoeHp) {
          idealDamage = std::min(idealDamage, targetHp);
        }
        REQUIRE(trueDamageRolls.val[i].val == idealDamage);
      }

      if (getKoUses) {
        REQUIRE(koUses.contains(entity));
        const auto& [trueKosUses] = koUses.get(entity);
        const auto& idealKoUses = idealDamageValues.koUses;

        REQUIRE(trueKosUses.minHits() == idealKoUses.val[0]);
        REQUIRE(trueKosUses.maxHits() == idealKoUses.val.back());
        REQUIRE(trueKosUses.guaranteedKo() == (idealKoUses.val.size() == 1U));

        REQUIRE(trueKosUses.val.size() == idealKoUses.val.size());
        for (types::damageRollIndex i = 0U; i < trueKosUses.val.size(); i++) {
          REQUIRE(trueKosUses.val[i] == idealKoUses.val[i]);
        }
      }
    }

    if (calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
      types::damage idealDamage = idealDamageValues.maxDamage.val;
      if (calculateUpToFoeHp) {
        idealDamage = std::min(idealDamage, targetHp);
      }
      REQUIRE(trueDamageRolls.max() == idealDamage);
    }

    if (calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE)) {
      types::damage idealDamage = idealDamageValues.minDamage.val;
      if (calculateUpToFoeHp) {
        idealDamage = std::min(idealDamage, targetHp);
      }
      REQUIRE(trueDamageRolls.min() == idealDamage);
    }

    if (calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE)) {
      types::damage trueAverageDamage = calc_damage::averageOfDamageRolls(trueDamageRolls, damageRollKind);
      types::damage idealDamage = idealDamageValues.average.val;
      if (calculateUpToFoeHp) {
        idealDamage = std::min(idealDamage, targetHp);
      }
      REQUIRE(trueAverageDamage == idealDamage);
    }
  });
}
}  // namespace pokesim
