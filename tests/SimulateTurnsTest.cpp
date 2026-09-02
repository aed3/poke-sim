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

TEST_CASE("Simulate Turn: Basic Switching", "[Simulation][SimulateTurn]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation{pokedex, BattleFormat::SINGLES};
  const types::registry& registry = simulation.registry;

  BattleCreationInfo battleCreationInfo;
  battleCreationInfo.runWithSimulateTurn = true;
  battleCreationInfo.turn = 1U;
  PokemonCreationInfo p1A{dex::Species::EMPOLEON}, p2A{dex::Species::AMPHAROS}, p1B{dex::Species::GARDEVOIR},
    p2B{dex::Species::PANGORO};
  p1A.moves = p1B.moves = p2A.moves = p2B.moves = {{dex::Move::SPLASH}};
  battleCreationInfo.sides.p1().team = {p1A, p1B};
  battleCreationInfo.sides.p2().team = {p2A, p2B};
  battleCreationInfo.decisionsToSimulate = {{
    {PlayerSideId::P1, types::slotDecisions{SwitchDecision{Slot::P1A, Slot::P1B}}},
    {PlayerSideId::P2, types::slotDecisions{SwitchDecision{Slot::P2A, Slot::P2B}}},
  }};

  pokedex.loadForBattleInfo({battleCreationInfo});
  simulation.createInitialStates({battleCreationInfo});
  simulation.simulateTurnOptions.setApplyChangesToInputBattle(true);

  types::entityVector specificallyCheckEntities;
  for (types::entity entity : registry.view<types::entity>()) {
    specificallyCheckEntities.push_back(entity);
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
    simulate_turn::TurnOutcomeBattles,
    simulate_turn::tags::SpeedSortNeeded,
    ParentBattle,
    RootBattle>();

  checks.checkViewForChanges<tags::Side, SideDecision, Team>();

  checks.checkViewForChanges<tags::Pokemon, tags::ActivePokemon>();

  for (types::entity side : registry.view<tags::Side>()) {
    const Team& currentTeam = registry.get<Team>(side);
    const Team& initialTeam = checks.getInitialComponents<Team>(side);

    REQUIRE(currentTeam.val[0] == initialTeam.val[1]);
    REQUIRE(currentTeam.val[1] == initialTeam.val[0]);
    REQUIRE(registry.all_of<tags::ActivePokemon>(currentTeam.val[0]));
    REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(currentTeam.val[1]));
  }
}

TEST_CASE("Simulate Turn: Battle ends on faint", "[Simulation][SimulateTurn]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation{pokedex, BattleFormat::SINGLES};
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

  checks.checkViewForChanges<tags::Side, SideDecision, FoesRemaining>();

  types::entity battle = turnOutcomeBattles[0];
  const auto& [turn, rootBattle, sides, winner] = registry.get<Turn, RootBattle, Sides, Winner>(battle);

  types::entity p1Side = sides.val.p1();
  types::entity p2Side = sides.val.p2();
  types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
  types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
  types::moveSlotIndex p1MoveIndex = 0U;
  types::moveSlotIndex p2MoveIndex = 0U;

  const FoesRemaining& p2Remaining = registry.get<FoesRemaining>(p1Side);
  const FoesRemaining& p1Remaining = registry.get<FoesRemaining>(p2Side);
  REQUIRE(p2Remaining.val == 1U);
  REQUIRE(p1Remaining.val == 0U);

  checks.checkEntityForChanges<stat::CurrentHp, tags::Fainted, tags::ActivePokemon, MoveSlots>(p1Pokemon);
  checks.checkEntityForChanges<LastUsedMove, MoveSlots>(p2Pokemon);

  auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
  REQUIRE(p2PokemonLastUsedMove.val == p2MoveIndex);

  auto p1PokemonHp = registry.get<stat::CurrentHp>(p1Pokemon);
  REQUIRE(p1PokemonHp.val == Constants::PokemonCurrentHpStat::MIN);
  REQUIRE(registry.all_of<tags::Fainted>(p1Pokemon));
  REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(p1Pokemon));

  checks.checkMovePpUsage(p1Pokemon, p1MoveIndex);
  checks.checkMovePpUsage(p1Pokemon, p2MoveIndex);

  REQUIRE(winner.val == PlayerSideId::P2);
}

TEST_CASE("Simulate Turn: Speed Ties", "[Simulation][SimulateTurn]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation{pokedex, BattleFormat::DOUBLES};
  const types::registry& registry = simulation.registry;

  types::activePokemonIndex speedTieCount = GENERATE(range(2U, 5U));
  types::probability branchProbabilityLimit = GENERATE(0.0F, 0.05F, 0.2F, 0.5F, 1.0F);

  BattleCreationInfo battleCreationInfo;
  battleCreationInfo.runWithSimulateTurn = true;
  battleCreationInfo.turn = 1U;
  PokemonCreationInfo p1A{dex::Species::EMPOLEON}, p2A{dex::Species::EMPOLEON}, p1B{dex::Species::EMPOLEON},
    p2B{dex::Species::EMPOLEON};
  p1A.moves = p1B.moves = p2A.moves = p2B.moves = {{dex::Move::SPLASH}};
  p1A.ivs.spe = p1B.ivs.spe = p2A.ivs.spe = p2B.ivs.spe = 31U;

  if (speedTieCount < 4U) {
    p2B.ivs.spe = 20U;
  }
  if (speedTieCount < 3U) {
    p1B.ivs.spe = 10U;
  }

  battleCreationInfo.sides.p1().team = {p1A, p1B};
  battleCreationInfo.sides.p2().team = {p2A, p2B};
  battleCreationInfo.decisionsToSimulate = {{
    {PlayerSideId::P1,
     types::slotDecisions{
       MoveDecision{Slot::P1A, Slot::P1A, dex::Move::SPLASH},
       MoveDecision{Slot::P1B, Slot::P1B, dex::Move::SPLASH},
     }},
    {PlayerSideId::P2,
     types::slotDecisions{
       MoveDecision{Slot::P2A, Slot::P2A, dex::Move::SPLASH},
       MoveDecision{Slot::P2B, Slot::P2B, dex::Move::SPLASH},
     }},
  }};

  types::eventPossibilities idealCloneCount = 1U;
  for (types::eventPossibilities i = speedTieCount; i > 1U; i--) {
    idealCloneCount *= i;
  }

  if (branchProbabilityLimit >= 1.0F / idealCloneCount) {
    idealCloneCount = 1U;
  }

  pokedex.loadForBattleInfo({battleCreationInfo});
  simulation.createInitialStates({battleCreationInfo});
  simulation.simulateTurnOptions.setApplyChangesToInputBattle(true);
  simulation.simulateTurnOptions.setMakeBranchesOnRandomEvents(true);
  simulation.simulateTurnOptions.setBranchProbabilityLowerLimit(branchProbabilityLimit);
  CAPTURE(speedTieCount, branchProbabilityLimit, idealCloneCount);

  SECTION("Check Action Queue Orders") {
    simulation.view<internal::simulate_turn::resolveDecision>();
    simulation.view<internal::simulate_turn::speedSort>();
    internal::simulate_turn::resolveSpeedTies(simulation);

    auto trueCloneCount = registry.view<tags::Battle>()->size();
    REQUIRE(trueCloneCount == idealCloneCount);
    auto actionQueueView = registry.view<ActionQueue>();

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
    auto result = simulation.simulateTurn();
    REQUIRE(result.turnOutcomeBattlesResults()->size() == 1U);

    const auto& turnOutcomeBattles = std::get<1>(*result.turnOutcomeBattlesResults().each().begin()).val;
    auto trueCloneCount = turnOutcomeBattles.size();
    REQUIRE(trueCloneCount == idealCloneCount);
  }
}

TEST_CASE("Simulate Turn: Decisions", "[Simulation][SimulateTurn]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  BattleCreationInfo battleCreationInfo;
  auto& p1Team = battleCreationInfo.sides.p1().team;
  auto& p2Team = battleCreationInfo.sides.p2().team;

  p1Team = {
    createPredefinedPokemon(pokedex, dex::Species::EMPOLEON),
    createPredefinedPokemon(pokedex, dex::Species::GARDEVOIR),
    createPredefinedPokemon(pokedex, dex::Species::DRAGAPULT),
  };
  p2Team = {
    createPredefinedPokemon(pokedex, dex::Species::AMPHAROS),
    createPredefinedPokemon(pokedex, dex::Species::PANGORO),
    createPredefinedPokemon(pokedex, dex::Species::RIBOMBEE),
  };

  pokedex.loadForBattleInfo({battleCreationInfo});
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  bool teamPreview = GENERATE(false, true);
  CAPTURE(battleFormat, teamPreview);

  Simulation simulation{pokedex, battleFormat};
  types::registry& registry = simulation.registry;

  battleCreationInfo.turn = teamPreview ? 0U : 1U;
  types::handle p1Side, p2Side;
  auto setSideHandles = [&]() {
    registry.clear();
    simulation.createInitialStates({battleCreationInfo});

    types::entity battleEntity = *registry.view<tags::Battle>().each().begin().base();
    auto sides = registry.view<Sides>();
    p1Side = {registry, sides.get<Sides>(battleEntity).val.p1()};
    p2Side = {registry, sides.get<Sides>(battleEntity).val.p2()};
  };

  setSideHandles();

  if (teamPreview) {
    simulate_turn::setTeamPreviewOptions(simulation);
    REQUIRE(p1Side.get<TeamPreviewOptions>().val == types::teamOrder{0U, 1U, 2U});
    REQUIRE(p2Side.get<TeamPreviewOptions>().val == types::teamOrder{0U, 1U, 2U});

    p1Team[0U].currentHp = Constants::PokemonCurrentHpStat::MIN;
    p2Team[2U].currentHp = Constants::PokemonCurrentHpStat::MIN;

    setSideHandles();

    simulate_turn::setTeamPreviewOptions(simulation);
    REQUIRE(p1Side.get<TeamPreviewOptions>().val == types::teamOrder{1U, 2U});
    REQUIRE(p2Side.get<TeamPreviewOptions>().val == types::teamOrder{0U, 1U});
  }
  else {
    simulate_turn::setSideOptions(simulation);

    if (battleFormat == BattleFormat::SINGLES) {
      SinglesSideOptions p1Options = p1Side.get<SinglesSideOptions>();
      SinglesSideOptions p2Options = p2Side.get<SinglesSideOptions>();

      REQUIRE(p1Options.switches.val == types::teamPositions<Slot>{Slot::P1B, Slot::P1C});
      REQUIRE_THAT(
        p1Options.moves,
        Catch::Matchers::UnorderedRangeEquals(
          types::moveSlots<SinglesMoveOption>{
            {dex::Move::KNOCK_OFF, Slot::P2A},
            {dex::Move::FURY_ATTACK, Slot::P2A},
          }));

      REQUIRE(p2Options.switches.val == types::teamPositions<Slot>{Slot::P2B, Slot::P2C});
      REQUIRE_THAT(
        p2Options.moves,
        Catch::Matchers::RangeEquals(
          types::moveSlots<SinglesMoveOption>{
            {dex::Move::THUNDERBOLT, Slot::P1A},
          }));
    }
    else {
      DoublesSideOptions p1Options = p1Side.get<DoublesSideOptions>();
      DoublesSideOptions p2Options = p2Side.get<DoublesSideOptions>();
      REQUIRE(p1Options.switches.val == types::teamPositions<Slot>{Slot::P1C});
      REQUIRE_THAT(
        p1Options.moves[0U],
        Catch::Matchers::UnorderedRangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::KNOCK_OFF, {Slot::P2A, Slot::P2B}},
            {dex::Move::FURY_ATTACK, {Slot::P2A, Slot::P2B}},
          }));
      REQUIRE_THAT(
        p1Options.moves[1U],
        Catch::Matchers::RangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::MOONBLAST, {Slot::P2A, Slot::P2B}},
          }));

      REQUIRE(p2Options.switches.val == types::teamPositions<Slot>{Slot::P2C});
      REQUIRE_THAT(
        p2Options.moves[0U],
        Catch::Matchers::RangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::THUNDERBOLT, {Slot::P1A, Slot::P1B}},
          }));
      REQUIRE_THAT(
        p2Options.moves[1U],
        Catch::Matchers::RangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::KNOCK_OFF, {Slot::P1A, Slot::P1B}},
          }));
    }

    std::swap(p2Team[2U], p2Team[0U]);
    p1Team[0U].moves[1U].pp = Constants::MovePp::MIN;
    p1Team[1U].currentHp = Constants::PokemonCurrentHpStat::MIN;
    p1Team[2U].currentHp = Constants::PokemonCurrentHpStat::MIN;

    setSideHandles();
    simulate_turn::setSideOptions(simulation);

    if (battleFormat == BattleFormat::SINGLES) {
      SinglesSideOptions p1Options = p1Side.get<SinglesSideOptions>();
      SinglesSideOptions p2Options = p2Side.get<SinglesSideOptions>();

      REQUIRE(p1Options.switches.val.empty());
      REQUIRE_THAT(
        p1Options.moves,
        Catch::Matchers::RangeEquals(
          types::moveSlots<SinglesMoveOption>{
            {dex::Move::FURY_ATTACK, Slot::P2A},
          }));

      REQUIRE(p2Options.switches.val == types::teamPositions<Slot>{Slot::P2B, Slot::P2C});
      REQUIRE_THAT(
        p2Options.moves,
        Catch::Matchers::RangeEquals(
          types::moveSlots<SinglesMoveOption>{
            {dex::Move::QUIVER_DANCE, Slot::P2A},
          }));
    }
    else {
      DoublesSideOptions p1Options = p1Side.get<DoublesSideOptions>();
      DoublesSideOptions p2Options = p2Side.get<DoublesSideOptions>();
      REQUIRE(p1Options.switches.val.empty());
      REQUIRE_THAT(
        p1Options.moves[0U],
        Catch::Matchers::RangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::FURY_ATTACK, {Slot::P2A, Slot::P2B}},
          }));
      REQUIRE(p1Options.moves[1U].empty());

      REQUIRE(p2Options.switches.val == types::teamPositions<Slot>{Slot::P2C});
      REQUIRE_THAT(
        p2Options.moves[0U],
        Catch::Matchers::RangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::QUIVER_DANCE, {Slot::P2A}},
          }));
      REQUIRE_THAT(
        p2Options.moves[1U],
        Catch::Matchers::RangeEquals(
          types::moveSlots<DoublesMoveOption>{
            {dex::Move::KNOCK_OFF, {Slot::P1A}},
          }));
    }
  }
}
}  // namespace pokesim
