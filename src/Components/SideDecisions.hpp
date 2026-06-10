#pragma once

#include <Types/Decisions.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Utilities/Variant.hpp>

namespace pokesim {
struct SideDecision {
  PlayerSideId sideId = PlayerSideId::NONE;
  internal::variant<types::slotDecisions, types::teamOrder> decisions{};

  bool operator==(const SideDecision other) const { return sideId == other.sideId && decisions == other.decisions; }
};
}  // namespace pokesim
