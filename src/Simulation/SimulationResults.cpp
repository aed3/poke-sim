#include "SimulationResults.hpp"

#include <Components/HitCount.hpp>
#include <Components/SimulationResults.hpp>
#include <Types/Entity.hpp>

#include "Simulation.hpp"

namespace pokesim {
namespace simulate_turn {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<TurnOutcomeBattles> Results::turnOutcomeBattlesResults() const {
  return simulation.registry.view<TurnOutcomeBattles>();
}
}  // namespace simulate_turn

namespace calc_damage {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<MaxDamage> Results::maxDamageResults() const {
  return simulation.registry.view<MaxDamage>();
}

types::view<MinUsesUntilKo> Results::minUsesUntilKoResults() const {
  return simulation.registry.view<MinUsesUntilKo>();
}

types::view<AttackerHpRecovered> Results::hpRecoveredResults() const {
  return simulation.registry.view<AttackerHpRecovered>();
}

types::view<AttackerHpLost> Results::hpLostResults() const {
  return simulation.registry.view<AttackerHpLost>();
}

types::view<HitCount> Results::hitCountResults() const {
  return simulation.registry.view<HitCount>();
}
}  // namespace calc_damage

namespace analyze_effect {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<EffectMultiplier> Results::effectMultiplierResults() const {
  return simulation.registry.view<EffectMultiplier>();
}

types::view<MultipliedMaxDamage> Results::multipliedMaxDamageResults() const {
  return simulation.registry.view<MultipliedMaxDamage>();
}

types::view<MultipliedDamageRolls> Results::multipliedDamageRollsResults() const {
  return simulation.registry.view<MultipliedDamageRolls>();
}

types::view<MultipliedKoChance> Results::multipliedKoChanceResults() const {
  return simulation.registry.view<MultipliedKoChance>();
}
}  // namespace analyze_effect
}  // namespace pokesim