#include "SimulateTurn.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/AddedTargets.hpp>
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
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Components/Turn.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/MoveHitSteps.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>

#include "ManageActionQueue.hpp"

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  pokesim::internal::SelectForBattleView<tags::SimulateTurn> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<assignRootBattle>();

  updateAllStats(simulation);
  simulation.view<resolveDecision>();
  simulation.registry.clear<SideDecision>();

  // simulation.viewForSelectedBattles<addBeforeTurnAction, Tags<>, entt::exclude_t<tags::BattleMidTurn>>();
  simulation.viewForSelectedBattles<speedSort>();
  simulation.viewForSelectedBattles<addResidualAction, Tags<>, entt::exclude_t<tags::BattleMidTurn>>();

  simulation.addToEntities<tags::BattleMidTurn, Turn, tags::SimulateTurn>();

  simulation.viewForSelectedBattles<setCurrentAction>();
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    runCurrentAction(simulation);
    simulation.viewForSelectedBattles<setCurrentAction>();
  }

  nextTurn(simulation);

  simulation.removeFromEntities<tags::BattleMidTurn, Turn, tags::SimulateTurn>();
  simulation.viewForSelectedBattles<collectTurnOutcomeBattles>();
}

void runCurrentAction(Simulation& simulation) {
  // runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearCurrentAction(simulation);
  // faint pokemon
  // Update
  // Switch requests

  if (!simulation.registry.view<tags::SpeStatUpdateRequired>().empty()) {
    updateSpe(simulation);
    simulation.viewForSelectedBattles<speedSort>();  // Should only speed sort battles affected
  }
  updateAllStats(simulation);
}

void runBeforeTurnAction(Simulation& /*simulation*/) {
  // Barely used, will find different way of handling it
}

void runMoveAction(Simulation& simulation) {
  pokesim::internal::SelectForBattleView<action::Move> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<setCurrentActionSource>();
  simulation.viewForSelectedBattles<setCurrentActionTarget>();
  simulation.viewForSelectedBattles<setCurrentActionMove>(simulation.pokedex);

  simulation.view<deductPp, Tags<tags::CurrentActionMoveSlot>>();
  simulation.view<setLastMoveUsed>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  pokesim::internal::SelectForBattleView<action::tags::Residual> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;
}

namespace internal {
void incrementTurn(Turn& turn) {
  turn.val++;
}

void updateActivePokemonPostTurn(types::handle pokemonHandle, const pokesim::MoveSlots& moveSlots) {
  pokemonHandle.registry()->remove<pokesim::move::tags::Disabled>(moveSlots.val.begin(), moveSlots.val.end());
}
}  // namespace internal

void nextTurn(Simulation& simulation) {
  simulation.viewForSelectedBattles<internal::incrementTurn>();

  pokesim::internal::SelectForPokemonView<tags::SimulateTurn, tags::ActivePokemon> selectedPokemon{simulation};
  if (!selectedPokemon.hasNoneSelected()) {
    simulation.viewForSelectedPokemon<internal::updateActivePokemonPostTurn>();
    runDisableMove(simulation);
  }
}

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

void resolveMoveTargets(CurrentActionTargets&) {}

void createActionMoveForTargets(
  types::handle targetHandle, const Battle& battle, const CurrentActionSource& source, const Pokedex& pokedex) {
  types::registry& registry = *targetHandle.registry();

  dex::Move move = registry.get<action::Move>(registry.get<CurrentAction>(battle.val).val).name;
  types::entity moveEntity = createActionMoveForTarget(targetHandle, battle.val, source.val, move, pokedex);

  registry.emplace<tags::SimulateTurn>(moveEntity);
}

void getMoveTargets(Simulation& simulation) {
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<addTargetAllyToTargets, Tags<tags::CurrentActionMove, move::added_targets::tags::TargetAlly>>();
    simulation.view<addUserAllyToTargets, Tags<tags::CurrentActionMove, move::added_targets::tags::UserAlly>>();
  }
  simulation.view<resolveMoveTargets, Tags<tags::CurrentActionMove>, entt::exclude_t<AddedTargets>>();
  simulation.view<createActionMoveForTargets, Tags<tags::CurrentActionMoveTarget>, entt::exclude_t<CurrentActionMoves>>(
    simulation.pokedex);
}

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  runModifyMove(simulation);

  getMoveTargets(simulation);
  runMoveHitChecks(simulation);
}
}  // namespace pokesim::simulate_turn