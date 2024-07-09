#pragma once

#include <Types/Enums/Type.hpp>
#include <array>

namespace pokesim {
// Contains the types a species has
struct SpeciesTypes {
  std::array<dex::Type, 2U> val{dex::Type::NO_TYPE, dex::Type::NO_TYPE};

  dex::Type& type1() { return val[0]; };
  dex::Type& type2() { return val[1]; };
  const dex::Type& type1() const { return val[0]; };
  const dex::Type& type2() const { return val[1]; };
  const std::uint8_t size() const { return type2() == dex::Type::NO_TYPE ? type1() == dex::Type::NO_TYPE ? 0 : 1 : 2; }
  const bool has(dex::Type type) const { return type1() == type || type2() == type; }
};
}  // namespace pokesim