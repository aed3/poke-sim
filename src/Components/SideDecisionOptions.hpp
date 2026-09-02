#pragma once

#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct SinglesMoveOption {
  dex::Move move = dex::Move::NO_MOVE;
  Slot target = Slot::NONE;

  bool operator==(const SinglesMoveOption& other) const { return move == other.move && target == other.target; }
};

struct DoublesMoveOption {
  dex::Move move = dex::Move::NO_MOVE;
  types::targets<Slot> possibleTargets{};

  bool operator==(const DoublesMoveOption& other) const {
    return move == other.move && possibleTargets == other.possibleTargets;
  }
};

struct SwitchOptions {
  types::teamPositions<Slot> val{};

  bool operator==(const SwitchOptions& other) const { return val == other.val; }
};

struct SinglesSideOptions {
  types::moveSlots<SinglesMoveOption> moves;
  SwitchOptions switches;

  bool operator==(const SinglesSideOptions& other) const { return moves == other.moves && switches == other.switches; }
};

struct DoublesSideOptions {
  types::sideSlots<types::moveSlots<DoublesMoveOption>> moves{moves.max_size(), {}};
  SwitchOptions switches;

  bool operator==(const DoublesSideOptions& other) const { return moves == other.moves && switches == other.switches; }
};

struct TeamPreviewOptions {
  types::teamOrder val{};
};

namespace tags {
struct SinglesMegaEvolutionOption {};
struct SinglesZMoveOption {};
struct SinglesDynamaxOption {};
struct SinglesTerastallizeOption {};
}  // namespace tags

struct DoublesMegaEvolutionOptions {
  types::sideSlots<Slot> val{};
};
struct DoublesZMoveOptions {
  types::sideSlots<Slot> val{};
};
struct DoublesDynamaxOptions {
  types::sideSlots<Slot> val{};
};
struct SinglesTerastallizeOptions {
  types::sideSlots<Slot> val{};
};
}  // namespace pokesim
