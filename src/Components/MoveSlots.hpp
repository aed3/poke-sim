#pragma once

#include <Types/Constants.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Move.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct MoveSlot {
  dex::Move move = dex::Move::NO_MOVE;
  types::pp pp = Constants::MovePp::DEFAULT;
  types::pp maxPp = Constants::MovePp::DEFAULT;

  constexpr bool operator==(const MoveSlot& other) const {
    return move == other.move && pp == other.pp && maxPp == other.maxPp;
  }
};

struct MoveSlots {
  types::moveSlots<MoveSlot> val{};
};
}  // namespace pokesim
