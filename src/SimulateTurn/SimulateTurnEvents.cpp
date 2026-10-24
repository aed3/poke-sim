#include "SimulateTurnEvents.hpp"

#include <Components/EntityHolders/ActiveWithUpdateEvents.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Indexes.hpp>
#include <Utilities/EntityFilter.hpp>

#include "ManageActionQueue.hpp"

namespace pokesim::internal::simulate_turn {
namespace {
void setRunUpdate(
  types::registry& registry, const ActiveWithUpdateEvents& activeWithUpdateEvents, types::activePokemonIndex index) {
  if (index >= activeWithUpdateEvents.val.size()) {
    return;
  }
  registry.emplace<pokesim::internal::tags::RunUpdate>(activeWithUpdateEvents.val[index]);
}
}  // namespace

void runActiveUpdateEvents(Simulation& simulation) {
  auto battleFilter = pokesim::internal::EntityFilter<pokesim::tags::SimulateTurn, pokesim::tags::Battle>{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<speedSortActiveWithUpdateEvents>();
  resolveUpdateEventSpeedTies(simulation);
  types::activePokemonIndex totalPossibleActive = simulation.activePokemonSlotsPerSide() * Constants::SIDE_COUNT;
  for (types::activePokemonIndex index = 0U; index < totalPossibleActive; index++) {
    battleFilter.view<setRunUpdate>(index);
    runUpdate(simulation);
    simulation.removeFromEntities<pokesim::internal::tags::RunUpdate>();
  }
}
}  // namespace pokesim::internal::simulate_turn
