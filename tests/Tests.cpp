#ifdef USE_SINGLE_HEADER
#include "../include/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>

namespace pokesim {
TEST_CASE("Example", "") {
  Pokemon eevee{};
  eevee.species = Dex::Names::EEVEE;
  REQUIRE(eevee.isSpecies(Dex::Names::EEVEE));
  REQUIRE_FALSE(eevee.isSpecies(Dex::Names::VAPOREON, Dex::Names::JOLTEON, Dex::Names::FLAREON));

  REQUIRE(Dex::Names::toString(eevee.species) == "Eevee");
  REQUIRE(Dex::Names::fromNameToSpeciesEnum("Eevee") == eevee.species);
}
};  // namespace pokesim

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;

  return session.run();
}
