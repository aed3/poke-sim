#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Volt Switch: Request switch after successful hit",
  "[Simulation][SimulateTurn][SingleBattle][Move][VoltSwitch][SelfSwitch][Switching]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Move::VOLT_SWITCH),
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::VOLT_SWITCH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle(
    Tags<tags::BattleMidTurn, MidTurnDecisionsRequested, ActionQueue>{},
    Tags<MidTurnDecisionsRequested>{});

  types::entity ampharos = entities.p1A;
  types::entity empoleon = entities.p1B;
  types::entity gardevoir = entities.p2A;

  types::registry& registry = test.registry();
  test.checks.checkUsedMovePokemon<tags::Switching, tags::RequestingMidTurnDecision>(ampharos);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(gardevoir);
  test.checks.checkEntityForChanges(empoleon);

  auto [initialP2Hp, currentP2Hp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(gardevoir);
  REQUIRE(currentP2Hp.val < initialP2Hp.val);

  REQUIRE(registry.all_of<tags::Switching>(ampharos));
  REQUIRE(registry.all_of<tags::RequestingMidTurnDecision>(ampharos));
  REQUIRE(registry.get<MidTurnDecisionsRequested>(entities.p1Side).val == 1U);

  REQUIRE(registry.get<MidTurnDecisionsRequested>(entities.battle).val == 1U);
  REQUIRE(registry.all_of<tags::BattleMidTurn>(entities.battle));
  REQUIRE(registry.get<ActionQueue>(entities.battle).val.size() == 1U);

  test.checks.reset();
  simulate_turn::setSideOptions(test.simulation);

  test.checks.checkViewForChanges<tags::Pokemon>();
  test.checks.checkViewForChanges<tags::Battle>();
  test.checks.checkEntityForChanges<SwitchOptions>(entities.p1Side);
  test.checks.checkEntityForChanges(entities.p2Side);

  SwitchOptions switchOptions = registry.get<SwitchOptions>(entities.p1Side);
  REQUIRE(switchOptions.val.size() == 1U);
  REQUIRE(switchOptions.val[0] == Slot::P1B);

  registry.emplace<MidTurnSideDecision>(entities.p1Side, MidTurnSideDecision{{{Slot::P1A, Slot::P1B}}});
  test.simulateOneBranchingBattle(
    Tags<tags::BattleMidTurn, MidTurnDecisionsRequested, ActionQueue>{},
    Tags<MidTurnDecisionsRequested, MidTurnSideDecision, Team>{});

  test.checks.checkEntityForChanges<
    tags::Switching,
    LastUsedMove,
    tags::RequestingMidTurnDecision,
    pokesim::tags::ActivePokemon>(ampharos);
  test.checks.checkEntityForChanges<pokesim::tags::ActivePokemon>(empoleon);
  test.checks.checkEntityForChanges(gardevoir);

  REQUIRE_FALSE(registry.all_of<tags::Switching>(ampharos));
  REQUIRE_FALSE(registry.all_of<LastUsedMove>(ampharos));
  REQUIRE_FALSE(registry.all_of<tags::RequestingMidTurnDecision>(ampharos));
  REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(ampharos));
  REQUIRE(registry.all_of<tags::ActivePokemon>(empoleon));

  REQUIRE_FALSE(registry.all_of<MidTurnDecisionsRequested>(entities.p1Side));
  REQUIRE_FALSE(registry.all_of<MidTurnSideDecision>(entities.p1Side));
  const Team& p1Team = registry.get<Team>(entities.p1Side);
  REQUIRE(p1Team.val[0] == empoleon);
  REQUIRE(p1Team.val[1] == ampharos);

  REQUIRE_FALSE(registry.all_of<MidTurnDecisionsRequested>(entities.battle));
  REQUIRE_FALSE(registry.all_of<tags::BattleMidTurn>(entities.battle));
  REQUIRE(registry.get<ActionQueue>(entities.battle).val.empty());
};
}  // namespace pokesim
