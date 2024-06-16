#include "RandomChance.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventCheck.hpp>
#include <Components/Tags/RandomChanceTags.hpp>
#include <Types/Random.hpp>
#include <Utilities/RNG.hpp>

#include "Simulation.hpp"

namespace pokesim {
void setRandomBinaryChoice(
  const Simulation& simulation, types::handle battleHandle, types::percentChance percentChance) {
  if (percentChance >= simulation.simulateTurnOptions.randomChanceUpperLimit) {
    battleHandle.emplace<tags::RandomEventCheckPassed>();
  }
  else if (percentChance <= simulation.simulateTurnOptions.randomChanceLowerLimit) {
    battleHandle.emplace<tags::RandomEventCheckFailed>();
  }
  else {
    battleHandle.emplace<RandomBinaryEventCheckChance>(percentChance);
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation) {
  static_assert(POSSIBLE_EVENT_COUNT > 2);
  static_assert(POSSIBLE_EVENT_COUNT <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto checkView = registry.view<RandomEventCheck<POSSIBLE_EVENT_COUNT>>();
    ENTT_ASSERT(
      checkView.empty() || POSSIBLE_EVENT_COUNT > 2U,
      "RandomEventCheck should only be used for events with more than two options. Use RandomBinaryEventCheckChance "
      "instead");
    registry.insert<tags::CloneFrom>(checkView.begin(), checkView.end());
    auto clonedEntityMap = clone(registry, POSSIBLE_EVENT_COUNT - 1);

    for (const auto [originalBattle, clonedBattles] : clonedEntityMap) {
      registry.emplace<tags::RandomEvent1>(originalBattle);
      registry.emplace<tags::RandomEvent2>(clonedBattles[0]);
      registry.emplace<tags::RandomEvent3>(clonedBattles[1]);

      if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
        registry.emplace<tags::RandomEvent4>(clonedBattles[2]);
      }
      if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
        registry.emplace<tags::RandomEvent4>(clonedBattles[3]);
      }
    }
  }
  else {
    registry.view<RandomEventCheck<POSSIBLE_EVENT_COUNT>, RngSeed>().each(
      [&registry](types::entity entity, const RandomEventCheck<POSSIBLE_EVENT_COUNT>& eventCheck, RngSeed& rngSeed) {
        types::percentChance rng = internal::nextBoundedRandomValue(rngSeed, 100);

        if (rng <= eventCheck.chances[0]) {
          registry.emplace<tags::RandomEvent1>(entity);
        }
        else if (rng <= eventCheck.chances[1]) {
          registry.emplace<tags::RandomEvent2>(entity);
        }
        else if (rng <= eventCheck.chances[2]) {
          registry.emplace<tags::RandomEvent3>(entity);
        }
        else if (POSSIBLE_EVENT_COUNT >= 4U && rng <= eventCheck.chances[3]) {
          registry.emplace<tags::RandomEvent4>(entity);
        }
        else if (POSSIBLE_EVENT_COUNT == 5U && rng <= eventCheck.chances[4]) {
          registry.emplace<tags::RandomEvent5>(entity);
        }
      });
  }

  registry.clear<RandomEventCheck<POSSIBLE_EVENT_COUNT>>();
}

void randomBinaryChance(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto binaryCheckView = registry.view<RandomBinaryEventCheckChance>();
    registry.insert<tags::CloneFrom>(binaryCheckView.begin(), binaryCheckView.end());
    auto clonedEntityMap = clone(registry, 1);
    for (const auto [originalBattle, clonedBattle] : clonedEntityMap) {
      registry.emplace<tags::RandomEventCheckPassed>(originalBattle);
      registry.emplace<tags::RandomEventCheckFailed>(clonedBattle[0]);
    }
  }
  else {
    registry.view<RandomBinaryEventCheckChance, RngSeed>().each(
      [&registry](types::entity entity, const RandomBinaryEventCheckChance& eventCheck, RngSeed& rngSeed) {
        types::percentChance rng = internal::nextBoundedRandomValue(rngSeed, 100);

        if (rng <= eventCheck.val) {
          registry.emplace<tags::RandomEventCheckPassed>(entity);
        }
        else {
          registry.emplace<tags::RandomEventCheckFailed>(entity);
        }
      });
  }

  registry.clear<RandomBinaryEventCheckChance>();
}

void clearRandomChanceResult(Simulation& simulation) {
  simulation.registry.clear<tags::RandomEventCheckPassed>();
  simulation.registry.clear<tags::RandomEventCheckFailed>();
  simulation.registry.clear<tags::RandomEvent1>();
  simulation.registry.clear<tags::RandomEvent2>();
  simulation.registry.clear<tags::RandomEvent3>();
  simulation.registry.clear<tags::RandomEvent4>();
  simulation.registry.clear<tags::RandomEvent5>();
}

template <>
void randomChance<2U>(Simulation& simulation) {
  randomBinaryChance(simulation);
}
template void randomChance<3U>(Simulation& simulation);
template void randomChance<4U>(Simulation& simulation);
template void randomChance<5U>(Simulation& simulation);

void sampleRandomChance(Simulation& simulation) {}
}  // namespace pokesim