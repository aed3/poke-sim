#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct Sides;
struct MoveSlots;
class Pokedex;

types::entity slotToSideEntity(const Sides& sides, Slot targetSlot);
types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot);
types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
types::moveSlotIndex moveToMoveSlot(const MoveSlots& moveSlots, dex::Move move);

namespace internal {
void setupActionMoveBuild(
  types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
  types::entity actionMoveEntity, pokesim::dex::Move move);
}
}  // namespace pokesim
