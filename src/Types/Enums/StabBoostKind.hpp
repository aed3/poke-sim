#pragma once

#include <cstdint>

namespace pokesim {
enum class StabBoostKind : std::uint8_t {
  NONE = 100,
  STANDARD = 150,
  STELLAR_NON_STAB = 120,
  ADAPTABILITY_OR_STELLAR_STAB_OR_TERA_STAB = 200,
  ADAPTABILITY_AND_TERA_STAB = 225,
};
}