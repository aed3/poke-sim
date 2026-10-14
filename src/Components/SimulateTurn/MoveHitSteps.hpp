#pragma once

#include <Types/Stats.hpp>

namespace pokesim::internal {
struct MoveAccuracyBoost {
  types::boost val{};
};

namespace tags {
struct HitsFinished {};
}  // namespace tags
}  // namespace pokesim::internal
