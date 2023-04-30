#include "SpeciesSetup.hpp"

#include <Components/DexData/DexData.hpp>

namespace pokesim::dex::internal {
void SpeciesSetup::setName(Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesSetup::setType(Type type1, Type type2) {
  handle.emplace<Types>(type1, type2);
}

void SpeciesSetup::setBaseStats(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}
}  // namespace pokesim::dex::internal