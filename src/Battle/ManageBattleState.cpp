#include "ManageBattleState.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationResults.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void assignRootBattle(types::handle battleHandle) {
  const ParentBattle* parentBattle = battleHandle.try_get<ParentBattle>();
  types::entity rootBattle = parentBattle == nullptr ? battleHandle.entity() : parentBattle->val;
  battleHandle.emplace<RootBattle>(rootBattle);
}

void collectTurnOutcomeBattles(types::handle leafBattleHandle, const RootBattle& root) {
  leafBattleHandle.registry()->get_or_emplace<simulate_turn::TurnOutcomeBattles>(root.val).val.push_back(
    leafBattleHandle.entity());
}

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
    battleHandle.emplace<CurrentActionTargets>(types::targets<types::entity>{targetEntity});
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

  types::entity moveEntity =
    createActionMoveForTarget({registry, targets.only()}, battleHandle.entity(), source.val, move.name, pokedex);

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
  registry.clear<CurrentActionMovesAsSource>();
  registry.clear<CurrentActionMovesAsTarget>();
  registry.clear<CurrentActionMoveSlot>();

  registry.clear<tags::CurrentActionMoveTarget>();
  registry.clear<tags::CurrentActionMoveSource>();
  registry.clear<tags::CurrentActionMoveSlot>();

  auto actionMoves = registry.view<tags::CurrentActionMove>();
  registry.destroy(actionMoves.begin(), actionMoves.end());
  auto currentActions = registry.view<action::tags::Current>();
  registry.destroy(currentActions.begin(), currentActions.end());

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