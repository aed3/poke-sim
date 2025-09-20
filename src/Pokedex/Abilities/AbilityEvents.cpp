#include <Components/EventModifier.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {
void plusOnModifySpa(types::handle, EventModifier&) {}
}  // namespace

void Plus::onModifySpA(Simulation& simulation) {
  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    return;
  }
  simulation.viewForSelectedPokemon<plusOnModifySpa, Tags<ability::tags::Plus>>();
}

void Static::onDamagingHit(Simulation&) {}
}  // namespace pokesim::dex::events