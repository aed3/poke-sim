#include "Tests.hpp"

namespace pokesim {
namespace {
const DamageRollKind AVERAGE_DAMAGE = DamageRollKind::AVERAGE_DAMAGE;
const DamageRollKind MIN_DAMAGE = DamageRollKind::MIN_DAMAGE;
const DamageRollKind MAX_DAMAGE = DamageRollKind::MAX_DAMAGE;
const DamageRollKind GUARANTEED_CRIT_CHANCE = DamageRollKind::GUARANTEED_CRIT_CHANCE;
const DamageRollKind NO_CRIT_CHANCE = DamageRollKind::NO_CRIT_CHANCE;
const DamageRollKind ALL_DAMAGE_ROLLS = DamageRollKind::ALL_DAMAGE_ROLLS;

const std::array<DamageRollKind, 25U> damageRollKindCombinations = {
  AVERAGE_DAMAGE,
  MIN_DAMAGE,
  MAX_DAMAGE,
  AVERAGE_DAMAGE | MIN_DAMAGE,
  AVERAGE_DAMAGE | MAX_DAMAGE,
  AVERAGE_DAMAGE | GUARANTEED_CRIT_CHANCE,
  AVERAGE_DAMAGE | NO_CRIT_CHANCE,
  MIN_DAMAGE | MAX_DAMAGE,
  MIN_DAMAGE | GUARANTEED_CRIT_CHANCE,
  MAX_DAMAGE | GUARANTEED_CRIT_CHANCE,
  MIN_DAMAGE | NO_CRIT_CHANCE,
  MAX_DAMAGE | NO_CRIT_CHANCE,
  AVERAGE_DAMAGE | MIN_DAMAGE | MAX_DAMAGE,
  AVERAGE_DAMAGE | MIN_DAMAGE | GUARANTEED_CRIT_CHANCE,
  AVERAGE_DAMAGE | MAX_DAMAGE | GUARANTEED_CRIT_CHANCE,
  MIN_DAMAGE | MAX_DAMAGE | GUARANTEED_CRIT_CHANCE,
  AVERAGE_DAMAGE | MIN_DAMAGE | MAX_DAMAGE | GUARANTEED_CRIT_CHANCE,
  AVERAGE_DAMAGE | MIN_DAMAGE | NO_CRIT_CHANCE,
  AVERAGE_DAMAGE | MAX_DAMAGE | NO_CRIT_CHANCE,
  MIN_DAMAGE | MAX_DAMAGE | NO_CRIT_CHANCE,
  AVERAGE_DAMAGE | MIN_DAMAGE | MAX_DAMAGE | NO_CRIT_CHANCE,
  AVERAGE_DAMAGE | MIN_DAMAGE | MAX_DAMAGE | GUARANTEED_CRIT_CHANCE | NO_CRIT_CHANCE,
  ALL_DAMAGE_ROLLS,
  ALL_DAMAGE_ROLLS | GUARANTEED_CRIT_CHANCE,
  ALL_DAMAGE_ROLLS | NO_CRIT_CHANCE,
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
}  // namespace

TEST_CASE("Type Immunities", "[Simulation][CalculateDamage][SingleBattle][Immunities]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::TACKLE)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::TACKLE}});

  test.calcDamageOptions().setDamageRollOptions({GENERATE(from_range(damageRollKindCombinations))});
  auto result = test.calculateDamage();
  result.damageRollResults().each([](const DamageRolls& damageRolls) {
    REQUIRE(damageRolls.max() == Constants::Damage::IMMUNE);
    REQUIRE(damageRolls.min() == Constants::Damage::IMMUNE);
  });
}

TEST_CASE("Calculate Damage: Vertical Slice 1", "[Simulation][CalculateDamage][SingleBattle][VerticalSlice1]") {
  const IdealDamageValues furyAttackBaseDamage{
    {14U, 13U, 13U, 13U, 13U, 13U, 13U, 13U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 11U},
    {{{21U, 1U}, {23U, 7U}, {25U, 7U}, {27U, 1U}}},
    12U,
  };

  const IdealDamageValues thunderboltBaseDamage{
    {434U, 428U, 426U, 420U, 416U, 410U, 408U, 402U, 398U, 392U, 390U, 386U, 380U, 378U, 372U, 368U},
    {{{1U, 16U}}},
    401U,
  };

  const IdealDamageValues thunderboltAVDamage{
    {290U, 288U, 284U, 282U, 278U, 276U, 272U, 270U, 266U, 264U, 260U, 258U, 254U, 252U, 248U, 246U},
    {{{1U, 6U}, {2U, 10U}}},
    268U,
  };

  const IdealDamageValues furyAttackCritDamage{
    {21U, 20U, 20U, 20U, 20U, 19U, 19U, 19U, 19U, 19U, 18U, 18U, 18U, 18U, 18U, 17U},
    {{{14U, 1U}, {15U, 4U}, {16U, 5U}, {17U, 5U}, {18U, 1U}}},
    19U,
  };

  const IdealDamageValues thunderboltCritDamage{
    {650U, 642U, 636U, 630U, 624U, 618U, 608U, 602U, 596U, 590U, 584U, 578U, 570U, 564U, 558U, 552U},
    {{{1U, 16U}}},
    601U,
  };

  const IdealDamageValues thunderboltCritAVDamage{
    {434U, 428U, 426U, 420U, 416U, 410U, 408U, 402U, 398U, 392U, 390U, 386U, 380U, 378U, 372U, 368U},
    {{{1U, 16U}}},
    401U,
  };

  const IdealDamageValues thunderboltSpdBoostAVDamage{
    {194U, 192U, 188U, 188U, 186U, 182U, 182U, 180U, 176U, 176U, 174U, 170U, 170U, 168U, 164U, 164U},
    {{{2U, 16U}}},
    179U,
  };
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};

  for (std::size_t i = 0; i < 8U; i++) {
    auto p1Info = test.pokemon(dex::Species::EMPOLEON, Level{99U}, dex::Move::FURY_ATTACK);
    auto p2Info = test.pokemon(dex::Species::AMPHAROS, dex::Nature::MODEST, dex::Move::THUNDERBOLT);

    if (i & 1U) {
      p1Info.item = dex::Item::ASSAULT_VEST;
    }
    else {
      p1Info.item = std::nullopt;
    }

    if (i & 2U) {
      p1Info.currentBoosts.spd = std::nullopt;
    }
    else {
      p1Info.currentBoosts.spd = 1;
    }

    p2Info.nature = dex::Nature::MODEST;
    if (i & 4U) {
      p2Info.item = dex::Item::CHOICE_SPECS;
      p2Info.currentBoosts.spa = std::nullopt;
    }
    else {
      p2Info.item = std::nullopt;
      p2Info.currentBoosts.spa = 1;
    }

    test.setupBattle(
      test.side(p1Info),
      test.side(p2Info),
      CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::FURY_ATTACK}},
      CalcDamageInputInfo{Slot::P2A, Slot::P1A, {dex::Move::THUNDERBOLT}});
  }

  DamageRollOptions damageRollOptions;
  bool getKoUses = GENERATE(false, true);
  bool calculateUpToFoeHp = GENERATE(false, true);
  damageRollOptions.setP1(GENERATE(from_range(damageRollKindCombinations)));
  damageRollOptions.setP2(GENERATE(from_range(damageRollKindCombinations)));

  CAPTURE(getKoUses, calculateUpToFoeHp);

  auto& options = test.calcDamageOptions();
  options.setCalculateUpToFoeHp(calculateUpToFoeHp)
    .setNoKoChanceCalculation(!getKoUses)
    .setDamageRollOptions(damageRollOptions);

  auto results = test.calculateDamage();

  const auto damageRolls = results.damageRollResults();
  const auto hpLost = results.hpLostResults();
  const auto hpRecovered = results.hpRecoveredResults();
  const auto koUses = results.usesUntilKoResults();

  auto& battleInfoList = test.battleInfoList;
  REQUIRE(damageRolls.size() == battleInfoList.size() * 2U);
  REQUIRE(hpLost.empty());
  REQUIRE(hpRecovered.empty());

  if (options.getNoKoChanceCalculation()) {
    REQUIRE(koUses.empty());
  }
  else {
    std::size_t idealKoUsesSize = 0U;
    if (damageRollOptions.getP1() & ALL_DAMAGE_ROLLS) {
      idealKoUsesSize += battleInfoList.size();
    }
    if (damageRollOptions.getP2() & ALL_DAMAGE_ROLLS) {
      idealKoUsesSize += battleInfoList.size();
    }
    REQUIRE(koUses.size() == idealKoUsesSize);
  }

  auto pickIdealDamageValues = [&](
                                 dex::Move move,
                                 DamageRollKind damageRollKind,
                                 bool targetHasAssaultVest,
                                 bool targetHasSpdBoost) -> const IdealDamageValues& {
    REQUIRE((move == dex::Move::FURY_ATTACK || move == dex::Move::THUNDERBOLT));
    bool isCrit = (damageRollKind & GUARANTEED_CRIT_CHANCE) && !(damageRollKind & NO_CRIT_CHANCE);

    if (move == dex::Move::FURY_ATTACK) {
      return isCrit ? furyAttackCritDamage : furyAttackBaseDamage;
    }

    if (isCrit) {
      return targetHasAssaultVest ? thunderboltCritAVDamage : thunderboltCritDamage;
    }
    if (targetHasSpdBoost) {
      return targetHasAssaultVest ? thunderboltSpdBoostAVDamage : thunderboltAVDamage;
    }
    return targetHasAssaultVest ? thunderboltAVDamage : thunderboltBaseDamage;
  };

  const types::registry& registry = test.registry();
  damageRolls.each([&](types::entity entity, const DamageRolls& trueDamageRolls) {
    types::entity source = registry.get<CurrentActionSource>(entity).val;
    types::entity target = registry.get<CurrentActionTarget>(entity).val;
    types::stat targetHp = registry.get<stat::CurrentHp>(target).val;
    dex::Move move = registry.get<MoveName>(entity).val;

    PlayerSideId sideId = registry.get<PlayerSide>(registry.get<Side>(target).val).val;
    DamageRollKind damageRollKind = sideId == PlayerSideId::P1 ? damageRollOptions.getP1() : damageRollOptions.getP2();

    bool sourceHasChoiceSpecs = registry.all_of<dex::ChoiceSpecs>(source);
    bool sourceHasSpaBoost = registry.all_of<SpaBoost>(source);
    bool targetHasAssaultVest = registry.all_of<dex::AssaultVest>(target);
    bool targetHasSpdBoost = registry.all_of<SpdBoost>(target);

    CAPTURE(move, damageRollKind, sourceHasChoiceSpecs, sourceHasSpaBoost, targetHasAssaultVest, targetHasSpdBoost);

    const IdealDamageValues& idealDamageValues =
      pickIdealDamageValues(move, damageRollKind, targetHasAssaultVest, targetHasSpdBoost);

    if (damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS) {
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

        REQUIRE(trueKosUses.minUses() == idealKoUses.val[0]);
        REQUIRE(trueKosUses.maxUses() == idealKoUses.val.back());
        REQUIRE(trueKosUses.guaranteedKo() == (idealKoUses.val.size() == 1U && idealKoUses.val[0].hits == 1U));

        REQUIRE(trueKosUses.val.size() == idealKoUses.val.size());
        for (types::damageRollIndex i = 0U; i < trueKosUses.val.size(); i++) {
          REQUIRE(trueKosUses.val[i] == idealKoUses.val[i]);
        }
      }
    }

    if (damageRollKind & DamageRollKind::MAX_DAMAGE) {
      types::damage idealDamage = idealDamageValues.maxDamage.val;
      if (calculateUpToFoeHp) {
        idealDamage = std::min(idealDamage, targetHp);
      }
      REQUIRE(trueDamageRolls.max() == idealDamage);
    }

    if (damageRollKind & DamageRollKind::MIN_DAMAGE) {
      types::damage idealDamage = idealDamageValues.minDamage.val;
      if (calculateUpToFoeHp) {
        idealDamage = std::min(idealDamage, targetHp);
      }
      REQUIRE(trueDamageRolls.min() == idealDamage);
    }

    if (damageRollKind & DamageRollKind::AVERAGE_DAMAGE) {
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
