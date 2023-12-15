#include "SpeciesDexDataSetup.hpp"

#include <Components/DexData/headers.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
void SpeciesDexDataSetup::setName(Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesDexDataSetup::setType(Type type1, Type type2) {
  handle.emplace<SpeciesTypes>(type1, type2);
}

void SpeciesDexDataSetup::setBaseStats(
  types::BaseStat hp, types::BaseStat atk, types::BaseStat def, types::BaseStat spa, types::BaseStat spd,
  types::BaseStat spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}

void SpeciesDexDataSetup::setPrimaryAbility(Ability ability) {
  handle.emplace<PrimaryAbility>(ability);
}

void SpeciesDexDataSetup::setSecondaryAbility(Ability ability) {
  handle.emplace<SecondaryAbility>(ability);
}

void SpeciesDexDataSetup::setHiddenAbility(Ability ability) {
  handle.emplace<HiddenAbility>(ability);
}
}  // namespace pokesim::dex::internal