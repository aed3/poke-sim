#pragma once

#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>
#include <Types/Utilities/variant.hpp>
#include <optional>

namespace pokesim {
struct SlotDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  bool megaEvolve = false;
  bool primalRevert = false;
  bool dynamax = false;
  bool terastallize = false;

  std::optional<dex::Move> moveChoice;
  std::optional<dex::Item> itemChoice;
};

struct SideDecision {
  PlayerSideId sideId = PlayerSideId::NONE;
  types::internal::variant<types::SideSlots<SlotDecision>, types::TeamPositions<types::TeamPositionIndex>> decisions;
};
}  // namespace pokesim