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
    REQUIRE(metronomeEffect.successiveHits == std::min((types::moveHits)5U, i));

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
    REQUIRE(metronomeEffect.successiveHits == std::min((types::moveHits)5U, i));

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
  REQUIRE(metronomeEffect.successiveHits == 0U);

  test.applyDecision(entities.battle, test.turnDecision(moveUsed, dex::Move::SPLASH));

  test.simulateOneNonBranchingBattle();
  metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.lastMove == moveUsed);
  REQUIRE(metronomeEffect.successiveHits == 0U);

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
    REQUIRE(metronomeEffect.successiveHits == std::min((types::moveHits)5U, i));

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
  REQUIRE(metronomeEffect.successiveHits == 0U);
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
    REQUIRE(metronomeEffect.successiveHits == std::min((types::moveHits)5U, i));

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
  REQUIRE(metronomeEffect.successiveHits == 0U);

  test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));
  test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);

  test.applyDecision(entities.battle, test.turnDecision(dex::Move::TACKLE, Slot::P2B));
  test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  test.checks.checkUsedMovePokemon<dex::MetronomeItem::Effect>(entities.p1A);
  metronomeEffect = test.registry().get<dex::MetronomeItem::Effect>(entities.p1A);
  REQUIRE(metronomeEffect.lastMove == dex::Move::TACKLE);
  REQUIRE(metronomeEffect.successiveHits == 1U);
}

TEST_CASE("Metronome: Boosts with CalculateDamage", "[Simulation][CalculateDamage][SingleBattle][Item][Metronome]") {
  types::moveHits metronomeHits = GENERATE(range(0U, 10U));
  CAPTURE(metronomeHits);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      Level{25U},
      dex::Item::METRONOME,
      dex::MetronomeItem::Effect{metronomeHits, dex::Move::FLASH_CANNON},
      dex::Move::FLASH_CANNON)),
    test.side(test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH)),
    CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::FLASH_CANNON}});

  test.calcDamageOptions().setDamageRollOptions(DamageRollKind::MAX_DAMAGE);
  test.calculateDamage().damageRollResults().each([&](const DamageRolls& damageRolls) {
    types::damage idealDamage = 0U;
    switch (metronomeHits) {
      case 0U: idealDamage = 25U; break;
      case 1U: idealDamage = 30U; break;
      case 2U: idealDamage = 35U; break;
      case 3U: idealDamage = 40U; break;
      case 4U: idealDamage = 45U; break;
      default:
      case 5U: idealDamage = 50U; break;
    }

    REQUIRE(damageRolls.max() == idealDamage);
  });
}

TEST_CASE(
  "Metronome: Accounts for successive hits in CalculateDamage when option enabled",
  "[Simulation][CalculateDamage][SingleBattle][Item][Metronome]") {
  bool successiveUseEffectsInKoChanceCalculation = GENERATE(false, true);
  CAPTURE(successiveUseEffectsInKoChanceCalculation);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      Level{25U},
      dex::Item::METRONOME,
      dex::MetronomeItem::Effect{0U, dex::Move::FLASH_CANNON},
      dex::Move::FLASH_CANNON)),
    test.side(test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH)),
    CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::FLASH_CANNON}});
  test.calcDamageOptions().setDamageRollOptions(DamageRollKind::ALL_DAMAGE_ROLLS);
  test.calcDamageOptions().setNoKoChanceCalculation(false);

  test.calcDamageOptions().setSuccessiveUseEffectsInKoChanceCalculation(successiveUseEffectsInKoChanceCalculation);
  auto results = test.calculateDamage();

  results.damageRollResults().each([&](const DamageRolls& damageRolls) {
    DamageRolls idealDamageRolls{{25U, 24U, 24U, 24U, 24U, 24U, 22U, 22U, 22U, 22U, 22U, 22U, 21U, 21U, 21U, 21U}};
    REQUIRE(idealDamageRolls.val.size() == damageRolls.val.size());
    for (types::damageRollIndex i = 0U; i < idealDamageRolls.val.size(); i++) {
      CAPTURE(i);
      REQUIRE(idealDamageRolls.val[i].val == damageRolls.val[i].val);
    }
  });

  results.usesUntilKoResults().each([&](const calc_damage::UsesUntilKo& usesUntilKo) {
    calc_damage::UsesUntilKo idealUsesUntilKo;
    if (successiveUseEffectsInKoChanceCalculation) {
      idealUsesUntilKo = {{{5U, 6U}, {6U, 10U}}};
    }
    else {
      idealUsesUntilKo = {{{7U, 6U}, {8U, 10U}}};
    }
    REQUIRE(idealUsesUntilKo.val.size() == usesUntilKo.val.size());
    for (types::damageRollIndex i = 0U; i < idealUsesUntilKo.val.size(); i++) {
      CAPTURE(i);
      REQUIRE(idealUsesUntilKo.val[i] == usesUntilKo.val[i]);
    }
  });
}

TEST_CASE(
  "Metronome: Does not boost when effect is not present",
  "[Simulation][CalculateDamage][SingleBattle][Item][Metronome]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, Level{25U}, dex::Item::METRONOME, dex::Move::FLASH_CANNON)),
    test.side(test.pokemon(dex::Species::PANGORO, Level{50U}, dex::Move::SPLASH)),
    CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::FLASH_CANNON}});
  test.calcDamageOptions().setDamageRollOptions(DamageRollKind::ALL_DAMAGE_ROLLS);
  test.calcDamageOptions().setNoKoChanceCalculation(false);

  test.calcDamageOptions().setSuccessiveUseEffectsInKoChanceCalculation(true);
  auto results = test.calculateDamage();

  results.damageRollResults().each([&](const DamageRolls& damageRolls) {
    DamageRolls idealDamageRolls{{25U, 24U, 24U, 24U, 24U, 24U, 22U, 22U, 22U, 22U, 22U, 22U, 21U, 21U, 21U, 21U}};
    REQUIRE(idealDamageRolls.val.size() == damageRolls.val.size());
    for (types::damageRollIndex i = 0U; i < idealDamageRolls.val.size(); i++) {
      CAPTURE(i);
      REQUIRE(idealDamageRolls.val[i].val == damageRolls.val[i].val);
    }
  });

  results.usesUntilKoResults().each([&](const calc_damage::UsesUntilKo& usesUntilKo) {
    calc_damage::UsesUntilKo idealUsesUntilKo{{{7U, 6U}, {8U, 10U}}};
    REQUIRE(idealUsesUntilKo.val.size() == usesUntilKo.val.size());
    for (types::damageRollIndex i = 0U; i < idealUsesUntilKo.val.size(); i++) {
      CAPTURE(i);
      REQUIRE(idealUsesUntilKo.val[i] == usesUntilKo.val[i]);
    }
  });
}
}  // namespace pokesim
