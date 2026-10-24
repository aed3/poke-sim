#include "ManageBattleState.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Pokemon/ManagePokemonTags.hpp>
#include <Components/Accuracy.hpp>
#include <Components/AddedTargets.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/Current.hpp>
#include <Components/EffectStack.hpp>
#include <Components/Effects/AddedFlinchChance.hpp>
#include <Components/EntityHolders/ActiveWithUpdateEvents.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/HitCount.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/Pokedex/CritStageBoost.hpp>
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
#include <Components/Tags/TargetTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Components/TeamRemaining.hpp>
#include <Pokedex/Effects/headers.hpp>
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

bool removeFailedMoveFromSource(types::registry& registry, types::entity moveEntity, types::entity source) {
  POKESIM_REQUIRE(
    !registry.all_of<CurrentActionMovesAsSourceExtended>(source),
    "This function is only meant for simulate turn and the non-extended version of source moves.");

  CurrentActionMovesAsSource* moves = registry.try_get<CurrentActionMovesAsSource>(source);
  if (!moves) {
    return true;
  }

  moves->val.unordered_remove(moveEntity);

  if (moves->val.empty()) {
    registry.remove<pokesim::tags::CurrentActionSource, CurrentActionMovesAsSource>(source);
    return true;
  }
  return false;
}

void removeFailedMoveFromTarget(types::registry& registry, types::entity target) {
  POKESIM_REQUIRE(
    !registry.all_of<CurrentActionMovesAsTargetExtended>(target),
    "This function is only meant for simulate turn and the non-extended version of target moves.");

  registry.remove<pokesim::tags::CurrentActionTarget, CurrentActionMovesAsTarget>(target);
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

template <typename... OtherComponents, typename View>
void clearEffectComponents(types::registry& registry, const View& view) {
  registry.remove<
    OtherComponents...,
    AtkBoost,
    DefBoost,
    SpaBoost,
    SpdBoost,
    SpeBoost,
    AccBoost,
    EvaBoost,
    AddedFlinchChance,
    dex::Flinch,
    dex::Paralysis,
    dex::Burn>(view.begin(), view.end());
}

template <typename View>
void clearActionMoveComponents(types::registry& registry, const View& view) {
  clearEffectComponents<pokesim::tags::SimulateTurn, Battle, TypeName>(registry, view);
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

void setNextInEffectStack(types::handle handle, EffectStack& effectStack) {
  EffectStackItem item = effectStack.val.back();
  handle.emplace<CurrentEffectSource>(item.source);
  handle.emplace<CurrentEffectTarget>(item.target);
  handle.registry()->emplace<CurrentEffectsAsSource>(item.source).val.push_back(handle.entity());
  handle.registry()->emplace<CurrentEffectAsTarget>(item.target, handle.entity());

  auto effect = item.effect;
  if (effect.holds<AtkBoost>()) {
    handle.emplace<AtkBoost>(effect.get<AtkBoost>());
  }
  else if (effect.holds<SpaBoost>()) {
    handle.emplace<SpaBoost>(effect.get<SpaBoost>());
  }
  else {
    POKESIM_REQUIRE_FAIL(
      "EffectStackItem has unimplemented effect of type index " + std::to_string(effect.index()) + ".");
  }

  effectStack.val.pop_back();
  if (effectStack.val.empty()) {
    handle.remove<EffectStack>();
  }
}
}  // namespace

bool sideHasPossibleSwitch(types::registry& registry, Side side, const Simulation& simulation) {
  TeamRemaining teamRemaining = registry.get<TeamRemaining>(side.val);
  return teamRemaining.val > simulation.activePokemonSlotsPerSide();
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

  bool removedAllMoves = removeFailedMoveFromSource(registry, moveHandle.entity(), source.val);
  removeFailedMoveFromTarget(registry, target.val);

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

  if (moveHandle.all_of<pokesim::tags::SimulateTurn>()) {
    registry.erase<CurrentActionMoveSlot>(battle.val);
    updateCurrentActionTargets(registry, registry.get<CurrentAction>(battle.val));
  }
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
    pokesim::move::tags::BypassProtect,
    pokesim::move::tags::BypassSubstitute,
    pokesim::move::tags::Contact,
    pokesim::move::tags::IgnoreImmunities,
    pokesim::move::tags::Punch,
    pokesim::move::tags::SelfSwitch,
    pokesim::move::tags::Sound,
    pokesim::move::tags::VariableHitCount,
    CritStageBoost,
    BaseEffectChance,
    Accuracy,
    BasePower,
    HitCount>();

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

void clearCurrentEffect(Simulation& simulation) {
  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectAsTarget>();
}

void setCurrentEffectFromStack(Simulation& simulation) {
  POKESIM_REQUIRE(simulation.hasNone<CurrentEffectSource>(), "An effect cannot be set while others are active.");
  POKESIM_REQUIRE(simulation.hasNone<CurrentEffectTarget>(), "An effect cannot be set while others are active.");
  POKESIM_REQUIRE(simulation.hasNone<CurrentEffectsAsSource>(), "An effect cannot be set while others are active.");
  POKESIM_REQUIRE(simulation.hasNone<CurrentEffectAsTarget>(), "An effect cannot be set while others are active.");

  simulation.view<setNextInEffectStack>();
}

void clearCurrentBattleEffect(Simulation& simulation) {
  clearEffectComponents(simulation.registry, simulation.registry.view<pokesim::tags::Battle>());
  clearCurrentEffect(simulation);
}

void removeFromActiveWithUpdateEvents(types::handle pokemonHandle, Battle battle) {
  types::registry& registry = *pokemonHandle.registry();
  ActiveWithUpdateEvents* activeWithUpdateEvents = registry.try_get<ActiveWithUpdateEvents>(battle.val);
  if (!activeWithUpdateEvents) {
    return;
  }

  activeWithUpdateEvents->val.unordered_remove(pokemonHandle.entity());
  if (activeWithUpdateEvents->val.empty()) {
    registry.remove<ActiveWithUpdateEvents>(battle.val);
  }
}

void tryAddToActiveWithUpdateEvents(types::handle pokemonHandle, Battle battle) {
  if (!hasUpdateEvent(pokemonHandle)) {
    return;
  }

  ActiveWithUpdateEvents& activeWithUpdateEvents =
    pokemonHandle.registry()->get_or_emplace<ActiveWithUpdateEvents>(battle.val);

  POKESIM_REQUIRE(
    std::find(activeWithUpdateEvents.val.begin(), activeWithUpdateEvents.val.end(), pokemonHandle.entity()) ==
      activeWithUpdateEvents.val.end(),
    "Pokemon should only be in this list once.");

  activeWithUpdateEvents.val.push_back(pokemonHandle.entity());
}

void checkIfRemoveFromActiveWithUpdateEvents(types::handle pokemonHandle, Battle battle) {
  if (!hasUpdateEvent(pokemonHandle)) {
    removeFromActiveWithUpdateEvents(pokemonHandle, battle);
  }
}

}  // namespace pokesim::internal
