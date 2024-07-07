#include "CalcDamageSpecifics.hpp"

#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Helpers.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/Damage.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Utilities/MaxSizedVector.hpp>
#include <Utilities/SelectForView.hpp>
#include <array>

namespace pokesim::simulate_turn {
namespace internal {
void applyDamageRollIndex(Damage& damage, const RandomEventIndex& damageRollIndex) {
  pokesim::calc_damage::applyDamageRoll(damage, damageRollIndex.val);
}

template <typename RandomInput>
void assignProbability(types::registry& registry, const Battle& battle, const RandomInput&) {
  Probability& probability = registry.get<Probability>(battle.val);
  probability.val *= 1.0F / (types::probability)MechanicConstants::MAX_DAMAGE_ROLL_COUNT;
}

template <std::uint8_t Index, typename... T>
void runApplyDamageRollView(Simulation& simulation, DamageRollKind damageRollsConsidered) {
  if constexpr (Index < 1) {
    if (calc_damage::damageKindsMatch(damageRollsConsidered, DamageRollKind::AVERAGE_DAMAGE)) {
      simulation.viewForSelectedMoves<calc_damage::applyAverageDamageRoll, Tags<T...>>();
      return;
    }
  }
  if constexpr (Index < 2) {
    if (calc_damage::damageKindsMatch(damageRollsConsidered, DamageRollKind::MIN_DAMAGE)) {
      simulation.viewForSelectedMoves<calc_damage::applyMinDamageRoll, Tags<T...>>();
      return;
    }
  }
  if constexpr (Index < 3) {
    // Do nothing as max damage is what Damage already stores
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, PlayerSideId SideForDamageRolls>
void handleDamageRollOptions(Simulation& simulation) {
  std::array<types::percentChance, POSSIBLE_EVENT_COUNT> values{};
  values.fill(0U);
  values[POSSIBLE_EVENT_COUNT - 1U] = 100U;
  setRandomChoice<POSSIBLE_EVENT_COUNT, pokesim::tags::SelectedForViewMove>(simulation, values, false);

  auto applyChoices = [](Simulation& sim) {
    DamageRollKind damageRollsConsidered{};

    if constexpr (SideForDamageRolls == PlayerSideId::P1) {
      damageRollsConsidered = sim.simulateTurnOptions.damageRollsConsidered.p1;
    }
    else {
      damageRollsConsidered = sim.simulateTurnOptions.damageRollsConsidered.p2;
    }
    if constexpr (POSSIBLE_EVENT_COUNT >= 1U) {
      runApplyDamageRollView<0U, pokesim::tags::RandomEventA>(sim, damageRollsConsidered);
    }
    if constexpr (POSSIBLE_EVENT_COUNT >= 2U) {
      runApplyDamageRollView<1U, pokesim::tags::RandomEventB>(sim, damageRollsConsidered);
    }
    if constexpr (POSSIBLE_EVENT_COUNT == 3U) {
      runApplyDamageRollView<2U, pokesim::tags::RandomEventC>(sim, damageRollsConsidered);
    }
  };

  auto updateProbabilities = [](Simulation& sim) {
    sim.viewForSelectedMoves<internal::assignProbability<RandomEventChances<POSSIBLE_EVENT_COUNT>>>();
  };

  randomEventChances<POSSIBLE_EVENT_COUNT>(simulation, applyChoices, updateProbabilities);
}
}  // namespace internal

void getDamageRole(Simulation& simulation, PlayerSideId sideForDamageRolls) {
  ENTT_ASSERT(
    sideForDamageRolls != PlayerSideId::NONE,
    "The damage roll kinds are unique per side in the simulation's simulate turn options, so pick a side.");
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  DamageRollKind damageRollsConsidered = sideForDamageRolls == PlayerSideId::P1
                                           ? simulation.simulateTurnOptions.damageRollsConsidered.p1
                                           : simulation.simulateTurnOptions.damageRollsConsidered.p2;
  ENTT_ASSERT(
    damageRollsConsidered != DamageRollKind::NONE,
    "Cannot calculate damage without knowing what rolls to consider");

  if (calc_damage::damageKindsMatch(damageRollsConsidered, DamageRollKind::ALL_DAMAGE_ROLES)) {
    setRandomEqualChoice<pokesim::tags::SelectedForViewMove>(simulation);
    randomEqualChance(simulation, MechanicConstants::MAX_DAMAGE_ROLL_COUNT, [](Simulation& sim) {
      sim.viewForSelectedMoves<internal::applyDamageRollIndex>();
    });
    return;
  }

  types::damageRoll damageRollsUsed =
    (calc_damage::damageKindsMatch(damageRollsConsidered, DamageRollKind::AVERAGE_DAMAGE) ? 1 : 0) +
    (calc_damage::damageKindsMatch(damageRollsConsidered, DamageRollKind::MIN_DAMAGE) ? 1 : 0) +
    (calc_damage::damageKindsMatch(damageRollsConsidered, DamageRollKind::MAX_DAMAGE) ? 1 : 0);

  switch (damageRollsUsed) {
    case 1: {
      internal::runApplyDamageRollView<0U>(simulation, damageRollsConsidered);
      break;
    }
    case 2: {
      if (sideForDamageRolls == PlayerSideId::P1) {
        internal::handleDamageRollOptions<2U, PlayerSideId::P1>(simulation);
      }
      else {
        internal::handleDamageRollOptions<2U, PlayerSideId::P2>(simulation);
      }
      break;
    }
    case 3: {
      if (sideForDamageRolls == PlayerSideId::P1) {
        internal::handleDamageRollOptions<3U, PlayerSideId::P1>(simulation);
      }
      else {
        internal::handleDamageRollOptions<3U, PlayerSideId::P2>(simulation);
      }
      break;
    }
  }
}

void setIfMoveCrits(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  setReciprocalRandomBinaryChoice<calc_damage::CritChanceDivisor, pokesim::tags::SelectedForViewMove>(simulation);
  reciprocalRandomBinaryChance(simulation, [](Simulation& sim) {
    sim.addToEntities<calc_damage::tags::Crit, pokesim::tags::RandomEventCheckPassed>();
  });
}
}  // namespace pokesim::simulate_turn