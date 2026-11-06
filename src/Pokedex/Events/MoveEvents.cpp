#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/BasePower.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/SimulateTurn/MoveHitSteps.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SideTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "../Moves/headers.hpp"
#include "EventsHelpers.hpp"

namespace pokesim::dex {
namespace {
void allySwitchOnHit(types::handle handle, Battle battle, CurrentActionTarget target) {
  types::registry& registry = *handle.registry();
  Side side = registry.get<Side>(target.val);
  Team& team = registry.get<Team>(side.val);
  if (team.val.size() <= 1U) {
    return;
  }

  if (registry.all_of<tags::Fainted>(team.val[0]) || registry.all_of<tags::Fainted>(team.val[1])) {
    return;
  }

  std::swap(team.val[0], team.val[1]);
  internal::swapActionQueueSourceSlots(registry, battle, side, 0U, 1U);
}

void allySwitchOnTryMoveApply(
  types::registry& registry, CurrentActionSource source, const dex::AllySwitch::Effect& allySwitch) {
  (void)registry.get_or_emplace<AllySwitch::Effect>(source.val, allySwitch);
}

void allySwitchOnTryMove(
  types::handle handle, Battle battle, AllySwitch::Effect& allySwitch, const CurrentActionMovesAsSource& moves,
  types::percentChance stallExponentialBase, const Simulation& simulation) {
  float exponential = std::pow((float)stallExponentialBase, (float)allySwitch.uses);
  for (types::entity move : moves) {
    if (!handle.registry()->all_of<AllySwitch>(move)) {
      return;
    }
  }

  pokesim::internal::setReciprocalRandomBinaryChanceFromProbability(
    handle,
    battle,
    simulation,
    exponential * Constants::PercentChanceToProbability);
}

void allySwitchOnTryMovePass(AllySwitch::Effect& allySwitch, types::effectDuration turnsRemaining) {
  if (allySwitch.uses < Constants::StallMoveUses::MAX) {
    allySwitch.uses++;
  }
  allySwitch.turnsRemaining = turnsRemaining;
}

void knockOffOnBasePowerCheckRemovableItem(
  types::registry& registry, CurrentActionSource source, CurrentActionTarget target, EventModifier&) {
  if (
    registry.all_of<tags::HasItem>(target.val) &&
    registry.get<stat::CurrentHp>(source.val).val > Constants::PokemonCurrentHpStat::MIN) {
    registry.get_or_emplace<tags::CanRemoveItem>(target.val);
  }
}

void knockOffOnAfterHitCheckRemovableItem(types::registry& registry, CurrentActionTarget target) {
  if (
    registry.all_of<tags::HasItem>(target.val) &&
    registry.get<stat::CurrentHp>(target.val).val > Constants::PokemonCurrentHpStat::MIN) {
    registry.get_or_emplace<tags::CanRemoveItem>(target.val);
  }
}

void reflectOnStart(types::handle handle, CurrentActionTarget target, const Pokedex& pokedex) {
  types::registry& registry = *handle.registry();
  Side targetSide = registry.get<Side>(target.val);
  if (registry.all_of<Reflect::Effect>(targetSide.val)) {
    handle.emplace<tags::FailedCurrentMoveHit>();
    return;
  }

  types::effectDuration duration = pokedex.getStaticValue<Reflect::Effect::duration>();
  // if (registry.all_of<LightClay>(source.val)) duration = pokedex.getStaticValue<Reflect::Effect::extendedDuration>();
  registry.emplace<Reflect::Effect>(targetSide.val, duration);
}

void reflectOnAnyModifyDamage(
  types::registry& registry, const Team& team, Reflect::Effect, types::eventModifier numerator,
  types::eventModifier denominator, types::teamPositionIndex activePokemonSlotsPerSide) {
  for (types::teamPositionIndex i = 0U; i < activePokemonSlotsPerSide; i++) {
    types::entity teamMember = team.val[i];
    auto* moves = registry.try_get<CurrentActionMovesAsTarget>(teamMember);
    auto* movesExtended = registry.try_get<CurrentActionMovesAsTargetExtended>(teamMember);

    if (moves) {
      internal::ModifyDamage<CurrentActionMovesAsTarget, move::tags::Special>::run(
        registry,
        *moves,
        numerator,
        denominator);
    }
    if (movesExtended) {
      internal::ModifyDamage<CurrentActionMovesAsTargetExtended, move::tags::Special>::run(
        registry,
        *movesExtended,
        numerator,
        denominator);
    }
  }
}

void knockOffOnBasePower(
  types::registry& registry, EventModifier& eventModifier, CurrentActionTarget target,
  types::effectMultiplier modifier) {
  if (!registry.all_of<tags::CanRemoveItem>(target.val)) {
    return;
  }
  internal::chainComponentToModifier(eventModifier, modifier);
}

void reversalOnSetBasePower(types::handle handle, CurrentActionSource source, const Pokedex& pokedex) {
  auto [currentHp, maxHp] = handle.registry()->get<stat::CurrentHp, stat::Hp>(source.val);
  types::basePower basePower = pokedex.getStaticValue<Reversal::onSetBasePowerFromSourceHp>(currentHp.val, maxHp.val);
  handle.emplace<BasePower>(basePower);
}

void transformOnHit(
  types::registry& registry, CurrentActionSource source, CurrentActionTarget target, const Pokedex& pokedex) {
  internal::transformInto(registry, source.val, target.val, pokedex);
}
}  // namespace

void AllySwitch::targetPrimaryEffect::onHit(Simulation& simulation) {
  if (simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    simulation.view<allySwitchOnHit, Tags<AllySwitch, tags::CurrentMoveHit>>();
  }
}

void AllySwitch::Effect::onTryMove(Simulation& simulation) {
  if (!simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    return;
  }

  auto stallExponentialBase = simulation.pokedex().getStaticValue<MechanicConstants::STALL_EXPONENTIAL_BASE>();

  simulation.view<allySwitchOnTryMoveApply, Tags<MainSource, tags::CurrentActionMove>>(
    AllySwitch{Constants::StallMoveUses::DEFAULT, simulation.pokedex().getStaticValue<duration>()});

  simulation.view<allySwitchOnTryMove, Tags<tags::CurrentActionSource>>(stallExponentialBase, simulation);
  internal::randomBinaryChance(
    simulation,
    [](Simulation& sim) {
      auto turnsRemaining = sim.pokedex().getStaticValue<duration>();
      sim.view<allySwitchOnTryMovePass, Tags<internal::tags::RandomEventCheckPassed>>(turnsRemaining);
      sim.removeFromEntities<AllySwitch, internal::tags::RandomEventCheckFailed>();
      sim.view<internal::setFailedActionMove, Tags<internal::tags::RandomEventCheckFailed>>();
    },
    std::nullopt);
}

void AllySwitch::Effect::onResidual(Simulation& simulation) {
  simulation.view<internal::IncrementTurnsRemaining<AllySwitch>::run>();
}

void KnockOff::onBasePower(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<onBasePowerMultiplier>();

  simulation.view<knockOffOnBasePowerCheckRemovableItem, Tags<KnockOff>>();
  internal::checkIfCanRemoveItem(simulation);
  simulation.view<knockOffOnBasePower, Tags<KnockOff>>(modifier);

  simulation.removeFromEntities<tags::CanRemoveItem>();
}

void KnockOff::onAfterHit(Simulation& simulation) {
  simulation.view<knockOffOnAfterHitCheckRemovableItem, Tags<KnockOff, tags::CurrentMoveHit>>();
  internal::tryRemoveItem(simulation);
  simulation.removeFromEntities<tags::CanRemoveItem>();
}

void Reflect::Effect::onStart(Simulation& simulation) {
  simulation.view<reflectOnStart, Tags<dex::Reflect, tags::CurrentMoveHit>>(simulation.pokedex());
}

void Reflect::Effect::onAnyModifyDamage(Simulation& simulation) {
  const auto numerator = simulation.isBattleFormat(BattleFormat::SINGLES)
                           ? simulation.pokedex().getStaticValue<onAnyModifyDamageSinglesNumerator>()
                           : simulation.pokedex().getStaticValue<onAnyModifyDamageDoublesNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<onAnyModifyDamageDenominator>();
  const auto activePokemonSlotsPerSide = simulation.activePokemonSlotsPerSide();

  simulation.view<reflectOnAnyModifyDamage>(numerator, denominator, activePokemonSlotsPerSide);
}

void Reflect::Effect::onResidual(Simulation& simulation) {
  simulation.view<internal::IncrementTurnsRemaining<Reflect>::run>();
}

void Reversal::onSetBasePower(Simulation& simulation) {
  simulation.view<reversalOnSetBasePower, Tags<Reversal>>(simulation.pokedex());
}

void SpiritShackle::targetSecondaryEffect::onHit(Simulation& simulation) {
  simulation.addToEntities<internal::tags::TryTrap, internal::tags::RunEffect, SpiritShackle>();

  internal::tryTrap(simulation);

  simulation.removeFromEntities<internal::tags::TryTrap>();
}

void Transform::targetPrimaryEffect::onHit(Simulation& simulation) {
  simulation.view<transformOnHit, Tags<Transform, tags::CurrentMoveHit>>(simulation.pokedex());
  internal::runStartAbilityEvent(simulation);
  simulation.removeFromEntities<internal::tags::StartAbility>();
}
}  // namespace pokesim::dex
