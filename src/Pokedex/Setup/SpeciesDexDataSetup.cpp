#include "SpeciesDexDataSetup.hpp"

#include <Components/Names/SpeciesNames.hpp>
#include <Components/Pokedex/headers.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::internal::dex {
void SpeciesDexDataSetup::setName(pokesim::dex::Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesDexDataSetup::setType(pokesim::dex::Type type1, pokesim::dex::Type type2) {
  handle.emplace<SpeciesTypes>(type1, type2);
}

void SpeciesDexDataSetup::setBaseStats(
  types::baseStat hp, types::baseStat atk, types::baseStat def, types::baseStat spa, types::baseStat spd,
  types::baseStat spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}

void SpeciesDexDataSetup::setPrimaryAbility(pokesim::dex::Ability ability) {
  handle.emplace<PrimaryAbility>(ability);
}

void SpeciesDexDataSetup::setSecondaryAbility(pokesim::dex::Ability ability) {
  handle.emplace<SecondaryAbility>(ability);
}

void SpeciesDexDataSetup::setHiddenAbility(pokesim::dex::Ability ability) {
  handle.emplace<HiddenAbility>(ability);
}
}  // namespace pokesim::internal::dex
