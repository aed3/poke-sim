#pragma once

#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>
#include <Types/Variant.hpp>

namespace pokesim {
struct MoveDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  dex::Move move = dex::Move::NO_MOVE;

  constexpr bool operator==(const MoveDecision& other) const {
    return sourceSlot == other.sourceSlot && targetSlot == other.targetSlot && move == other.move;
  }
};

struct MegaEvolveAndMoveDecision : MoveDecision {};
struct ZMoveDecision : MoveDecision {};
struct DynamaxAndMoveDecision : MoveDecision {};
struct TerastallizeAndMoveDecision : MoveDecision {};

struct SwitchDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  constexpr bool operator==(const SwitchDecision& other) const {
    return sourceSlot == other.sourceSlot && targetSlot == other.targetSlot;
  }
};

struct ItemDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  dex::Item item = dex::Item::NO_ITEM;

  constexpr bool operator==(const ItemDecision& other) const {
    return sourceSlot == other.sourceSlot && targetSlot == other.targetSlot && item == other.item;
  }
};

namespace types {
struct slotDecision : variant<
                        MoveDecision, MegaEvolveAndMoveDecision, TerastallizeAndMoveDecision, DynamaxAndMoveDecision,
                        ZMoveDecision, SwitchDecision, ItemDecision> {
  using variant = variant<
    MoveDecision, MegaEvolveAndMoveDecision, TerastallizeAndMoveDecision, DynamaxAndMoveDecision, ZMoveDecision,
    SwitchDecision, ItemDecision>;
  using variant::variant;

  constexpr Slot sourceSlot() const {
    return std::visit([](auto&& decision) { return decision.sourceSlot; }, (variant::base) * this);
  }

  constexpr Slot targetSlot() const {
    return std::visit([](auto&& decision) { return decision.targetSlot; }, (variant::base) * this);
  }
};

using slotDecisions = sideSlots<slotDecision>;
}  // namespace types
}  // namespace pokesim
