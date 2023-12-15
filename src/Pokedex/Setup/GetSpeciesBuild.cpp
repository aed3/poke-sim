#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Species.hpp>
#include <entt/entity/fwd.hpp>
#include <type_traits>

#include "../Species/headers.hpp"
#include "SpeciesDexDataSetup.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildSpecies {
 private:
  enum class Optional {
    primaryAbility,
    secondaryAbility,
    hiddenAbility,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename = T, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::primaryAbility, Type, void_t<Type::primaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::secondaryAbility, Type, void_t<Type::secondaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::hiddenAbility, Type, void_t<Type::hiddenAbility>> : std::true_type {};

 public:
  static entt::entity build(Pokedex* pokedex) {
    dex::internal::SpeciesDexDataSetup species(pokedex);

    species.setName(T::name);
    species.setBaseStats(T::hp, T::atk, T::def, T::spa, T::spd, T::spe);
    species.setType(T::type.type1, T::type.type2);

    if constexpr (has<Optional::primaryAbility>::value) {
      species.setPrimaryAbility(T::primaryAbility);
    }
    if constexpr (has<Optional::secondaryAbility>::value) {
      species.setSecondaryAbility(T::secondaryAbility);
    }
    if constexpr (has<Optional::hiddenAbility>::value) {
      species.setHiddenAbility(T::hiddenAbility);
    }

    return species.entity();
  }
};

template <template <GameMechanics> class T>
auto buildSpeciesSV(Pokedex* pokedex) {
  return internal::BuildSpecies<T<GameMechanics::SCARLET_VIOLET>>::build(pokedex);
}
};  // namespace internal

entt::entity Pokedex::buildSpecies(dex::Species species) {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (species) {
        case Species::AMPHAROS: return buildSpeciesSV<Ampharos>(this);
        case Species::GARDEVOIR: return buildSpeciesSV<Gardevoir>(this);
        case Species::EMPOLEON: return buildSpeciesSV<Empoleon>(this);
        case Species::PANGORO: return buildSpeciesSV<Pangoro>(this);
        case Species::RIBOMBEE: return buildSpeciesSV<Ribombee>(this);
        case Species::DRAGAPULT: return buildSpeciesSV<Dragapult>(this);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building a species that does not exist");
  return entt::entity{};
}
};  // namespace pokesim