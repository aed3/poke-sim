#include "ManageBattleState.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
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
#include <algorithm>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
namespace {
template <typename CurrentActionTag>
bool removeFailedMove(types::registry& registry, types::entity moveEntity, types::entity entity) {
  using CurrentActionMoves = std::conditional_t<
    std::is_same_v<CurrentActionTag, tags::CurrentActionMoveSource>,
    CurrentActionMovesAsSource,
    CurrentActionMovesAsTarget>;

  CurrentActionMoves& moves = registry.get<CurrentActionMoves>(entity);
  auto newMovesEnd = std::remove(moves.val.begin(), moves.val.end(), moveEntity);
  moves.val.erase(newMovesEnd, moves.val.end());

  if (moves.val.empty()) {
    registry.remove<CurrentActionTag, tags::CurrentActionMoveSource>(entity);
    return true;
  }
  return false;
}

void updateCurrentActionTargets(types::registry& registry, CurrentActionTargets& targets) {
  types::activePokemonIndex deleteCount = 0U;
  for (types::entity& target : targets.val) {
    if (!registry.all_of<tags::CurrentActionMoveTarget>(target)) {
      types::activePokemonIndex swapIndex = targets.val.size() - 1 - deleteCount;
      POKESIM_REQUIRE(swapIndex < targets.val.size(), "Swap index out of bounds.");
      std::swap(target, targets.val[swapIndex]);
      deleteCount++;
    }
  }

  targets.val.pop_count(deleteCount);
}
}  // namespace

void assignRootBattle(types::handle battleHandle) {
  const ParentBattle* parentBattle = battleHandle.try_get<ParentBattle>();
  types::entity rootBattle = parentBattle == nullptr ? battleHandle.entity() : parentBattle->val;
  battleHandle.emplace<RootBattle>(rootBattle);
}

void collectTurnOutcomeBattles(types::handle leafBattleHandle, const RootBattle& root) {
  leafBattleHandle.registry()->get_or_emplace<simulate_turn::TurnOutcomeBattles>(root.val).val.push_back(
    leafBattleHandle.entity());
}

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, CurrentAction action) {
  types::registry& registry = *battleHandle.registry();
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(action.val);
  types::entity sourceEntity = slotToPokemonEntity(registry, sides, sourceSlotName.val);

  battleHandle.emplace<CurrentActionSource>(sourceEntity);
  registry.emplace<tags::CurrentActionMoveSource>(sourceEntity);
}

void setCurrentActionTarget(
  types::handle battleHandle, const Sides& sides, CurrentAction action, CurrentActionSource source,
  const Simulation& simulation) {
  types::registry& registry = *battleHandle.registry();
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(action.val);
  types::entity targetEntity = slotToPokemonEntity(registry, sides, targetSlotName.val);

  bool pickedTarget = false;
  if (!registry.any_of<tags::Fainted>(targetEntity)) {
    pickedTarget = true;
  }
  else if (simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    // Set tag to get random target after this function exits
  }

  if (pickedTarget) {
    battleHandle.emplace<CurrentActionTargets>(types::targets<types::entity>{targetEntity});
    registry.emplace<tags::CurrentActionMoveTarget>(targetEntity);
    registry.emplace<CurrentActionSource>(targetEntity, source);
  }
  else {
    types::entity sourceEntity = battleHandle.get<CurrentActionSource>().val;
    battleHandle.remove<CurrentActionSource>();
    registry.remove<tags::CurrentActionMoveSource>(sourceEntity);
  }
}

void setCurrentActionMove(
  types::handle battleHandle, CurrentActionSource source, const CurrentActionTargets& targets, CurrentAction action,
  const Pokedex& pokedex) {
  types::registry& registry = *battleHandle.registry();
  const MoveName& move = registry.get<MoveName>(action.val);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(source.val);

  for (types::entity target : targets.val) {
    createActionMoveForTarget({registry, target}, battleHandle.entity(), source.val, move.val, pokedex);
  }

  types::entity moveSlotEntity = moveToEntity(registry, moveSlots, move.val);
  battleHandle.emplace<CurrentActionMoveSlot>(moveSlotEntity);
  registry.emplace<tags::CurrentActionMoveSlot>(moveSlotEntity);
}

void setFailedActionMove(
  types::handle moveHandle, Battle battle, CurrentActionSource source, CurrentActionTarget target) {
  types::registry& registry = *moveHandle.registry();

  bool removedAllMoves = true;
  removedAllMoves =
    removeFailedMove<tags::CurrentActionMoveTarget>(registry, moveHandle.entity(), target.val) && removedAllMoves;
  removedAllMoves =
    removeFailedMove<tags::CurrentActionMoveSource>(registry, moveHandle.entity(), source.val) && removedAllMoves;

  moveHandle.remove<tags::CurrentActionMove, CurrentActionSource, CurrentActionTarget>();
  moveHandle.emplace<tags::FailedCurrentActionMove>();
  moveHandle.emplace<FailedCurrentActionSource>(source.val);
  moveHandle.emplace<FailedCurrentActionTarget>(target.val);

  if (removedAllMoves) {
    registry.remove<CurrentActionSource, CurrentActionTarget>(battle.val);
    registry.emplace<FailedCurrentActionSource>(battle.val, source.val);
    registry.emplace<FailedCurrentActionTarget>(battle.val, target.val);
  }

  types::entity moveSlotEntity = registry.get<CurrentActionMoveSlot>(battle.val).val;
  registry.erase<CurrentActionMoveSlot>(battle.val);
  registry.erase<tags::CurrentActionMoveSlot>(moveSlotEntity);

  updateCurrentActionTargets(registry, registry.get<CurrentActionTargets>(battle.val));
}

void clearCurrentAction(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<CurrentAction>();
  registry.clear<CurrentActionTargets>();
  registry.clear<CurrentActionSource>();
  registry.clear<CurrentActionTarget>();
  registry.clear<FailedCurrentActionSource>();
  registry.clear<FailedCurrentActionTarget>();
  registry.clear<CurrentActionMovesAsSource>();
  registry.clear<CurrentActionMovesAsTarget>();
  registry.clear<CurrentActionMoveSlot>();

  registry.clear<tags::CurrentActionMoveSource>();
  registry.clear<tags::CurrentActionMoveTarget>();
  registry.clear<tags::CurrentActionMoveSlot>();

  auto actionMoves = registry.view<tags::CurrentActionMove>();
  auto failedActionMoves = registry.view<tags::FailedCurrentActionMove>();
  auto currentActions = registry.view<action::tags::Current>();
  registry.destroy(actionMoves.begin(), actionMoves.end());
  registry.destroy(failedActionMoves.begin(), failedActionMoves.end());
  registry.destroy(currentActions.begin(), currentActions.end());

  auto battles = simulation.selectedBattleEntities();
  registry.remove<
    action::tags::Item,
    ItemName,
    action::tags::Move,
    MoveName,
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
