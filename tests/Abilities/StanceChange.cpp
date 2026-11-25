#include "../Tests.hpp"

namespace pokesim {
namespace {
constexpr types::stat LOWER_STAT = 105U;
constexpr types::stat HIGHER_STAT = 285U;
constexpr types::stat SPEED_STAT = 125U;

void checkAegislashStats(
  TestSimulation& test, types::entity entity, bool usedMove, types::stat attacking, types::stat defending) {
  debug::TypesToIgnore typesToIgnore;
  typesToIgnore.add<
    SpeciesName,
    TemporaryFormChangedFrom,
    dex::Stall,
    stat::Atk,
    stat::Def,
    stat::Spa,
    stat::Spd,
    stat::EffectiveAtk,
    stat::EffectiveDef,
    stat::EffectiveSpa,
    stat::EffectiveSpd>();

  if (usedMove) {
    test.checks.checkUsedMovePokemon(entity, typesToIgnore);
  }
  else {
    typesToIgnore.add<tags::ActivePokemon>();
    test.checks.checkEntityForChanges(entity, typesToIgnore);
  }

  auto [hp, atk, def, spa, spd, spe, effectiveAtk, effectiveDef, effectiveSpa, effectiveSpd, effectiveSpe] =
    test.registry()
      .get<
        stat::Hp,
        stat::Atk,
        stat::Def,
        stat::Spa,
        stat::Spd,
        stat::Spe,
        stat::EffectiveAtk,
        stat::EffectiveDef,
        stat::EffectiveSpa,
        stat::EffectiveSpd,
        stat::EffectiveSpe>(entity);

  REQUIRE(hp.val == 230U);
  REQUIRE(spe.val == SPEED_STAT);
  REQUIRE(effectiveSpe.val == SPEED_STAT);

  REQUIRE(atk.val == attacking);
  REQUIRE(spa.val == attacking);
  REQUIRE(effectiveAtk.val == attacking);
  REQUIRE(effectiveSpa.val == attacking);
  REQUIRE(def.val == defending);
  REQUIRE(spd.val == defending);
  REQUIRE(effectiveDef.val == defending);
  REQUIRE(effectiveSpd.val == defending);
}

void checkShieldStats(TestSimulation& test, types::entity entity, bool usedMove = true) {
  REQUIRE(test.registry().get<SpeciesName>(entity).val == dex::Species::SHIELD_AEGISLASH);
  checkAegislashStats(test, entity, usedMove, LOWER_STAT, HIGHER_STAT);
}

void checkBladeStats(TestSimulation& test, types::entity entity, bool usedMove = true) {
  REQUIRE(test.registry().get<SpeciesName>(entity).val == dex::Species::BLADE_AEGISLASH);
  checkAegislashStats(test, entity, usedMove, HIGHER_STAT, LOWER_STAT);
}
}  // namespace

TEST_CASE(
  "Stance Change: Maintains form with normal status move",
  "[Simulation][SimulateTurn][SingleBattle][Ability][StanceChange][FormChange]") {
  dex::Species startingForm =
    GENERATE(dex::Species::AEGISLASH, dex::Species::SHIELD_AEGISLASH, dex::Species::BLADE_AEGISLASH);
  CAPTURE(startingForm);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(startingForm, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p2A);

  if (startingForm == dex::Species::BLADE_AEGISLASH) {
    checkBladeStats(test, entities.p1A);
  }
  else {
    checkShieldStats(test, entities.p1A);
  }
}

TEST_CASE(
  "Stance Change: Changes to blade form on attack",
  "[Simulation][SimulateTurn][SingleBattle][Ability][StanceChange][FormChange]") {
  dex::Species startingForm =
    GENERATE(dex::Species::AEGISLASH, dex::Species::SHIELD_AEGISLASH, dex::Species::BLADE_AEGISLASH);
  dex::Move attackingMove = GENERATE(dex::Move::TACKLE, dex::Move::VOLT_SWITCH, dex::Move::LEAFAGE);
  CAPTURE(startingForm, attackingMove);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(startingForm, attackingMove)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(attackingMove, dex::Move::SPLASH));

  types::registry& registry = test.registry();
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
  types::stat p2Hp = registry.get<stat::CurrentHp>(entities.p2A).val;
  switch (attackingMove) {
    case dex::Move::TACKLE: {
      REQUIRE(p2Hp == 286U);
      break;
    }
    case dex::Move::VOLT_SWITCH: {
      REQUIRE(p2Hp == 231U);
      break;
    }
    case dex::Move::LEAFAGE: {
      REQUIRE(p2Hp == 255U);
      break;
    }
    default: FAIL();
  }

  checkBladeStats(test, entities.p1A);
  REQUIRE(registry.get<TemporaryFormChangedFrom>(entities.p1A).val == dex::Species::SHIELD_AEGISLASH);
}

TEST_CASE(
  "Stance Change: Changes to shield form when using King's Shield",
  "[Simulation][SimulateTurn][SingleBattle][Ability][StanceChange][FormChange]") {
  dex::Species startingForm =
    GENERATE(dex::Species::AEGISLASH, dex::Species::SHIELD_AEGISLASH, dex::Species::BLADE_AEGISLASH);
  CAPTURE(startingForm);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(startingForm, dex::Move::KINGS_SHIELD)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::KINGS_SHIELD, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p2A);

  checkShieldStats(test, entities.p1A);
  if (startingForm == dex::Species::BLADE_AEGISLASH) {
    REQUIRE(test.registry().get<TemporaryFormChangedFrom>(entities.p1A).val == dex::Species::SHIELD_AEGISLASH);
  }
  else {
    REQUIRE_FALSE(test.registry().all_of<TemporaryFormChangedFrom>(entities.p1A));
  }
}

TEST_CASE(
  "Stance Change: Revert to shield form on switch out",
  "[Simulation][SimulateTurn][SingleBattle][Ability][StanceChange][FormChange]") {
  dex::Species startingForm =
    GENERATE(dex::Species::AEGISLASH, dex::Species::SHIELD_AEGISLASH, dex::Species::BLADE_AEGISLASH);
  CAPTURE(startingForm);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(startingForm, dex::Move::SPLASH), test.pokemon(dex::Species::CLAYDOL, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(Slot::P1B, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  checkShieldStats(test, entities.p1B, false);
  REQUIRE_FALSE(test.registry().all_of<TemporaryFormChangedFrom>(entities.p1B));
}
}  // namespace pokesim
