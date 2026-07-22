
#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Choice Lock: Choice lock starts", "[Simulation][SimulateTurn][Effect][Volatile][ChoiceLock]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES);
  const types::registry& registry = simulation.registry;

  BattleCreationInfo battleCreationInfo;
  battleCreationInfo.sides = {
    {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON, true)}},
    {{createPredefinedPokemon(pokedex, dex::Species::RIBOMBEE, true)}},
  };
  battleCreationInfo.turn = 1U;
  battleCreationInfo.sides.p2().team[0].item = dex::Item::CHOICE_SPECS;
  pokedex.loadForBattleInfo({battleCreationInfo});

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  MoveDecision p1MoveDecision{Slot::P1A, Slot::P2A, dex::Move::SPLASH};
  MoveDecision p2MoveDecision{Slot::P2A, Slot::P2A, dex::Move::SPLASH};
  p1Decision.decisions = types::slotDecisions{p1MoveDecision};
  p2Decision.decisions = types::slotDecisions{p2MoveDecision};

  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
  simulation.createInitialStates({battleCreationInfo});
  auto& options = simulation.simulateTurnOptions;

  options.setApplyChangesToInputBattle(true);
  options.setMakeBranchesOnRandomEvents(true);

  types::entityVector specificallyCheckEntities;
  for (types::entity battle : registry.view<tags::Battle>()) {
    specificallyCheckEntities.push_back(battle);
  }
  for (types::entity pokemon : registry.view<tags::Pokemon>()) {
    specificallyCheckEntities.push_back(pokemon);
  }
  for (types::entity side : registry.view<tags::Side>()) {
    specificallyCheckEntities.push_back(side);
  }
  for (types::entity move : registry.view<MoveName>()) {
    specificallyCheckEntities.push_back(move);
  }

  TestChecks checks{simulation, specificallyCheckEntities};
  auto result = simulation.simulateTurn();
  checks.checkRemainingOutputs();

  REQUIRE(result.turnOutcomeBattlesResults().size() == 1U);
  const auto& turnOutcomeBattles = std::get<1>(*result.turnOutcomeBattlesResults().each().begin()).val;
  REQUIRE(turnOutcomeBattles.size() == 1U);

  checks.checkViewForChanges<
    tags::Battle,
    Turn,
    Probability,
    simulate_turn::TurnOutcomeBattles,
    simulate_turn::tags::SpeedSortNeeded,
    ParentBattle,
    RootBattle>();

  checks.checkViewForChanges<tags::Side, SideDecision>();

  types::entity battle = turnOutcomeBattles[0];
  const auto& [turn, rootBattle, sides] = registry.get<Turn, RootBattle, Sides>(battle);

  types::entity p1Side = sides.val.p1();
  types::entity p2Side = sides.val.p2();
  types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
  types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
  types::moveSlotIndex p1MoveIndex = 0U;
  types::moveSlotIndex p2MoveIndex = 0U;

  checks.checkEntityForChanges<LastUsedMove, MoveSlots>(p1Pokemon);
  checks.checkEntityForChanges<LastUsedMove, ChoiceLock, MoveSlots, DisabledMoveSlots>(p2Pokemon);

  auto p1PokemonLastUsedMove = registry.get<LastUsedMove>(p1Pokemon);
  REQUIRE(p1PokemonLastUsedMove.val == p1MoveIndex);

  auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
  REQUIRE(p2PokemonLastUsedMove.val == p2MoveIndex);

  auto choiceLock = registry.get<ChoiceLock>(p2Pokemon);
  REQUIRE(choiceLock.val == p2MoveIndex);

  auto disabledMoveSlots = registry.get<DisabledMoveSlots>(p2Pokemon);
  REQUIRE(disabledMoveSlots.val[p2MoveIndex] == true);

  checks.checkMovePpUsage(p1Pokemon, p1MoveIndex);
  checks.checkMovePpUsage(p2Pokemon, p2MoveIndex);
}

TEST_CASE(
  "Choice Lock: Choice lock ends when item is removed", "[Simulation][SimulateTurn][Effect][Volatile][ChoiceLock]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES);
  const types::registry& registry = simulation.registry;

  BattleCreationInfo battleCreationInfo;
  battleCreationInfo.sides = {
    {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON)}},
    {{createPredefinedPokemon(pokedex, dex::Species::RIBOMBEE, true)}},
  };
  battleCreationInfo.turn = 1U;
  battleCreationInfo.sides.p1().team[0].status = dex::Status::NO_STATUS;
  battleCreationInfo.sides.p2().team[0].item = dex::Item::CHOICE_SCARF;
  pokedex.loadForBattleInfo({battleCreationInfo});

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  MoveDecision p1MoveDecision{Slot::P1A, Slot::P2A, dex::Move::KNOCK_OFF};
  MoveDecision p2MoveDecision{Slot::P2A, Slot::P2A, dex::Move::SPLASH};
  p1Decision.decisions = types::slotDecisions{p1MoveDecision};
  p2Decision.decisions = types::slotDecisions{p2MoveDecision};

  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
  simulation.createInitialStates({battleCreationInfo});
  auto& options = simulation.simulateTurnOptions;

  options.setDamageRollsConsidered({
    DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
  });
  options.setApplyChangesToInputBattle(true);
  options.setMakeBranchesOnRandomEvents(true);

  types::entityVector specificallyCheckEntities;
  for (types::entity battle : registry.view<tags::Battle>()) {
    specificallyCheckEntities.push_back(battle);
  }
  for (types::entity pokemon : registry.view<tags::Pokemon>()) {
    specificallyCheckEntities.push_back(pokemon);
  }
  for (types::entity side : registry.view<tags::Side>()) {
    specificallyCheckEntities.push_back(side);
  }
  for (types::entity move : registry.view<MoveName>()) {
    specificallyCheckEntities.push_back(move);
  }

  TestChecks checks{simulation, specificallyCheckEntities};
  auto result = simulation.simulateTurn();
  checks.checkRemainingOutputs();

  REQUIRE(result.turnOutcomeBattlesResults().size() == 1U);
  const auto& turnOutcomeBattles = std::get<1>(*result.turnOutcomeBattlesResults().each().begin()).val;
  REQUIRE(turnOutcomeBattles.size() == 1U);

  checks.checkViewForChanges<
    tags::Battle,
    Turn,
    Probability,
    simulate_turn::TurnOutcomeBattles,
    simulate_turn::tags::SpeedSortNeeded,
    ParentBattle,
    RootBattle>();

  checks.checkViewForChanges<tags::Side, SideDecision>();

  types::entity battle = turnOutcomeBattles[0];
  const auto& [turn, rootBattle, sides] = registry.get<Turn, RootBattle, Sides>(battle);

  types::entity p1Side = sides.val.p1();
  types::entity p2Side = sides.val.p2();
  types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
  types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
  types::moveSlotIndex p1MoveIndex = 1U;
  types::moveSlotIndex p2MoveIndex = 0U;

  checks.checkEntityForChanges<LastUsedMove, MoveSlots>(p1Pokemon);
  checks.checkEntityForChanges<
    LastUsedMove,
    tags::HasItem,
    item::tags::ChoiceScarf,
    item::tags::Choice,
    stat::EffectiveSpe,
    stat::CurrentHp,
    MoveSlots>(p2Pokemon);

  auto p1PokemonLastUsedMove = registry.get<LastUsedMove>(p1Pokemon);
  REQUIRE(p1PokemonLastUsedMove.val == p1MoveIndex);

  auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
  REQUIRE(p2PokemonLastUsedMove.val == p2MoveIndex);

  REQUIRE_FALSE(registry.all_of<tags::HasItem>(p2Pokemon));
  REQUIRE_FALSE(registry.all_of<item::tags::ChoiceScarf>(p2Pokemon));
  REQUIRE_FALSE(registry.all_of<item::tags::Choice>(p2Pokemon));

  checks.checkMovePpUsage(p1Pokemon, p1MoveIndex);
  checks.checkMovePpUsage(p2Pokemon, p2MoveIndex);
}
}  // namespace pokesim
