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

  internal::simulate_turn::speedSort(handle, sortedQueue);

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

TEST_CASE("Action Queue Order", "[Simulation][SimulateTurn][ActionQueue]") {
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

  SECTION("Speed Tie Order Uniqueness") {
    static constexpr types::activePokemonIndex MAX_ACTIVE = Constants::ActivePokemon::MAX;
    types::activePokemonIndex speedTieCount = GENERATE(range(2U, MAX_ACTIVE + 1U));
    types::activePokemonIndex speedTieStart = MAX_ACTIVE - speedTieCount;
    SpeedTieIndexes speedTies{{{speedTieStart, speedTieCount}}};
    ActionQueue originalQueue;

    for (types::activePokemonIndex i = 0U; i < MAX_ACTIVE; i++) {
      ActionQueueItem& item = originalQueue.val.emplace_back(ActionQueueItem{ActionOrder::MOVE});
      item.decision = MoveDecision{Slot::P1A, Slot::P1A, (dex::Move)i};
    }

    types::eventPossibilities possibleOrders = 1U;
    for (types::eventPossibilities i = 2U; i <= speedTieCount; i++) {
      possibleOrders *= i;
    }

    entt::dense_set<std::uint32_t> foundOrders;
    for (types::eventPossibilities randomEventIndex = 0U; randomEventIndex < possibleOrders; randomEventIndex++) {
      CAPTURE(speedTieCount, possibleOrders, randomEventIndex);

      ActionQueue queue = originalQueue;
      internal::simulate_turn::setSpeedTieOrder(queue, speedTies, {randomEventIndex});

      std::uint32_t foundOrder = 0;
      for (types::activePokemonIndex i = 0U; i < MAX_ACTIVE; i++) {
        auto originalIndex = (types::activePokemonIndex)queue.val[i].decision.get<MoveDecision>().move;
        if (i < speedTieStart) {
          REQUIRE(i == originalIndex);
        }
        foundOrder += originalIndex << i * 8U;
      }

      REQUIRE_FALSE(foundOrders.contains(foundOrder));
      foundOrders.insert(foundOrder);
    }
  }
}

TEST_CASE("Basic Switching", "[Simulation][SimulateTurn][SingleBattle][Switching]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH),
      test.pokemon(dex::Species::PANGORO, dex::Move::SPLASH)),
    test.turnDecision(Slot::P1B, Slot::P2B));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  test.checks.checkViewForChanges<tags::Pokemon, tags::ActivePokemon>();

  for (types::entity side : {entities.p1Side, entities.p2Side}) {
    const auto& [initialTeam, currentTeam] = test.checks.getInitialAndCurrent<Team>(side);

    REQUIRE(currentTeam.val[0] == initialTeam.val[1]);
    REQUIRE(currentTeam.val[1] == initialTeam.val[0]);
    REQUIRE(test.registry().all_of<tags::ActivePokemon>(currentTeam.val[0]));
    REQUIRE_FALSE(test.registry().all_of<tags::ActivePokemon>(currentTeam.val[1]));
  }
}

TEST_CASE("Battle ends on faint", "[Simulation][SimulateTurn][SingleBattle][Fainting]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::THUNDERBOLT)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::THUNDERBOLT));

  test.simulateTurnOptions()
    .setDamageRollsConsidered({
      DamageRollKind::MAX_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
    })
    .setMakeBranchesOnRandomEvents(true);

  auto entities = test.simulateOneNonBranchingBattle(Tags<Winner>{}, Tags<FoesRemaining>{});
  const types::registry& registry = test.registry();

  const auto& [turn, winner] = registry.get<Turn, Winner>(entities.battle);

  const FoesRemaining& p2Remaining = registry.get<FoesRemaining>(entities.p1Side);
  const FoesRemaining& p1Remaining = registry.get<FoesRemaining>(entities.p2Side);
  REQUIRE(p2Remaining.val == 1U);
  REQUIRE(p1Remaining.val == 0U);

  test.checks.checkEntityForChanges<stat::CurrentHp, tags::Fainted, tags::ActivePokemon, MoveSlots>(entities.p1A);
  test.checks.checkMovePpUsage(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  auto p1PokemonHp = registry.get<stat::CurrentHp>(entities.p1A);
  REQUIRE(p1PokemonHp.val == Constants::PokemonCurrentHpStat::MIN);
  REQUIRE(registry.all_of<tags::Fainted>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(entities.p1A));

  REQUIRE(winner.val == PlayerSideId::P2);
  REQUIRE(turn.val == 1U);
}

TEST_CASE("Speed Ties", "[Simulation][SimulateTurn][DoubleBattle][ActionQueue]") {
  types::activePokemonIndex speedTieCount = GENERATE(range(2U, 5U));
  types::probability branchProbabilityLimit = GENERATE(0.0F, 0.05F, 0.2F, 0.5F, 1.0F);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH, test.ivs({{dex::Stat::SPE, 31U}})),
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH, test.ivs({{dex::Stat::SPE, 31U}}))),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH, test.ivs({{dex::Stat::SPE, 31U}})),
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH, test.ivs({{dex::Stat::SPE, 31U}}))),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));

  if (speedTieCount < 4U) {
    test.getPokemonCreationInfo(Slot::P2B).ivs.spe = 20U;
  }
  if (speedTieCount < 3U) {
    test.getPokemonCreationInfo(Slot::P1B).ivs.spe = 10U;
  }

  test.simulateTurnOptions()
    .setApplyChangesToInputBattle(true)
    .setMakeBranchesOnRandomEvents(true)
    .setBranchProbabilityLowerLimit(branchProbabilityLimit);

  types::eventPossibilities idealCloneCount = 1U;
  for (types::eventPossibilities i = speedTieCount; i > 1U; i--) {
    idealCloneCount *= i;
  }
  if (branchProbabilityLimit >= Constants::Probability::MAX / idealCloneCount) {
    idealCloneCount = 1U;
  }
  CAPTURE(speedTieCount, branchProbabilityLimit, idealCloneCount);

  test.initializeSimulation();
  SECTION("Check Action Queue Orders") {
    test.simulation.view<internal::simulate_turn::resolveDecision>();
    test.simulation.view<internal::simulate_turn::speedSort>();
    internal::simulate_turn::resolveSpeedTies(test.simulation);

    auto trueCloneCount = test.registry().view<tags::Battle>()->size();
    REQUIRE(trueCloneCount == idealCloneCount);
    auto actionQueueView = test.registry().view<ActionQueue>();

    entt::dense_set<std::uint32_t> foundOrders;
    for (types::entity entity : actionQueueView) {
      const auto& [queue] = actionQueueView.get(entity);

      std::uint32_t foundOrder = 0U;
      types::stat lastSpeed = Constants::PokemonStat::MAX;
      for (types::activePokemonIndex i = 0U; i < queue.val.size(); i++) {
        const ActionQueueItem& item = queue.val[i];
        if (i != 0U && i < speedTieCount) {
          REQUIRE(lastSpeed == item.speed);
        }
        else {
          REQUIRE(lastSpeed > item.speed);
        }

        foundOrder += (std::uint8_t)item.decision.sourceSlot() << i * 8U;
        lastSpeed = item.speed;
      }

      REQUIRE_FALSE(foundOrders.contains(foundOrder));
      foundOrders.insert(foundOrder);
    }
  }

  SECTION("Check Entire Simulation Branching") {
    simulate_turn::Results results = test.simulateOneBattle(Tags<Probability>{});
    auto trueCloneCount = results.battleOutcomes().size();
    REQUIRE(trueCloneCount == idealCloneCount);
  }
}

TEST_CASE("Decisions", "[Simulation][SimulateTurn][ActionQueue]") {
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  bool teamPreview = GENERATE(false, true);
  CAPTURE(battleFormat, teamPreview);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, battleFormat};
  test.setupBattle(
    Turn{(types::battleTurn)(teamPreview ? 0U : 1U)},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::FURY_ATTACK, dex::Move::FLASH_CANNON),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::MOONBLAST),
      test.pokemon(dex::Species::DRAGAPULT, dex::Move::WILL_O_WISP)),
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Move::THUNDERBOLT),
      test.pokemon(dex::Species::PANGORO, dex::Move::KNOCK_OFF),
      test.pokemon(dex::Species::RIBOMBEE, dex::Move::QUIVER_DANCE)));

  auto [p1A, p1B, p1C, p2A, p2B, p2C] =
    test.getPokemonCreationInfo(Slot::P1A, Slot::P1B, Slot::P1C, Slot::P2A, Slot::P2B, Slot::P2C);

  types::handle p1Side, p2Side;
  auto setSideHandles = [&]() {
    test.initializeSimulation();
    types::registry& registry = test.registry();

    types::entity battleEntity = *registry.view<tags::Battle>().each().begin().base();
    auto sides = registry.view<Sides>().get<Sides>(battleEntity).val;
    p1Side = {registry, sides.p1()};
    p2Side = {registry, sides.p2()};
  };

  setSideHandles();

  if (teamPreview) {
    simulate_turn::setTeamPreviewOptions(test.simulation);
    REQUIRE(p1Side.get<TeamPreviewOptions>().val == types::teamOrder{0U, 1U, 2U});
    REQUIRE(p2Side.get<TeamPreviewOptions>().val == types::teamOrder{0U, 1U, 2U});

    p1A.currentHp = Constants::PokemonCurrentHpStat::MIN;
    p2B.currentHp = Constants::PokemonCurrentHpStat::MIN;

    setSideHandles();

    simulate_turn::setTeamPreviewOptions(test.simulation);
    REQUIRE(p1Side.get<TeamPreviewOptions>().val == types::teamOrder{1U, 2U});
    REQUIRE(p2Side.get<TeamPreviewOptions>().val == types::teamOrder{0U, 2U});
  }
  else {
    simulate_turn::setSideOptions(test.simulation);

    if (battleFormat == BattleFormat::SINGLES) {
      SinglesSideOptions p1Options = p1Side.get<SinglesSideOptions>();
      SinglesSideOptions p2Options = p2Side.get<SinglesSideOptions>();

      REQUIRE(p1Options.switches.val == types::teamPositions<Slot>{Slot::P1B, Slot::P1C});
      REQUIRE_THAT(
        p1Options.moves,
        Catch::Matchers::UnorderedRangeEquals(types::moveSlots<SinglesMoveOption>{
          {dex::Move::FLASH_CANNON, Slot::P2A},
          {dex::Move::FURY_ATTACK, Slot::P2A},
        }));

      REQUIRE(p2Options.switches.val == types::teamPositions<Slot>{Slot::P2B, Slot::P2C});
      REQUIRE_THAT(
        p2Options.moves,
        Catch::Matchers::RangeEquals(types::moveSlots<SinglesMoveOption>{
          {dex::Move::THUNDERBOLT, Slot::P1A},
        }));
    }
    else {
      DoublesSideOptions p1Options = p1Side.get<DoublesSideOptions>();
      DoublesSideOptions p2Options = p2Side.get<DoublesSideOptions>();
      for (types::activePokemonIndex i = 0U; i < Constants::ActivePokemonSlotsPerSide::DOUBLES; i++) {
        REQUIRE(p1Options.switches[i].val == types::teamPositions<Slot>{Slot::P1C});
        REQUIRE(p2Options.switches[i].val == types::teamPositions<Slot>{Slot::P2C});
      }

      REQUIRE_THAT(
        p1Options.moves[0U],
        Catch::Matchers::UnorderedRangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::FURY_ATTACK, {Slot::P2A, Slot::P2B}},
          {dex::Move::FLASH_CANNON, {Slot::P2A, Slot::P2B}},
        }));
      REQUIRE_THAT(
        p1Options.moves[1U],
        Catch::Matchers::RangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::MOONBLAST, {Slot::P2A, Slot::P2B}},
        }));

      REQUIRE_THAT(
        p2Options.moves[0U],
        Catch::Matchers::RangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::THUNDERBOLT, {Slot::P1A, Slot::P1B}},
        }));
      REQUIRE_THAT(
        p2Options.moves[1U],
        Catch::Matchers::RangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::KNOCK_OFF, {Slot::P1A, Slot::P1B}},
        }));
    }

    std::swap(p2C, p2A);
    p1A.moves[1U].pp = Constants::MovePp::MIN;
    p1B.currentHp = Constants::PokemonCurrentHpStat::MIN;
    p1C.currentHp = Constants::PokemonCurrentHpStat::MIN;

    setSideHandles();
    simulate_turn::setSideOptions(test.simulation);

    if (battleFormat == BattleFormat::SINGLES) {
      SinglesSideOptions p1Options = p1Side.get<SinglesSideOptions>();
      SinglesSideOptions p2Options = p2Side.get<SinglesSideOptions>();

      REQUIRE(p1Options.switches.val.empty());
      REQUIRE_THAT(
        p1Options.moves,
        Catch::Matchers::RangeEquals(types::moveSlots<SinglesMoveOption>{
          {dex::Move::FURY_ATTACK, Slot::P2A},
        }));

      REQUIRE(p2Options.switches.val == types::teamPositions<Slot>{Slot::P2B, Slot::P2C});
      REQUIRE_THAT(
        p2Options.moves,
        Catch::Matchers::RangeEquals(types::moveSlots<SinglesMoveOption>{
          {dex::Move::QUIVER_DANCE, Slot::P2A},
        }));
    }
    else {
      DoublesSideOptions p1Options = p1Side.get<DoublesSideOptions>();
      DoublesSideOptions p2Options = p2Side.get<DoublesSideOptions>();
      for (types::activePokemonIndex i = 0U; i < Constants::ActivePokemonSlotsPerSide::DOUBLES; i++) {
        REQUIRE(p1Options.switches[i].val.empty());
        REQUIRE(p2Options.switches[i].val == types::teamPositions<Slot>{Slot::P2C});
      }

      REQUIRE_THAT(
        p1Options.moves[0U],
        Catch::Matchers::RangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::FURY_ATTACK, {Slot::P2A, Slot::P2B}},
        }));
      REQUIRE(p1Options.moves[1U].empty());

      REQUIRE_THAT(
        p2Options.moves[0U],
        Catch::Matchers::RangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::QUIVER_DANCE, {Slot::P2A}},
        }));
      REQUIRE_THAT(
        p2Options.moves[1U],
        Catch::Matchers::RangeEquals(types::moveSlots<DoublesMoveOption>{
          {dex::Move::KNOCK_OFF, {Slot::P1A}},
        }));
    }
  }
}
}  // namespace pokesim
