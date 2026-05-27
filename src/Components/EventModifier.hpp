#pragma once

#include <Types/Constants.hpp>
#include <Types/Event.hpp>

namespace pokesim {
struct EventModifier {
  types::eventModifier val = Constants::FIXED_POINT_SCALE;
};
}  // namespace pokesim
