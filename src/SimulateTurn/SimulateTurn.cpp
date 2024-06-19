#include "SimulateTurn.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/AddedTargets.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/PP.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Components/Turn.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/MoveHitSteps.hpp>
#include <Simulation/Simulation.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>

#include "ManageActionQueue.hpp"

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  internal::SelectForBattleView<tags::SimulateTurn> selectedBattle{simulation};

  updateSpeed(simulation);
  simulation.view<resolveDecision>();
  simulation.registry.clear<SideDecision>();

  // simulation.viewForSelectedBattles<addBeforeTurnAction, Tags<>, entt::exclude_t<tags::BattleMidTurn>>();
  simulation.viewForSelectedBattles<speedSort>();
  simulation.viewForSelectedBattles<addResidualAction, Tags<>, entt::exclude_t<tags::BattleMidTurn>>();

  auto turnEntities = simulation.registry.view<Turn, tags::SimulateTurn>();
  simulation.registry.insert<tags::BattleMidTurn>(turnEntities.begin(), turnEntities.end());

  simulation.viewForSelectedBattles<setCurrentAction>();
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    runCurrentAction(simulation);
    simulation.viewForSelectedBattles<setCurrentAction>();
  }

  nextTurn(simulation);
}

void runCurrentAction(Simulation& simulation) {
  // runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearCurrentAction(simulation);
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
  internal::SelectForBattleView<action::Move> selectedBattle{simulation};

  simulation.viewForSelectedBattles<setCurrentActionTarget>();
  simulation.viewForSelectedBattles<setCurrentActionSource>();
  simulation.viewForSelectedBattles<setCurrentActionMove>(simulation.pokedex);

  simulation.view<deductPp, Tags<tags::CurrentActionMoveSlot>>();
  simulation.view<setLastMoveUsed>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  internal::SelectForBattleView<action::tags::Residual> selectedBattle{simulation};
}

void nextTurn(Simulation& /*simulation*/) {}

void addTargetAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, targetSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void addUserAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, sourceSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void resolveMoveTargets(CurrentActionTargets&) {}

void getMoveTargets(Simulation& simulation) {
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<addTargetAllyToTargets, Tags<tags::CurrentActionMove, move::added_targets::tags::TargetAlly>>();
    simulation.view<addUserAllyToTargets, Tags<tags::CurrentActionMove, move::added_targets::tags::UserAlly>>();
  }
  simulation.view<resolveMoveTargets, Tags<tags::CurrentActionMove>, entt::exclude_t<AddedTargets>>();
}

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  // ModifyMove

  getMoveTargets(simulation);
  runMoveHitChecks(simulation);
}
}  // namespace pokesim::simulate_turn