#include "RunEvent.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Components/ChoiceLock.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/Items/headers.hpp>
#include <Pokedex/Moves/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/Tags.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>

#include "Simulation.hpp"

// TODO(aed3) Autogenerate?

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

void runBeforeMove(Simulation& simulation) {
  pokesim::dex::Paralysis::onBeforeMove(simulation);
  pokesim::dex::ChoiceLock::onBeforeMove(simulation);
}

void runResidual(Simulation& simulation) {
  pokesim::dex::Burn::onResidual(simulation);
}

void runAccuracyEvent(Simulation&) {}

void runModifyAccuracyEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::CurrentMoveHit, Accuracy>();

  pokesim::dex::BrightPowder::onModifyAccuracy(simulation);

  simulation.view<applyEventModifier<Accuracy>>();
  simulation.registry.clear<EventModifier>();
}

void runModifyCritBoostEvent(Simulation&) {}

void runBasePowerEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::CurrentMoveHit, BasePower>();

  pokesim::dex::KnockOff::onBasePower(simulation);

  simulation.view<applyBasePowerEventModifier>();
  simulation.registry.clear<EventModifier>();
}

void runModifyDamageEvent(Simulation& simulation) {
  pokesim::dex::LifeOrb::onModifyDamage(simulation);
}

void runAfterModifyDamageEvent(Simulation& simulation) {
  pokesim::dex::FocusSash::onAfterModifyDamage(simulation);
}

void runDamageEvent(Simulation& simulation) {
  pokesim::dex::FocusSash::onDamage(simulation);

  simulation.registry.clear<calc_damage::tags::RanAfterModifyDamage>();
}

void runDamagingHitEvent(Simulation& simulation) {
  pokesim::dex::Static::onDamagingHit(simulation);
}

void runAfterHitEvent(Simulation& simulation) {
  pokesim::dex::KnockOff::onAfterHit(simulation);
}

void runAfterMoveUsedEvent(Simulation& simulation) {
  pokesim::dex::LifeOrb::onAfterMoveUsed(simulation);
}

void runModifySecondariesEvent(Simulation&) {}
template <typename StatusType>
void runStatusImmunityEvent(Simulation&) {
  // This is where Corrosion will add CanSetStatus back, the `onSetStatus` events that only block status (i.e. Misty
  // Terrain), and all the `onImmunity` events that relate to non-volatile status conditions will go
}

template void runStatusImmunityEvent<pokesim::status::tags::Burn>(Simulation&);
template void runStatusImmunityEvent<pokesim::status::tags::Freeze>(Simulation&);
template void runStatusImmunityEvent<pokesim::status::tags::Paralysis>(Simulation&);
template void runStatusImmunityEvent<pokesim::status::tags::Poison>(Simulation&);
template void runStatusImmunityEvent<pokesim::status::tags::Sleep>(Simulation&);
template void runStatusImmunityEvent<pokesim::status::tags::Toxic>(Simulation&);

void runAfterSetStatusEvent(Simulation&) {}

void runChangeBoostEvent(Simulation&) {}
void runTryBoostEvent(Simulation&) {}

template <typename BoostType>
void runAfterEachBoostEvent(Simulation&) {}
template void runAfterEachBoostEvent<AtkBoost>(Simulation&);
template void runAfterEachBoostEvent<DefBoost>(Simulation&);
template void runAfterEachBoostEvent<SpaBoost>(Simulation&);
template void runAfterEachBoostEvent<SpdBoost>(Simulation&);
template void runAfterEachBoostEvent<SpeBoost>(Simulation&);

void runAfterBoostEvent(Simulation&) {}

void runModifyTarget(Simulation&) {}

void runModifyMove(Simulation& simulation) {
  pokesim::dex::ChoiceScarf::onSourceModifyMove(simulation);
  pokesim::dex::ChoiceSpecs::onSourceModifyMove(simulation);
}

void runDisableMove(Simulation& simulation) {
  pokesim::dex::ChoiceLock::onDisableMove(simulation);
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::SpaStatUpdateRequired>();

  // Priority 1
  pokesim::dex::ChoiceSpecs::onModifySpa(simulation);

  // Priority 5
  pokesim::dex::Plus::onModifySpA(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpa>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpd(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::SpdStatUpdateRequired>();

  pokesim::dex::AssaultVest::onModifySpd(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpd>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, pokesim::tags::SpeStatUpdateRequired>();

  pokesim::dex::ChoiceScarf::onModifySpe(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpe>>();
  simulation.registry.clear<EventModifier>();

  pokesim::dex::Paralysis::onModifySpe(simulation);
}

void runStartSleep(Simulation&) {}
void runStartFreeze(Simulation&) {
  // Just Shaymin
}

void runTryTakeItemEvent(Simulation&) {}
void runAfterUseItemEvent(Simulation&) {}

void runEndItemEvent(Simulation& simulation) {
  pokesim::dex::AssaultVest::onEnd(simulation);
  pokesim::dex::ChoiceScarf::onEnd(simulation);
  pokesim::dex::ChoiceSpecs::onEnd(simulation);
}

void runEndAbilityEvent(Simulation&) {}
void runFaintEvent(Simulation&) {}
void runAfterFaintEvent(Simulation&) {}
}  // namespace pokesim::internal
