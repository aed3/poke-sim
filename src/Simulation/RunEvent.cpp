#include "RunEvent.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/Items/headers.hpp>
#include <Simulation/RegistryContainer.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>
#include <cstdint>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

#include "Simulation.hpp"

// TODO(aed3) Auto generate?

namespace pokesim {
namespace internal {
template <typename ModifiedComponent>
void applyEventModifier(ModifiedComponent& component, const EventModifier& eventModifier) {
  applyChainedModifier(component.val, eventModifier.val);
}

template <typename... PokemonSpecifiers>
RegistryContainer::SelectionFunction getMoveEventPokemonSelector() {
  static const size_t SelectAnyPokemon = sizeof...(PokemonSpecifiers) == 0U;
  return {[](const void*, const types::registry& registry) -> std::vector<types::entity> {
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
        entities.insert(registry.get<CurrentActionTargets>(entity).only());
      }
    });

    return {entities.begin(), entities.end()};
  }};
}
}  // namespace internal

void runAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyCritBoostEvent(Simulation& /*simulation*/) {}

void runBasePowerEvent(Simulation& /*simulation*/) {}

void runDamagingHitEvent(Simulation& simulation) {
  dex::latest::Static::onDamagingHit(simulation);
}

void runModifyMove(Simulation& simulation) {
  internal::SelectForPokemonView<> selectedPokemon{
    simulation,
    internal::getMoveEventPokemonSelector<tags::CurrentActionMoveSource>()};

  simulation.viewForSelectedPokemon<
    dex::latest::ChoiceScarf::onSourceModifyMove,
    Tags<item::tags::ChoiceScarf, tags::CurrentActionMoveSource>,
    entt::exclude_t<ChoiceLock>>();

  simulation.viewForSelectedPokemon<
    dex::latest::ChoiceSpecs::onSourceModifyMove,
    Tags<item::tags::ChoiceSpecs, tags::CurrentActionMoveSource>,
    entt::exclude_t<ChoiceLock>>();
}

void runDisableMove(Simulation& simulation) {
  simulation.viewForSelectedPokemon<dex::latest::ChoiceLock::onDisableMove>();
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  simulation.viewForSelectedPokemon<dex::latest::ChoiceSpecs::onModifySpa, Tags<item::tags::ChoiceSpecs>>();

  simulation.viewForSelectedPokemon<internal::applyEventModifier<stat::EffectiveSpa>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpd(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  simulation.viewForSelectedPokemon<dex::latest::AssaultVest::onModifySpd, Tags<item::tags::AssaultVest>>();

  simulation.viewForSelectedPokemon<internal::applyEventModifier<stat::EffectiveSpd>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  simulation.viewForSelectedPokemon<dex::latest::ChoiceScarf::onModifySpe, Tags<item::tags::ChoiceScarf>>();

  simulation.viewForSelectedPokemon<internal::applyEventModifier<stat::EffectiveSpe>>();
  simulation.registry.clear<EventModifier>();

  simulation.viewForSelectedPokemon<
    dex::latest::Static::onModifySpe,
    Tags<status::tags::Paralysis> /*, entt::exclude_t<ability::tags::QuickFeet>*/>();
}
}  // namespace pokesim