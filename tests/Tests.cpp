#ifdef USE_SINGLE_HEADER
#include "../include/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>

namespace pokesim {
TEST_CASE("Example", "") {
  REQUIRE(Dex::toString(Dex::EEVEE) == "Eevee");
  REQUIRE(Dex::fromNameToSpeciesEnum("Eevee") == Dex::EEVEE);
}
};  // namespace pokesim

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;

  return session.run();
}
