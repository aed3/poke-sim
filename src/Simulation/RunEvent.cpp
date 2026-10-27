#include "RunEvent.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/Items/headers.hpp>
#include <Pokedex/Moves/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/Tags.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>

#include "Simulation.hpp"

// TODO(aed3): Autogenerate?

namespace pokesim::internal {
namespace {
template <typename ModifiedComponent>
void applyEventModifier(ModifiedComponent& component, EventModifier eventModifier) {
  component.val = applyChainedModifier(component.val, eventModifier.val);
}

void applyBasePowerEventModifier(
  BasePower basePower, calc_damage::DamageFormulaVariables& damageFormulaVariables, EventModifier eventModifier) {
  damageFormulaVariables.power = applyChainedModifier(basePower.val, eventModifier.val);
}
}  // namespace

void runUpdate(Simulation& simulation) {
  dex::LumBerry::onUpdate(simulation);
}

void runBeforeMove(Simulation& simulation) {
  dex::Paralysis::onBeforeMove(simulation);
  dex::Flinch::onBeforeMove(simulation);
  dex::ChoiceLock::onBeforeMove(simulation);
}

void runResidual(Simulation& simulation) {
  dex::Reflect::Effect::onResidual(simulation);
  dex::Flinch::onResidual(simulation);
  dex::Burn::onResidual(simulation);
}

void runFractionalPriority(Simulation& simulation) {
  dex::QuickClaw::onFractionalPriority(simulation);
}

void runModifyPriority(Simulation& simulation) {
  dex::Prankster::onModifyPriority(simulation);
}

void runAccuracyEvent(Simulation&) {}

void runModifyAccuracyEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::CurrentMoveHit, Accuracy>();

  dex::BrightPowder::onModifyAccuracy(simulation);

  simulation.view<applyEventModifier<Accuracy>>();
  simulation.removeFromEntities<EventModifier>();
}

void runModifyCritBoostEvent(Simulation&) {}
void runRemoveCriticalHitEvent(Simulation&) {}

void runBasePowerEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::CurrentMoveHit, BasePower>();

  dex::Analytic::onBasePower(simulation);

  dex::KnockOff::onBasePower(simulation);

  simulation.view<applyBasePowerEventModifier>();
  simulation.removeFromEntities<EventModifier>();
}

void runModifyDamageEvent(Simulation& simulation) {
  dex::LifeOrb::onModifyDamage(simulation);
  dex::MetronomeItem::Effect::onModifyDamage(simulation);
  dex::Reflect::Effect::onAnyModifyDamage(simulation);
}

void runAfterModifyDamageEvent(Simulation& simulation) {
  dex::FocusSash::onAfterModifyDamage(simulation);
}

void runDamageEvent(Simulation& simulation) {
  dex::FocusSash::onDamage(simulation);

  simulation.removeFromEntities<calc_damage::tags::RanAfterModifyDamage>();
}

void runDamagingHitEvent(Simulation& simulation) {
  dex::Static::onDamagingHit(simulation);
  dex::RockyHelmet::onDamagingHit(simulation);
}

void runHitEvent(Simulation& simulation) {
  dex::SpiritShackle::targetSecondaryEffect::onHit(simulation);
  dex::Transform::targetPrimaryEffect::onHit(simulation);
}

void runAfterHitEvent(Simulation& simulation) {
  dex::KnockOff::onAfterHit(simulation);
}

void runMoveFailEvent(Simulation& simulation) {
  dex::MetronomeItem::Effect::onMoveFail(simulation);
}

void runAfterMoveUsedEvent(Simulation& simulation) {
  dex::LifeOrb::onAfterMoveUsed(simulation);
}

void runModifySecondariesEvent(Simulation&) {}
template <typename StatusType>
void runStatusImmunityEvent(Simulation&) {
  // This is where Corrosion will add CanSetStatus back, the `onSetStatus` events that only block status (i.e. Misty
  // Terrain), and all the `onImmunity` events that relate to non-volatile status conditions will go
}

template void runStatusImmunityEvent<dex::Burn>(Simulation&);
template void runStatusImmunityEvent<dex::Freeze>(Simulation&);
template void runStatusImmunityEvent<dex::Paralysis>(Simulation&);
template void runStatusImmunityEvent<dex::Poison>(Simulation&);
template void runStatusImmunityEvent<dex::Sleep>(Simulation&);
template void runStatusImmunityEvent<dex::Toxic>(Simulation&);

void runAfterSetStatusEvent(Simulation& simulation) {
  dex::LumBerry::onAfterSetStatus(simulation);
}

void runChangeBoostEvent(Simulation&) {}
void runTryBoostEvent(Simulation&) {}

void runAfterEachBoostEvent(Simulation& simulation) {
  dex::Competitive::onAfterEachBoost(simulation);
  dex::Defiant::onAfterEachBoost(simulation);
}

void runAfterBoostEvent(Simulation&) {}
void runModifyMoveAccuracyBoostEvent(Simulation&) {}

void runModifyTarget(Simulation&) {}

void runModifyMove(Simulation& simulation) {
  dex::ChoiceScarf::onSourceModifyMove(simulation);
  dex::ChoiceSpecs::onSourceModifyMove(simulation);
  dex::KingsRock::onModifyMove(simulation);
  dex::LongReach::onModifyMove(simulation);

  dex::Scrappy::onModifyMove(simulation);
}

void runTryMoveEvent(Simulation& simulation) {
  dex::MetronomeItem::Effect::onTryMove(simulation);
}

void runSetBasePower(Simulation& simulation) {
  dex::Reversal::onSetBasePower(simulation);
}

void runResetDisabledMove(Simulation& simulation) {
  dex::ChoiceLock::onResetDisabledMove(simulation);
}

void runResetTrappedPokemon(Simulation& simulation) {
  simulation.addToEntities<tags::ResetTrappedPokemon, tags::ActiveAtTurnEnd, dex::Trapped>();
  simulation.removeFromEntities<dex::Trapped, tags::ActiveAtTurnEnd>();

  dex::Trapped::onResetTrappedPokemon(simulation);

  simulation.removeFromEntities<tags::ResetTrappedPokemon>();
}

void runOnImmunity(Simulation& simulation) {
  dex::Prankster::onImmunity(simulation);
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::SpaStatUpdateRequired>();

  // Priority 1
  dex::ChoiceSpecs::onModifySpa(simulation);

  // Priority 5
  dex::Plus::onModifySpA(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpa>>();
  simulation.removeFromEntities<EventModifier>();
}

void runModifySpd(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::SpdStatUpdateRequired>();

  dex::AssaultVest::onModifySpd(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpd>>();
  simulation.removeFromEntities<EventModifier>();
}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::SpeStatUpdateRequired>();

  dex::ChoiceScarf::onModifySpe(simulation);
  dex::QuickPowder::onModifySpe(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpe>>();
  simulation.removeFromEntities<EventModifier>();

  dex::Paralysis::onModifySpe(simulation);
}

void runStartSleep(Simulation&) {}
void runStartFreeze(Simulation&) {
  // Just Shaymin
}

void runTryTakeItemEvent(Simulation&) {}
void runEatItemEvent(Simulation& simulation) {
  dex::LumBerry::onEat(simulation);
}

void runAfterUseItemEvent(Simulation&) {}
void runAfterEatItemEvent(Simulation&) {}

void runRemoveItemEvent(Simulation& simulation) {
  dex::AssaultVest::onRemove(simulation);
  dex::ChoiceScarf::onRemove(simulation);
  dex::ChoiceSpecs::onRemove(simulation);
}

void runStartAbilityEvent(Simulation&) {}
void runEndAbilityEvent(Simulation&) {}
void runRemoveAbilityEvent(Simulation& simulation) {
  dex::Prankster::onRemove(simulation);
}

void runBeforeSwitchOutEvent(Simulation&) {}

void runSwitchInEvent(Simulation& simulation) {
  dex::Imposter::onSwitchIn(simulation);
}

void runSwitchOutEvent(Simulation& simulation) {
  dex::Trapper::onSwitchOut(simulation);
}

void runFaintEvent(Simulation&) {}
void runAfterFaintEvent(Simulation&) {}
}  // namespace pokesim::internal
