#include "SpeedSort.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/SimulateTurn/SpeedTieIndexes.hpp>
#include <Components/SpeedSort.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <algorithm>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim {
void speedSort(types::handle handle, ActionQueue& actionQueue) {
  std::vector<types::entity>& entityList = actionQueue.actionQueue;

  if (entityList.size() == 1) return;
  const types::registry* registry = handle.registry();

  std::vector<std::pair<SpeedSort, types::entity>> speedSortList;
  speedSortList.reserve(entityList.size());

  for (types::entity entity : entityList) {
    speedSortList.push_back({registry->get<SpeedSort>(entity), entity});
  }

  // TODO (aed3): Test how different sorting algorithms effect speed
  std::sort(speedSortList.begin(), speedSortList.end(), [](const auto& pairA, const auto& pairB) {
    if (pairA.first.order != pairB.first.order) {
      return pairA.first.order < pairB.first.order;
    }

    if (pairA.first.priority != pairB.first.priority) {
      return pairB.first.priority < pairA.first.priority;
    }

    if (pairA.first.fractionalPriority != pairB.first.fractionalPriority) {
      return pairB.first.fractionalPriority;
    }

    if (pairA.first.speed != pairB.first.speed) {
      return pairB.first.speed < pairA.first.speed;
    }

    return true;
  });

  SpeedTieIndexes speedTies;
  std::size_t lastEqual = 0, tieCount = 1;

  auto speedSortEqual = [](const SpeedSort& speedSortA, const SpeedSort& speedSortB) {
    return speedSortA.order == speedSortB.order && speedSortA.priority == speedSortB.priority &&
           speedSortA.speed == speedSortB.speed && speedSortA.fractionalPriority == speedSortB.fractionalPriority;
  };

  for (std::size_t i = 0; i < speedSortList.size(); i++) {
    entityList[i] = speedSortList[i].second;

    if (i > 0 && speedSortEqual(speedSortList[i].first, speedSortList[i - 1].first)) {
      tieCount++;
    }
    else {
      if (tieCount > 1) {
        speedTies.spans.push_back({lastEqual, tieCount});
      }
      lastEqual = i;
      tieCount = 1;
    }
  }

  if (tieCount > 1) {
    speedTies.spans.push_back({lastEqual, tieCount});
  }

  if (!speedTies.spans.empty()) {
    handle.emplace<SpeedTieIndexes>(speedTies);
  }
}
}  // namespace pokesim