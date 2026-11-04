#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Mirror Herb: Boost stats when foe boosts", "[Simulation][SimulateTurn][SingleBattle][Item][MirrorHerb]") {
  dex::Move move = GENERATE(
    dex::Move::SWORDS_DANCE,
    dex::Move::IRON_DEFENSE,
    dex::Move::NASTY_PLOT,
    dex::Move::AMNESIA,
    dex::Move::AGILITY,
    dex::Move::BULK_UP,
    dex::Move::QUIVER_DANCE,
    dex::Move::CLOSE_COMBAT);
  types::boost atkBoosts = 0, defBoosts = 0, spaBoosts = 0, spdBoosts = 0, speBoosts = 0;
  debug::TypesToIgnore changingP1Components, changingP2Components;
  changingP2Components.add<LastUsedMove, MoveSlots>();
  changingP1Components = changingP2Components;
  changingP1Components.add<tags::HasItem, dex::MirrorHerb>();

  switch (move) {
    case dex::Move::SWORDS_DANCE: {
      atkBoosts = 2;
      changingP1Components.add<stat::EffectiveAtk, AtkBoost>();
      changingP2Components.add<stat::EffectiveAtk, AtkBoost>();
      break;
    }
    case dex::Move::IRON_DEFENSE: {
      defBoosts = 2;
      changingP1Components.add<stat::EffectiveDef, DefBoost>();
      changingP2Components.add<stat::EffectiveDef, DefBoost>();
      break;
    }
    case dex::Move::NASTY_PLOT: {
      spaBoosts = 2;
      changingP1Components.add<stat::EffectiveSpa, SpaBoost>();
      changingP2Components.add<stat::EffectiveSpa, SpaBoost>();
      break;
    }
    case dex::Move::AMNESIA: {
      spdBoosts = 2;
      changingP1Components.add<stat::EffectiveSpd, SpdBoost>();
      changingP2Components.add<stat::EffectiveSpd, SpdBoost>();
      break;
    }
    case dex::Move::AGILITY: {
      speBoosts = 2;
      changingP1Components.add<stat::EffectiveSpe, SpeBoost>();
      changingP2Components.add<stat::EffectiveSpe, SpeBoost>();
      break;
    }
    case dex::Move::BULK_UP: {
      atkBoosts = defBoosts = 1;
      changingP1Components.add<stat::EffectiveAtk, AtkBoost, stat::EffectiveDef, DefBoost>();
      changingP2Components.add<stat::EffectiveAtk, AtkBoost, stat::EffectiveDef, DefBoost>();
      break;
    }
    case dex::Move::QUIVER_DANCE: {
      spaBoosts = spdBoosts = speBoosts = 1;
      changingP1Components
        .add<stat::EffectiveSpa, SpaBoost, stat::EffectiveSpd, SpdBoost, stat::EffectiveSpe, SpeBoost>();
      changingP2Components
        .add<stat::EffectiveSpa, SpaBoost, stat::EffectiveSpd, SpdBoost, stat::EffectiveSpe, SpeBoost>();
      break;
    }
    case dex::Move::CLOSE_COMBAT: {
      defBoosts = spdBoosts = -1;
      changingP1Components.add<stat::CurrentHp>();
      changingP2Components.add<stat::EffectiveDef, DefBoost, stat::EffectiveSpd, SpdBoost>();
      break;
    }
    default: FAIL();
  }
  CAPTURE(move, atkBoosts, defBoosts, spaBoosts, spdBoosts, speBoosts);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::MIRROR_HERB, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::AMPHAROS, move)),
    test.turnDecision(dex::Move::SPLASH, move));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkEntityForChanges(entities.p1A, changingP1Components);
  test.checks.checkEntityForChanges(entities.p2A, changingP2Components);

  types::registry& registry = test.registry();
  types::handle p1A{registry, entities.p1A};
  types::handle p2A{registry, entities.p2A};
  if (move == dex::Move::CLOSE_COMBAT) {
    REQUIRE(p1A.all_of<tags::HasItem>());
    REQUIRE(p1A.all_of<dex::MirrorHerb>());
  }
  else {
    REQUIRE_FALSE(p1A.all_of<tags::HasItem>());
    REQUIRE_FALSE(p1A.all_of<dex::MirrorHerb>());
  }

  if (atkBoosts) {
    REQUIRE(p1A.get<AtkBoost>().val == atkBoosts);
    REQUIRE(p2A.get<AtkBoost>().val == atkBoosts);
  }

  if (defBoosts) {
    REQUIRE(p2A.get<DefBoost>().val == defBoosts);
    if (defBoosts > 0) {
      REQUIRE(p1A.get<DefBoost>().val == defBoosts);
    }
  }

  if (spaBoosts) {
    REQUIRE(p1A.get<SpaBoost>().val == spaBoosts);
    REQUIRE(p2A.get<SpaBoost>().val == spaBoosts);
  }

  if (spdBoosts) {
    REQUIRE(p2A.get<SpdBoost>().val == spdBoosts);
    if (spdBoosts > 0) {
      REQUIRE(p1A.get<SpdBoost>().val == spdBoosts);
    }
  }

  if (speBoosts) {
    REQUIRE(p1A.get<SpeBoost>().val == speBoosts);
    REQUIRE(p2A.get<SpeBoost>().val == speBoosts);
  }
}

TEST_CASE(
  "Mirror Herb: Item doesn't cause other item holders to boost",
  "[Simulation][SimulateTurn][DoubleBattle][Item][MirrorHerb]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Item::MIRROR_HERB, dex::Move::SWORDS_DANCE),
      test.pokemon(dex::Species::GARDEVOIR, dex::Item::MIRROR_HERB, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Item::MIRROR_HERB, dex::Move::SPLASH),
      test.pokemon(dex::Species::PANGORO, dex::Item::MIRROR_HERB, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SWORDS_DANCE, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p1B);
  test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk, tags::HasItem, dex::MirrorHerb>(entities.p2A);
  test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk, tags::HasItem, dex::MirrorHerb>(entities.p2B);

  REQUIRE(test.registry().get<AtkBoost>(entities.p1A).val == 2);
  REQUIRE(test.registry().get<AtkBoost>(entities.p2A).val == 2);
  REQUIRE(test.registry().get<AtkBoost>(entities.p2B).val == 2);
}

TEST_CASE("Mirror Herb: Handles speed ties", "[Simulation][SimulateTurn][DoubleBattle][Item][MirrorHerb]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::MIRROR_HERB, dex::Move::SWORDS_DANCE)),
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Item::MIRROR_HERB, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Item::MIRROR_HERB, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SWORDS_DANCE, TestSimulation::SkipDecision{}, dex::Move::SPLASH, dex::Move::SPLASH));

  auto battleEntitiesList = test.simulateOneBranchingBattle();
  REQUIRE(battleEntitiesList.size() == 4U);  // The normal speed tie and the Mirror Herb speed tie
  for (auto entities : battleEntitiesList) {
    test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p1A);
    test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk, tags::HasItem, dex::MirrorHerb>(entities.p2A);
    test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk, tags::HasItem, dex::MirrorHerb>(entities.p2B);

    REQUIRE(test.registry().get<AtkBoost>(entities.p1A).val == 2);
    REQUIRE(test.registry().get<AtkBoost>(entities.p2A).val == 2);
    REQUIRE(test.registry().get<AtkBoost>(entities.p2B).val == 2);
  }
}
}  // namespace pokesim
