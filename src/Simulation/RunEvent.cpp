#include "RunEvent.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/Items/headers.hpp>
#include <Pokedex/Moves/headers.hpp>
#include <Simulation/RegistryContainer.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>

#include "Simulation.hpp"

// TODO(aed3) Autogenerate?

namespace pokesim {
namespace {
template <typename ModifiedComponent>
void applyEventModifier(ModifiedComponent& component, EventModifier eventModifier) {
  component.val = applyChainedModifier(component.val, eventModifier.val);
}

void applyBasePowerEventModifier(types::handle moveHandle, BasePower basePower, EventModifier eventModifier) {
  calc_damage::Power& power = moveHandle.emplace<calc_damage::Power>(basePower.val);
  power.val = applyChainedModifier(power.val, eventModifier.val);
}
}  // namespace

void runBeforeMove(Simulation& simulation) {
  dex::Paralysis::onBeforeMove(simulation);
  dex::ChoiceLock::onBeforeMove(simulation);
}

void runResidual(Simulation& simulation) {
  pokesim::internal::SelectForPokemonView<tags::ActivePokemon> selectedPokemon{simulation};
  if (selectedPokemon.hasNoneSelected()) {
    return;
  }
  dex::Burn::onResidual(simulation);
}

void runAccuracyEvent(Simulation&) {}

void runModifyAccuracyEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewMove, Accuracy>();

  dex::BrightPowder::onModifyAccuracy(simulation);

  simulation.viewForSelectedMoves<applyEventModifier<Accuracy>>();
  simulation.registry.clear<EventModifier>();
}

void runModifyCritBoostEvent(Simulation&) {}

void runBasePowerEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewMove, BasePower>();

  dex::KnockOff::onBasePower(simulation);

  simulation.viewForSelectedMoves<applyBasePowerEventModifier>();
  simulation.registry.clear<EventModifier>();
}

void runModifyDamageEvent(Simulation& simulation) {
  dex::LifeOrb::onModifyDamage(simulation);
}

void runAfterModifyDamageEvent(Simulation& simulation) {
  dex::FocusSash::onAfterModifyDamage(simulation);
}

void runDamageEvent(Simulation& simulation) {
  dex::FocusSash::onDamage(simulation);

  simulation.registry.clear<calc_damage::tags::RanAfterModifyDamage>();
}

void runDamagingHitEvent(Simulation& simulation) {
  dex::Static::onDamagingHit(simulation);
}

void runAfterHitEvent(Simulation& simulation) {
  dex::KnockOff::onAfterHit(simulation);
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

template void runStatusImmunityEvent<status::tags::Burn>(Simulation&);
template void runStatusImmunityEvent<status::tags::Freeze>(Simulation&);
template void runStatusImmunityEvent<status::tags::Paralysis>(Simulation&);
template void runStatusImmunityEvent<status::tags::Poison>(Simulation&);
template void runStatusImmunityEvent<status::tags::Sleep>(Simulation&);
template void runStatusImmunityEvent<status::tags::Toxic>(Simulation&);

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

void runModifyMove(Simulation& simulation) {
  // internal::SelectForPokemonView<> selectedPokemon{
  //   simulation,
  //   getMoveEventPokemonSelector<tags::CurrentActionMoveSource>()};

  dex::ChoiceScarf::onSourceModifyMove(simulation);
  dex::ChoiceSpecs::onSourceModifyMove(simulation);
}

void runDisableMove(Simulation& simulation) {
  dex::ChoiceLock::onDisableMove(simulation);
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SpaStatUpdateRequired>();

  // Priority 1
  dex::ChoiceSpecs::onModifySpa(simulation);

  // Priority 5
  dex::Plus::onModifySpA(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpa>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpd(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SpdStatUpdateRequired>();

  dex::AssaultVest::onModifySpd(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpd>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SpeStatUpdateRequired>();

  dex::ChoiceScarf::onModifySpe(simulation);

  simulation.view<applyEventModifier<stat::EffectiveSpe>>();
  simulation.registry.clear<EventModifier>();

  dex::Paralysis::onModifySpe(simulation);
}

void runStartSleep(Simulation&) {}
void runStartFreeze(Simulation&) {
  // Just Shaymin
}

void runTryTakeItemEvent(Simulation&) {}
void runAfterUseItemEvent(Simulation&) {}

void runEndItemEvent(Simulation& simulation) {
  dex::AssaultVest::onEnd(simulation);
  dex::ChoiceScarf::onEnd(simulation);
  dex::ChoiceSpecs::onEnd(simulation);
}

void runEndAbilityEvent(Simulation&) {}
void runFaintEvent(Simulation&) {}
void runAfterFaintEvent(Simulation&) {}
}  // namespace pokesim
