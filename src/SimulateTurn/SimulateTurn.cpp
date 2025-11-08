#include "SimulateTurn.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/Helpers.hpp>
#include <Components/AddedTargets.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/PP.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Components/Turn.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/MoveHitSteps.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>

#include "ManageActionQueue.hpp"
#include "SimulateTurnDebugChecks.hpp"

namespace pokesim::simulate_turn {
namespace {
void addTargetAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, targetSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void addUserAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, sourceSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void resolveMoveTargets(types::registry& registry, CurrentActionTargets& targets) {
  for (types::entity target : targets.val) {
    registry.emplace_or_replace<pokesim::tags::CurrentActionMoveTarget>(target);
  }

  // More to do...
}

void createActionMoveForTargets(
  types::handle targetHandle, Battle battle, CurrentActionSource source, const Pokedex& pokedex) {
  types::registry& registry = *targetHandle.registry();

  dex::Move move = registry.get<action::Move>(registry.get<CurrentAction>(battle.val).val).name;
  types::entity moveEntity = createActionMoveForTarget(targetHandle, battle.val, source.val, move, pokedex);

  registry.emplace<pokesim::tags::SimulateTurn>(moveEntity);
}

void getMoveTargets(Simulation& simulation) {
  if (simulation.battleFormat() == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation
      .view<addTargetAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::TargetAlly>>();
    simulation
      .view<addUserAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::UserAlly>>();
  }
  simulation.view<resolveMoveTargets, Tags<pokesim::tags::CurrentActionMove>, entt::exclude_t<AddedTargets>>();
  simulation.view<
    createActionMoveForTargets,
    Tags<pokesim::tags::CurrentActionMoveTarget>,
    entt::exclude_t<CurrentActionMovesAsTarget>>(simulation.pokedex());
}

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  runModifyMove(simulation);

  getMoveTargets(simulation);
  runMoveHitChecks(simulation);
  runAfterMoveUsedEvent(simulation);
}

void runMoveAction(Simulation& simulation) {
  pokesim::internal::SelectForBattleView<action::Move> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<setCurrentActionSource>();
  simulation.viewForSelectedBattles<setCurrentActionTarget>();
  simulation.viewForSelectedBattles<setCurrentActionMove>(simulation.pokedex());

  simulation.view<deductPp, Tags<pokesim::tags::CurrentActionMoveSlot>>();
  simulation.view<setLastMoveUsed>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  pokesim::internal::SelectForBattleView<action::tags::Residual> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;
}

void runBeforeTurnAction(Simulation&) {
  // Barely used, will find different way of handling it
}

void runCurrentAction(Simulation& simulation) {
  runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearCurrentAction(simulation);
  // faint pokemon
  // Update
  // Switch requests

  updateAllStats(simulation);
  simulation.viewForSelectedBattles<speedSort, Tags<tags::SpeedSortNeeded>>();
  simulation.registry.clear<tags::SpeedSortNeeded>();
}

void incrementTurn(Turn& turn) {
  turn.val++;
}

void updateActivePokemonPostTurn(types::registry& registry, const pokesim::MoveSlots& moveSlots) {
  registry.remove<pokesim::move::tags::Disabled>(moveSlots.val.begin(), moveSlots.val.end());
}

void nextTurn(Simulation& simulation) {
  simulation.viewForSelectedBattles<incrementTurn>();

  pokesim::internal::SelectForPokemonView<pokesim::tags::SimulateTurn, pokesim::tags::ActivePokemon> selectedPokemon{
    simulation};
  if (!selectedPokemon.hasNoneSelected()) {
    simulation.viewForSelectedPokemon<updateActivePokemonPostTurn>();
    runDisableMove(simulation);
  }
}

void simulateTurn(Simulation& simulation) {
  const auto& options = simulation.simulateTurnOptions;
#ifndef POKESIM_ALL_DAMAGE_ALL_BRANCHES
  POKESIM_REQUIRE(
    !options.makeBranchesOnRandomEvents || !(options.damageRollsConsidered.p1 & DamageRollKind::ALL_DAMAGE_ROLLS ||
                                             options.damageRollsConsidered.p2 & DamageRollKind::ALL_DAMAGE_ROLLS),
    "Creating a branch for every damage roll is disabled by default to prevent easily reaching the battle count limit. "
    "Rebuild PokeSim with the flag POKESIM_ALL_DAMAGE_ALL_BRANCHES to enable this option combination.");
#endif

  if (!options.applyChangesToInputBattle) {
    simulation.addToEntities<pokesim::tags::CloneFrom, pokesim::tags::Battle, pokesim::tags::SimulateTurn>();
    const auto entityMap = clone(simulation.registry, 1U);
    for (const auto& inputBattleMapping : entityMap) {
      simulation.registry.emplace<tags::Input>(inputBattleMapping.first);
    }
  }

  pokesim::internal::SelectForBattleView<pokesim::tags::SimulateTurn> selectedBattles{
    simulation,
    entt::exclude<tags::Input>};
  pokesim::internal::SelectForSideView<pokesim::tags::SimulateTurn> selectedSides{
    simulation,
    entt::exclude<tags::Input>};
  if (selectedBattles.hasNoneSelected() || selectedSides.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<assignRootBattle>();

  updateAllStats(simulation);
  simulation.viewForSelectedSides<resolveDecision>();
  simulation.removeFromEntities<SideDecision, pokesim::tags::SelectedForViewSide>();

  // simulation.viewForSelectedBattles<addBeforeTurnAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();
  simulation.viewForSelectedBattles<speedSort>();
  simulation.viewForSelectedBattles<addResidualAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();

  simulation.addToEntities<pokesim::tags::BattleMidTurn, Turn, pokesim::tags::SelectedForViewBattle>();

  simulation.viewForSelectedBattles<setCurrentAction>();
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    runCurrentAction(simulation);
    simulation.viewForSelectedBattles<setCurrentAction>();
  }

  nextTurn(simulation);

  simulation.removeFromEntities<pokesim::tags::BattleMidTurn, Turn, pokesim::tags::SelectedForViewBattle>();
  simulation.viewForSelectedBattles<collectTurnOutcomeBattles>();
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