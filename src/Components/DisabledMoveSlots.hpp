#pragma once

#include <Types/State.hpp>

namespace pokesim {
struct DisabledMoveSlots {
  types::moveSlots<bool> val{};
};
}  // namespace pokesim
