#include "Pokedex.hpp"

#include <Components/Pokedex/Abilities.hpp>
#include <Components/Tags/Selection.hpp>
#include <Config/Require.hpp>
#include <Pokedex/EnumToTag/AbilityEnumToTag.hpp>
#include <Pokedex/EnumToTag/MoveEnumToTag.hpp>
#include <Simulation/BattleCreationInfo.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/headers.hpp>
#include <Types/Registry.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
namespace {
template <typename Ability>
struct AddAssociatedFormsToSet {
  template <typename, typename = void>
  struct hasAssociatedForms : std::false_type {};
  template <typename T>
  struct hasAssociatedForms<T, std::void_t<typename T::associatedForms>> : std::true_type {};

  static void run(entt::dense_set<dex::Species>& speciesSet, const Pokedex* pokedex) {
    if constexpr (hasAssociatedForms<Ability>::value) {
      for (dex::Species species : pokedex->getStaticValue<Ability::associatedForms::all>()) {
        speciesSet.insert(species);
      }
    }
  }
};
}  // namespace

template <typename Build, typename T>
void Pokedex::load(entt::dense_map<T, types::entity>& map, const entt::dense_set<T>& list, Build build) {
  map.reserve(map.size() + list.size());
  for (T listItem : list) {
    if (!map.contains(listItem)) {
      map[listItem] = build(listItem);
    }
  }
}

void Pokedex::loadSpecies(const entt::dense_set<dex::Species>& speciesSet) {
  load(speciesMap, speciesSet, [this](dex::Species species) { return buildSpecies(species, dexRegistry); });
}

void Pokedex::loadItems(const entt::dense_set<dex::Item>& itemSet) {
  load(itemsMap, itemSet, [this](dex::Item item) { return buildItem(item, dexRegistry); });
}

void Pokedex::loadAbilities(const entt::dense_set<dex::Ability>& abilitySet) {
  load(abilitiesMap, abilitySet, [this](dex::Ability ability) { return buildAbility(ability, dexRegistry); });
}

void Pokedex::loadMoves(const entt::dense_set<dex::Move>& moveSet) {
  for (dex::Move move : moveSet) {
    if (movesMap.contains(move)) {
      continue;
    }

    types::entity moveEntity = dexRegistry.create();
    movesMap[move] = moveEntity;
    dex::emplaceTagFromEnum(move, dexRegistry, moveEntity);
    dexRegistry.emplace<internal::tags::BuildPokedexMove>(moveEntity);
  }

  buildMoves(dexRegistry);
  dexRegistry.clear<internal::tags::BuildPokedexMove>();
}

void Pokedex::loadForBattleInfo(const std::vector<BattleCreationInfo>& battleInfoList) {
  entt::dense_set<dex::Species> speciesSet{};
  entt::dense_set<dex::Item> itemSet{};
  entt::dense_set<dex::Move> moveSet{};
  entt::dense_set<dex::Ability> abilitySet{};
  entt::dense_set<dex::Species> speciesMissingAbility{};
  entt::dense_set<dex::Species> associatedFormsForAbilities{};

  for (const BattleCreationInfo& battleCreationInfo : battleInfoList) {
    for (const auto& side : battleCreationInfo.sides) {
      for (const auto& pokemon : side.team) {
        speciesSet.insert(pokemon.species);
        for (const auto& moveSlot : pokemon.moves) {
          moveSet.insert(moveSlot.name);
        }
        if (pokemon.item.value_or(dex::Item::NO_ITEM) != dex::Item::NO_ITEM) {
          itemSet.insert(pokemon.item.value());
        }

        if (pokemon.ability.value_or(dex::Ability::NO_ABILITY) != dex::Ability::NO_ABILITY) {
          abilitySet.insert(pokemon.ability.value());
        }
        else if (!pokemon.ability.has_value()) {
          speciesMissingAbility.insert(pokemon.species);
        }
      }
    }

    for (const auto& damageCalculation : battleCreationInfo.damageCalculations) {
      moveSet.insert(damageCalculation.moves.begin(), damageCalculation.moves.end());
    }

    for (const auto& effectToAnalyze : battleCreationInfo.effectsToAnalyze) {
      moveSet.insert(effectToAnalyze.moves.begin(), effectToAnalyze.moves.end());
    }
  }

  loadSpecies(speciesSet);
  loadItems(itemSet);
  loadMoves(moveSet);

  for (dex::Species species : speciesMissingAbility) {
    abilitySet.insert(getSpeciesData<PrimaryAbility>(species).val);
  }
  loadAbilities(abilitySet);

  for (dex::Ability ability : abilitySet) {
    dex::enumToTag<AddAssociatedFormsToSet>(ability, associatedFormsForAbilities, this);
  }
  loadSpecies(associatedFormsForAbilities);
}
}  // namespace pokesim
