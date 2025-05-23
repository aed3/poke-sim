#pragma once

#include <Types/Enums/Type.hpp>
#include <array>

namespace pokesim {
// Contains the types a species has
struct SpeciesTypes {
  std::array<dex::Type, 2U> val{dex::Type::NO_TYPE, dex::Type::NO_TYPE};

  dex::Type& type1() { return val[0]; };
  dex::Type& type2() { return val[1]; };
  constexpr const dex::Type& type1() const { return val[0]; };
  constexpr const dex::Type& type2() const { return val[1]; };
  constexpr std::uint8_t size() const {
    if (type2() == dex::Type::NO_TYPE) {
      return type1() == dex::Type::NO_TYPE ? 0 : 1;
    }
    return 2;
  }
  constexpr bool has(dex::Type type) const { return type1() == type || type2() == type; }
};
}  // namespace pokesim