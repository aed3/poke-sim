#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Scrappy: Ignores normal and fighting type immunities",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Scrappy][Immunities]") {
  dex::Move attackingMove = GENERATE(dex::Move::TACKLE, dex::Move::CLOSE_COMBAT);
  CAPTURE(attackingMove);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::HISUIAN_DECIDUEYE, dex::Ability::SCRAPPY, attackingMove)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Item::ROCKY_HELMET, dex::Move::SPLASH)),
    test.turnDecision(attackingMove, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp, DefBoost, SpdBoost, stat::EffectiveDef, stat::EffectiveSpd>(
    entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

  auto [p1InitialHp, p1CurrentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p1A);
  REQUIRE(p1CurrentHp.val < p1InitialHp.val);
  auto [p2InitialHp, p2CurrentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
  REQUIRE(p2CurrentHp.val < p2InitialHp.val);
}

TEST_CASE(
  "Scrappy: Does not ignores other immunities",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Scrappy][Immunities]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::HISUIAN_DECIDUEYE, dex::Ability::SCRAPPY, dex::Move::THUNDERBOLT)),
    test.side(test.pokemon(dex::Species::CLAYDOL, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::THUNDERBOLT, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);
}

TEST_CASE(
  "Scrappy: Ignores normal and fighting type immunities in CalculateDamage",
  "[Simulation][CalculateDamage][SingleBattle][Ability][Scrappy][Immunities]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::HISUIAN_DECIDUEYE, dex::Ability::SCRAPPY, dex::Move::TACKLE)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Item::ROCKY_HELMET, dex::Move::SPLASH)),
    CalcDamageInputInfo{
      Slot::P1A,
      Slot::P2A,
      {
        dex::Move::TACKLE,
        dex::Move::CLOSE_COMBAT,
        dex::Move::LEAFAGE,
      }});

  auto result = test.calculateDamage();
  result.damageRollResults().each([&test](types::entity entity, const DamageRolls& damageRolls) {
    REQUIRE_FALSE(damageRolls.min() == Constants::Damage::IMMUNE);
    REQUIRE_FALSE(test.registry().all_of<calc_damage::tags::DefenderImmune>(entity));
  });
}
}  // namespace pokesim
