#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/EventModifier.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "headers.hpp"

namespace pokesim::dex {
void internal::ChoiceSpecsEvents::onModifySpa(EventModifier& eventModifier) {
  chainToModifier(eventModifier.val, latest::ChoiceSpecs::onModifySpaModifier);
}
}  // namespace pokesim::dex