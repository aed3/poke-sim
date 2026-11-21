#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("King's Shield: Does not block status moves", "[Simulation][SimulateTurn][SingleBattle][Move][KingsShield]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::AEGISLASH, dex::Move::KINGS_SHIELD)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::GLARE)),
    test.turnDecision(dex::Move::KINGS_SHIELD, dex::Move::GLARE));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<dex::Paralysis, tags::HasStatus, dex::Stall, stat::EffectiveSpe>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  types::registry& registry = test.registry();
  REQUIRE(registry.all_of<dex::Paralysis>(entities.p1A));
  REQUIRE(registry.all_of<tags::HasStatus>(entities.p1A));
  dex::Stall stall = registry.get<dex::Stall>(entities.p1A);
  REQUIRE(stall.uses == 1U);
  REQUIRE(stall.turnsRemaining == 1U);
}

TEST_CASE("King's Shield: Blocks attacking moves", "[Simulation][SimulateTurn][SingleBattle][Move][KingsShield]") {
  dex::Move attackingMove = GENERATE(
    dex::Move::LEAFAGE,
    dex::Move::MOONBLAST,
    dex::Move::MUD_SLAP,
    dex::Move::SPIRIT_SHACKLE,
    dex::Move::ZAP_CANNON);
  CAPTURE(attackingMove);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::AEGISLASH, dex::Move::KINGS_SHIELD)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, attackingMove)),
    test.turnDecision(dex::Move::KINGS_SHIELD, attackingMove));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<dex::Stall>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  types::registry& registry = test.registry();
  dex::Stall stall = registry.get<dex::Stall>(entities.p1A);
  REQUIRE(stall.uses == 1U);
  REQUIRE(stall.turnsRemaining == 1U);
}

TEST_CASE(
  "King's Shield: Blocks contact moves and drops attacker's attack",
  "[Simulation][SimulateTurn][SingleBattle][Move][KingsShield]") {
  dex::Move attackingMove = GENERATE(
    dex::Move::CLOSE_COMBAT,
    dex::Move::KNOCK_OFF,
    dex::Move::FURY_ATTACK,
    dex::Move::QUICK_ATTACK,
    dex::Move::TACKLE);
  dex::Ability ability = GENERATE(dex::Ability::IRON_FIST, dex::Ability::SCRAPPY);
  CAPTURE(attackingMove, ability);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::AEGISLASH, dex::Move::KINGS_SHIELD)),
    test.side(test.pokemon(dex::Species::PANGORO, ability, attackingMove)),
    test.turnDecision(dex::Move::KINGS_SHIELD, attackingMove));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<dex::Stall>(entities.p1A);
  test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p2A);

  types::registry& registry = test.registry();
  dex::Stall stall = registry.get<dex::Stall>(entities.p1A);
  REQUIRE(stall.uses == 1U);
  REQUIRE(stall.turnsRemaining == 1U);

  AtkBoost atkBoost = registry.get<AtkBoost>(entities.p2A);
  REQUIRE(atkBoost.val == -1);
}

TEST_CASE(
  "Stall: Has progressively less chance to work when used successfully",
  "[Simulation][SimulateTurn][SingleBattle][Effect][Stall]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::AEGISLASH, dex::Move::KINGS_SHIELD, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::KINGS_SHIELD, dex::Move::SPLASH));
  types::registry& registry = test.registry();

  types::probability idealProbability = Constants::Probability::MAX;
  types::entity keptBattle = test.simulateOneNonBranchingBattle().battle;

  for (types::stallUses uses = 1U; uses <= Constants::StallMoveUses::MAX; uses++) {
    CAPTURE(uses);

    auto turnDecision = test.turnDecision(dex::Move::KINGS_SHIELD, dex::Move::SPLASH);
    test.applyDecision(keptBattle, turnDecision);
    auto battleEntitiesList = test.simulateOneBranchingBattle();
    REQUIRE(battleEntitiesList.size() == 2U);

    TestSimulation::BattleEntities stallPassed{entt::null}, stallFailed{entt::null};
    for (auto entities : battleEntitiesList) {
      test.checks.checkUsedMovePokemon<dex::Stall>(entities.p1A);
      test.checks.checkUsedMovePokemon(entities.p2A);

      dex::Stall* stall = registry.try_get<dex::Stall>(entities.p1A);
      if (stall) {
        stallPassed = entities;
        REQUIRE(stall->uses == std::min((types::stallUses)(uses + 1U), Constants::StallMoveUses::MAX));
        REQUIRE(stall->turnsRemaining == 1U);
      }
      else {
        stallFailed = entities;
      }
    }

    REQUIRE_FALSE((stallPassed.battle == entt::null));
    REQUIRE_FALSE((stallFailed.battle == entt::null));
    types::probability succeedProbability = 1.0F;
    switch (uses) {
      case 1U: succeedProbability /= 3U; break;
      case 2U: succeedProbability /= 9U; break;
      case 3U: succeedProbability /= 27U; break;
      case 4U: succeedProbability /= 81U; break;
      case 5U: succeedProbability /= 243U; break;
      case 6U:
      case 7U: succeedProbability /= 729U; break;
      default: FAIL();
    }

    REQUIRE_THAT(
      registry.get<Probability>(stallPassed.battle).val,
      Catch::Matchers::WithinRel(idealProbability * succeedProbability));
    REQUIRE_THAT(
      registry.get<Probability>(stallFailed.battle).val,
      Catch::Matchers::WithinRel(idealProbability * (Constants::Probability::MAX - succeedProbability)));

    idealProbability *= succeedProbability;
    keptBattle = stallPassed.battle;
    registry.remove<tags::SimulateTurn>(stallFailed.battle);
  }

  test.applyDecision(keptBattle, test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));
  auto entities = test.simulateOneNonBranchingBattle();
  REQUIRE_FALSE(registry.all_of<dex::Stall>(entities.p1A));
}
}  // namespace pokesim
