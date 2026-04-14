#include "Tests.hpp"

namespace pokesim {
namespace {
using SpeedSortList = std::vector<SpeedSort>;

void runSpeedSortTest(
  const SpeedSortList& speedSortList, const SpeedSortList& idealSortedList,
  const SpeedTieIndexes& idealSpeedTies = {}) {
  types::registry registry;
  ActionQueue initialQueue;

  for (const SpeedSort& speedSort : speedSortList) {
    types::entity entity = registry.create();
    registry.emplace<SpeedSort>(entity, speedSort);
    initialQueue.val.push_back(entity);
  }

  types::handle handle{registry, registry.create()};
  ActionQueue sortedQueue = initialQueue;

  simulate_turn::speedSort(handle, sortedQueue);

  REQUIRE(initialQueue.val.size() == sortedQueue.val.size());
  for (types::entity entity : initialQueue.val) {
    bool entityFound = false;
    for (types::entity sortedEntity : sortedQueue.val) {
      if (sortedEntity == entity) {
        entityFound = true;
        break;
      }
    }

    REQUIRE(entityFound);
  }

  for (std::size_t i = 0U; i < idealSortedList.size(); i++) {
    INFO(std::to_string(i));
    const SpeedSort& idealSpeedSort = idealSortedList[i];
    const SpeedSort& trueSpeedSort = registry.get<SpeedSort>(sortedQueue.val[i]);

    REQUIRE(trueSpeedSort.order == idealSpeedSort.order);
    REQUIRE(trueSpeedSort.priority == idealSpeedSort.priority);
    REQUIRE(trueSpeedSort.fractionalPriority == idealSpeedSort.fractionalPriority);
    REQUIRE(trueSpeedSort.speed == idealSpeedSort.speed);
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

TEST_CASE("Simulate Turn: SpeedSort", "[Simulation][SimulateTurn]") {
  SECTION("One Queue Item") {
    SpeedSort emptySpeedSort{};
    runSpeedSortTest({emptySpeedSort}, {emptySpeedSort});
  }

  SECTION("Two Identical Items") {
    SpeedSort emptySpeedSort{};
    runSpeedSortTest(
      {emptySpeedSort, emptySpeedSort},
      {emptySpeedSort, emptySpeedSort},
      SpeedTieIndexes{
        {SpeedTieIndexes::Span{0U, 2U}},
      });
  }

  SECTION("Sort By Order") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::TEAM},
      SpeedSort{ActionOrder::START},
      SpeedSort{ActionOrder::BEFORE_TURN},
      SpeedSort{ActionOrder::SWITCH},
      SpeedSort{ActionOrder::MOVE},
      SpeedSort{ActionOrder::NONE},
    };

    runSpeedSortTest(
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
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 5},
      SpeedSort{ActionOrder::MOVE, 3},
      SpeedSort{ActionOrder::MOVE, 1},
      SpeedSort{ActionOrder::MOVE, 0},
      SpeedSort{ActionOrder::MOVE, -2},
      SpeedSort{ActionOrder::MOVE, -3},
      SpeedSort{ActionOrder::MOVE, -7},
    };

    runSpeedSortTest(
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
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 5, false},
      SpeedSort{ActionOrder::MOVE, 3, true},
      SpeedSort{ActionOrder::MOVE, 0, false},
      SpeedSort{ActionOrder::MOVE, 0, true},
      SpeedSort{ActionOrder::MOVE, -3, false},
      SpeedSort{ActionOrder::MOVE, -7, true},
    };

    runSpeedSortTest(
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
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 0, false},
      SpeedSort{ActionOrder::MOVE, 0, true},
    };

    runSpeedSortTest({idealList[1], idealList[0]}, idealList);
  }

  SECTION("Sort By Speed") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 0, false, 772U},
      SpeedSort{ActionOrder::MOVE, 0, false, 621U},
      SpeedSort{ActionOrder::MOVE, 0, false, 584U},
      SpeedSort{ActionOrder::MOVE, 0, false, 444U},
      SpeedSort{ActionOrder::MOVE, 0, false, 305U},
      SpeedSort{ActionOrder::MOVE, 0, false, 152U},
      SpeedSort{ActionOrder::MOVE, 0, false, 90U},
      SpeedSort{ActionOrder::MOVE, 0, false, 11U},
    };

    runSpeedSortTest(
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
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::TEAM},
      SpeedSort{ActionOrder::START},
      SpeedSort{ActionOrder::BEFORE_TURN, 0, false, 584U},
      SpeedSort{ActionOrder::BEFORE_TURN, 0, false, 444U},
      SpeedSort{ActionOrder::SWITCH, 0, false, 52U},
      SpeedSort{ActionOrder::SWITCH, 0, false, 40U},
      SpeedSort{ActionOrder::MOVE, 1, false, 152U},
      SpeedSort{ActionOrder::MOVE, 0, false, 315U},
      SpeedSort{ActionOrder::MOVE, -3, true, 700U},
    };

    runSpeedSortTest(
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
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  const types::registry& registry = simulation.registry;

  Simulation::BattleCreationInfo battleCreationInfo;
  battleCreationInfo.p1 = {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON, true)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(pokedex, dex::Species::AMPHAROS)}};
  battleCreationInfo.p2.team[0].item = dex::Item::NO_ITEM;
  battleCreationInfo.turn = 1U;
  loadPokedexForBattleInfo(battleCreationInfo, pokedex);

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P1A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};
  p1SlotDecision.moveChoice = dex::Move::SPLASH;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::THUNDERBOLT;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};

  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
  simulation.createInitialStates({battleCreationInfo});
  auto& options = simulation.simulateTurnOptions;

  options.damageRollsConsidered = {
    DamageRollKind::MAX_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
    DamageRollKind::MAX_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
  };
  options.applyChangesToInputBattle = true;
  options.makeBranchesOnRandomEvents = true;

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

  types::entity p1Side = sides.p1();
  types::entity p2Side = sides.p2();
  types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
  types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
  types::entity p1Move = registry.get<MoveSlots>(p1Pokemon).val[0];
  types::entity p2Move = registry.get<MoveSlots>(p2Pokemon).val[0];

  checks.checkEntityForChanges<stat::CurrentHp, tags::Fainted, tags::ActivePokemon>(p1Pokemon);
  checks.checkEntityForChanges<LastUsedMove>(p2Pokemon);

  auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
  REQUIRE(p2PokemonLastUsedMove.val == p2Move);

  auto p1PokemonHp = registry.get<stat::CurrentHp>(p1Pokemon);
  REQUIRE(p1PokemonHp.val == MechanicConstants::PokemonCurrentHpStat::MIN);
  REQUIRE(registry.all_of<tags::Fainted>(p1Pokemon));
  REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(p1Pokemon));

  checks.checkMovePpUsage(p1Move);
  checks.checkMovePpUsage(p2Move);

  REQUIRE(winner.val == PlayerSideId::P2);
}
}  // namespace pokesim
