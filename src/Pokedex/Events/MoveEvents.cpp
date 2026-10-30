#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/BasePower.hpp>
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
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "../Moves/headers.hpp"
#include "EventsHelpers.hpp"

namespace pokesim::dex {
namespace {
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

void reflectOnResidual(types::handle handle, Reflect::Effect& reflect) {
  reflect.val--;
  if (reflect.val == Constants::EffectDuration::MIN) {
    handle.remove<Reflect::Effect>();
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
  simulation.view<reflectOnResidual>();
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
