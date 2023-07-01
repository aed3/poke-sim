#include "SpeciesDexDataSetup.hpp"

#include <Components/DexData/headers.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
void SpeciesDexDataSetup::setName(Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesDexDataSetup::setType(Type type1, Type type2) {
  handle.emplace<SpeciesTypes>(type1, type2);
}

void SpeciesDexDataSetup::setBaseStats(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}
}  // namespace pokesim::dex::internal