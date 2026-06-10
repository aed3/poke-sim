#pragma once

#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>
#include <Utilities/Variant.hpp>

namespace pokesim {
struct SlotDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  bool megaEvolve = false;
  bool primalRevert = false;
  bool dynamax = false;
  bool terastallize = false;

  internal::variant<std::monostate, dex::Move, dex::Item> moveOrItem;

  bool operator==(const SlotDecision& other) const {
    return sourceSlot == other.sourceSlot && targetSlot == other.targetSlot && megaEvolve == other.megaEvolve &&
           primalRevert == other.primalRevert && dynamax == other.dynamax && terastallize == other.terastallize &&
           moveOrItem == other.moveOrItem;
  }
};

namespace types {
using slotDecisions = types::sideSlots<SlotDecision>;
}
}  // namespace pokesim
