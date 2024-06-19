#include "RandomChance.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Components/Accuracy.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventCheck.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Tags/RandomChanceTags.hpp>
#include <Types/Random.hpp>
#include <Utilities/RNG.hpp>
#include <Utilities/Tags.hpp>

#include "Simulation.hpp"

namespace pokesim {
template <typename Component, typename... T>
void setRandomBinaryChoice(Simulation& simulation) {
  types::percentChance autoPassLimit = simulation.simulateTurnOptions.randomChanceUpperLimit.value_or(100);
  types::percentChance autoFailLimit = simulation.simulateTurnOptions.randomChanceLowerLimit.value_or(0);
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<internal::setRandomBinaryChoice<Component, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>(
      autoPassLimit,
      autoFailLimit);
  }
  else {
    simulation.view<internal::setRandomBinaryChoice<Component, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>(
      autoPassLimit,
      autoFailLimit);
  }
}

template <typename Component, BattleFormat Format>
void internal::setRandomBinaryChoice(
  types::registry& registry, const Battle& battle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit) {
  if (percentChance.val >= autoPassLimit) {
    registry.emplace<tags::RandomEventCheckPassed>(battle.val);
  }
  else if (percentChance.val <= autoFailLimit) {
    registry.emplace<tags::RandomEventCheckFailed>(battle.val);
  }
  else {
    if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
      registry.emplace<RandomBinaryEventChance>(battle.val, percentChance.val);
    }
    else {
      registry.get_or_emplace<RandomBinaryEventChanceStack>(battle.val).val.emplace_back(percentChance.val);
    }
  }
}

void internal::updateProbability(Probability& currentProbability, types::percentChance percentChance) {
  currentProbability.val *= (types::probability)percentChance / 100.0F;
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void internal::assignRandomEvent(
  types::handle battleHandle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventCheck, RngSeed& rngSeed,
  Probability& probability) {
  types::percentChance rng = (types::percentChance)internal::nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventCheck.val[0]) {
    battleHandle.emplace<tags::RandomEventA>();
    updateProbability(probability, eventCheck.val[0]);
    return;
  }
  if (rng <= eventCheck.val[1]) {
    battleHandle.emplace<tags::RandomEventB>();
    updateProbability(probability, eventCheck.val[1] - eventCheck.val[0]);
    return;
  }
  if (rng <= eventCheck.val[2]) {
    battleHandle.emplace<tags::RandomEventC>();
    updateProbability(probability, eventCheck.val[2] - eventCheck.val[1]);
    return;
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
    if (rng <= eventCheck.val[3]) {
      battleHandle.emplace<tags::RandomEventD>();
      updateProbability(probability, eventCheck.val[3] - eventCheck.val[2]);
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
    if (rng <= eventCheck.val[4]) {
      battleHandle.emplace<tags::RandomEventE>();
      updateProbability(probability, eventCheck.val[4] - eventCheck.val[3]);
    }
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void internal::placeRandomEventChanceFromStack(
  types::handle battleHandle, RandomEventChancesStack<POSSIBLE_EVENT_COUNT>& stack) {
  battleHandle.emplace<RandomEventChances<POSSIBLE_EVENT_COUNT>>(stack.val.back());
  stack.val.pop_back();
  if (stack.val.empty()) {
    battleHandle.remove<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>();
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation) {
  static_assert(POSSIBLE_EVENT_COUNT > 2);
  static_assert(POSSIBLE_EVENT_COUNT <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<internal::placeRandomEventChanceFromStack<POSSIBLE_EVENT_COUNT>>();
  }

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto checkView = registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>>();
    ENTT_ASSERT(
      checkView.empty() || POSSIBLE_EVENT_COUNT > 2U,
      "RandomEventChances should only be used for events with more than two options. Use RandomBinaryEventChance "
      "instead");
    if (checkView.empty()) {
      return;
    }

    registry.insert<tags::CloneFrom>(checkView.begin(), checkView.end());
    auto clonedEntityMap = clone(registry, POSSIBLE_EVENT_COUNT - 1);

    for (const auto [originalBattle, clonedBattles] : clonedEntityMap) {
      registry.emplace<tags::RandomEventA>(originalBattle);
      registry.emplace<tags::RandomEventB>(clonedBattles[0]);
      registry.emplace<tags::RandomEventC>(clonedBattles[1]);

      if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
        registry.emplace<tags::RandomEventD>(clonedBattles[2]);
      }
      if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
        registry.emplace<tags::RandomEventE>(clonedBattles[3]);
      }
    }

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEventA>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[0]);
      });

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEventB>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[1] - eventChances.val[0]);
      });

    registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEventC>().each(
      [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
        internal::updateProbability(probability, eventChances.val[2] - eventChances.val[1]);
      });

    if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
      registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEventD>().each(
        [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
          internal::updateProbability(probability, eventChances.val[3] - eventChances.val[2]);
        });
    }

    if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
      registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>, Probability, tags::RandomEventE>().each(
        [](const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, Probability& probability) {
          internal::updateProbability(probability, eventChances.val[4] - eventChances.val[3]);
        });
    }
  }
  else {
    simulation.view<internal::assignRandomEvent<POSSIBLE_EVENT_COUNT>>();
  }

  registry.clear<RandomEventChances<POSSIBLE_EVENT_COUNT>>();

  if (
    simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT &&
    !registry.view<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>().empty()) {
    randomBinaryChance(simulation);
  }
}

void internal::assignRandomBinaryEvent(
  types::handle battleHandle, const RandomBinaryEventChance& eventCheck, RngSeed& rngSeed, Probability& probability) {
  types::percentChance rng = (types::percentChance)internal::nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventCheck.val) {
    battleHandle.emplace<tags::RandomEventCheckPassed>();
    updateProbability(probability, eventCheck.val);
  }
  else {
    battleHandle.emplace<tags::RandomEventCheckFailed>();
    updateProbability(probability, 100 - eventCheck.val);
  }
}

void internal::placeRandomBinaryEventChanceFromStack(types::handle battleHandle, RandomBinaryEventChanceStack& stack) {
  battleHandle.emplace<RandomBinaryEventChance>(stack.val.back());
  stack.val.pop_back();
  if (stack.val.empty()) {
    battleHandle.remove<RandomBinaryEventChanceStack>();
  }
}

void randomBinaryChance(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<internal::placeRandomBinaryEventChanceFromStack>();
  }

  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto binaryCheckView = registry.view<RandomBinaryEventChance>();
    if (binaryCheckView.empty()) {
      return;
    }

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
  if (
    simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT &&
    !registry.view<RandomBinaryEventChanceStack>().empty()) {
    randomBinaryChance(simulation);
  }
}

void clearRandomChanceResult(Simulation& simulation) {
  simulation.registry.clear<tags::RandomEventCheckPassed>();
  simulation.registry.clear<tags::RandomEventCheckFailed>();
  simulation.registry.clear<tags::RandomEventA>();
  simulation.registry.clear<tags::RandomEventB>();
  simulation.registry.clear<tags::RandomEventC>();
  simulation.registry.clear<tags::RandomEventD>();
  simulation.registry.clear<tags::RandomEventE>();
}

template void randomChance<3U>(Simulation& simulation);
template void randomChance<4U>(Simulation& simulation);
template void randomChance<5U>(Simulation& simulation);

template void setRandomBinaryChoice<Accuracy, tags::internal::TargetCanBeHit>(Simulation& simulation);

void sampleRandomChance(Simulation& /*simulation*/) {}
}  // namespace pokesim