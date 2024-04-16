#include "SimulateTurn.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/SpeedSort.hpp>
#include <Simulation/Simulation.hpp>

#include "Actions.hpp"

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  simulation.view<resolveDecision>();
  simulation.registry.clear<SideDecision>();

  simulation.view<speedSort>();
}
}  // namespace pokesim::simulate_turn