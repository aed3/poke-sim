#pragma once

#include <Types/Decisions.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <Types/Variant.hpp>

namespace pokesim {
struct SideDecision {
  PlayerSideId sideId = PlayerSideId::NONE;
  types::variant<types::slotDecisions, types::teamOrder> decisions{};

  bool operator==(const SideDecision other) const { return sideId == other.sideId && decisions == other.decisions; }
};

struct MidTurnSideDecision {
  types::sideSlots<SwitchDecision> val{};
};
}  // namespace pokesim
