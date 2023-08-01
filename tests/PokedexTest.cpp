#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Pokedex Data Building: Species", "[Pokedex]") {
  Pokedex pokedex(SCARLET_VIOLET_GAME_MECHANICS);

  entt::dense_set<dex::Species> speciesList{};
  speciesList.insert(dex::EMPOLEON);
  pokedex.loadSpecies(speciesList);

  auto [species, types, baseStats] = pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats>(dex::EMPOLEON);

  REQUIRE(species.name == dex::EMPOLEON);
  REQUIRE(types.t1 == dex::WATER_TYPE);
  REQUIRE(types.t2 == dex::STEEL_TYPE);
  REQUIRE(baseStats.hp == 84);
  REQUIRE(baseStats.atk == 86);
  REQUIRE(baseStats.spd == 101);
}

TEST_CASE("Pokedex Data Building: Item", "[Pokedex]") {
  Pokedex pokedex(SCARLET_VIOLET_GAME_MECHANICS);

  entt::dense_set<dex::Item> itemList{};
  itemList.insert(dex::CHOICE_SPECS);
  pokedex.loadItems(itemList);

  auto item = pokedex.getItemData<ItemName>(dex::CHOICE_SPECS);

  REQUIRE(item.name == dex::CHOICE_SPECS);
}

TEST_CASE("Pokedex Data Building: Move", "[Pokedex]") {
  Pokedex pokedex(SCARLET_VIOLET_GAME_MECHANICS);

  entt::dense_set<dex::Move> moveList{};
  moveList.insert(dex::MOONBLAST);
  pokedex.loadMoves(moveList);

  auto [move, type, power, secondaryEffect] =
    pokedex.getMoveData<MoveName, TypeName, BasePower, MoveEffect>(dex::MOONBLAST);

  REQUIRE(move.name == dex::MOONBLAST);
  REQUIRE(type.name == dex::FAIRY_TYPE);
  REQUIRE(power.basePower == 95);
  REQUIRE_FALSE(secondaryEffect.primary);
}
}  // namespace pokesim