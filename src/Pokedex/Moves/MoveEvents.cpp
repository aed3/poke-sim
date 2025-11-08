#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {
void knockOffOnBasePowerCheckRemovableItem(types::registry& registry, CurrentActionTarget target) {
  registry.emplace_or_replace<tags::CanRemoveItem>(target.val);
}

void knockOffOnAfterHitCheckRemovableItem(types::registry& registry, CurrentActionTarget target) {
  if (registry.get<stat::CurrentHp>(target.val).val) {
    registry.emplace_or_replace<tags::CanRemoveItem>(target.val);
  }
}

void knockOffOnBasePower(types::registry& registry, EventModifier& eventModifier, CurrentActionTarget target) {
  if (!registry.all_of<tags::CanRemoveItem>(target.val)) {
    return;
  }
  static constexpr auto modifier = latest::KnockOff::onBasePowerMultiplier;
  chainComponentToModifier(eventModifier, modifier);
}
}  // namespace

void KnockOff::onBasePower(Simulation& simulation) {
  simulation.viewForSelectedMoves<knockOffOnBasePowerCheckRemovableItem, Tags<move::tags::KnockOff>>();
  checkIfCanRemoveItem(simulation);
  simulation.viewForSelectedMoves<knockOffOnBasePower, Tags<move::tags::KnockOff>>();

  simulation.registry.clear<tags::CanRemoveItem>();
}

void KnockOff::onAfterHit(Simulation& simulation) {
  simulation.viewForSelectedMoves<knockOffOnAfterHitCheckRemovableItem, Tags<move::tags::KnockOff>>();
  tryRemoveItem(simulation);
  simulation.registry.clear<tags::CanRemoveItem>();
}
}  // namespace pokesim::dex::events