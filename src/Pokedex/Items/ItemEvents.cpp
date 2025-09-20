#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {
void setChoiceLock(types::handle pokemonHandle, Battle battle) {
  types::entity moveSlot = pokemonHandle.registry()->get<CurrentActionMoveSlot>(battle.val).val;
  pokemonHandle.emplace<pokesim::ChoiceLock>(moveSlot);
}
}  // namespace

void AssaultVest::onModifySpd(Simulation& simulation) {
  constexpr auto modifier = latest::AssaultVest::onModifySpdModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::AssaultVest>>(
    modifier);
}

void ChoiceScarf::onModifySpe(Simulation& simulation) {
  constexpr auto modifier = latest::ChoiceScarf::onModifySpeModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::ChoiceScarf>>(
    modifier);
}

void ChoiceScarf::onSourceModifyMove(Simulation& simulation) {
  simulation
    .view<setChoiceLock, Tags<item::tags::ChoiceScarf, tags::CurrentActionMoveSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceSpecs::onModifySpa(Simulation& simulation) {
  constexpr auto modifier = latest::ChoiceSpecs::onModifySpaModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::ChoiceSpecs>>(
    modifier);
}

void ChoiceSpecs::onSourceModifyMove(Simulation& simulation) {
  simulation
    .view<setChoiceLock, Tags<item::tags::ChoiceScarf, tags::CurrentActionMoveSource>, entt::exclude_t<ChoiceLock>>();
}
}  // namespace pokesim::dex::events