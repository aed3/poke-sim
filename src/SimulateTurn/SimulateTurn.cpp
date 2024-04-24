#include "SimulateTurn.hpp"

#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Active.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/PP.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Turn.hpp>
#include <Simulation/MoveHitSteps.hpp>
#include <Simulation/Simulation.hpp>

#include "ManageActionQueue.hpp"

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  simulation.view<resolveDecision, tags::SimulateTurn>();
  simulation.registry.clear<SideDecision>();

  simulation.view<addBeforeTurnAction, tags::SimulateTurn>(entt::exclude_t<tags::BattleMidTurn>{});
  simulation.view<speedSort>();
  simulation.view<addResidualAction, tags::SimulateTurn>(entt::exclude_t<tags::BattleMidTurn>{});

  auto turnEntities = simulation.registry.view<Turn, tags::SimulateTurn>();
  simulation.registry.insert<tags::BattleMidTurn>(turnEntities.begin(), turnEntities.end());

  simulation.view<setActiveAction, tags::SimulateTurn>();
  while (!simulation.registry.view<action::tags::Active>().empty()) {
    runActiveAction(simulation);
    simulation.view<setActiveAction, tags::SimulateTurn>();
  }

  nextTurn(simulation);
}

void runActiveAction(Simulation& simulation) {
  runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);
}

void runBeforeTurnAction(Simulation& /*simulation*/) {}

void runMoveAction(Simulation& simulation) {
  simulation.view<setActiveTarget, tags::SimulateTurn>();
  simulation.view<setActiveMove, tags::SimulateTurn>();

  simulation.view<deductPp, tags::ActiveMove>();
  simulation.view<setLastMoveUsed, tags::ActiveMove>();

  useMove(simulation);
}

void runResidualAction(Simulation& /*simulation*/) {}

void nextTurn(Simulation& /*simulation*/) {}

void useMove(Simulation& simulation) {
  runMoveHitSteps(simulation);
}
}  // namespace pokesim::simulate_turn