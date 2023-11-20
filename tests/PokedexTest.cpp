#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Pokedex Data Building: Species", "[Pokedex]") {
  Pokedex pokedex(GameMechanics::SCARLET_VIOLET_GAME_MECHANICS);

  entt::dense_set<dex::Species> speciesList{};
  speciesList.insert(dex::Species::EMPOLEON);
  pokedex.loadSpecies(speciesList);

  auto [species, types, baseStats] =
    pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats>(dex::Species::EMPOLEON);

  REQUIRE(species.name == dex::Species::EMPOLEON);
  REQUIRE(types.t1 == dex::Type::WATER_TYPE);
  REQUIRE(types.t2 == dex::Type::STEEL_TYPE);
  REQUIRE(baseStats.hp == 84);
  REQUIRE(baseStats.atk == 86);
  REQUIRE(baseStats.spd == 101);
}

TEST_CASE("Pokedex Data Building: Item", "[Pokedex]") {
  Pokedex pokedex(GameMechanics::SCARLET_VIOLET_GAME_MECHANICS);

  entt::dense_set<dex::Item> itemList{};
  itemList.insert(dex::Item::CHOICE_SPECS);
  pokedex.loadItems(itemList);

  auto item = pokedex.getItemData<ItemName>(dex::Item::CHOICE_SPECS);

  REQUIRE(item.name == dex::Item::CHOICE_SPECS);
}

TEST_CASE("Pokedex Data Building: Move", "[Pokedex]") {
  Pokedex pokedex(GameMechanics::SCARLET_VIOLET_GAME_MECHANICS);

  entt::dense_set<dex::Move> moveList{};
  moveList.insert(dex::Move::MOONBLAST);
  pokedex.loadMoves(moveList);

  auto [move, type, power, secondaryEffect] =
    pokedex.getMoveData<MoveName, TypeName, BasePower, MoveEffect>(dex::Move::MOONBLAST);

  REQUIRE(move.name == dex::Move::MOONBLAST);
  REQUIRE(type.name == dex::Type::FAIRY_TYPE);
  REQUIRE(power.basePower == 95);
  REQUIRE_FALSE(secondaryEffect.primary);
}
}  // namespace pokesim