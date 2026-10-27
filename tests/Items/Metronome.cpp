#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Metronome: Increase the damage of moves used consecutively",
  "[Simulation][SimulateTurn][SingleBattle][Item][Metronome]") {
  dex::Move moveUsed = GENERATE(dex::Move::TACKLE, dex::Move::MUD_SLAP);
  CAPTURE(moveUsed);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, Level{25U}, dex::Item::METRONOME, moveUsed)),
    test.side(test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH)),
    test.turnDecision(moveUsed, dex::Move::SPLASH));

  types::damage lastDamage = 0U;
  for (types::moveHits i = 0U; i <= 6U; i++) {
    auto entities = test.simulateOneNonBranchingBattle();
    test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
    test.checks.checkUsedMovePokemon<stat::CurrentHp, AccBoost>(entities.p2A);

    dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
    REQUIRE(metronomeEffect.lastMove == moveUsed);
    REQUIRE(metronomeEffect.consecutiveHits == i);

    auto [initialHp, currentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
    types::damage damage = initialHp.val - currentHp.val;
    if (i == 6U) {
      REQUIRE(lastDamage == damage);
    }
    else {
      REQUIRE(lastDamage < damage);
      test.applyDecision(entities.battle, test.turnDecision(moveUsed, dex::Move::SPLASH));
    }

    lastDamage = damage;
  }
}

TEST_CASE(
  "Metronome: Resets damage increase after a different move is used",
  "[Simulation][SimulateTurn][SingleBattle][Item][Metronome]") {
  dex::Move moveUsed = GENERATE(dex::Move::TACKLE, dex::Move::MUD_SLAP);
  types::moveHits resetAfterHits = GENERATE(range(1U, 10U));
  CAPTURE(moveUsed, resetAfterHits);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, Level{25U}, dex::Item::METRONOME, moveUsed, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH)),
    test.turnDecision(moveUsed, dex::Move::SPLASH));

  types::damage firstDamage = 0U;
  for (types::moveHits i = 0U; i < resetAfterHits; i++) {
    auto entities = test.simulateOneNonBranchingBattle();
    test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
    test.checks.checkUsedMovePokemon<stat::CurrentHp, AccBoost>(entities.p2A);

    dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
    REQUIRE(metronomeEffect.lastMove == moveUsed);
    REQUIRE(metronomeEffect.consecutiveHits == i);

    auto [initialHp, currentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
    types::damage damage = initialHp.val - currentHp.val;
    if (i == 0U) {
      firstDamage = damage;
    }
    if (i < resetAfterHits - 1U) {
      test.applyDecision(entities.battle, test.turnDecision(moveUsed, dex::Move::SPLASH));
    }
    else {
      test.applyDecision(entities.battle, test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));
    }
  }

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A, 1U);
  test.checks.checkUsedMovePokemon<AccBoost>(entities.p2A);

  dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.lastMove == dex::Move::SPLASH);
  REQUIRE(metronomeEffect.consecutiveHits == 0U);

  test.applyDecision(entities.battle, test.turnDecision(moveUsed, dex::Move::SPLASH));

  test.simulateOneNonBranchingBattle();
  metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.lastMove == moveUsed);
  REQUIRE(metronomeEffect.consecutiveHits == 0U);

  auto [initialHp, currentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
  types::damage damage = initialHp.val - currentHp.val;
  REQUIRE(damage == firstDamage);
}

TEST_CASE("Metronome: Resets after switching", "[Simulation][SimulateTurn][SingleBattle][Item][Metronome]") {
  types::moveHits resetAfterHits = GENERATE(range(1U, 10U));
  CAPTURE(resetAfterHits);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, Level{25U}, dex::Item::METRONOME, dex::Move::TACKLE),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));

  for (types::moveHits i = 0U; i < resetAfterHits; i++) {
    auto entities = test.simulateOneNonBranchingBattle();
    test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

    dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
    REQUIRE(metronomeEffect.lastMove == dex::Move::TACKLE);
    REQUIRE(metronomeEffect.consecutiveHits == i);

    if (i < resetAfterHits - 1U) {
      test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));
    }
    else {
      test.applyDecision(entities.battle, test.turnDecision(Slot::P1B, dex::Move::SPLASH));
    }
  }

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.checks.checkEntityForChanges<tags::ActivePokemon, LastUsedMove, dex::MetronomeItem::Effect>(entities.p1B);
  test.checks.checkUsedMovePokemon(entities.p2A);
  REQUIRE_FALSE(test.registry().all_of<dex::MetronomeItem::Effect>(entities.p1B));

  test.applyDecision(entities.battle, test.turnDecision(Slot::P1B, dex::Move::SPLASH));
  test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));

  entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

  dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.lastMove == dex::Move::TACKLE);
  REQUIRE(metronomeEffect.consecutiveHits == 0U);
}

TEST_CASE("Metronome: Resets after failing", "[Simulation][SimulateTurn][SingleBattle][Item][Metronome]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, Level{25U}, dex::Item::METRONOME, dex::Move::TACKLE)),
    test.side(
      test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH),
      test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));

  for (types::moveHits i = 0U; i < 2U; i++) {
    auto entities = test.simulateOneNonBranchingBattle();
    test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

    dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
    REQUIRE(metronomeEffect.lastMove == dex::Move::TACKLE);
    REQUIRE(metronomeEffect.consecutiveHits == i);

    if (i == 0U) {
      test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));
    }
    else {
      test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, Slot::P2B));
    }
  }

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  dex::MetronomeItem::Effect metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.consecutiveHits == 0U);

  test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));
  test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);

  test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, Slot::P2B));
  test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
  metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.lastMove == dex::Move::TACKLE);
  REQUIRE(metronomeEffect.consecutiveHits == 1U);
}
}  // namespace pokesim
