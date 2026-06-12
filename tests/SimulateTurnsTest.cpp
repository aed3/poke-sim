#include "Tests.hpp"

namespace pokesim {
namespace {
using ActionQueueList = std::vector<ActionQueueItem>;

void runQueueOrderTest(
  const ActionQueueList& actionQueueList, const ActionQueueList& idealActionQueueList,
  const SpeedTieIndexes& idealSpeedTies = {}) {
  ActionQueue initialQueue{actionQueueList};

  types::registry registry;
  types::handle handle{registry, registry.create()};
  ActionQueue sortedQueue = initialQueue;

  simulate_turn::speedSort(handle, sortedQueue);

  REQUIRE(initialQueue.val.size() == sortedQueue.val.size());
  for (const ActionQueueItem& initialItem : initialQueue.val) {
    bool itemFound = false;
    for (const ActionQueueItem& sortedItem : sortedQueue.val) {
      if (sortedItem == initialItem) {
        itemFound = true;
        break;
      }
    }

    REQUIRE(itemFound);
  }

  for (std::size_t i = 0U; i < idealActionQueueList.size(); i++) {
    INFO(std::to_string(i));
    const ActionQueueItem& idealQueueItem = idealActionQueueList[i];
    const ActionQueueItem& trueQueueItem = sortedQueue.val[i];

    REQUIRE(trueQueueItem.order == idealQueueItem.order);
    REQUIRE(trueQueueItem.priority == idealQueueItem.priority);
    REQUIRE(trueQueueItem.fractionalPriority == idealQueueItem.fractionalPriority);
    REQUIRE(trueQueueItem.speed == idealQueueItem.speed);
  }

  if (idealSpeedTies.val.empty()) {
    REQUIRE_FALSE(handle.all_of<SpeedTieIndexes>());
    return;
  }

  REQUIRE(handle.all_of<SpeedTieIndexes>());
  const SpeedTieIndexes& trueSpeedTies = handle.get<SpeedTieIndexes>();

  REQUIRE(trueSpeedTies.val.size() == idealSpeedTies.val.size());
  for (const auto& idealSpeedTie : idealSpeedTies.val) {
    bool found =
      std::any_of(trueSpeedTies.val.begin(), trueSpeedTies.val.end(), [&idealSpeedTie](const auto& trueSpeedTie) {
        return trueSpeedTie.start == idealSpeedTie.start && trueSpeedTie.length == trueSpeedTie.length;
      });

    REQUIRE(found);
  }
};
}  // namespace

TEST_CASE("Simulate Turn: Action Queue Order", "[Simulation][SimulateTurn]") {
  SECTION("One Queue Item") {
    ActionQueueItem emptyQueueItem{};
    runQueueOrderTest({emptyQueueItem}, {emptyQueueItem});
  }

  SECTION("Two Identical Items") {
    ActionQueueItem emptyQueueItem{};
    runQueueOrderTest(
      {emptyQueueItem, emptyQueueItem},
      {emptyQueueItem, emptyQueueItem},
      SpeedTieIndexes{
        {SpeedTieIndexes::Span{0U, 2U}},
      });
  }

  SECTION("Sort By Order") {
    ActionQueueList idealList = {
      ActionQueueItem{ActionOrder::TEAM},
      ActionQueueItem{ActionOrder::START},
      ActionQueueItem{ActionOrder::BEFORE_TURN},
      ActionQueueItem{ActionOrder::SWITCH},
      ActionQueueItem{ActionOrder::MOVE},
      ActionQueueItem{ActionOrder::NONE},
    };

    runQueueOrderTest(
      {
        idealList[2],
        idealList[0],
        idealList[1],
        idealList[3],
        idealList[5],
        idealList[4],
      },
      idealList);
  }

  SECTION("Sort By Priority") {
    ActionQueueList idealList = {
      ActionQueueItem{ActionOrder::MOVE, 5},
      ActionQueueItem{ActionOrder::MOVE, 3},
      ActionQueueItem{ActionOrder::MOVE, 1},
      ActionQueueItem{ActionOrder::MOVE, 0},
      ActionQueueItem{ActionOrder::MOVE, -2},
      ActionQueueItem{ActionOrder::MOVE, -3},
      ActionQueueItem{ActionOrder::MOVE, -7},
    };

    runQueueOrderTest(
      {
        idealList[1],
        idealList[0],
        idealList[2],
        idealList[3],
        idealList[6],
        idealList[5],
        idealList[4],
      },
      idealList);
  }

  SECTION("Sort By Priority and Fractional Priority") {
    ActionQueueList idealList = {
      ActionQueueItem{ActionOrder::MOVE, 5, false},
      ActionQueueItem{ActionOrder::MOVE, 3, true},
      ActionQueueItem{ActionOrder::MOVE, 0, false},
      ActionQueueItem{ActionOrder::MOVE, 0, true},
      ActionQueueItem{ActionOrder::MOVE, -3, false},
      ActionQueueItem{ActionOrder::MOVE, -7, true},
    };

    runQueueOrderTest(
      {
        idealList[5],
        idealList[4],
        idealList[3],
        idealList[1],
        idealList[0],
        idealList[2],
      },
      idealList);
  }

  SECTION("Sort By Fractional Priority") {
    ActionQueueList idealList = {
      ActionQueueItem{ActionOrder::MOVE, 0, false},
      ActionQueueItem{ActionOrder::MOVE, 0, true},
    };

    runQueueOrderTest({idealList[1], idealList[0]}, idealList);
  }

  SECTION("Sort By Speed") {
    ActionQueueList idealList = {
      ActionQueueItem{ActionOrder::MOVE, 0, false, 772U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 621U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 584U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 444U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 305U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 152U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 90U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 11U},
    };

    runQueueOrderTest(
      {
        idealList[1],
        idealList[0],
        idealList[5],
        idealList[7],
        idealList[3],
        idealList[4],
        idealList[2],
        idealList[6],
      },
      idealList);
  }

  SECTION("Sort Combination") {
    ActionQueueList idealList = {
      ActionQueueItem{ActionOrder::TEAM},
      ActionQueueItem{ActionOrder::START},
      ActionQueueItem{ActionOrder::BEFORE_TURN, 0, false, 584U},
      ActionQueueItem{ActionOrder::BEFORE_TURN, 0, false, 444U},
      ActionQueueItem{ActionOrder::SWITCH, 0, false, 52U},
      ActionQueueItem{ActionOrder::SWITCH, 0, false, 40U},
      ActionQueueItem{ActionOrder::MOVE, 1, false, 152U},
      ActionQueueItem{ActionOrder::MOVE, 0, false, 315U},
      ActionQueueItem{ActionOrder::MOVE, -3, true, 700U},
    };

    runQueueOrderTest(
      {
        idealList[8],
        idealList[7],
        idealList[5],
        idealList[3],
        idealList[1],
        idealList[0],
        idealList[4],
        idealList[6],
        idealList[2],
      },
      idealList);
  }
}

TEST_CASE("Simulate Turn: Battle ends on faint", "[Simulation][SimulateTurn]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES);
  const types::registry& registry = simulation.registry;

  BattleCreationInfo battleCreationInfo;
  battleCreationInfo.sides = {
    {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON, true)}},
    {{createPredefinedPokemon(pokedex, dex::Species::AMPHAROS)}},
  };
  battleCreationInfo.sides.p2().team[0].item = dex::Item::NO_ITEM;
  battleCreationInfo.turn = 1U;
  pokedex.loadForBattleInfo({battleCreationInfo});

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  MoveDecision p1MoveDecision{Slot::P1A, Slot::P1A, dex::Move::SPLASH};
  MoveDecision p2MoveDecision{Slot::P2A, Slot::P1A, dex::Move::THUNDERBOLT};
  p1Decision.decisions = types::slotDecisions{p1MoveDecision};
  p2Decision.decisions = types::slotDecisions{p2MoveDecision};

  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
  simulation.createInitialStates({battleCreationInfo});
  auto& options = simulation.simulateTurnOptions;

  options.setDamageRollsConsidered({
    DamageRollKind::MAX_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
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

  updateAllStats(simulation);

  TestChecks checks{simulation, specificallyCheckEntities};
  auto result = simulation.simulateTurn();
  checks.checkRemainingOutputs();

  REQUIRE(result.turnOutcomeBattlesResults().size() == 1U);
  const auto& turnOutcomeBattles = std::get<1>(*result.turnOutcomeBattlesResults().each().begin()).val;
  REQUIRE(turnOutcomeBattles.size() == 1U);

  checks.checkViewForChanges<
    tags::Battle,
    Turn,
    simulate_turn::TurnOutcomeBattles,
    simulate_turn::tags::SpeedSortNeeded,
    ParentBattle,
    Winner,
    RootBattle>();

  checks.checkViewForChanges<tags::Side, SideDecision>();

  types::entity battle = turnOutcomeBattles[0];
  const auto& [turn, rootBattle, sides, winner] = registry.get<Turn, RootBattle, Sides, Winner>(battle);

  types::entity p1Side = sides.val.p1();
  types::entity p2Side = sides.val.p2();
  types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
  types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
  types::entity p1Move = registry.get<MoveSlots>(p1Pokemon).val[0];
  types::entity p2Move = registry.get<MoveSlots>(p2Pokemon).val[0];

  checks.checkEntityForChanges<stat::CurrentHp, tags::Fainted, tags::ActivePokemon>(p1Pokemon);
  checks.checkEntityForChanges<LastUsedMove>(p2Pokemon);

  auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
  REQUIRE(p2PokemonLastUsedMove.val == p2Move);

  auto p1PokemonHp = registry.get<stat::CurrentHp>(p1Pokemon);
  REQUIRE(p1PokemonHp.val == Constants::PokemonCurrentHpStat::MIN);
  REQUIRE(registry.all_of<tags::Fainted>(p1Pokemon));
  REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(p1Pokemon));

  checks.checkMovePpUsage(p1Move);
  checks.checkMovePpUsage(p2Move);

  REQUIRE(winner.val == PlayerSideId::P2);
}
}  // namespace pokesim
