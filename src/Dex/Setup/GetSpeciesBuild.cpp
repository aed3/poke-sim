#include <Dex/Dex.hpp>

#include "../Species/headers.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim::dex::build {
entt::entity (*Dex::getSpeciesBuild(dex::Species species))(Dex&) {
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