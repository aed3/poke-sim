#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Example", "") {
  REQUIRE(dex::toString(dex::EEVEE) == "Eevee");
  REQUIRE(dex::fromNameToSpeciesEnum("Eevee") == dex::EEVEE);
}
};  // namespace pokesim

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;

  return session.run();
}
