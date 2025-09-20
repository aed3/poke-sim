#include "Pokedex.hpp"

#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/headers.hpp>
#include <Types/Registry.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
template <typename Build, typename T>
void Pokedex::load(entt::dense_map<T, types::entity>& map, const entt::dense_set<T>& list, Build build) {
  map.reserve(map.size() + list.size());
  for (T listItem : list) {
    POKESIM_REQUIRE(!map.contains(listItem), "Shouldn't build data entries twice.");
    map[listItem] = build(listItem);
  }
}

void Pokedex::loadSpecies(const entt::dense_set<dex::Species>& speciesSet) {
  load(speciesMap, speciesSet, [this](dex::Species species) { return buildSpecies(species, dexRegistry); });
}

void Pokedex::loadItems(const entt::dense_set<dex::Item>& itemSet) {
  load(itemsMap, itemSet, [this](dex::Item item) { return buildItem(item, dexRegistry); });
}

void Pokedex::loadMoves(const entt::dense_set<dex::Move>& moveSet) {
  load(movesMap, moveSet, [this](dex::Move move) { return buildMove(move, dexRegistry, false); });
}

void Pokedex::loadAbilities(const entt::dense_set<dex::Ability>& abilitySet) {
  load(abilitiesMap, abilitySet, [this](dex::Ability ability) { return buildAbility(ability, dexRegistry); });
}

types::entity Pokedex::buildActionMove(dex::Move move, types::registry& registry) const {
  return buildMove(move, registry, true);
}
}  // namespace pokesim