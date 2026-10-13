#include "../Tests.hpp"

namespace pokesim {
constexpr GameMechanics TestMechanic = GameMechanics::SCARLET_VIOLET;
constexpr types::boost COMPETITIVE_BOOST = dex::Competitive::onAfterEachBoostSpaBoost(TestMechanic);
constexpr types::boost DEFIANT_BOOST = dex::Defiant::onAfterEachBoostAtkBoost(TestMechanic);

TEST_CASE("Competitive: Boost when SPA is lowered", "[Simulation][SimulateTurn][SingleBattle][Ability][Competitive]") {
  TestSimulation test{TestMechanic, BattleFormat::SINGLES};
  dex::Move p2Move = GENERATE(dex::Move::MOONBLAST, dex::Move::CONFIDE);
  types::boost startingBoost = GENERATE(0, 2, -3);
  CAPTURE(p2Move, startingBoost);

  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Ability::COMPETITIVE, dex::Move::SPLASH, SpaBoost{startingBoost})),
    test.side(test.pokemon(dex::Species::RIBOMBEE, p2Move)),
    test.turnDecision(dex::Move::SPLASH, p2Move));

  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::PercentChance::MIN);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveSpa, SpaBoost>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  REQUIRE(
    test.registry().get<SpaBoost>(entities.p1A).val ==
    (COMPETITIVE_BOOST + test.pokedex.getMoveData<SpaBoost>(p2Move).val) + startingBoost);
}

TEST_CASE("Defiant: Boost when ATK is lowered", "[Simulation][SimulateTurn][SingleBattle][Ability][Defiant]") {
  TestSimulation test{TestMechanic, BattleFormat::SINGLES};
  dex::Move p2Move = GENERATE(dex::Move::CHILLING_WATER, dex::Move::GROWL);
  types::boost startingBoost = GENERATE(0, 2, -3);
  CAPTURE(p2Move, startingBoost);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Ability::DEFIANT, dex::Move::SPLASH, AtkBoost{startingBoost})),
    test.side(test.pokemon(dex::Species::RIBOMBEE, p2Move)),
    test.turnDecision(dex::Move::SPLASH, p2Move));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  REQUIRE(
    test.registry().get<AtkBoost>(entities.p1A).val ==
    (DEFIANT_BOOST + test.pokedex.getMoveData<AtkBoost>(p2Move).val) + startingBoost);
}

TEST_CASE(
  "Competitive: Boost when stat that's not SPA is lowered",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Competitive]") {
  TestSimulation test{TestMechanic, BattleFormat::SINGLES};
  dex::Move p2Move = GENERATE(
    dex::Move::CHILLING_WATER,
    dex::Move::GROWL,
    dex::Move::FLASH_CANNON,
    dex::Move::TRIPLE_ARROWS,
    dex::Move::LOW_SWEEP,
    dex::Move::MUD_SLAP);
  types::boost startingBoost = GENERATE(0, -1, 4);
  CAPTURE(p2Move, startingBoost);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::COMPETITIVE,
      dex::Move::SPLASH,
      AtkBoost{startingBoost},
      DefBoost{startingBoost},
      SpdBoost{startingBoost},
      SpeBoost{startingBoost},
      AccBoost{startingBoost})),
    test.side(test.pokemon(dex::Species::RIBOMBEE, Level{20U}, p2Move)),
    test.turnDecision(dex::Move::SPLASH, p2Move));

  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::PercentChance::MIN);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p2A);

  switch (p2Move) {
    case dex::Move::CHILLING_WATER:
    case dex::Move::GROWL:          {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveSpa, SpaBoost, stat::EffectiveAtk, AtkBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<AtkBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<AtkBoost>(p2Move).val);
      break;
    }
    case dex::Move::FLASH_CANNON: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveSpa, SpaBoost, stat::EffectiveSpd, SpdBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<SpdBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<SpdBoost>(p2Move).val);
      break;
    }
    case dex::Move::TRIPLE_ARROWS: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveSpa, SpaBoost, stat::EffectiveDef, DefBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<DefBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<DefBoost>(p2Move).val);
      break;
    }
    case dex::Move::LOW_SWEEP: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveSpa, SpaBoost, stat::EffectiveSpe, SpeBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<SpeBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<SpeBoost>(p2Move).val);
      break;
    }
    case dex::Move::MUD_SLAP: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveSpa, SpaBoost, AccBoost>(entities.p1A);
      REQUIRE(
        test.registry().get<AccBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<AccBoost>(p2Move).val);
      break;
    }
    default: FAIL();
  }

  REQUIRE(test.registry().get<SpaBoost>(entities.p1A).val == COMPETITIVE_BOOST);
}

TEST_CASE(
  "Defiant: Boost when stat that's not ATK is lowered", "[Simulation][SimulateTurn][SingleBattle][Ability][Defiant]") {
  TestSimulation test{TestMechanic, BattleFormat::SINGLES};
  dex::Move p2Move = GENERATE(
    dex::Move::MOONBLAST,
    dex::Move::CONFIDE,
    dex::Move::FLASH_CANNON,
    dex::Move::TRIPLE_ARROWS,
    dex::Move::LOW_SWEEP,
    dex::Move::MUD_SLAP);
  types::boost startingBoost = GENERATE(0, -1, 4);
  CAPTURE(p2Move, startingBoost);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::DEFIANT,
      dex::Move::SPLASH,
      DefBoost{startingBoost},
      SpaBoost{startingBoost},
      SpdBoost{startingBoost},
      SpeBoost{startingBoost},
      AccBoost{startingBoost})),
    test.side(test.pokemon(dex::Species::RIBOMBEE, Level{20U}, p2Move)),
    test.turnDecision(dex::Move::SPLASH, p2Move));

  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::PercentChance::MIN);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p2A);

  switch (p2Move) {
    case dex::Move::MOONBLAST:
    case dex::Move::CONFIDE:   {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost, stat::EffectiveSpa, SpaBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<SpaBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<SpaBoost>(p2Move).val);
      break;
    }
    case dex::Move::FLASH_CANNON: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost, stat::EffectiveSpd, SpdBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<SpdBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<SpdBoost>(p2Move).val);
      break;
    }
    case dex::Move::TRIPLE_ARROWS: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost, stat::EffectiveDef, DefBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<DefBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<DefBoost>(p2Move).val);
      break;
    }
    case dex::Move::LOW_SWEEP: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost, stat::EffectiveSpe, SpeBoost>(
        entities.p1A);
      REQUIRE(
        test.registry().get<SpeBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<SpeBoost>(p2Move).val);
      break;
    }
    case dex::Move::MUD_SLAP: {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost, AccBoost>(entities.p1A);
      REQUIRE(
        test.registry().get<AccBoost>(entities.p1A).val ==
        startingBoost + test.pokedex.getMoveData<AccBoost>(p2Move).val);
      break;
    }
    default: FAIL();
  }

  REQUIRE(test.registry().get<AtkBoost>(entities.p1A).val == DEFIANT_BOOST);
}

TEST_CASE(
  "Competitive/Defiant: Boost multiple times when a move lowers multiple stats",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Competitive][Defiant]") {
  TestSimulation test{TestMechanic, BattleFormat::SINGLES};
  dex::Ability p1Ability = GENERATE(dex::Ability::COMPETITIVE, dex::Ability::DEFIANT);
  CAPTURE(p1Ability);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, p1Ability, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Move::NOBLE_ROAR)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::NOBLE_ROAR));

  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::PercentChance::MIN);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveAtk, AtkBoost, stat::EffectiveSpa, SpaBoost>(
    entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);
  auto [nobleRoarAtkBoost, nobleRoarSpaBoost] = test.pokedex.getMoveData<AtkBoost, SpaBoost>(dex::Move::NOBLE_ROAR);

  types::boost idealAtkBoost = nobleRoarAtkBoost.val;
  types::boost idealSpaBoost = nobleRoarSpaBoost.val;

  if (p1Ability == dex::Ability::COMPETITIVE) {
    idealSpaBoost += COMPETITIVE_BOOST * 2U;
  }
  else {
    idealAtkBoost += DEFIANT_BOOST * 2U;
  }

  REQUIRE(test.registry().get<AtkBoost>(entities.p1A).val == idealAtkBoost);
  REQUIRE(test.registry().get<SpaBoost>(entities.p1A).val == idealSpaBoost);
}

TEST_CASE(
  "Competitive/Defiant: Do not boost SPA/ATK when lowering is caused by an ally",
  "[Simulation][SimulateTurn][DoubleBattle][Ability][Competitive][Defiant]") {
  TestSimulation test{TestMechanic, BattleFormat::DOUBLES};
  dex::Ability p1AAbility = GENERATE(dex::Ability::COMPETITIVE, dex::Ability::DEFIANT);
  CAPTURE(p1AAbility);

  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, p1AAbility, dex::Move::CLOSE_COMBAT),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::FLASH_CANNON)),
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Ability::PLUS, dex::Move::SPLASH),
      test.pokemon(dex::Species::PANGORO, dex::Move::SPLASH)),
    test.turnDecision(
      dex::Move::CLOSE_COMBAT,
      MoveDecision{Slot::P1B, Slot::P1A, dex::Move::FLASH_CANNON},
      dex::Move::SPLASH,
      dex::Move::SPLASH));

  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::PercentChance::MIN);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp, stat::EffectiveDef, stat::EffectiveSpd, DefBoost, SpdBoost>(
    entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p1B);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
  test.checks.checkUsedMovePokemon(entities.p2B);

  REQUIRE(
    test.registry().get<DefBoost>(entities.p1A).val == dex::CloseCombat::sourcePrimaryEffect::defBoost(TestMechanic));
  REQUIRE(
    test.registry().get<SpdBoost>(entities.p1A).val ==
    dex::CloseCombat::sourcePrimaryEffect::defBoost(TestMechanic) +
      dex::FlashCannon::targetSecondaryEffect::spdBoost(TestMechanic));
}
}  // namespace pokesim
