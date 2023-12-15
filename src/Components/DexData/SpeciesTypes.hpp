#pragma once

#include <Types/Enums/Type.hpp>

namespace pokesim {
// Contains the types a species has
struct SpeciesTypes {
  dex::Type type1 = dex::Type::NO_TYPE;
  dex::Type type2 = dex::Type::NO_TYPE;
};
}  // namespace pokesim