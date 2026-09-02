#include "Decisions.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/DisabledMoveSlots.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/SideDecisionOptions.hpp>
#include <Components/SideDecisions.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SpeedTieIndexes.hpp>
#include <Components/SimulateTurn/TeamAction.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Components/Turn.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Decisions.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/Indexes.hpp>
#include <Types/MaxSizedVector.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Types/Variant.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
namespace {
bool canSwitchIn(const types::registry& registry, types::entity entity) {
  return !registry.all_of<tags::Fainted>(entity);
}

void setSwitchOptions(
  SwitchOptions& switchOptions, const types::registry& registry, PlayerSide playerSide, const Team& team,
  types::teamPositionIndex totalActive) {
  for (types::teamPositionIndex i = totalActive; i < team.val.size(); i++) {
    if (canSwitchIn(registry, team.val[i])) {
      Slot slot = sideIdAndPositionToSlot(playerSide.val, i);
      switchOptions.val.push_back(slot);
    }
  }
}

void setDoublesMoveTargets(
  types::targets<Slot>& possibleTargets, const Pokedex& pokedex, dex::Move move, Slot sourceSlot, Slot allySlot,
  types::sideSlots<Slot> foeSlots) {
  if (pokedex.moveHasAll<move::singles_target::tags::Self>(move)) {
    possibleTargets.push_back(sourceSlot);
  }

  if (pokedex.moveHasAll<move::singles_target::tags::Foe>(move)) {
    for (Slot foe : foeSlots) {
      possibleTargets.push_back(foe);
    }
  }

  if (allySlot != Slot::NONE) {
    bool canTargetAlly = pokedex.moveHasAny<
      move::tags::AnySingleTarget,
      move::tags::AnySingleAlly,
      move::tags::AllyOrSelf,
      move::tags::AlliesAndSelf,
      move::added_targets::tags::SourceAlly>(move);
    if (canTargetAlly) {
      possibleTargets.push_back(allySlot);
    }
  }
}

template <typename MoveOption, typename... DoublesArguments>
void setMoveOptions(
  types::moveSlots<MoveOption>& moveOptions, types::handle handle, const MoveSlots& moveSlots, Slot slot,
  const Pokedex& pokedex, DoublesArguments... doublesArguments) {
  const DisabledMoveSlots* disabledMoveSlots = handle.try_get<DisabledMoveSlots>();

  for (types::moveSlotIndex i = 0U; i < moveSlots.val.size(); i++) {
    if (disabledMoveSlots && disabledMoveSlots->val[i]) {
      continue;
    }

    MoveSlot moveSlot = moveSlots.val[i];
    if (moveSlot.pp == Constants::MovePp::MIN) {
      continue;
    }

    MoveOption moveOption{moveSlot.move};

    if constexpr (std::is_same_v<MoveOption, SinglesMoveOption>) {
      if (pokedex.moveHasAll<move::singles_target::tags::Self>(moveOption.move)) {
        moveOption.target = slot;
      }
      else {
        moveOption.target = slot == Slot::P1A ? Slot::P2A : Slot::P1A;
      }
    }
    else {
      setDoublesMoveTargets(moveOption.possibleTargets, pokedex, moveOption.move, slot, doublesArguments...);
    }

    moveOptions.push_back(moveOption);
  }
}

void setSinglesMidTurnSideOptions(
  types::registry& registry, PlayerSide playerSide, const Team& team, SwitchOptions& switchOptions) {
  setSwitchOptions(switchOptions, registry, playerSide, team, Constants::ActivePokemonSlotsPerSide::SINGLES);
}

void setDoublesMidTurnSideOptions(
  types::registry& registry, PlayerSide playerSide, const Team& team, SwitchOptions& switchOptions) {
  setSwitchOptions(switchOptions, registry, playerSide, team, Constants::ActivePokemonSlotsPerSide::DOUBLES);
}

void setSinglesSwitchOptions(
  types::registry& registry, PlayerSide playerSide, const Team& team, SinglesSideOptions& sideOptions) {
  setSinglesMidTurnSideOptions(registry, playerSide, team, sideOptions.switches);
}

void setDoublesSwitchOptions(
  types::registry& registry, PlayerSide playerSide, const Team& team, DoublesSideOptions& sideOptions) {
  setDoublesMidTurnSideOptions(registry, playerSide, team, sideOptions.switches);
}

void setSinglesMoveOptions(types::handle handle, Side side, const MoveSlots& moveSlots, const Pokedex& pokedex) {
  types::registry& registry = *handle.registry();
  SinglesSideOptions& sideOptions = registry.get<SinglesSideOptions>(side.val);
  PlayerSide playerSide = registry.get<PlayerSide>(side.val);

  setMoveOptions<SinglesMoveOption>(
    sideOptions.moves,
    handle,
    moveSlots,
    sideIdAndPositionToSlot(playerSide.val, 0U),
    pokedex);
}

void setDoublesMoveOptions(types::handle handle, Side side, const MoveSlots& moveSlots, const Pokedex& pokedex) {
  types::registry& registry = *handle.registry();
  DoublesSideOptions& sideOptions = registry.get<DoublesSideOptions>(side.val);
  const auto& [team, playerSide] = registry.get<Team, PlayerSide>(side.val);

  POKESIM_REQUIRE(
    handle.entity() == team.val[0U] || handle.entity() == team.val[1U],
    "This entity must be in a valid active team slot.");
  types::teamPositionIndex slotPosition = handle.entity() == team.val[0U] ? 0U : 1U;
  Slot sourceSlot = sideIdAndPositionToSlot(playerSide.val, slotPosition);
  Slot allySlot = Slot::NONE;
  types::sideSlots<Slot> foeSlots;

  if (registry.all_of<tags::ActivePokemon>(team.val[slotPosition ? 1U : 0U])) {
    allySlot = sideIdAndPositionToSlot(playerSide.val, slotPosition ? 1U : 0U);
  }

  PlayerSideId foeSide = playerSide.val == PlayerSideId::P1 ? PlayerSideId::P2 : PlayerSideId::P1;
  const Team& foeTeam = registry.get<Team>(registry.get<FoeSide>(side.val).val);
  for (types::teamPositionIndex i = 0U; i < Constants::ActivePokemonSlotsPerSide::DOUBLES; i++) {
    if (registry.all_of<tags::ActivePokemon>(foeTeam.val[i])) {
      foeSlots.push_back(sideIdAndPositionToSlot(foeSide, i));
    }
  }

  setMoveOptions<DoublesMoveOption>(
    sideOptions.moves[slotPosition],
    handle,
    moveSlots,
    sourceSlot,
    pokedex,
    allySlot,
    foeSlots);
}

void setTeamPreviewOptionsFromBattle(types::registry& registry, Turn turn, const Sides& sides) {
  if (turn.val != Constants::TurnCount::MIN) {
    return;
  }

  for (types::entity side : sides.val) {
    const Team& team = registry.get<Team>(side);
    TeamPreviewOptions& options = registry.emplace<TeamPreviewOptions>(side);

    for (types::teamPositionIndex i = 0U; i < team.val.size(); i++) {
      if (canSwitchIn(registry, team.val[i])) {
        options.val.push_back(i);
      }
    }
  }
}

template <typename Decision>
void resolveSlotDecision(types::handle sideHandle, const types::slotDecision& slotDecision, ActionQueue& actionQueue) {
  if (!slotDecision.holds<Decision>()) {
    return;
  }

  types::registry& registry = *sideHandle.registry();
  const auto& decision = slotDecision.get<Decision>();

  ActionQueueItem actionQueueItem;
  actionQueueItem.decision = decision;

  types::entity sourceEntity = slotToPokemonEntity(registry, sideHandle.entity(), decision.sourceSlot);
  actionQueueItem.speed = registry.get<stat::EffectiveSpe>(sourceEntity).val;

  if constexpr (std::is_base_of_v<MoveDecision, Decision>) {
    actionQueueItem.order = ActionOrder::MOVE;
    actionQueueItem.priority = Constants::MovePriority::DEFAULT;  // TODO (aed3): Move priority + modify priority
    actionQueueItem.fractionalPriority = false;                   // TODO (aed3): get fractionalPriority

    if constexpr (!std::is_same_v<MoveDecision, Decision>) {
      POKESIM_REQUIRE_FAIL(std::string(entt::type_name<Decision>().value()) + " is not yet supported.");
    }
  }
  else if constexpr (std::is_same_v<SwitchDecision, Decision>) {
    actionQueueItem.order = ActionOrder::SWITCH;
  }
  else if constexpr (std::is_same_v<ItemDecision, Decision>) {
    actionQueueItem.order = ActionOrder::ITEM;
  }
  else {
    POKESIM_REQUIRE_FAIL(std::string(entt::type_name<Decision>().value()) + " is not yet supported.");
  }

  actionQueue.val.push_back(actionQueueItem);
}

void resolveSlotDecisions(types::handle sideHandle, const types::slotDecisions& decisions, ActionQueue& actionQueue) {
  for (const types::slotDecision& decision : decisions) {
    resolveSlotDecision<MoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<MegaEvolveAndMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<ZMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<DynamaxAndMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<TerastallizeAndMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<SwitchDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<ItemDecision>(sideHandle, decision, actionQueue);
  }
}

void resolveTeamDecision(types::registry&, const types::teamOrder&, ActionQueue&) {}
}  // namespace

namespace simulate_turn {
void setSideOptions(Simulation& simulation) {
  const Pokedex& pokedex = simulation.pokedex();
  using WantsMidTurnOptions = pokesim::tags::BattleRequestingDecision;
  auto wantsRegularOptionsView = simulation.registry.view<pokesim::tags::Side>(entt::exclude_t<WantsMidTurnOptions>());

  simulation.addToEntities<SwitchOptions, WantsMidTurnOptions>();
  if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
    simulation.view<setSinglesMidTurnSideOptions, Tags<WantsMidTurnOptions>>();

    simulation.registry.insert<SinglesSideOptions>(wantsRegularOptionsView.begin(), wantsRegularOptionsView.end());
    simulation.view<setSinglesMoveOptions, Tags<pokesim::tags::ActivePokemon>, entt::exclude_t<WantsMidTurnOptions>>(
      pokedex);
    simulation.view<setSinglesSwitchOptions, Tags<pokesim::tags::Side>, entt::exclude_t<WantsMidTurnOptions>>();
  }
  else {
    simulation.view<setDoublesMidTurnSideOptions, Tags<WantsMidTurnOptions>>();

    simulation.registry.insert<DoublesSideOptions>(wantsRegularOptionsView.begin(), wantsRegularOptionsView.end());
    simulation.view<setDoublesMoveOptions, Tags<pokesim::tags::ActivePokemon>, entt::exclude_t<WantsMidTurnOptions>>(
      pokedex);
    simulation.view<setDoublesSwitchOptions, Tags<pokesim::tags::Side>, entt::exclude_t<WantsMidTurnOptions>>();
  }
}

void setTeamPreviewOptions(Simulation& simulation) {
  simulation.view<setTeamPreviewOptionsFromBattle>();
}
}  // namespace simulate_turn

namespace internal::simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision) {
  Battle battle = sideHandle.get<Battle>();
  types::registry& registry = *sideHandle.registry();
  ActionQueue& actionQueue = registry.get<ActionQueue>(battle.val);

  if (sideDecision.decisions.holds<types::slotDecisions>()) {
    POKESIM_REQUIRE(
      registry.get<Turn>(battle.val).val != Constants::TurnCount::MIN,
      "Slot decisions only have an effect after a battle starts. Make sure to pass a `teamOrder` decision at the start "
      "of a battle (aka team preview).");
    const auto& decisions = sideDecision.decisions.get<types::slotDecisions>();

    resolveSlotDecisions(sideHandle, decisions, actionQueue);
  }
  else if (sideDecision.decisions.holds<types::teamOrder>()) {
    POKESIM_REQUIRE(
      registry.get<Turn>(battle.val).val == Constants::TurnCount::MIN,
      "Team order decisions only have an effect at the start of a battle (aka team preview). Make sure to pass a "
      "`slotDecisions` decision for battles in progress.");
    const auto& teamOrder = sideDecision.decisions.get<types::teamOrder>();

    POKESIM_REQUIRE(
      sideHandle.get<Team>().val.size() == teamOrder.size(),
      "Must pick a placement for each Pokemon on the team.");
    resolveTeamDecision(*sideHandle.registry(), teamOrder, actionQueue);
  }
  else {
    POKESIM_REQUIRE_FAIL(
      "Decision kind of index " + std::to_string(sideDecision.decisions.index()) + "not implemented yet.");
  }
}

void resolveMidTurnDecisions(types::handle sideHandle, const MidTurnSideDecision& switchDecisions) {
  Battle battle = sideHandle.get<Battle>();
  types::registry& registry = *sideHandle.registry();
  ActionQueue& actionQueue = registry.get<ActionQueue>(battle.val);

  for (SwitchDecision switchDecision : switchDecisions.val) {
    ActionQueueItem actionQueueItem{ActionOrder::MID_TURN_SWITCH};
    actionQueueItem.decision = switchDecision;

    types::entity sourceEntity = slotToPokemonEntity(registry, sideHandle.entity(), switchDecision.sourceSlot);
    actionQueueItem.speed = registry.get<stat::EffectiveSpe>(sourceEntity).val;
    actionQueue.val.insert(actionQueue.val.begin(), actionQueueItem);
  }
}
}  // namespace internal::simulate_turn
}  // namespace pokesim
