#pragma once

#include <Types/Enums/Item.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/TargetSlot.hpp>
#include <Types/State.hpp>
#include <optional>

namespace pokesim {
struct SlotDecision {
  TargetSlot targetSlot = TargetSlot::NONE;
  bool megaEvolve = false;
  bool primalRevert = false;
  bool dynamax = false;
  bool terastallize = false;

  std::optional<types::MoveSlotPosition> moveSlotUsed = std::nullopt;
  std::optional<types::TeamPositionIndex> teamSlotSwitchingIn = std::nullopt;
  std::optional<dex::Item> itemUsed = std::nullopt;
};

struct SideDecision {
  PlayerSideId sideId = PlayerSideId::P1;
  std::optional<types::SideSlots<SlotDecision>> slotDecisions = std::nullopt;
  std::optional<types::TeamPositions<types::TeamPositionIndex>> teamPreviewSelection = std::nullopt;
};
}  // namespace pokesim