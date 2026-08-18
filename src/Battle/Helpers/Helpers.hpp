#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/Indexes.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
struct Sides;
struct MoveSlots;
class Pokedex;

PlayerSideId slotToSideId(Slot targetSlot);
types::entity slotToSideEntity(const Sides& sides, Slot targetSlot);
types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot);
types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
void swapEntitySlots(types::registry& registry, types::entity sideEntity, Slot slot1, Slot slot2);
void swapEntitySlots(types::registry& registry, const Sides& sides, Slot slot1, Slot slot2);
types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
types::moveSlotIndex moveToMoveSlot(const MoveSlots& moveSlots, dex::Move move);

namespace internal {
void setupActionMoveBuild(
  types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
  types::entity actionMoveEntity, pokesim::dex::Move move);
}
}  // namespace pokesim
