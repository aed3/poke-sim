#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::internal {
void setChoiceLock(types::handle pokemonHandle, const Battle& battle) {
  types::entity moveSlot = pokemonHandle.registry()->get<CurrentActionMoveSlot>(battle.val).val;
  pokemonHandle.emplace<ChoiceLock>(moveSlot);
}
}  // namespace pokesim::dex::internal

namespace pokesim::dex::internal {
void AssaultVestEvents::onModifySpd(EventModifier& eventModifier) {
  chainToModifier(eventModifier.val, latest::AssaultVest::onModifySpdModifier);
}

void ChoiceScarfEvents::onModifySpe(EventModifier& eventModifier) {
  chainToModifier(eventModifier.val, latest::ChoiceScarf::onModifySpeModifier);
}

void ChoiceScarfEvents::onSourceModifyMove(types::handle pokemonHandle, const Battle& battle) {
  setChoiceLock(pokemonHandle, battle);
}

void ChoiceSpecsEvents::onModifySpa(EventModifier& eventModifier) {
  chainToModifier(eventModifier.val, latest::ChoiceSpecs::onModifySpaModifier);
}

void ChoiceSpecsEvents::onSourceModifyMove(types::handle pokemonHandle, const Battle& battle) {
  setChoiceLock(pokemonHandle, battle);
}
}  // namespace pokesim::dex::internal