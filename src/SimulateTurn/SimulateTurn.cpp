#include "SimulateTurn.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
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
#include <Components/FoesRemaining.hpp>
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
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).action);
  addAddedTarget(registry, battle, targetSlotName.val);
}

void addUserAllyToTargets(types::registry& registry, const Battle& battle) {
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).action);
  addAddedTarget(registry, battle, sourceSlotName.val);
}

void setTargetReferenceComponents(types::registry& registry, CurrentAction& action) {
  for (types::entity target : action.targets) {
    registry.emplace<pokesim::tags::CurrentActionMoveTarget>(target);
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
  internal::setupActionMoveBuild(registry, battleHandle.entity(), action.source, target, actionMove.val, move.val);
}

void setActionMoveData(Simulation& simulation) {
  simulation.addToEntities<pokesim::tags::SimulateTurn, pokesim::internal::tags::BuildActionMove>();
  simulation.pokedex().buildMoves(simulation.registry);
}

void setCurrentActionMoveSlot(types::handle handle, CurrentAction& action) {
  types::registry& registry = *handle.registry();
  const MoveName& move = registry.get<MoveName>(action.action);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(action.source);

  types::moveSlotIndex moveSlotIndex = moveToMoveSlot(moveSlots, move.val);
  handle.emplace<CurrentActionMoveSlot>(moveSlotIndex);
}

void setMoveTargets(Simulation& simulation) {
  pokesim::internal::EntityFilter<action::tags::Move, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<internal::setCurrentActionTarget>(simulation);

  battleFilter.view<setActionMoveReferenceComponents<RecycledActionMove>>();
  setActionMoveData(simulation);
  simulation.removeFromEntities<pokesim::internal::tags::BuildActionMove>();

  internal::runModifyTarget(simulation);
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
  internal::runModifyMove(simulation);

  internal::runMoveHitChecks(simulation);
  internal::runAfterMoveUsedEvent(simulation);
}

template <typename ActionTag>
void removeActionBySource(types::handle sourceHandle, Battle battle) {
  types::registry& registry = *sourceHandle.registry();
  registry.remove<ActionTag>(battle.val);
  registry.remove<CurrentActionSource>(battle.val);
  sourceHandle.remove<pokesim::tags::CurrentActionMoveSource>();
}

void runMoveAction(Simulation& simulation) {
  getBattleFilter(simulation).view<internal::setCurrentActionSource, Tags<action::tags::Move>>();

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

  internal::runBeforeMove(simulation);

  simulation.view<internal::setLastMoveUsed>();
  simulation.view<internal::deductPp, Tags<pokesim::tags::CurrentActionMoveSource>>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  pokesim::internal::EntityFilter<action::tags::Residual, pokesim::tags::Battle> battleFilter{simulation};
  if (battleFilter.hasNoneSelected()) {
    return;
  }

  battleFilter.view<internal::simulate_turn::speedSort, Tags<tags::SpeedSortNeeded>>();
  simulation.registry.clear<tags::SpeedSortNeeded>();

  internal::runResidual(simulation);
}

void runBeforeTurnAction(Simulation&) {
  // Barely used, will find different way of handling it
}

void setFainting(types::registry& registry, FaintQueue& faintQueue) {
  types::entity pokemon = faintQueue.val.front();
  faintQueue.val.erase(faintQueue.val.begin());
  registry.emplace<pokesim::tags::Fainting>(pokemon);
  registry.get<FoesRemaining>(registry.get<FoeSide>(registry.get<Side>(pokemon).val).val).val--;
}

void clearFaintQueue(types::handle battleHandle, const FaintQueue& faintQueue) {
  if (faintQueue.val.empty()) {
    battleHandle.remove<FaintQueue>();
  }
}

void checkWin(types::handle battleHandle, const Sides& sides) {
  types::registry& registry = *battleHandle.registry();

  for (types::entity sideEntity : sides.val) {
    types::teamPositionIndex foesRemaining = registry.get<FoesRemaining>(sideEntity).val;
    if (!foesRemaining) {
      battleHandle.emplace<Winner>(registry.get<PlayerSide>(sideEntity).val);
      internal::simulate_turn::clearActionQueue(battleHandle.get<ActionQueue>());
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

    internal::runFaintEvent(simulation);
    internal::runEndAbilityEvent(simulation);

    pokemonFilter.addToSelected<pokesim::internal::tags::EndItem>();
    internal::runEndItemEvent(simulation);
    simulation.registry.clear<pokesim::internal::tags::EndItem>();

    pokemonFilter.view<internal::clearVolatiles>();

    simulation.addToEntities<pokesim::tags::Fainted, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::ActivePokemon, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::Fainting>();

    battleFilter.view<clearFaintQueue>();
    iterations++;
  }

  if (iterations != LoopLimits::MIN) {
    battleFilter.view<checkWin, Tags<>, entt::exclude_t<Winner>>();
  }

  internal::runAfterFaintEvent(simulation);
}

void runCurrentAction(Simulation& simulation) {
  runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  internal::clearCurrentAction(simulation);
  faintPokemon(simulation);
  // Update
  // Switch requests

  internal::updateAllStats(simulation);
  getBattleFilter(simulation).view<internal::simulate_turn::speedSort, Tags<tags::SpeedSortNeeded>>();
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

    pokemonFilter.addToSelected<pokesim::internal::tags::DisableMove>();
    internal::runDisableMove(simulation);
    simulation.registry.clear<pokesim::internal::tags::DisableMove>();
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
        simulation.registry.emplace<internal::simulate_turn::tags::Input>(original);
        simulation.registry.remove<pokesim::tags::SimulateTurn>(original);
      }
    }
  }

  battleFilter.view<internal::assignRootBattle>();

  internal::updateAllStats(simulation);
  simulation.view<internal::simulate_turn::resolveDecision, Tags<pokesim::tags::SimulateTurn>>();
  simulation.removeFromEntities<SideDecision, pokesim::tags::SimulateTurn>();

  // battleFilter.view<internal::simulate_turn::addBeforeTurnAction, Tags<>,
  // entt::exclude_t<pokesim::tags::BattleMidTurn>>();
  battleFilter.view<internal::simulate_turn::speedSort>();
  battleFilter
    .view<internal::simulate_turn::addResidualAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();

  simulation.addToEntities<pokesim::tags::BattleMidTurn, pokesim::tags::SimulateTurn, pokesim::tags::Battle>();

  battleFilter.view<internal::simulate_turn::setCurrentAction>();
  using ActionsLimit = Constants::ActionQueueLength;
  types::actionQueueIndex actionsTaken = ActionsLimit::MIN;
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    POKESIM_REQUIRE(
      actionsTaken <= ActionsLimit::MAX,
      "More actions in a turn were queued to be taken than in at least one battle than are possible.");

    runCurrentAction(simulation);
    battleFilter.view<internal::simulate_turn::setCurrentAction, Tags<>, entt::exclude_t<Winner>>();
    actionsTaken++;
  }

  nextTurn(simulation);

  simulation.removeFromEntities<pokesim::tags::BattleMidTurn, pokesim::tags::SimulateTurn, pokesim::tags::Battle>();
  battleFilter.view<internal::collectTurnOutcomeBattles>();

  simulation.addToEntities<pokesim::tags::SimulateTurn, internal::simulate_turn::tags::Input>();
  simulation.registry.clear<internal::simulate_turn::tags::Input>();
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  simulateTurn(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::simulate_turn
