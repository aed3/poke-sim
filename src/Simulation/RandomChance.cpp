#include "RandomChance.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/Probability.hpp>
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
    battleHandle.emplace<RandomBinaryEventChance>(percentChance);
  }
}

void internal::updateProbability(Probability& currentProbability, types::percentChance percentChance) {
  currentProbability.val *= (types::probability)percentChance / 100.0F;
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void internal::assignRandomEvent(
  types::handle battleHandle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventCheck, RngSeed& rngSeed,
  Probability& probability) {
  types::percentChance rng = internal::nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventCheck.val[0]) {
    battleHandle.emplace<tags::RandomEvent1>();
    updateProbability(probability, eventCheck.val[0]);
  }
  else if (rng <= eventCheck.val[1]) {
    battleHandle.emplace<tags::RandomEvent2>();
    updateProbability(probability, eventCheck.val[1] - eventCheck.val[0]);
  }
  else if (rng <= eventCheck.val[2]) {
    battleHandle.emplace<tags::RandomEvent3>();
    updateProbability(probability, eventCheck.val[2] - eventCheck.val[1]);
  }
  else if (POSSIBLE_EVENT_COUNT >= 4U && rng <= eventCheck.val[3]) {
    battleHandle.emplace<tags::RandomEvent4>();
    updateProbability(probability, eventCheck.val[3] - eventCheck.val[2]);
  }
  else if (POSSIBLE_EVENT_COUNT == 5U && rng <= eventCheck.val[4]) {
    battleHandle.emplace<tags::RandomEvent5>();
    updateProbability(probability, eventCheck.val[4] - eventCheck.val[3]);
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation) {
  static_assert(POSSIBLE_EVENT_COUNT > 2);
  static_assert(POSSIBLE_EVENT_COUNT <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto checkView = registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>>();
    ENTT_ASSERT(
      checkView.empty() || POSSIBLE_EVENT_COUNT > 2U,
      "RandomEventChances should only be used for events with more than two options. Use RandomBinaryEventChance "
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
        registry.emplace<tags::RandomEvent5>(clonedBattles[3]);
      }
    }

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEvent1>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[0]);
      });

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEvent2>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[1] - eventChances.val[0]);
      });

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEvent3>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[2] - eventChances.val[1]);
      });

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEvent4>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[3] - eventChances.val[2]);
      });

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEvent5>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[4] - eventChances.val[3]);
      });
  }
  else {
    simulation.view<internal::assignRandomEvent<POSSIBLE_EVENT_COUNT>>();
  }

  registry.clear<RandomEventChances<POSSIBLE_EVENT_COUNT>>();
}

void internal::assignRandomBinaryEvent(
  types::handle battleHandle, const RandomBinaryEventChance& eventCheck, RngSeed& rngSeed, Probability& probability) {
  types::percentChance rng = internal::nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventCheck.val) {
    battleHandle.emplace<tags::RandomEventCheckPassed>();
    updateProbability(probability, eventCheck.val);
  }
  else {
    battleHandle.emplace<tags::RandomEventCheckFailed>();
    updateProbability(probability, 100 - eventCheck.val);
  }
}

void randomBinaryChance(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto binaryCheckView = registry.view<RandomBinaryEventChance>();
    registry.insert<tags::CloneFrom>(binaryCheckView.begin(), binaryCheckView.end());
    auto clonedEntityMap = clone(registry, 1);
    for (const auto [originalBattle, clonedBattle] : clonedEntityMap) {
      registry.emplace<tags::RandomEventCheckPassed>(originalBattle);
      registry.emplace<tags::RandomEventCheckFailed>(clonedBattle[0]);
    }

    registry.view<RandomBinaryEventChance, Probability, tags::RandomEventCheckPassed>().each(
      [](const RandomBinaryEventChance& eventChance, Probability& probability) {
        internal::updateProbability(probability, eventChance.val);
      });

    registry.view<RandomBinaryEventChance, Probability, tags::RandomEventCheckFailed>().each(
      [](const RandomBinaryEventChance& eventChance, Probability& probability) {
        internal::updateProbability(probability, 100 - eventChance.val);
      });
  }
  else {
    simulation.view<internal::assignRandomBinaryEvent>();
  }

  registry.clear<RandomBinaryEventChance>();
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

template void randomChance<3U>(Simulation& simulation);
template void randomChance<4U>(Simulation& simulation);
template void randomChance<5U>(Simulation& simulation);

void sampleRandomChance(Simulation& simulation) {}
}  // namespace pokesim