#include <Dex/Dex.hpp>

#include "../Species/headers.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Dex::getSpeciesBuild(dex::Species species))(Dex&) {
  switch (species) {
    case dex::AMPHAROS: return dex::build::Ampharos::build;
    case dex::GARDEVOIR: return dex::build::Gardevoir::build;
    case dex::EMPOLEON: return dex::build::Empoleon::build;
    case dex::PANGORO: return dex::build::Pangoro::build;
    case dex::RIBOMBEE: return dex::build::Ribombee::build;
    case dex::DRAGAPULT: return dex::build::Dragapult::build;
    default: {
      ENTT_ASSERT(false, "Building a species that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim