#pragma once

#include <Types/Random.hpp>

namespace pokesim {
struct RngSeed {
  types::rngState val = 1U;
};
}  // namespace pokesim