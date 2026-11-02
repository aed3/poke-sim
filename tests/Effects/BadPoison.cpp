#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Bad Poison: Inflict 1/16 of match HP per stage",
  "[Simulation][SimulateTurn][SingleBattle][Effect][Status][BadPoison]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  static constexpr types::badPoisonStages STAGE_LIMIT = Constants::BadPoisonStages::MAX + 1U;
  types::badPoisonStages startingBadPoisonStage =
    GENERATE(range(Constants::BadPoisonStages::MIN, Constants::BadPoisonStages::MAX));
  CAPTURE(startingBadPoisonStage);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::PANGORO,
      dex::Status::TOX,
      dex::BadPoison::Effect{startingBadPoisonStage},
      dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  types::registry& registry = test.registry();
  types::stat maxHp = 300U;
  types::stat idealHp = maxHp;
  for (types::badPoisonStages stage = startingBadPoisonStage + 1U; stage < STAGE_LIMIT; stage++) {
    CAPTURE(stage);
    types::damage toxicDamage = (maxHp / (STAGE_LIMIT)) * stage;
    if (doesDamageKo(idealHp, toxicDamage)) {
      break;
    }

    idealHp -= toxicDamage;
    auto entities = test.simulateOneNonBranchingBattle();
    test.checks.checkUsedMovePokemon<stat::CurrentHp, dex::BadPoison::Effect>(entities.p1A);
    test.checks.checkUsedMovePokemon(entities.p2A);
    REQUIRE(registry.get<dex::BadPoison::Effect>(entities.p1A).val == stage);
    REQUIRE(registry.get<stat::CurrentHp>(entities.p1A).val == idealHp);

    test.applyDecision(entities.battle, test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));
  }

  auto entities = test.simulateOneNonBranchingBattle(Tags<Winner>{}, Tags<TeamRemaining>{});
  REQUIRE(registry.get<Winner>(entities.battle).val == PlayerSideId::P2);
  REQUIRE(registry.all_of<tags::Fainted>(entities.p1A));
  REQUIRE(registry.get<stat::CurrentHp>(entities.p1A).val == Constants::PokemonCurrentHpStat::MIN);
}

TEST_CASE(
  "Bad Poison: Reset stage on switch in", "[Simulation][SimulateTurn][SingleBattle][Effect][Status][BadPoison]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};

  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::PANGORO, dex::Status::TOX, dex::BadPoison::Effect{4U}, dex::Move::SPLASH),
      test.pokemon(dex::Species::RIBOMBEE, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(Slot::P1B, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.applyDecision(entities.battle, test.turnDecision(Slot::P1B, dex::Move::SPLASH));
  entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  REQUIRE(test.registry().get<dex::BadPoison::Effect>(entities.p1A).val == Constants::BadPoisonStages::MIN + 1U);
  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == 282U);
}
}  // namespace pokesim
