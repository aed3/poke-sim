#include "ManageBattleState.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void setCurrentActionTarget(types::handle battleHandle, const Sides& sides, const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(action.currentAction);
  types::entity targetEntity = slotToEntity(registry, sides, targetSlotName.targetSlot);

  if (!registry.any_of<tags::Fainted>(targetEntity)) {
    battleHandle.emplace<CurrentActionTarget>(targetEntity);
    registry.emplace<tags::CurrentActionMoveTarget>(targetEntity);
  }
  else {
    // Set tag to get random target after this function exits
  }
}

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(action.currentAction);
  types::entity sourceEntity = slotToEntity(registry, sides, sourceSlotName.sourceSlot);

  battleHandle.emplace<CurrentActionSource>(sourceEntity);
  registry.emplace<tags::CurrentActionMoveSource>(sourceEntity);
}

void setCurrentActionMove(types::handle battleHandle, const Pokedex& pokedex, const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const action::Move& move = registry.get<action::Move>(action.currentAction);

  types::entity moveEntity = pokedex.buildActionMove(move.name, registry);

  battleHandle.emplace<CurrentActionMove>(moveEntity);
  registry.emplace<tags::CurrentActionMove>(moveEntity);
}

void clearCurrentAction(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<CurrentAction>();
  registry.clear<CurrentActionTarget>();
  registry.clear<CurrentActionSource>();
  registry.clear<CurrentActionMove>();

  registry.clear<tags::CurrentActionMoveTarget>();
  registry.clear<tags::CurrentActionMoveSource>();

  auto actionMoves = registry.view<tags::CurrentActionMove>();
  registry.destroy(actionMoves.begin(), actionMoves.end());

  auto battles = simulation.selectedBattleEntities();
  registry.remove<
    action::Item,
    action::Move,
    action::tags::BeforeTurn,
    action::tags::Dynamax,
    action::tags::MegaEvolve,
    action::tags::MidTurnSwitchIn,
    action::tags::PostFoeFaintSwitchIn,
    action::tags::PreSwitchOut,
    action::tags::PreTurnSwitchIn,
    action::tags::PrimalRevert,
    action::tags::Residual,
    action::tags::RevivalBlessing,
    action::tags::Switch,
    action::tags::SwitchOut,
    action::tags::Terastallize>(battles.begin(), battles.end());
}
}  // namespace pokesim