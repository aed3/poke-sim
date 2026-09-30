#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Imposter: Transform on switch in", "[Simulation][SimulateTurn][SingleBattle][Ability][Imposter][Transform]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPLASH),
      test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.turnDecision(Slot::P1B, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  REQUIRE(test.registry().all_of<TransformedFrom>(entities.p1A));
  REQUIRE(test.registry().get<SpeciesName>(entities.p1A).val == dex::Species::EMPOLEON);
  test.checks.checkUsedMovePokemon(entities.p2A);
};

TEST_CASE(
  "Imposter: Transform to slot across", "[Simulation][SimulateTurn][DoubleBattle][Ability][Imposter][Transform]") {
  bool switchP2A = GENERATE(true, false);
  CAPTURE(switchP2A);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  BattleCreationInfo& battleInfo = test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPLASH),
      test.pokemon(dex::Species::CLAYDOL, dex::Move::SPLASH),
      test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH)));

  if (switchP2A) {
    battleInfo.decisionsToSimulate.push_back(
      test.turnDecision(Slot::P1C, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));
  }
  else {
    battleInfo.decisionsToSimulate.push_back(
      test.turnDecision(dex::Move::SPLASH, Slot::P1C, dex::Move::SPLASH, dex::Move::SPLASH));
  }

  battleInfo.runWithSimulateTurn = true;
  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1C);
  test.checks.checkUsedMovePokemon(entities.p2A);
  test.checks.checkUsedMovePokemon(entities.p2B);

  if (switchP2A) {
    REQUIRE(test.registry().all_of<TransformedFrom>(entities.p1A));
    REQUIRE(test.registry().get<SpeciesName>(entities.p1A).val == dex::Species::EMPOLEON);
    test.checks.checkUsedMovePokemon(entities.p1B);
  }
  else {
    REQUIRE(test.registry().all_of<TransformedFrom>(entities.p1B));
    REQUIRE(test.registry().get<SpeciesName>(entities.p1B).val == dex::Species::AMPHAROS);
    test.checks.checkUsedMovePokemon(entities.p1A);
  }
};

TEST_CASE(
  "Imposter: Do not transform when slot across is empty",
  "[Simulation][SimulateTurn][DoubleBattle][Ability][Imposter][Transform]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPLASH),
      test.pokemon(dex::Species::CLAYDOL, dex::Move::SPLASH),
      test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, Slot::P1C, dex::Move::SPLASH, TestSimulation::SkipDecision{}));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1B);
  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1C);
  test.checks.checkUsedMovePokemon(entities.p2A);
};

TEST_CASE(
  "Imposter: Do not transform when slot across is not active",
  "[Simulation][SimulateTurn][DoubleBattle][Ability][Imposter][Transform]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPLASH),
      test.pokemon(dex::Species::CLAYDOL, dex::Move::SPLASH),
      test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, stat::CurrentHp{Constants::PokemonCurrentHpStat::MIN}, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH)),
    test.turnDecision(Slot::P1C, dex::Move::SPLASH, TestSimulation::SkipDecision{}, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p1B);
  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1C);
  test.checks.checkEntityForChanges(entities.p2A);
  test.checks.checkUsedMovePokemon(entities.p2B);
};
}  // namespace pokesim
