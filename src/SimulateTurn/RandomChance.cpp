#include "RandomChance.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Components/Accuracy.hpp>
#include <Components/CalcDamage/CalcDamageTarget.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventCheck.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/RandomChanceTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Random.hpp>
#include <Utilities/RNG.hpp>
#include <Utilities/Tags.hpp>

namespace pokesim {
template <std::uint8_t POSSIBLE_EVENT_COUNT, BattleFormat Format, bool CumulativeSumChances>
void setRandomChoice(types::handle handle, std::array<types::percentChance, POSSIBLE_EVENT_COUNT> chances) {
  if constexpr (CumulativeSumChances) {
    types::percentChance chanceSum = 0;
    for (types::percentChance& chance : chances) {
      chanceSum += chance;
      chance = chanceSum;
    }

    ENTT_ASSERT(chanceSum == 100, "The total probability of all possible outcomes should add up to 100%.");
  }
  else {
    ENTT_ASSERT(chances.back() == 100, "The total probability of all possible outcomes should add up to 100%.");
    for (std::uint8_t i = 1; i < POSSIBLE_EVENT_COUNT; i++) {
      ENTT_ASSERT(
        chances[i - 1] < chances[i],
        "Chances should be a cumulative sum where each value is greater than the last.");
    }
  }

  if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
    handle.emplace<RandomEventChances<POSSIBLE_EVENT_COUNT>>(chances);
  }
  else {
    handle.get_or_emplace<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>().val.emplace_back(chances);
  }
}

template <typename Type>
bool internal::checkPercentChanceLimits(
  types::handle handle, Type percentChance, types::percentChance autoPassLimit, types::percentChance autoFailLimit) {
  if (percentChance >= autoPassLimit) {
    handle.emplace<tags::RandomEventCheckPassed>();
    return true;
  }

  if (percentChance <= autoFailLimit) {
    handle.emplace<tags::RandomEventCheckFailed>();
    return true;
  }
  return false;
}

template <BattleFormat Format>
void internal::setBinaryChanceByFormat(types::handle handle, types::percentChance percentChance) {
  if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
    handle.emplace<RandomBinaryEventChance>(percentChance);
  }
  else {
    handle.get_or_emplace<RandomBinaryEventChanceStack>().val.emplace_back(percentChance);
  }
}

template <typename Component, BattleFormat Format>
void internal::setRandomBinaryChoice(
  types::handle handle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit) {
  if (!checkPercentChanceLimits(handle, percentChance.val, autoPassLimit, autoFailLimit)) {
    setBinaryChanceByFormat<Format>(handle, percentChance.val);
  }
}

template <typename Component, BattleFormat Format>
void internal::setReciprocalRandomBinaryChoice(
  types::handle handle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit) {
  float passChance = 100 - 100.0 / percentChance.val;
  if (!checkPercentChanceLimits(handle, passChance, autoPassLimit, autoFailLimit)) {
    setBinaryChanceByFormat<Format>(handle, percentChance.val);
  }
}

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

template <typename Component, typename... T>
void setReciprocalRandomBinaryChoice(Simulation& simulation) {
  types::percentChance autoPassLimit = simulation.simulateTurnOptions.randomChanceUpperLimit.value_or(100);
  types::percentChance autoFailLimit = simulation.simulateTurnOptions.randomChanceLowerLimit.value_or(0);
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation
      .view<internal::setReciprocalRandomBinaryChoice<Component, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>(
        autoPassLimit,
        autoFailLimit);
  }
  else {
    simulation
      .view<internal::setReciprocalRandomBinaryChoice<Component, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>(
        autoPassLimit,
        autoFailLimit);
  }
}

template <typename Type>
void internal::updateProbability(Probability& currentProbability, Type percentChance) {
  currentProbability.val *= (types::probability)percentChance / 100.0F;
}

template <std::uint8_t POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void internal::updateProbabilityFromRandomChance(
  types::registry& registy, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, const Battle& battle) {
  Probability& probability = registy.get<Probability>(battle.val);

  if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventA>) {
    updateProbability(probability, eventChances.chanceA());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventB>) {
    updateProbability(probability, eventChances.chanceB());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventC>) {
    updateProbability(probability, eventChances.chanceC());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventD>) {
    updateProbability(probability, eventChances.chanceD());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventE>) {
    updateProbability(probability, eventChances.chanceE());
  }
}

template <bool Reciprocal, typename RandomEventTag>
void internal::updateProbabilityFromRandomBinaryChance(
  types::registry& registy, const RandomBinaryEventChance& eventChance, const Battle& battle) {
  Probability& probability = registy.get<Probability>(battle.val);

  if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventCheckPassed>) {
    if constexpr (Reciprocal) {
      internal::updateProbability(probability, eventChance.reciprocalPass());
    }
    else {
      internal::updateProbability(probability, eventChance.pass());
    }
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventCheckFailed>) {
    if constexpr (Reciprocal) {
      internal::updateProbability(probability, eventChance.reciprocalFail());
    }
    else {
      internal::updateProbability(probability, eventChance.fail());
    }
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void internal::assignRandomEvent(
  types::handle handle, const Battle& battle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventChances.val[0]) {
    handle.emplace<tags::RandomEventA>();
    updateProbability(probability, eventChances.chanceA());
    return;
  }
  if (rng <= eventChances.val[1]) {
    handle.emplace<tags::RandomEventB>();
    updateProbability(probability, eventChances.chanceB());
    return;
  }
  if (rng <= eventChances.val[2]) {
    handle.emplace<tags::RandomEventC>();
    updateProbability(probability, eventChances.chanceC());
    return;
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
    if (rng <= eventChances.val[3]) {
      handle.emplace<tags::RandomEventD>();
      updateProbability(probability, eventChances.chanceD());
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
    if (rng <= eventChances.val[4]) {
      handle.emplace<tags::RandomEventE>();
      updateProbability(probability, eventChances.chanceE());
    }
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void internal::placeRandomEventChanceFromStack(
  types::handle handle, RandomEventChancesStack<POSSIBLE_EVENT_COUNT>& stack) {
  handle.emplace<RandomEventChances<POSSIBLE_EVENT_COUNT>>(stack.val.back());
  stack.val.pop_back();
  if (stack.val.empty()) {
    handle.remove<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>();
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&)) {
  static_assert(POSSIBLE_EVENT_COUNT > 2);
  static_assert(POSSIBLE_EVENT_COUNT <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<internal::placeRandomEventChanceFromStack<POSSIBLE_EVENT_COUNT>>();
  }

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto chanceEntityView = registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>>();
    ENTT_ASSERT(
      chanceEntityView.empty() || POSSIBLE_EVENT_COUNT > 2U,
      "RandomEventChances should only be used for events with more than two options. Use RandomBinaryEventChance "
      "instead");
    if (chanceEntityView.empty()) {
      handleRandomEventChoice(simulation);
      clearRandomChanceResult(simulation);
      return;
    }
    registry.view<Battle, RandomEventChances<POSSIBLE_EVENT_COUNT>>().each(
      [&registry](const Battle& battle, auto&&... args) { registry.emplace<tags::CloneFrom>(battle.val); });

    std::vector<types::entity> chacneEntities{chanceEntityView.begin(), chanceEntityView.end()};
    auto clonedEntityMap = clone(registry, POSSIBLE_EVENT_COUNT - 1);

    for (types::entity original : chacneEntities) {
      const auto& cloned = clonedEntityMap[original];
      registry.emplace<tags::RandomEventA>(original);
      registry.emplace<tags::RandomEventB>(cloned[0]);
      registry.emplace<tags::RandomEventC>(cloned[1]);

      if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
        registry.emplace<tags::RandomEventD>(cloned[2]);
      }
      if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
        registry.emplace<tags::RandomEventE>(cloned[3]);
      }
    }

    simulation.view<
      internal::updateProbabilityFromRandomChance<POSSIBLE_EVENT_COUNT, tags::RandomEventA>,
      Tags<tags::RandomEventA>>();
    simulation.view<
      internal::updateProbabilityFromRandomChance<POSSIBLE_EVENT_COUNT, tags::RandomEventB>,
      Tags<tags::RandomEventB>>();
    simulation.view<
      internal::updateProbabilityFromRandomChance<POSSIBLE_EVENT_COUNT, tags::RandomEventC>,
      Tags<tags::RandomEventC>>();

    if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
      simulation.view<
        internal::updateProbabilityFromRandomChance<POSSIBLE_EVENT_COUNT, tags::RandomEventD>,
        Tags<tags::RandomEventD>>();
    }

    if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
      simulation.view<
        internal::updateProbabilityFromRandomChance<POSSIBLE_EVENT_COUNT, tags::RandomEventE>,
        Tags<tags::RandomEventE>>();
    }
  }
  else {
    simulation.view<internal::assignRandomEvent<POSSIBLE_EVENT_COUNT>>();
  }

  registry.clear<RandomEventChances<POSSIBLE_EVENT_COUNT>>();
  handleRandomEventChoice(simulation);
  clearRandomChanceResult(simulation);
  if (
    simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT &&
    !registry.view<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>().empty()) {
    randomBinaryChance(simulation, handleRandomEventChoice);
  }
}

void internal::assignRandomBinaryEvent(
  types::handle handle, const Battle& battle, const RandomBinaryEventChance& eventChance) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventChance.pass()) {
    handle.emplace<tags::RandomEventCheckPassed>();
    updateProbability(probability, eventChance.pass());
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
    updateProbability(probability, eventChance.fail());
  }
}

void internal::assignReciprocalRandomBinaryEvent(
  types::handle handle, const Battle& battle, const RandomBinaryEventChance& eventChance) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, eventChance.val);

  if (rng > 1U) {
    handle.emplace<tags::RandomEventCheckPassed>();
    updateProbability(probability, eventChance.reciprocalPass());
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
    updateProbability(probability, eventChance.reciprocalFail());
  }
}

void internal::placeRandomBinaryEventChanceFromStack(types::handle handle, RandomBinaryEventChanceStack& stack) {
  handle.emplace<RandomBinaryEventChance>(stack.val.back());
  stack.val.pop_back();
  if (stack.val.empty()) {
    handle.remove<RandomBinaryEventChanceStack>();
  }
}

template <bool Reciprocal>
void internal::randomBinaryChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&)) {
  types::registry& registry = simulation.registry;
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<internal::placeRandomBinaryEventChanceFromStack>();
  }

  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto chanceEntityView = registry.view<RandomBinaryEventChance>();
    if (chanceEntityView.empty()) {
      handleRandomEventChoice(simulation);
      clearRandomChanceResult(simulation);
      return;
    }

    registry.view<Battle, RandomBinaryEventChance>().each(
      [&registry](const Battle& battle, auto&&... args) { registry.emplace<tags::CloneFrom>(battle.val); });

    std::vector<types::entity> chacneEntities{chanceEntityView.begin(), chanceEntityView.end()};
    auto clonedEntityMap = clone(registry, 1);

    for (types::entity original : chacneEntities) {
      const auto& cloned = clonedEntityMap[original];
      registry.emplace<tags::RandomEventCheckPassed>(original);
      registry.emplace<tags::RandomEventCheckFailed>(cloned[0]);
    }

    simulation.view<
      internal::updateProbabilityFromRandomBinaryChance<Reciprocal, tags::RandomEventCheckPassed>,
      Tags<tags::RandomEventCheckPassed>>();
    simulation.view<
      internal::updateProbabilityFromRandomBinaryChance<Reciprocal, tags::RandomEventCheckFailed>,
      Tags<tags::RandomEventCheckFailed>>();
  }
  else if constexpr (Reciprocal) {
    simulation.view<internal::assignReciprocalRandomBinaryEvent>();
  }
  else {
    simulation.view<internal::assignRandomBinaryEvent>();
  }

  registry.clear<RandomBinaryEventChance>();
  handleRandomEventChoice(simulation);
  clearRandomChanceResult(simulation);
  if (
    simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT &&
    !registry.view<RandomBinaryEventChanceStack>().empty()) {
    randomBinaryChance(simulation, handleRandomEventChoice);
  }
}

void randomBinaryChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&)) {
  internal::randomBinaryChance<false>(simulation, handleRandomEventChoice);
}

void reciprocalRandomBinaryChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&)) {
  internal::randomBinaryChance<true>(simulation, handleRandomEventChoice);
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

template void randomChance<3U>(Simulation&, void (*)(Simulation&));
template void randomChance<4U>(Simulation&, void (*)(Simulation&));
template void randomChance<5U>(Simulation&, void (*)(Simulation&));

template void setRandomChoice<3U, BattleFormat::SINGLES_BATTLE_FORMAT, false>(
  types::handle, std::array<types::percentChance, 3U>);
template void setRandomChoice<3U, BattleFormat::DOUBLES_BATTLE_FORMAT, false>(
  types::handle, std::array<types::percentChance, 3U>);
template void setRandomChoice<3U, BattleFormat::SINGLES_BATTLE_FORMAT, true>(
  types::handle, std::array<types::percentChance, 3U>);
template void setRandomChoice<3U, BattleFormat::DOUBLES_BATTLE_FORMAT, true>(
  types::handle, std::array<types::percentChance, 3U>);

template void setRandomChoice<4U, BattleFormat::SINGLES_BATTLE_FORMAT, false>(
  types::handle, std::array<types::percentChance, 4U>);
template void setRandomChoice<4U, BattleFormat::DOUBLES_BATTLE_FORMAT, false>(
  types::handle, std::array<types::percentChance, 4U>);
template void setRandomChoice<4U, BattleFormat::SINGLES_BATTLE_FORMAT, true>(
  types::handle, std::array<types::percentChance, 4U>);
template void setRandomChoice<4U, BattleFormat::DOUBLES_BATTLE_FORMAT, true>(
  types::handle, std::array<types::percentChance, 4U>);

template void setRandomChoice<5U, BattleFormat::SINGLES_BATTLE_FORMAT, false>(
  types::handle, std::array<types::percentChance, 5U>);
template void setRandomChoice<5U, BattleFormat::DOUBLES_BATTLE_FORMAT, false>(
  types::handle, std::array<types::percentChance, 5U>);
template void setRandomChoice<5U, BattleFormat::SINGLES_BATTLE_FORMAT, true>(
  types::handle, std::array<types::percentChance, 5U>);
template void setRandomChoice<5U, BattleFormat::DOUBLES_BATTLE_FORMAT, true>(
  types::handle, std::array<types::percentChance, 5U>);

template void setRandomBinaryChoice<Accuracy, tags::internal::TargetCanBeHit>(Simulation&);
template void setReciprocalRandomBinaryChoice<calc_damage::CritChanceDivisor, calc_damage::tags::CalcDamageTarget>(
  Simulation&);

void sampleRandomChance(Simulation& /*simulation*/) {}
}  // namespace pokesim