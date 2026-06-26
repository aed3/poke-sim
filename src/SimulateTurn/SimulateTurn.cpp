#include "SimulateTurn.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Side/ManageSideState.hpp>
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
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/LastUsedMove.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RecycledEntities.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/TargetTags.hpp>
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

#include "ManageActionQueue.hpp"
#include "SimulateTurnDebugChecks.hpp"

namespace pokesim::simulate_turn {
namespace {
auto getBattleFilter(Simulation& simulation) {
  return pokesim::internal::EntityFilter<pokesim::tags::SimulateTurn, pokesim::tags::Battle>{simulation};
}

void addAddedTarget(types::registry& registry, Battle battle, Slot allySlot) {
  const Sides& sides = registry.get<Sides>(battle.val);
  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, allySlot);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);

  POKESIM_REQUIRE(!targets.val.empty(), "Added targets should not be the first in the list of targets.");

  if (targets.val.size() == 1U) {
    registry.emplace<pokesim::tags::AddedRecycledActionMove1>(battle.val);
  }
  else {
    registry.emplace<pokesim::tags::AddedRecycledActionMove2>(battle.val);
  }

  targets.val.push_back(allyEntity);
}

void addTargetAllyToTargets(types::registry& registry, Battle battle) {
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).val);
  addAddedTarget(registry, battle, targetSlotName.val);
}

void addUserAllyToTargets(types::registry& registry, const Battle& battle) {
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).val);
  addAddedTarget(registry, battle, sourceSlotName.val);
}

void setTargetReferenceComponents(
  types::registry& registry, const CurrentActionTargets& targets, CurrentActionSource source) {
  for (types::entity target : targets.val) {
    registry.emplace<pokesim::tags::CurrentActionMoveTarget>(target);
    registry.emplace<CurrentActionSource>(target, source.val);
  }
}

template <typename RecycledActionMoveType>
void setActionMoveReferenceComponents(
  types::handle battleHandle, CurrentActionSource source, CurrentActionTargets targets, CurrentAction action,
  RecycledActionMoveType actionMove) {
  types::registry& registry = *battleHandle.registry();
  types::handle actionMoveHandle{registry, actionMove.val};
  types::entity target;

  if constexpr (std::is_same_v<RecycledActionMoveType, RecycledActionMove>) {
    target = targets.val[0];
  }
  else if constexpr (std::is_same_v<RecycledActionMoveType, AddedRecycledActionMove1>) {
    target = targets.val[1];
  }
  else if constexpr (std::is_same_v<RecycledActionMoveType, AddedRecycledActionMove2>) {
    target = targets.val[2];
  }
  else {
    static_assert(false, "Using a RecycledActionMoveType that isn't associated with a target.");
  }

  MoveName move = registry.get<MoveName>(action.val);
  setupActionMoveBuild(registry, battleHandle.entity(), source.val, target, actionMove.val, move.val);
}

void setActionMoveData(Simulation& simulation) {
  simulation.addToEntities<pokesim::tags::SimulateTurn, pokesim::internal::tags::BuildActionMove>();
  simulation.pokedex().buildMoves(simulation.registry);
}

void setCurrentActionMoveSlot(types::handle handle, CurrentActionSource source, CurrentAction action) {
  types::registry& registry = *handle.registry();
  const MoveName& move = registry.get<MoveName>(action.val);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(source.val);

  types::moveSlotIndex moveSlotIndex = moveToMoveSlot(moveSlots, move.val);
  handle.emplace<CurrentActionMoveSlot>(moveSlotIndex);
}

void setMoveTargets(Simulation& simulation) {
  pokesim::internal::EntityFilter<action::tags::Move, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<setCurrentActionTarget>(simulation);

  battleFilter.view<setActionMoveReferenceComponents<RecycledActionMove>>();
  setActionMoveData(simulation);
  simulation.removeFromEntities<pokesim::internal::tags::BuildActionMove>();

  runModifyTarget(simulation);
  if (simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    simulation
      .view<addTargetAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::TargetAlly>>();
    simulation
      .view<addUserAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::UserAlly>>();

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

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  runModifyMove(simulation);

  runMoveHitChecks(simulation);
  runAfterMoveUsedEvent(simulation);
}

template <typename ActionTag>
void removeActionBySource(types::handle sourceHandle, Battle battle) {
  types::registry& registry = *sourceHandle.registry();
  registry.remove<ActionTag>(battle.val);
  registry.remove<CurrentActionSource>(battle.val);
  sourceHandle.remove<pokesim::tags::CurrentActionMoveSource>();
}

void runMoveAction(Simulation& simulation) {
  getBattleFilter(simulation).view<setCurrentActionSource, Tags<action::tags::Move>>();

  simulation.view<
    removeActionBySource<action::tags::Move>,
    Tags<pokesim::tags::CurrentActionMoveSource, pokesim::tags::Fainted>>();
  simulation.view<
    removeActionBySource<action::tags::Move>,
    Tags<pokesim::tags::CurrentActionMoveSource>,
    entt::exclude_t<pokesim::tags::ActivePokemon>>();

  pokesim::internal::EntityFilter<action::tags::Move, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  setMoveTargets(simulation);
  battleFilter.view<setCurrentActionMoveSlot>();

  runBeforeMove(simulation);

  simulation.view<setLastMoveUsed>();
  simulation.view<deductPp, Tags<pokesim::tags::CurrentActionMoveSource>>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  pokesim::internal::EntityFilter<action::tags::Residual, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<speedSort, Tags<tags::SpeedSortNeeded>>();
  simulation.registry.clear<tags::SpeedSortNeeded>();

  runResidual(simulation);
}

void runBeforeTurnAction(Simulation&) {
  // Barely used, will find different way of handling it
}

void setFainting(types::registry& registry, FaintQueue& faintQueue) {
  types::entity pokemon = faintQueue.val.front();
  faintQueue.val.erase(faintQueue.val.begin());
  registry.emplace<pokesim::tags::Fainting>(pokemon);
}

void clearFaintQueue(types::handle battleHandle, const FaintQueue& faintQueue) {
  if (faintQueue.val.empty()) {
    battleHandle.remove<FaintQueue>();
  }
}

void checkWin(types::handle battleHandle, const Sides& sides) {
  types::registry& registry = *battleHandle.registry();

  for (types::entity sideEntity : sides.val) {
    types::teamPositionIndex pokemonLeft = foeSidePokemonLeft(registry, sideEntity);
    if (!pokemonLeft) {
      battleHandle.emplace<Winner>(registry.get<PlayerSide>(sideEntity).val);
      clearActionQueue(battleHandle.get<ActionQueue>());
      return;
    }
  }
}

void faintPokemon(Simulation& simulation) {
  auto battleFilter = getBattleFilter(simulation);
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  using LoopLimits = Constants::ActivePokemon;
  types::activePokemonIndex iterations = LoopLimits::MIN;
  while (!simulation.registry.view<FaintQueue>().empty()) {
    POKESIM_REQUIRE(
      iterations <= LoopLimits::MAX,
      "More Pokemon were queued to faint in at least one battle than possible.");

    battleFilter.view<setFainting>();

    pokesim::internal::EntityFilter<pokesim::tags::Fainting> pokemonFilter{simulation};
    POKESIM_REQUIRE(
      !pokemonFilter.hasNoneSelected(),
      "This loop should only be run if setFainting had Pokemon to set as fainting.");

    runFaintEvent(simulation);
    runEndAbilityEvent(simulation);

    pokemonFilter.addToSelected<pokesim::tags::EndItem>();
    runEndItemEvent(simulation);
    simulation.registry.clear<pokesim::tags::EndItem>();

    pokemonFilter.view<clearVolatiles>();

    simulation.addToEntities<pokesim::tags::Fainted, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::ActivePokemon, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::Fainting>();

    battleFilter.view<clearFaintQueue>();
    iterations++;
  }

  battleFilter.view<checkWin, Tags<>, entt::exclude_t<Winner>>();

  runAfterFaintEvent(simulation);
}

void runCurrentAction(Simulation& simulation) {
  runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearCurrentAction(simulation);
  faintPokemon(simulation);
  // Update
  // Switch requests

  updateAllStats(simulation);
  getBattleFilter(simulation).view<speedSort, Tags<tags::SpeedSortNeeded>>();
  simulation.registry.clear<tags::SpeedSortNeeded>();
}

void incrementTurn(Turn& turn) {
  turn.val++;
}

void nextTurn(Simulation& simulation) {
  getBattleFilter(simulation).view<incrementTurn>();

  pokesim::internal::EntityFilter<pokesim::tags::SimulateTurn, pokesim::tags::ActivePokemon> pokemonFilter{simulation};
  if (!pokemonFilter.hasNoneSelected()) {
    simulation.removeFromEntities<DisabledMoveSlots, pokesim::tags::SimulateTurn, pokesim::tags::ActivePokemon>();

    pokemonFilter.addToSelected<pokesim::tags::DisableMove>();
    runDisableMove(simulation);
    simulation.registry.clear<pokesim::tags::DisableMove>();
  }
}

void simulateTurn(Simulation& simulation) {
  const auto& options = simulation.simulateTurnOptions;
#ifndef POKESIM_ALL_DAMAGE_ALL_BRANCHES
  POKESIM_REQUIRE(
    !options.getMakeBranchesOnRandomEvents() ||
      !(options.getDamageRollsConsidered().getP1() & DamageRollKind::ALL_DAMAGE_ROLLS ||
        options.getDamageRollsConsidered().getP2() & DamageRollKind::ALL_DAMAGE_ROLLS),
    "Creating a branch for every damage roll is disabled by default to prevent easily reaching the battle count limit. "
    "Rebuild PokeSim with the flag POKESIM_ALL_DAMAGE_ALL_BRANCHES to enable this option combination.");
#endif

  auto battleFilter = getBattleFilter(simulation);
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  if (!options.getApplyChangesToInputBattle()) {
    simulation.addToEntities<pokesim::tags::CloneFrom, pokesim::tags::SimulateTurn, pokesim::tags::Battle>();
    const auto entityMap = clone(simulation.registry, 1U);
    for (const auto& inputBattleMapping : entityMap) {
      types::entity original = inputBattleMapping.first;
      if (simulation.registry.all_of<pokesim::tags::SimulateTurn>(original)) {
        simulation.registry.emplace<tags::Input>(original);
        simulation.registry.remove<pokesim::tags::SimulateTurn>(original);
      }
    }
  }

  battleFilter.view<assignRootBattle>();

  updateAllStats(simulation);
  simulation.view<resolveDecision, Tags<pokesim::tags::SimulateTurn>>();
  simulation.removeFromEntities<SideDecision, pokesim::tags::SimulateTurn>();

  // battleFilter.view<addBeforeTurnAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();
  battleFilter.view<speedSort>();
  battleFilter.view<addResidualAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();

  simulation.addToEntities<pokesim::tags::BattleMidTurn, pokesim::tags::SimulateTurn, pokesim::tags::Battle>();

  battleFilter.view<setCurrentAction>();
  using ActionsLimit = Constants::ActionQueueLength;
  types::actionQueueIndex actionsTaken = ActionsLimit::MIN;
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    POKESIM_REQUIRE(
      actionsTaken <= ActionsLimit::MAX,
      "More actions in a turn were queued to be taken than in at least one battle than are possible.");

    runCurrentAction(simulation);
    battleFilter.view<setCurrentAction, Tags<>, entt::exclude_t<Winner>>();
    actionsTaken++;
  }

  nextTurn(simulation);

  simulation.removeFromEntities<pokesim::tags::BattleMidTurn, pokesim::tags::SimulateTurn, pokesim::tags::Battle>();
  battleFilter.view<collectTurnOutcomeBattles>();

  simulation.addToEntities<pokesim::tags::SimulateTurn, tags::Input>();
  simulation.registry.clear<tags::Input>();
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  simulateTurn(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::simulate_turn
