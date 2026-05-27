#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "../Moves/headers.hpp"

namespace pokesim::dex {
namespace {
void knockOffOnBasePowerCheckRemovableItem(
  types::registry& registry, CurrentActionSource source, CurrentActionTarget target) {
  if (registry.get<stat::CurrentHp>(source.val).val > Constants::PokemonCurrentHpStat::MIN) {
    registry.emplace_or_replace<tags::CanRemoveItem>(target.val);
  }
}

void knockOffOnAfterHitCheckRemovableItem(types::registry& registry, CurrentActionTarget target) {
  if (registry.get<stat::CurrentHp>(target.val).val > Constants::PokemonCurrentHpStat::MIN) {
    registry.emplace_or_replace<tags::CanRemoveItem>(target.val);
  }
}

void knockOffOnBasePower(
  types::registry& registry, EventModifier& eventModifier, CurrentActionTarget target,
  types::effectMultiplier modifier) {
  if (!registry.all_of<tags::CanRemoveItem>(target.val)) {
    return;
  }
  chainComponentToModifier(eventModifier, modifier);
}
}  // namespace

void KnockOff::onBasePower(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<KnockOff::onBasePowerMultiplier>();
  simulation.viewForSelectedMoves<knockOffOnBasePowerCheckRemovableItem, Tags<move::tags::KnockOff>>();
  checkIfCanRemoveItem(simulation);
  simulation.viewForSelectedMoves<knockOffOnBasePower, Tags<move::tags::KnockOff>>(modifier);

  simulation.registry.clear<tags::CanRemoveItem>();
}

void KnockOff::onAfterHit(Simulation& simulation) {
  simulation.viewForSelectedMoves<knockOffOnAfterHitCheckRemovableItem, Tags<move::tags::KnockOff>>();
  tryRemoveItem(simulation);
  simulation.registry.clear<tags::CanRemoveItem>();
}
}  // namespace pokesim::dex
