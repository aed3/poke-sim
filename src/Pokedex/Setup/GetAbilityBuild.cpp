#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
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
  static types::entity build(Pokedex* pokedex) {
    dex::internal::AbilityDexDataSetup ability(pokedex);

    ability.setName(T::name);

    return ability.entity();
  }
};

template <template <GameMechanics> class T>
auto buildAbilitySV(Pokedex* pokedex) {
  return BuildAbility<T<GameMechanics::SCARLET_VIOLET>>::build(pokedex);
}
};  // namespace internal

types::entity Pokedex::buildAbility(dex::Ability ability) {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (ability) {
        case Ability::STATIC: return buildAbilitySV<Static>(this);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building an ability that does not exist");
  return types::entity{};
}
};  // namespace pokesim