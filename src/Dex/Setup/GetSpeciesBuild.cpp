#include <Dex/Dex.hpp>

#include "../Species/headers.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Dex::getSpeciesBuild(dex::Species species))(Dex&) {
  switch (species) {
    case dex::EMPOLEON: return dex::build::Empoleon::build;
    default: {
      ENTT_ASSERT(false, "Building a species that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim