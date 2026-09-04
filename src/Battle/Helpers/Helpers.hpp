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

Slot sideIdAndPositionToSlot(PlayerSideId sideId, types::teamPositionIndex position);
PlayerSideId slotToSideId(Slot slot);
types::entity slotToSideEntity(const Sides& sides, Slot slot);
types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot slot);
types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot slot);
void swapEntitySlots(types::registry& registry, types::entity sideEntity, Slot slot1, Slot slot2);
void swapEntitySlots(types::registry& registry, const Sides& sides, Slot slot1, Slot slot2);
types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot slot);
types::moveSlotIndex moveToMoveSlot(const MoveSlots& moveSlots, dex::Move move);
}  // namespace pokesim
