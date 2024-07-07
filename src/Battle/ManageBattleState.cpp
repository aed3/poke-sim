#include "ManageBattleState.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void setCurrentActionSource(types::handle battleHandle, const Sides& sides, const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(action.val);
  types::entity sourceEntity = slotToPokemonEntity(registry, sides, sourceSlotName.name);

  battleHandle.emplace<CurrentActionSource>(sourceEntity);
  registry.emplace<tags::CurrentActionMoveSource>(sourceEntity);
}

void setCurrentActionTarget(
  types::handle battleHandle, const Sides& sides, const CurrentAction& action, const CurrentActionSource& source) {
  types::registry& registry = *battleHandle.registry();
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(action.val);
  types::entity targetEntity = slotToPokemonEntity(registry, sides, targetSlotName.name);

  if (!registry.any_of<tags::Fainted>(targetEntity)) {
    battleHandle.emplace<CurrentActionTargets>(std::initializer_list<types::entity>{targetEntity});
    registry.emplace<tags::CurrentActionMoveTarget>(targetEntity);
    registry.emplace<CurrentActionSource>(targetEntity, source);
  }
  else {
    // Set tag to get random target after this function exits
  }
}

void setCurrentActionMove(
  types::handle battleHandle, const CurrentActionSource& source, const CurrentActionTargets& targets,
  const CurrentAction& action, const Pokedex& pokedex) {
  types::registry& registry = *battleHandle.registry();
  const action::Move& move = registry.get<action::Move>(action.val);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(source.val);

  types::entity moveSlotEntity = moveToEntity(registry, moveSlots, move.name);

  ENTT_ASSERT(targets.val.size() == 1, "How did we get here with more or less than 1 target?");
  types::entity moveEntity =
    createActionMoveForTarget({registry, targets.val[0]}, battleHandle.entity(), source.val, move.name, pokedex);

  if (battleHandle.all_of<tags::SimulateTurn>()) {
    registry.emplace<tags::SimulateTurn>(moveEntity);
  }

  battleHandle.emplace<CurrentActionMoveSlot>(moveSlotEntity);
  registry.emplace<tags::CurrentActionMoveSlot>(moveSlotEntity);
}

void clearCurrentAction(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<CurrentAction>();
  registry.clear<CurrentActionTargets>();
  registry.clear<CurrentActionSource>();
  registry.clear<CurrentActionMove>();
  registry.clear<CurrentActionMoveSlot>();

  registry.clear<tags::CurrentActionMoveTarget>();
  registry.clear<tags::CurrentActionMoveSource>();
  registry.clear<tags::CurrentActionMoveSlot>();

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