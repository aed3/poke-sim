#include "ManageBattleState.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/Accuracy.hpp>
#include <Components/AddedTargets.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/Current.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/HitCount.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/Priority.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RecycledEntities.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Pokedex/EnumToTag/MoveEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationResults.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <algorithm>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
namespace {
template <typename SlotName>
types::entity getActionEntity(const types::registry& registry, const Sides& sides, CurrentAction& action) {
  SlotName slotName = registry.get<SlotName>(action.action);
  return slotToPokemonEntity(registry, sides, slotName.val);
}

template <typename CurrentActionTag>
bool removeFailedMove(types::registry& registry, types::entity moveEntity, types::entity entity) {
  using CurrentActionMoves = std::conditional_t<
    std::is_same_v<CurrentActionTag, pokesim::tags::CurrentActionSource>,
    CurrentActionMovesAsSource,
    CurrentActionMovesAsTarget>;

  CurrentActionMoves* moves = registry.try_get<CurrentActionMoves>(entity);
  if (!moves) {
    return true;
  }
  auto newMovesEnd = std::remove(moves->val.begin(), moves->val.end(), moveEntity);
  moves->val.erase(newMovesEnd, moves->val.end());

  if (moves->val.empty()) {
    registry.remove<CurrentActionTag, CurrentActionMoves>(entity);
    return true;
  }
  return false;
}

void updateCurrentActionTargets(types::registry& registry, CurrentAction& action) {
  types::activePokemonIndex deleteCount = 0U;
  for (types::entity& target : action.targets) {
    if (!registry.all_of<pokesim::tags::CurrentActionTarget>(target)) {
      types::activePokemonIndex swapIndex = action.targets.size() - 1 - deleteCount;
      POKESIM_REQUIRE(swapIndex < action.targets.size(), "Swap index out of bounds.");
      std::swap(target, action.targets[swapIndex]);
      deleteCount++;
    }
  }

  action.targets.pop_count(deleteCount);
}

template <typename View>
void clearActionMoveComponents(types::registry& registry, const View& view) {
  registry.remove<
    pokesim::tags::SimulateTurn,
    pokesim::tags::CalculateDamage,
    pokesim::tags::AnalyzeEffect,
    Battle,
    TypeName,
    AtkBoost,
    DefBoost,
    SpaBoost,
    SpdBoost,
    SpeBoost,
    pokesim::status::tags::Paralysis,
    pokesim::status::tags::Burn>(view.begin(), view.end());
}

template <typename MoveTag>
struct ClearMoveTag {
  static void run(types::registry& registry) { registry.clear<MoveTag>(); }
};

void clearAction(Simulation& simulation) {
  simulation.registry.clear<
    CurrentActionSource,
    CurrentActionTarget,
    FailedCurrentActionSource,
    FailedCurrentActionTarget,
    pokesim::tags::CurrentActionSource,
    pokesim::tags::CurrentActionTarget>();
}
}  // namespace

void assignRootBattle(types::handle battleHandle) {
  const ParentBattle* parentBattle = battleHandle.try_get<ParentBattle>();
  types::entity rootBattle = parentBattle == nullptr ? battleHandle.entity() : parentBattle->val;
  battleHandle.emplace<RootBattle>(rootBattle);
}

void collectTurnOutcomeBattles(types::handle leafBattleHandle, RootBattle root) {
  leafBattleHandle.registry()->get_or_emplace<pokesim::simulate_turn::TurnOutcomeBattles>(root.val).val.push_back(
    leafBattleHandle.entity());
}

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  types::entity sourceEntity = getActionEntity<SourceSlotName>(registry, sides, action);

  action.source = sourceEntity;
  registry.emplace<pokesim::tags::CurrentActionSource>(sourceEntity);
}

void setCurrentActionSwitchSource(types::handle battleHandle, const Sides& sides, CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  types::entity sourceEntity = getActionEntity<SourceSlotName>(registry, sides, action);
  setCurrentActionSource(battleHandle, sides, action);

  bool sourceIsActive = registry.all_of<pokesim::tags::ActivePokemon>(sourceEntity);
  bool sourceIsFainted = registry.all_of<pokesim::tags::Fainted>(sourceEntity);
  if (sourceIsActive || sourceIsFainted) {
    registry.emplace<action::tags::ActiveSwitch>(sourceEntity);
    battleHandle.emplace<action::tags::ActiveSwitch>();
  }
  if (!sourceIsFainted) {
    registry.emplace<action::tags::NotFaintedActiveSwitch>(sourceEntity);
    battleHandle.emplace<action::tags::NotFaintedActiveSwitch>();
  }
}

void setCurrentActionSwitchTarget(types::handle battleHandle, const Sides& sides, CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  types::entity targetEntity = getActionEntity<TargetSlotName>(registry, sides, action);

  registry.emplace<pokesim::tags::CurrentActionTarget>(targetEntity);
  action.targets = {targetEntity};
}

void setCurrentActionMoveTarget(
  types::handle battleHandle, const Sides& sides, CurrentAction& action, const Simulation& simulation) {
  types::registry& registry = *battleHandle.registry();
  types::entity targetEntity = getActionEntity<TargetSlotName>(registry, sides, action);

  bool pickedTarget = false;
  if (!registry.any_of<pokesim::tags::Fainted>(targetEntity)) {
    pickedTarget = true;
  }
  else if (simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    // Set tag to get random target after this function exits
  }

  if (pickedTarget) {
    action.targets.push_back(targetEntity);
  }
  else {
    registry.remove<pokesim::tags::CurrentActionSource>(action.source);
  }
}

void setFailedActionMove(
  types::handle moveHandle, Battle battle, CurrentActionSource source, CurrentActionTarget target) {
  types::registry& registry = *moveHandle.registry();

  bool removedAllMoves = true;
  removedAllMoves =
    removeFailedMove<pokesim::tags::CurrentActionTarget>(registry, moveHandle.entity(), target.val) && removedAllMoves;
  removedAllMoves =
    removeFailedMove<pokesim::tags::CurrentActionSource>(registry, moveHandle.entity(), source.val) && removedAllMoves;

  moveHandle.remove<
    pokesim::tags::CurrentActionMove,
    pokesim::tags::CurrentMoveHit,
    CurrentActionSource,
    CurrentActionTarget>();
  moveHandle.emplace<pokesim::tags::FailedCurrentActionMove>();
  moveHandle.emplace<FailedCurrentActionSource>(source.val);
  moveHandle.emplace<FailedCurrentActionTarget>(target.val);

  if (removedAllMoves) {
    registry.remove<CurrentActionTarget>(battle.val);
    registry.emplace<FailedCurrentActionSource>(battle.val, source.val);
    registry.emplace<FailedCurrentActionTarget>(battle.val, target.val);
  }

  registry.erase<CurrentActionMoveSlot>(battle.val);

  updateCurrentActionTargets(registry, registry.get<CurrentAction>(battle.val));
}

void clearMoveAction(Simulation& simulation) {
  clearAction(simulation);

  types::registry& registry = simulation.registry;
  registry.clear<
    action::tags::Move,
    CurrentActionMovesAsSource,
    CurrentActionMovesAsTarget,
    CurrentActionMoveSlot,
    pokesim::tags::CurrentActionMoveSlot,
    pokesim::tags::CurrentMoveHit,
    pokesim::tags::FailedCurrentMoveHit>();

  registry.clear<
    pokesim::move::effect::tags::Primary,
    pokesim::move::effect::tags::Secondary,
    pokesim::move::effect::tags::MoveSource,
    pokesim::move::effect::tags::MoveTarget,
    pokesim::move::tags::Physical,
    pokesim::move::tags::Special,
    pokesim::move::tags::Status,
    pokesim::move::tags::Contact,
    pokesim::move::tags::BypassSubstitute,
    pokesim::move::tags::Punch,
    pokesim::move::tags::VariableHitCount,
    BaseEffectChance,
    Accuracy,
    BasePower,
    HitCount,
    pokesim::move::tags::AccuracyDependentHitCount,
    pokesim::move::tags::Self,
    pokesim::move::tags::AnySingleTarget,
    pokesim::move::tags::AnySingleAlly>();

  pokesim::dex::forEachMove<ClearMoveTag>(simulation.pokedex(), registry);

  auto actionMoves = registry.view<pokesim::tags::CurrentActionMove>();
  auto failedActionMoves = registry.view<pokesim::tags::FailedCurrentActionMove>();
  clearActionMoveComponents(registry, actionMoves);
  clearActionMoveComponents(registry, failedActionMoves);
  registry.clear<pokesim::tags::CurrentActionMove, pokesim::tags::FailedCurrentActionMove>();

  auto battles = registry.view<pokesim::tags::Battle>();
  registry.remove<MoveName>(battles.begin(), battles.end());
}

void clearSwitchAction(Simulation& simulation) {
  clearAction(simulation);
  simulation.registry.clear<action::tags::Switch, action::tags::ActiveSwitch, action::tags::NotFaintedActiveSwitch>();
}
}  // namespace pokesim::internal
