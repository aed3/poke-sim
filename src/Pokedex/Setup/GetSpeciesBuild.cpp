#include <Pokedex/Pokedex.hpp>

#include "../Species/headers.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Pokedex::getSpeciesBuild(dex::Species species))(Pokedex&) {
  // Tidy check ignored because "using namespace" is in function
  using namespace dex::build;  // NOLINT(google-build-using-namespace)

  switch (species) {
    case Ampharos::name: return Ampharos::build;
    case Gardevoir::name: return Gardevoir::build;
    case Empoleon::name: return Empoleon::build;
    case Pangoro::name: return Pangoro::build;
    case Ribombee::name: return Ribombee::build;
    case Dragapult::name: return Dragapult::build;
    default: {
      ENTT_ASSERT(false, "Building a species that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim