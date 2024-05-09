#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Simulate Turn: SpeedSort", "[Simulation][SimulateTurn]") {
  using SpeedSortList = std::vector<SpeedSort>;

  auto runSpeedSortTest = [](
                            SpeedSortList speedSortList,
                            SpeedSortList idealSortedList,
                            SpeedTieIndexes idealSpeedTies = {}) {
    types::registry registry;
    ActionQueue initialQueue;

    for (const SpeedSort& speedSort : speedSortList) {
      types::entity entity = registry.create();
      registry.emplace<SpeedSort>(entity, speedSort);
      initialQueue.actionQueue.push_back(entity);
    }

    types::handle handle{registry, registry.create()};
    ActionQueue sortedQueue = initialQueue;

    simulate_turn::speedSort(handle, sortedQueue);

    REQUIRE(initialQueue.actionQueue.size() == sortedQueue.actionQueue.size());
    for (types::entity entity : initialQueue.actionQueue) {
      bool entityFound = false;
      for (types::entity sortedEntity : sortedQueue.actionQueue) {
        if (sortedEntity == entity) {
          entityFound = true;
          break;
        }
      }

      REQUIRE(entityFound);
    }

    for (std::size_t i = 0; i < idealSortedList.size(); i++) {
      INFO(std::to_string(i));
      const SpeedSort& idealSpeedSort = idealSortedList[i];
      const SpeedSort& trueSpeedSort = registry.get<SpeedSort>(sortedQueue.actionQueue[i]);

      REQUIRE(trueSpeedSort.order == idealSpeedSort.order);
      REQUIRE(trueSpeedSort.priority == idealSpeedSort.priority);
      REQUIRE(trueSpeedSort.fractionalPriority == idealSpeedSort.fractionalPriority);
      REQUIRE(trueSpeedSort.speed == idealSpeedSort.speed);
    }

    if (idealSpeedTies.spans.empty()) {
      REQUIRE_FALSE(handle.all_of<SpeedTieIndexes>());
      return;
    }

    REQUIRE(handle.all_of<SpeedTieIndexes>());
    const SpeedTieIndexes& trueSpeedTies = handle.get<SpeedTieIndexes>();

    REQUIRE(trueSpeedTies.spans.size() == idealSpeedTies.spans.size());
    for (const auto& idealSpeedTie : idealSpeedTies.spans) {
      bool found =
        std::any_of(trueSpeedTies.spans.begin(), trueSpeedTies.spans.end(), [&idealSpeedTie](const auto& trueSpeedTie) {
          return trueSpeedTie.start == idealSpeedTie.start && trueSpeedTie.length == trueSpeedTie.length;
        });

      REQUIRE(found);
    }
  };

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
        {SpeedTieIndexes::Span{0, 2}},
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
      SpeedSort{ActionOrder::MOVE, 0, false, 772},
      SpeedSort{ActionOrder::MOVE, 0, false, 621},
      SpeedSort{ActionOrder::MOVE, 0, false, 584},
      SpeedSort{ActionOrder::MOVE, 0, false, 444},
      SpeedSort{ActionOrder::MOVE, 0, false, 305},
      SpeedSort{ActionOrder::MOVE, 0, false, 152},
      SpeedSort{ActionOrder::MOVE, 0, false, 90},
      SpeedSort{ActionOrder::MOVE, 0, false, 11},
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
      SpeedSort{ActionOrder::BEFORE_TURN, 0, false, 584},
      SpeedSort{ActionOrder::BEFORE_TURN, 0, false, 444},
      SpeedSort{ActionOrder::SWITCH, 0, false, 52},
      SpeedSort{ActionOrder::SWITCH, 0, false, 40},
      SpeedSort{ActionOrder::MOVE, 1, false, 152},
      SpeedSort{ActionOrder::MOVE, 0, false, 315},
      SpeedSort{ActionOrder::MOVE, -3, true, 700},
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

TEST_CASE("Simulate Turn: Vertical Slice 1", "[Simulation][SimulateTurn]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, true);

  battleCreationInfo.runWithSimulateTurn = true;
  simulation.createInitialStates({battleCreationInfo});
  simulation.simulateTurn();
}
}  // namespace pokesim