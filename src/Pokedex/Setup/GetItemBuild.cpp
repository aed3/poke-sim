#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Item.hpp>
#include <entt/entity/fwd.hpp>

#include "../Items/headers.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Pokedex::getItemBuild(dex::Item item))(Pokedex&) {
  // Tidy check ignored because "using namespace" is in function
  using namespace dex::build;  // NOLINT(google-build-using-namespace)

  switch (item) {
    case BrightPowder::name: return BrightPowder::build;
    case LifeOrb::name: return LifeOrb::build;
    case FocusSash::name: return FocusSash::build;
    case ChoiceScarf::name: return ChoiceScarf::build;
    case ChoiceSpecs::name: return ChoiceSpecs::build;
    case AssaultVest::name: return AssaultVest::build;
    default: {
      ENTT_ASSERT(false, "Building a item that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim