#pragma once

#include <Types/Enums/Item.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/TargetSlot.hpp>
#include <Types/State.hpp>
#include <variant>

namespace pokesim {
struct SlotDecision {
  TargetSlot targetSlot = TargetSlot::NONE;
  bool megaEvolve = false;
  bool primalRevert = false;
  bool dynamax = false;
  bool terastallize = false;

  std::variant<types::MoveSlotPosition, types::TeamPositionIndex, dex::Item> used;
};

struct SideDecision {
  PlayerSideId sideId = PlayerSideId::P1;
  std::variant<types::SideSlots<SlotDecision>, types::TeamPositions<types::TeamPositionIndex>> decision;
};
}  // namespace pokesim