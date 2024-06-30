#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>

namespace pokesim {
struct Sides;
struct MoveSlots;
class Pokedex;

types::entity slotToSideEntity(const Sides& sides, Slot targetSlot);
types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot);
types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
types::entity moveToEntity(const types::registry& registry, const MoveSlots& moveSlots, dex::Move move);

types::entity createActionMoveForTarget(
  types::handle targetHandle, types::entity battleEntity, types::entity sourceEntity, dex::Move move,
  const Pokedex& pokedex);
}  // namespace pokesim