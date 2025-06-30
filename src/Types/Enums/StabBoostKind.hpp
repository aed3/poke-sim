#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class StabBoostKind : std::uint8_t {
  NONE = 100,
  STANDARD = 150,
  STELLAR_NON_STAB = 120,
  ADAPTABILITY_OR_STELLAR_STAB_OR_TERA_STAB = 200,
  ADAPTABILITY_AND_TERA_STAB = 225,
};

static constexpr inline std::array<StabBoostKind, 5U> VALID_STAB_BOOST_KINDS = {
  StabBoostKind::NONE,
  StabBoostKind::STANDARD,
  StabBoostKind::STELLAR_NON_STAB,
  StabBoostKind::ADAPTABILITY_OR_STELLAR_STAB_OR_TERA_STAB,
  StabBoostKind::ADAPTABILITY_AND_TERA_STAB,
};
}  // namespace pokesim