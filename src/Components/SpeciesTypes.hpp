#pragma once

#include <Types/Constants.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Indexes.hpp>
#include <array>

namespace pokesim {
// Contains the types a species has
struct SpeciesTypes {
  std::array<dex::Type, Constants::TYPES_PER_POKEMON> val{};

  constexpr dex::Type& type1() { return val[0]; };
  constexpr dex::Type& type2() { return val[1]; };
  constexpr const dex::Type& type1() const { return val[0]; };
  constexpr const dex::Type& type2() const { return val[1]; };

  constexpr bool has(dex::Type type) const {
    for (dex::Type speciesType : val) {
      if (type == speciesType) {
        return true;
      }
    }
    return false;
  }

  constexpr types::speciesTypeIndex size() const {
    for (types::speciesTypeIndex i = 0; i < Constants::TYPES_PER_POKEMON; i++) {
      if (val[i] == dex::Type::NO_TYPE) {
        return i;
      }
    }
    return Constants::TYPES_PER_POKEMON;
  }

  constexpr bool operator==(const SpeciesTypes& other) const {
    for (types::speciesTypeIndex i = 0; i < Constants::TYPES_PER_POKEMON; i++) {
      if (val[i] != other.val[i]) {
        return false;
      }
    }
    return true;
  }
};
}  // namespace pokesim
