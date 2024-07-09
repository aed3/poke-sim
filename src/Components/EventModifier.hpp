#pragma once

#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>

namespace pokesim {
struct EventModifier {
  types::eventModifier val = MechanicConstants::FIXED_POINT_SCALE;
};
}  // namespace pokesim