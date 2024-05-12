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
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Turn.hpp>
#include <Simulation/MoveHitSteps.hpp>
#include <Simulation/Simulation.hpp>
#include <Utilities/SelectForView.hpp>

#include "ManageActionQueue.hpp"

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  internal::SelectForBattleView<tags::SimulateTurn> selectedBattle{simulation};

  updateSpeed(simulation);
  simulation.view<resolveDecision>();
  simulation.registry.clear<SideDecision>();

  // simulation.viewForSelectedBattles<addBeforeTurnAction>(entt::exclude_t<tags::BattleMidTurn>{});
  simulation.viewForSelectedBattles<speedSort>();
  simulation.viewForSelectedBattles<addResidualAction>(entt::exclude_t<tags::BattleMidTurn>{});

  auto turnEntities = simulation.registry.view<Turn, tags::SimulateTurn>();
  simulation.registry.insert<tags::BattleMidTurn>(turnEntities.begin(), turnEntities.end());

  simulation.viewForSelectedBattles<setActiveAction>();
  while (!simulation.registry.view<action::tags::Active>().empty()) {
    runActiveAction(simulation);
    simulation.viewForSelectedBattles<setActiveAction>();
  }

  nextTurn(simulation);
}

void runActiveAction(Simulation& simulation) {
  // runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearActive(simulation);
  // faint pokemon
  // Update
  // Switch requests

  if (!simulation.registry.view<tags::SpeedUpdateRequired>().empty()) {
    updateSpeed(simulation);
    simulation.viewForSelectedBattles<speedSort>();  // Should only speed sort battles affected
  }
}

void runBeforeTurnAction(Simulation& /*simulation*/) {
  // Barely used, will find different way of handling it
}

void runMoveAction(Simulation& simulation) {
  simulation.viewForSelectedBattles<setActiveTarget>();
  simulation.viewForSelectedBattles<setActiveMove>();

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