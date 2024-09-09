#include "SimulationResults.hpp"

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

types::view<UsesUntilKo> Results::usesUntilKoResults() const {
  return simulation.registry.view<UsesUntilKo>();
}

types::view<AttackerHpRecovered> Results::hpRecoveredResults() const {
  return simulation.registry.view<AttackerHpRecovered>();
}

types::view<AttackerHpLost> Results::hpLostResults() const {
  return simulation.registry.view<AttackerHpLost>();
}
}  // namespace calc_damage

namespace analyze_effect {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<EffectMultiplier> Results::effectMultiplierResults() const {
  return simulation.registry.view<EffectMultiplier>();
}

types::view<MultipliedDamage> Results::multipliedMaxDamageResults() const {
  return simulation.registry.view<MultipliedDamage>();
}

types::view<MultipliedDamageRolls> Results::multipliedDamageRollsResults() const {
  return simulation.registry.view<MultipliedDamageRolls>();
}

types::view<MultipliedUsesUntilKo> Results::multipliedUsesUntilKoResults() const {
  return simulation.registry.view<MultipliedUsesUntilKo>();
}
}  // namespace analyze_effect
}  // namespace pokesim