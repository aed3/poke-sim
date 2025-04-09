#pragma once

#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>
#include <Utilities/Variant.hpp>
#include <optional>

namespace pokesim {
struct SlotDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  bool megaEvolve = false;
  bool primalRevert = false;
  bool dynamax = false;
  bool terastallize = false;

  std::optional<dex::Move> moveChoice = std::nullopt;
  std::optional<dex::Item> itemChoice = std::nullopt;

  bool operator==(const SlotDecision& other) const {
    return sourceSlot == other.sourceSlot && targetSlot == other.targetSlot && megaEvolve == other.megaEvolve &&
           primalRevert == other.primalRevert && dynamax == other.dynamax && terastallize == other.terastallize &&
           moveChoice == other.moveChoice && itemChoice == other.itemChoice;
  }
};

namespace types {
using slotDecisions = types::sideSlots<SlotDecision>;
}

struct SideDecision {
  PlayerSideId sideId = PlayerSideId::NONE;
  internal::variant<types::slotDecisions, types::teamOrder> decisions{};

  bool operator==(const SideDecision other) const { return sideId == other.sideId && decisions == other.decisions; }
};
}  // namespace pokesim