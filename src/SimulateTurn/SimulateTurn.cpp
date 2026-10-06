#include "SimulateTurn.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Helpers/InternalHelpers.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/Helpers.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/AddedTargets.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/Current.hpp>
#include <Components/DisabledMoveSlots.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/FaintQueue.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/LastUsedMove.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/SideDecisionOptions.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RecycledEntities.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SideTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Components/TeamRemaining.hpp>
#include <Components/Turn.hpp>
#include <Components/Winner.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/MoveHitSteps.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/EntityFilter.hpp>
#include <Utilities/Tags.hpp>

#include "Decisions.hpp"
#include "ManageActionQueue.hpp"
#include "SimulateTurnDebugChecks.hpp"

namespace pokesim::simulate_turn {
namespace {
auto getSimulateTurnFilter(Simulation& simulation) {
  return pokesim::internal::EntityFilter<pokesim::tags::SimulateTurn>{simulation};
}

auto getBattleFilter(Simulation& simulation) {
  return pokesim::internal::EntityFilter<pokesim::tags::SimulateTurn, pokesim::tags::Battle>{simulation};
}

template <typename Filter>
void speedSort(Filter battleFilter, Simulation& simulation) {
  battleFilter.template view<internal::simulate_turn::speedSort, Tags<tags::SpeedSortNeeded>>();
  battleFilter.template removeFromSelected<tags::SpeedSortNeeded>();
  internal::simulate_turn::resolveSpeedTies(simulation);
}

void midTurnSwitch(Simulation& simulation) {
  auto sideFilter = getSimulateTurnFilter(simulation);
  if (sideFilter.hasNoneSelected<MidTurnSideDecision>()) {
    return;
  }

  sideFilter.view<internal::simulate_turn::resolveMidTurnDecisions>();
  sideFilter.removeFromSelected<MidTurnSideDecision>();
  getBattleFilter(simulation).view<internal::simulate_turn::speedSortMidTurnSwitches>();
  internal::simulate_turn::resolveSpeedTies(simulation);
}

template <typename ActionTag>
void removeActionBySource(types::handle sourceHandle, Battle battle) {
  types::registry& registry = *sourceHandle.registry();
  registry.remove<ActionTag>(battle.val);
  registry.remove<CurrentActionSource>(battle.val);
  sourceHandle.remove<pokesim::tags::CurrentActionSource>();
}

template <typename ActionTag>
auto setSourceForAction(Simulation& simulation) {
  getBattleFilter(simulation).view<internal::setCurrentActionSource, Tags<ActionTag>>();

  simulation.view<removeActionBySource<ActionTag>, Tags<pokesim::tags::CurrentActionSource, pokesim::tags::Fainted>>();
  simulation.view<
    removeActionBySource<ActionTag>,
    Tags<pokesim::tags::CurrentActionSource>,
    entt::exclude_t<pokesim::tags::ActivePokemon>>();

  return pokesim::internal::EntityFilter<ActionTag, pokesim::tags::Battle>{simulation};
}

void addAddedTarget(types::registry& registry, Battle battle, Slot allySlot) {
  const Sides& sides = registry.get<Sides>(battle.val);
  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, allySlot);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentAction& action = registry.get<CurrentAction>(battle.val);

  POKESIM_REQUIRE(!action.targets.empty(), "Added targets should not be the first in the list of targets.");

  if (action.targets.size() == 1U) {
    registry.emplace<pokesim::tags::AddedRecycledActionMove1>(battle.val);
  }
  else {
    registry.emplace<pokesim::tags::AddedRecycledActionMove2>(battle.val);
  }

  action.targets.push_back(allyEntity);
}

void addTargetAllyToTargets(types::registry& registry, Battle battle) {
  TargetSlotName targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).action);
  addAddedTarget(registry, battle, targetSlotName.val);
}

void addUserAllyToTargets(types::registry& registry, Battle battle) {
  SourceSlotName sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).action);
  addAddedTarget(registry, battle, sourceSlotName.val);
}

void setTargetReferenceComponents(types::registry& registry, CurrentAction& action) {
  for (types::entity target : action.targets) {
    registry.emplace<pokesim::tags::CurrentActionTarget>(target);
    registry.emplace<CurrentActionSource>(target, action.source);
  }
}

template <typename RecycledActionMoveType>
void setActionMoveReferenceComponents(
  types::handle battleHandle, CurrentAction& action, RecycledActionMoveType actionMove) {
  types::registry& registry = *battleHandle.registry();
  types::handle actionMoveHandle{registry, actionMove.val};
  types::entity target;

  if (action.targets.empty()) {
    return;
  }

  if constexpr (std::is_same_v<RecycledActionMoveType, RecycledActionMove>) {
    target = action.targets[0];
  }
  else if constexpr (std::is_same_v<RecycledActionMoveType, AddedRecycledActionMove1>) {
    target = action.targets[1];
  }
  else if constexpr (std::is_same_v<RecycledActionMoveType, AddedRecycledActionMove2>) {
    target = action.targets[2];
  }
  else {
    POKESIM_REQUIRE_FAIL("Using a RecycledActionMoveType that isn't associated with a target.");
  }

  MoveName move = registry.get<MoveName>(action.action);
  internal::setupActionMoveBuild(
    registry,
    battleHandle.entity(),
    action.source,
    target,
    actionMove.val,
    move.val,
    false);
}

void setActionMoveData(Simulation& simulation) {
  simulation.addToEntities<pokesim::tags::SimulateTurn, pokesim::internal::tags::BuildActionMove>();
  simulation.pokedex().buildMoves(simulation.registry);
}

void setCurrentActionMoveSlot(types::handle handle, CurrentAction& action) {
  types::registry& registry = *handle.registry();
  MoveName move = registry.get<MoveName>(action.action);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(action.source);

  types::moveSlotIndex moveSlotIndex = moveToMoveSlot(moveSlots, move.val);
  handle.emplace<CurrentActionMoveSlot>(moveSlotIndex);
}

void setMoveTargets(Simulation& simulation) {
  pokesim::internal::EntityFilter<action::tags::Move, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<internal::setCurrentActionMoveTarget>(simulation);

  battleFilter.view<setActionMoveReferenceComponents<RecycledActionMove>>();
  setActionMoveData(simulation);
  simulation.removeFromEntities<pokesim::internal::tags::BuildActionMove>();

  internal::runModifyTarget(simulation);
  if (simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    simulation
      .view<addTargetAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::TargetAlly>>();
    simulation
      .view<addUserAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::SourceAlly>>();

    battleFilter.view<
      setActionMoveReferenceComponents<AddedRecycledActionMove1>,
      Tags<pokesim::tags::AddedRecycledActionMove1>>();
    battleFilter.view<
      setActionMoveReferenceComponents<AddedRecycledActionMove2>,
      Tags<pokesim::tags::AddedRecycledActionMove2>>();
    setActionMoveData(simulation);
    simulation.removeFromEntities<pokesim::tags::AddedRecycledActionMove1, pokesim::tags::Battle>();
    simulation.removeFromEntities<pokesim::tags::AddedRecycledActionMove2, pokesim::tags::Battle>();
    simulation.removeFromEntities<pokesim::internal::tags::BuildActionMove>();
  }

  battleFilter.view<setTargetReferenceComponents>();
}

void swapPokemonSlots(types::registry& registry, const CurrentAction& action, const Sides& sides) {
  auto [sourceSlot, targetSlot] = registry.get<SourceSlotName, TargetSlotName>(action.action);

  swapEntitySlots(registry, sides, sourceSlot.val, targetSlot.val);
}

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  internal::runModifyMove(simulation);

  internal::runMoveHitChecks(simulation);
  internal::runAfterMoveUsedEvent(simulation);
}

void runMoveAction(Simulation& simulation) {
  auto battleFilter = setSourceForAction<action::tags::Move>(simulation);
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  setMoveTargets(simulation);
  battleFilter.view<setCurrentActionMoveSlot>();

  internal::runBeforeMove(simulation);

  simulation.view<internal::setLastMoveUsed>();
  simulation.view<internal::deductPp, Tags<pokesim::tags::CurrentActionSource>>();

  useMove(simulation);

  internal::clearMoveAction(simulation);
}

void runSwitchAction(Simulation& simulation) {
  auto battleFilter = internal::EntityFilter<action::tags::Switch, pokesim::tags::Battle>{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<internal::setCurrentActionSwitchSource>();
  battleFilter.view<internal::setCurrentActionSwitchTarget>();
  internal::runBeforeSwitchOutEvent(simulation);
  internal::runEachUpdate(simulation);
  internal::runSwitchOutEvent(simulation);

  internal::runEndAbilityEvent(simulation);

  simulation.addToEntities<pokesim::internal::tags::EndItem, action::tags::NotFaintedActiveSwitch>();
  internal::runEndItemEvent(simulation);
  simulation.removeFromEntities<pokesim::internal::tags::EndItem>();

  simulation.addToEntities<
    pokesim::internal::tags::ClearVolatiles,
    pokesim::tags::CurrentActionSource,
    action::tags::NotFaintedActiveSwitch>();
  internal::clearVolatiles(simulation);
  simulation.removeFromEntities<pokesim::internal::tags::ClearVolatiles>();

  simulation.removeFromEntities<pokesim::tags::ActivePokemon, pokesim::tags::CurrentActionSource>();
  simulation.addToEntities<pokesim::tags::ActivePokemon, pokesim::tags::CurrentActionTarget>();
  battleFilter.view<swapPokemonSlots>();

  internal::runSwitchInEvent(simulation);

  internal::clearSwitchAction(simulation);
}

void runResidualAction(Simulation& simulation) {
  pokesim::internal::EntityFilter<action::tags::Residual, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  internal::runResidual(simulation);

  simulation.removeFromEntities<action::tags::Residual>();
}

void runBeforeTurnAction(Simulation&) {
  // Barely used, will find different way of handling it
  // simulation.removeFromEntities<action::tags::BeforeTurn>();
}

void setFainting(types::registry& registry, FaintQueue& faintQueue) {
  types::entity pokemon = faintQueue.val.front();
  for (types::activePokemonIndex i = 1U; i < faintQueue.val.size(); i++) {
    faintQueue.val[i - 1U] = faintQueue.val[i];
  }
  faintQueue.val.pop_back();
  registry.emplace<pokesim::tags::Fainting>(pokemon);

  Side side = registry.get<Side>(pokemon);
  registry.get<TeamRemaining>(side.val).val--;
  registry.get_or_emplace<pokesim::tags::SideFaintOnThisTurn>(side.val);
}

void clearFaintQueue(types::handle battleHandle, const FaintQueue& faintQueue) {
  if (faintQueue.val.empty()) {
    battleHandle.remove<FaintQueue>();
  }
}

void checkWin(types::handle battleHandle, const Sides& sides) {
  types::registry& registry = *battleHandle.registry();

  for (types::entity sideEntity : sides.val) {
    types::teamPositionIndex foesRemaining = registry.get<TeamRemaining>(sides.val.foe(sideEntity)).val;
    if (!foesRemaining) {
      battleHandle.emplace<Winner>(registry.get<PlayerSide>(sideEntity).val);
      battleHandle.get<ActionQueue>().val.clear();
      return;
    }
  }
}

void faintPokemon(Simulation& simulation) {
  auto battleFilter = getBattleFilter(simulation);
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  auto faintCallback = simulation.simulateTurnOptions.faintCallback;
  bool useFaintCallback = faintCallback.has_value();

  using LoopLimits = Constants::ActivePokemon;
  types::activePokemonIndex iterations = LoopLimits::MIN;
  while (!simulation.hasNone<FaintQueue>()) {
    POKESIM_REQUIRE(
      iterations <= LoopLimits::MAX,
      "More Pokemon were queued to faint in at least one battle than possible.");

    battleFilter.view<setFainting>();

    pokesim::internal::EntityFilter<pokesim::tags::Fainting> pokemonFilter{simulation};
    POKESIM_REQUIRE(
      !pokemonFilter.hasNoneSelected(),
      "This loop should only be run if setFainting had Pokemon to set as fainting.");

    internal::runFaintEvent(simulation);
    internal::runEndAbilityEvent(simulation);

    pokemonFilter.addToSelected<pokesim::internal::tags::EndItem>();
    internal::runEndItemEvent(simulation);
    simulation.removeFromEntities<pokesim::internal::tags::EndItem>();

    pokemonFilter.addToSelected<pokesim::internal::tags::ClearVolatiles>();
    internal::clearVolatiles(simulation);
    simulation.removeFromEntities<pokesim::internal::tags::ClearVolatiles>();

    simulation.addToEntities<pokesim::tags::Fainted, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::ActivePokemon, pokesim::tags::Fainting>();

    if (useFaintCallback) {
      faintCallback.value()(simulation);
    }

    simulation.removeFromEntities<pokesim::tags::Fainting>();
    battleFilter.view<clearFaintQueue>();
    iterations++;
  }

  if (iterations != LoopLimits::MIN) {
    battleFilter.view<checkWin, Tags<>, entt::exclude_t<Winner>>();
  }

  internal::runAfterFaintEvent(simulation);
}

void requestMidTurnDecision(types::registry& registry, Battle battle, Side side) {
  registry.get_or_emplace<pokesim::MidTurnDecisionsRequested>(battle.val).val++;
  registry.get_or_emplace<pokesim::MidTurnDecisionsRequested>(side.val).val++;
}

void requestMidTurnSwitch(Simulation& simulation) {
  internal::EntityFilter<pokesim::tags::Switching, pokesim::tags::SimulateTurn> switchingFilter{simulation};
  switchingFilter.addToSelected<pokesim::tags::RequestingMidTurnDecision>();
  switchingFilter.view<requestMidTurnDecision>();
}

void runCurrentAction(Simulation& simulation) {
  runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runSwitchAction(simulation);
  runResidualAction(simulation);

  simulation.removeFromEntities<MoveName, action::tags::Current>();
  simulation.registry.clear<CurrentAction, SourceSlotName, TargetSlotName, action::tags::Current>();

  faintPokemon(simulation);
  requestMidTurnSwitch(simulation);

  // Update

  internal::updateAllStats(simulation);
  speedSort(getBattleFilter(simulation), simulation);
}

void incrementTurn(Turn& turn) {
  turn.val++;
}

void setActiveAtTurnEnd(types::handle handle, Battle battle) {
  if (handle.registry()->any_of<pokesim::tags::BattleMidTurn, Winner>(battle.val)) {
    return;
  }

  handle.emplace<pokesim::internal::tags::ActiveAtTurnEnd>();
}

void progressFaintedOnTurn(types::handle handle, Battle battle) {
  if (handle.registry()->all_of<pokesim::tags::BattleMidTurn>(battle.val)) {
    return;
  }

  handle.remove<pokesim::tags::SideFaintOnThisTurn>();

  // if (!handle.registry()->all_of<Winner>(battle.val)) {
  //   handle.emplace<pokesim::tags::SideHadFaintOnLastTurn>();
  // }
}

void setFaintedToSwitchOut(
  types::registry& registry, Battle battle, const Team& team, TeamRemaining teamRemaining,
  types::activePokemonIndex activePerSide) {
  if (registry.all_of<Winner>(battle.val)) {
    return;
  }

  types::activePokemonIndex active = 0U;
  for (types::activePokemonIndex i = 0U; i < team.val.size() && i < activePerSide; i++) {
    if (!registry.all_of<pokesim::tags::Fainted>(team.val[i])) {
      active++;
    }
  }

  POKESIM_REQUIRE(active <= teamRemaining.val, "Cannot have more team members active than remaining.");
  if (active == teamRemaining.val) {
    return;
  }

  for (types::activePokemonIndex i = 0U; i < team.val.size() && i < activePerSide && i < teamRemaining.val; i++) {
    if (registry.all_of<pokesim::tags::Fainted>(team.val[i])) {
      registry.emplace<pokesim::tags::Switching>(team.val[i]);
    }
  }
}

void nextTurn(Simulation& simulation) {
  getBattleFilter(simulation).view<incrementTurn, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn, Winner>>();
  auto simulateTurnFilter = getSimulateTurnFilter(simulation);

  simulateTurnFilter.view<setActiveAtTurnEnd, Tags<pokesim::tags::ActivePokemon>>();
  simulateTurnFilter.view<progressFaintedOnTurn, Tags<pokesim::tags::SideFaintOnThisTurn>>();

  pokesim::internal::EntityFilter<pokesim::internal::tags::ActiveAtTurnEnd> pokemonFilter{simulation};
  if (pokemonFilter.hasNoneSelected()) {
    return;
  }
  pokemonFilter.removeFromSelected<DisabledMoveSlots>();

  internal::runResetDisabledMove(simulation);
  internal::runResetTrappedPokemon(simulation);

  simulation.removeFromEntities<pokesim::internal::tags::ActiveAtTurnEnd>();
}

void cloneToPreventInputChanges(Simulation& simulation) {
  getBattleFilter(simulation).addToSelected<pokesim::tags::CloneFrom>();
  const auto entityMap = clone(simulation.registry, 1U);
  for (const auto& inputBattleMapping : entityMap) {
    types::entity original = inputBattleMapping.first;
    if (simulation.registry.all_of<pokesim::tags::SimulateTurn>(original)) {
      simulation.registry.emplace<internal::simulate_turn::tags::Input>(original);
      simulation.registry.remove<pokesim::tags::SimulateTurn>(original);
    }
  }
}

void simulateTurn(Simulation& simulation) {
  const auto& options = simulation.simulateTurnOptions;
  auto simulateTurnFilter = getSimulateTurnFilter(simulation);
  if (simulateTurnFilter.hasNoneSelected()) {
    return;
  }

  auto battleFilter = getBattleFilter(simulation);
  if (battleFilter.hasNoneSelected()) {
    return;
  }

#ifndef POKESIM_ALL_DAMAGE_ALL_BRANCHES
  POKESIM_REQUIRE(
    !options.getMakeBranchesOnRandomEvents() ||
      !(options.getDamageRollsConsidered().getP1() & DamageRollKind::ALL_DAMAGE_ROLLS ||
        options.getDamageRollsConsidered().getP2() & DamageRollKind::ALL_DAMAGE_ROLLS),
    "Creating a branch for every damage roll is disabled by default to prevent easily reaching the battle count limit. "
    "Rebuild PokeSim with the flag POKESIM_ALL_DAMAGE_ALL_BRANCHES to enable this option combination.");
#endif

  simulation.removeFromEntities<tags::BattleOutcome>();

  if (!options.getApplyChangesToInputBattle()) {
    cloneToPreventInputChanges(simulation);
  }

  internal::updateAllStats(simulation);

  midTurnSwitch(simulation);
  simulateTurnFilter.view<internal::simulate_turn::resolveDecision>();
  simulateTurnFilter.removeFromSelected<SideDecision>();
  if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
    simulateTurnFilter.removeFromSelected<SinglesSideOptions>();
  }
  else {
    simulateTurnFilter.removeFromSelected<DoublesSideOptions>();
  }

  // battleFilter.view<internal::simulate_turn::addBeforeTurnAction, Tags<>,
  // entt::exclude_t<pokesim::tags::BattleMidTurn>>();
  speedSort(battleFilter, simulation);
  battleFilter
    .view<internal::simulate_turn::addResidualAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();

  battleFilter.addToSelectedWithExclude<pokesim::tags::BattleMidTurn>(entt::exclude<pokesim::tags::BattleMidTurn>);

  bool triedEndTurnSwitches = false;
  bool useDecisionCallback = options.decisionCallback.has_value();
  using ActionsLimit = Constants::ActionQueueLength;
  for (types::actionQueueIndex actionsTaken = ActionsLimit::MIN; actionsTaken <= ActionsLimit::MAX; actionsTaken++) {
    POKESIM_REQUIRE(
      actionsTaken < ActionsLimit::MAX,
      "More actions in a turn were queued to be taken than are possible in at least one battle.");

    battleFilter.view<
      internal::simulate_turn::setCurrentAction,
      Tags<>,
      entt::exclude_t<Winner, pokesim::MidTurnDecisionsRequested>>();

    if (simulation.hasNone<action::tags::Current>()) {
      if (triedEndTurnSwitches || simulateTurnFilter.hasNoneSelected<pokesim::tags::SideFaintOnThisTurn>()) {
        break;
      }

      triedEndTurnSwitches = true;
      simulateTurnFilter.view<setFaintedToSwitchOut, Tags<pokesim::tags::SideFaintOnThisTurn>>(
        simulation.isBattleFormat(BattleFormat::SINGLES) ? Constants::ActivePokemonSlotsPerSide::SINGLES
                                                         : Constants::ActivePokemonSlotsPerSide::DOUBLES);
    }

    runCurrentAction(simulation);

    if (useDecisionCallback && !simulation.hasNone<pokesim::tags::RequestingMidTurnDecision>()) {
      options.decisionCallback.value()(simulation);
      midTurnSwitch(simulation);
    }

    if (!useDecisionCallback && triedEndTurnSwitches) {
      break;
    }
  }

  battleFilter.removeFromSelected<pokesim::tags::BattleMidTurn>(entt::exclude<pokesim::MidTurnDecisionsRequested>);
  battleFilter.removeFromSelected<pokesim::tags::BattleMidTurn, Winner>();
  nextTurn(simulation);

  battleFilter.addToSelected<tags::BattleOutcome>();
  simulation.addToEntities<pokesim::tags::SimulateTurn, internal::simulate_turn::tags::Input>();
  simulation.removeFromEntities<internal::simulate_turn::tags::Input>();
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks{simulation};
  debugChecks.checkInputs();

  simulateTurn(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::simulate_turn
