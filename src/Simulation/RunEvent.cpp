#include "RunEvent.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Pokedex/Items/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/Tags.hpp>

#include "Simulation.hpp"

// TODO(aed3) Auto generate?

namespace pokesim {
namespace internal {
template <typename ModifiedComponent>
void applyEventModifier(ModifiedComponent& component, const EventModifier& eventModifier) {
  applyChainedModifier(component.val, eventModifier.val);
}
}  // namespace internal

void runAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyCritBoostEvent(Simulation& /*simulation*/) {}

void runBasePowerEvent(Simulation& /*simulation*/) {}

void runDamagingHitEvent(Simulation& simulation) {
  dex::latest::Static::onDamagingHit(simulation);
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();
  simulation.viewForSelectedPokemon<dex::latest::ChoiceSpecs::onModifySpa, Tags<item::tags::ChoiceSpecs>>();
  simulation.viewForSelectedPokemon<internal::applyEventModifier<stat::EffectiveSpa>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpd(Simulation&) {}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();
  simulation.viewForSelectedPokemon<internal::applyEventModifier<stat::EffectiveSpe>>();
  simulation.registry.clear<EventModifier>();

  simulation.viewForSelectedPokemon<
    dex::latest::Static::onModifySpe,
    Tags<status::tags::Paralysis> /*, entt::exclude_t<ability::tags::QuickFeet>*/>();
}
}  // namespace pokesim