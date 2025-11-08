#include "RunEvent.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/Items/headers.hpp>
#include <Pokedex/Moves/headers.hpp>
#include <Simulation/RegistryContainer.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>
#include <cstddef>
#include <cstdint>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

#include "Simulation.hpp"

// TODO(aed3) Auto generate?

namespace pokesim {
namespace {
template <typename ModifiedComponent>
void applyEventModifier(ModifiedComponent& component, EventModifier eventModifier) {
  component.val = applyChainedModifier(component.val, eventModifier.val);
}

template <typename... PokemonSpecifiers>
internal::RegistryContainer::SelectionFunction getMoveEventPokemonSelector() {
  static const size_t SelectAnyPokemon = sizeof...(PokemonSpecifiers) == 0U;
  return internal::RegistryContainer::SelectionFunction{
    [](const void*, const types::registry& registry) -> types::entityVector {
      entt::dense_set<types::entity> entities;
      auto selectedMoveView = registry.view<tags::SelectedForViewMove>();
      auto begin = selectedMoveView.begin();
      auto end = selectedMoveView.end();
      if (selectedMoveView.empty()) {
        auto anyMoveView = registry.view<tags::CurrentActionMove>();
        begin = anyMoveView.begin();
        end = anyMoveView.end();
      }

      std::for_each(begin, end, [&registry, &entities](types::entity entity) {
        if constexpr (
          SelectAnyPokemon || std::disjunction_v<std::is_same<PokemonSpecifiers, tags::CurrentActionMoveSource>...>) {
          entities.insert(registry.get<CurrentActionSource>(entity).val);
        }

        if constexpr (
          SelectAnyPokemon || std::disjunction_v<std::is_same<PokemonSpecifiers, tags::CurrentActionMoveTarget>...>) {
          entities.insert(registry.get<CurrentActionTarget>(entity).val);
        }
      });

      return {entities.begin(), entities.end()};
    }};
}

void applyBasePowerEventModifier(types::handle moveHandle, BasePower basePower, EventModifier eventModifier) {
  calc_damage::Power& power = moveHandle.emplace<calc_damage::Power>(basePower.val);
  power.val = applyChainedModifier(power.val, eventModifier.val);
}
}  // namespace

void runAccuracyEvent(Simulation&) {}

void runModifyAccuracyEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewMove, Accuracy>();

  dex::events::BrightPowder::onModifyAccuracy(simulation);

  simulation.viewForSelectedMoves<applyEventModifier<Accuracy>>();
  simulation.registry.clear<EventModifier>();
}

void runModifyCritBoostEvent(Simulation&) {}

void runBasePowerEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewMove, BasePower>();

  dex::events::KnockOff::onBasePower(simulation);

  simulation.viewForSelectedMoves<applyBasePowerEventModifier>();
  simulation.registry.clear<EventModifier>();
}

void runModifyDamageEvent(Simulation& simulation) {
  dex::events::LifeOrb::onModifyDamage(simulation);
}

void runAfterModifyDamageEvent(Simulation& simulation) {
  dex::events::FocusSash::onAfterModifyDamage(simulation);
}

void runDamageEvent(Simulation& simulation) {
  dex::events::FocusSash::onDamage(simulation);
}

void runDamagingHitEvent(Simulation& simulation) {
  dex::events::Static::onDamagingHit(simulation);
}

void runAfterHitEvent(Simulation& simulation) {
  dex::events::KnockOff::onAfterHit(simulation);
}

void runAfterMoveUsedEvent(Simulation& simulation) {
  dex::events::LifeOrb::onAfterMoveUsed(simulation);
}

void runModifySecondariesEvent(Simulation&) {}
template <typename StatusType>
void runStatusImmunityEvent(Simulation&) {
  // This is where Corrosion (will add CanSetStatus back), the `onSetStatus` events that only block status (i.e. Misty
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

  dex::events::ChoiceScarf::onSourceModifyMove(simulation);
  dex::events::ChoiceSpecs::onSourceModifyMove(simulation);
}

void runDisableMove(Simulation& simulation) {
  dex::events::ChoiceLock::onDisableMove(simulation);
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  // Priority 1
  dex::events::ChoiceSpecs::onModifySpa(simulation);

  // Priority 5
  dex::events::Plus::onModifySpA(simulation);

  simulation.viewForSelectedPokemon<applyEventModifier<stat::EffectiveSpa>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpd(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  dex::events::AssaultVest::onModifySpd(simulation);

  simulation.viewForSelectedPokemon<applyEventModifier<stat::EffectiveSpd>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  dex::events::ChoiceScarf::onModifySpe(simulation);

  simulation.viewForSelectedPokemon<applyEventModifier<stat::EffectiveSpe>>();
  simulation.registry.clear<EventModifier>();

  dex::events::Paralysis::onModifySpe(simulation);
}

void runStartSleep(Simulation&) {}
void runStartFreeze(Simulation&) {
  // Just Shaymin
}

void runTryTakeItemEvent(Simulation&) {}
void runAfterUseItemEvent(Simulation&) {}

void runEndItemEvent(Simulation& simulation) {
  dex::events::ChoiceScarf::onEnd(simulation);
  dex::events::ChoiceSpecs::onEnd(simulation);
}
}  // namespace pokesim