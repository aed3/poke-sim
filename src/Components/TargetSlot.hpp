#pragma once

#include <cstdint>

namespace pokesim {
struct TargetSlot {
  enum : uint8_t {
    P1A,
    P1B,
    P2A,
    P2B,
  } targetSlot = P1A;
};
}  // namespace pokesim