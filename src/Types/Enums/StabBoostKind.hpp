#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class StabBoostKind : std::uint8_t {
  NONE = 100U,
  STANDARD = 150U,
  STELLAR_NON_STAB = 120U,
  ADAPTABILITY_OR_STELLAR_STAB_OR_TERA_STAB = 200U,
  ADAPTABILITY_AND_TERA_STAB = 225U,
};

static constexpr inline std::array<StabBoostKind, 5U> VALID_STAB_BOOST_KINDS = {
  StabBoostKind::NONE,
  StabBoostKind::STANDARD,
  StabBoostKind::STELLAR_NON_STAB,
  StabBoostKind::ADAPTABILITY_OR_STELLAR_STAB_OR_TERA_STAB,
  StabBoostKind::ADAPTABILITY_AND_TERA_STAB,
};
}  // namespace pokesim