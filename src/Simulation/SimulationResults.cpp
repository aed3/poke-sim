#include "SimulationResults.hpp"

#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Types/Entity.hpp>

#include "Simulation.hpp"

namespace pokesim {
namespace simulate_turn {
Results::Results(const Simulation& simulation_) : simulation(&simulation_) {}

types::view<tags::BattleOutcome> Results::battleOutcomes() const {
  return simulation->registry.view<tags::BattleOutcome>();
}

types::entityVector Results::rootBattles() const {
  auto view =
    simulation->registry.view<pokesim::tags::SimulateTurn, pokesim::tags::Battle>(entt::exclude_t<RootBattle>{});
  return {view.begin(), view.end()};
}

types::entityVector Results::rootBattleOutcomes(types::entity rootBattle) const {
  types::entityVector outcomes;
  for (auto [entity, rootBattleComponent] :
       simulation->registry.view<pokesim::tags::SimulateTurn, RootBattle>().each()) {
    if (rootBattleComponent.val == rootBattle) {
      outcomes.push_back(entity);
    }
  }

  return outcomes;
}
}  // namespace simulate_turn

namespace calc_damage {
Results::Results(const Simulation& simulation_) : simulation(&simulation_) {}

types::view<DamageRolls> Results::damageRollResults() const {
  return simulation->registry.view<DamageRolls>();
}

types::view<UsesUntilKo> Results::usesUntilKoResults() const {
  return simulation->registry.view<UsesUntilKo>();
}

types::view<AttackerHpRecovered> Results::hpRecoveredResults() const {
  return simulation->registry.view<AttackerHpRecovered>();
}

types::view<AttackerHpLost> Results::hpLostResults() const {
  return simulation->registry.view<AttackerHpLost>();
}
}  // namespace calc_damage

namespace analyze_effect {
Results::Results(const Simulation& simulation_) : simulation(&simulation_) {}

types::view<EffectMultiplier> Results::effectMultiplierResults() const {
  return simulation->registry.view<EffectMultiplier>();
}

types::view<MultipliedDamageRolls> Results::multipliedDamageRollsResults() const {
  return simulation->registry.view<MultipliedDamageRolls>();
}

types::view<MultipliedUsesUntilKo> Results::multipliedUsesUntilKoResults() const {
  return simulation->registry.view<MultipliedUsesUntilKo>();
}
}  // namespace analyze_effect
}  // namespace pokesim
