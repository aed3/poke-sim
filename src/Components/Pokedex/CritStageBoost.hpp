#pragma once

#include <Types/Constants.hpp>
#include <Types/Damage.hpp>

namespace pokesim {
struct CritStageBoost {
  types::critStage val = Constants::CritStage::DEFAULT;
};
}  // namespace pokesim
