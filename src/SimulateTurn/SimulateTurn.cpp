#include "SimulateTurn.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/ActiveAction.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Turn.hpp>
#include <Simulation/Simulation.hpp>

#include "ManageActionQueue.hpp"

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  simulation.view<resolveDecision>();
  simulation.registry.clear<SideDecision>();

  simulation.view<addBeforeTurnAction>(entt::exclude_t<tags::BattleMidTurn>{});
  simulation.view<speedSort>();
  simulation.view<addResidualAction>(entt::exclude_t<tags::BattleMidTurn>{});

  auto turnEntities = simulation.registry.view<Turn>();
  simulation.registry.insert<tags::BattleMidTurn>(turnEntities.begin(), turnEntities.end());

  simulation.view<setActiveAction>();
  while (!simulation.registry.view<ActiveAction>().empty()) {
    runActiveAction(simulation);
    simulation.view<setActiveAction>();
  }

  nextTurn(simulation);
}

void runActiveAction(Simulation& simulation) {}
void nextTurn(Simulation& simulation) {}
}  // namespace pokesim::simulate_turn