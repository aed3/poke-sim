#ifdef USE_SINGLE_HEADER
#include "../include/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>
#include <entt/entt.hpp>

namespace pokesim {
TEST_CASE("Species", "") {
  Dex pokedex;

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
}  // namespace pokesim