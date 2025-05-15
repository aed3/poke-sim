#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Registry.hpp>
#include <type_traits>

#include "../Abilities/headers.hpp"
#include "AbilityDexDataSetup.hpp"

// TODO(aed3): Make this and the individual ability files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildAbility {
 private:
 public:
  static types::entity build(types::registry& registry, bool /*forActiveMove*/) {
    dex::internal::AbilityDexDataSetup ability(registry);

    ability.setName(T::name);

    return ability.entity();
  }
};

template <template <GameMechanics> class T>
auto buildAbilitySV(types::registry& registry, bool forActiveMove) {
  return BuildAbility<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
};  // namespace internal

types::entity Pokedex::buildAbility(dex::Ability ability, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (ability) {
        case Ability::STATIC: return buildAbilitySV<Static>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  POKESIM_ASSERT_FAIL("Building an ability that does not exist.");
  return types::entity{};
}
};  // namespace pokesim