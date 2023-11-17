#pragma once

#include <Types/Enums/Type.hpp>

namespace pokesim {
// Contains the types a species has
struct SpeciesTypes {
  dex::Type t1 = dex::NO_TYPE;
  dex::Type t2 = dex::NO_TYPE;
};
}  // namespace pokesim