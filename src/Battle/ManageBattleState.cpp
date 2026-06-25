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
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RecycledEntities.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Components/Tags/TargetTags.hpp>
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

template <typename View>
void clearActionMoveComponents(types::registry& registry, const View& view) {
  registry.remove<
    tags::SimulateTurn,
    tags::CalculateDamage,
    tags::AnalyzeEffect,
    Battle,
    TypeName,
    AtkBoost,
    DefBoost,
    SpaBoost,
    SpdBoost,
    SpeBoost,
    status::tags::Paralysis,
    status::tags::Burn>(view.begin(), view.end());
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
  }
  else {
    types::entity sourceEntity = battleHandle.get<CurrentActionSource>().val;
    battleHandle.remove<CurrentActionSource>();
    registry.remove<tags::CurrentActionMoveSource>(sourceEntity);
  }
}

void setFailedActionMove(
  types::handle moveHandle, Battle battle, CurrentActionSource source, CurrentActionTarget target) {
  types::registry& registry = *moveHandle.registry();

  bool removedAllMoves = true;
  removedAllMoves =
    removeFailedMove<tags::CurrentActionMoveTarget>(registry, moveHandle.entity(), target.val) && removedAllMoves;
  removedAllMoves =
    removeFailedMove<tags::CurrentActionMoveSource>(registry, moveHandle.entity(), source.val) && removedAllMoves;

  moveHandle.remove<tags::CurrentActionMove, tags::CurrentMoveHit, CurrentActionSource, CurrentActionTarget>();
  moveHandle.emplace<tags::FailedCurrentActionMove>();
  moveHandle.emplace<FailedCurrentActionSource>(source.val);
  moveHandle.emplace<FailedCurrentActionTarget>(target.val);

  if (removedAllMoves) {
    registry.remove<CurrentActionSource, CurrentActionTarget>(battle.val);
    registry.emplace<FailedCurrentActionSource>(battle.val, source.val);
    registry.emplace<FailedCurrentActionTarget>(battle.val, target.val);
  }

  registry.erase<CurrentActionMoveSlot>(battle.val);

  updateCurrentActionTargets(registry, registry.get<CurrentActionTargets>(battle.val));
}

void clearCurrentAction(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<
    CurrentAction,
    CurrentActionTargets,
    CurrentActionSource,
    CurrentActionTarget,
    FailedCurrentActionSource,
    FailedCurrentActionTarget,
    CurrentActionMovesAsSource,
    CurrentActionMovesAsTarget,
    CurrentActionMoveSlot,
    tags::CurrentActionMoveSource,
    tags::CurrentActionMoveTarget,
    tags::CurrentActionMoveSlot,
    tags::CurrentMoveHit,
    tags::FailedCurrentMoveHit>();

  registry.clear<
    move::effect::tags::Primary,
    move::effect::tags::Secondary,
    move::effect::tags::MoveSource,
    move::effect::tags::MoveTarget,
    move::tags::FuryAttack,
    move::tags::KnockOff,
    move::tags::Moonblast,
    move::tags::QuiverDance,
    move::tags::Splash,
    move::tags::Thunderbolt,
    move::tags::WillOWisp,
    move::tags::Physical,
    move::tags::Special,
    move::tags::Status,
    move::tags::Contact,
    move::tags::BypassSubstitute,
    move::tags::Punch,
    move::tags::VariableHitCount,
    BaseEffectChance,
    Accuracy,
    BasePower,
    HitCount,
    move::tags::AccuracyDependentHitCount,
    move::tags::Self,
    move::tags::AnySingleTarget,
    move::tags::AnySingleAlly>();

  registry.clear<SourceSlotName, TargetSlotName>();

  registry.clear<
    action::tags::Item,
    action::tags::Move,
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
    action::tags::Terastallize>();

  auto actionMoves = registry.view<tags::CurrentActionMove>();
  auto failedActionMoves = registry.view<tags::FailedCurrentActionMove>();
  clearActionMoveComponents(registry, actionMoves);
  clearActionMoveComponents(registry, failedActionMoves);
  registry.clear<tags::CurrentActionMove, tags::FailedCurrentActionMove>();

  simulation.removeFromEntities<MoveName, action::tags::Current>();
  registry.clear<action::tags::Current>();

  auto battles = registry.view<tags::Battle>();
  registry.remove<ItemName, MoveName>(battles.begin(), battles.end());
}
}  // namespace pokesim
