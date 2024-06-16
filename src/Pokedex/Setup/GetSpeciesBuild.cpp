#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Species.hpp>
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
  static types::entity build(types::registry& registry, bool /*forActiveMove*/) {
    dex::internal::SpeciesDexDataSetup species(registry);

    species.setName(T::name);
    species.setBaseStats(T::hp, T::atk, T::def, T::spa, T::spd, T::spe);
    species.setType(T::type.type1(), T::type.type2());

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
auto buildSpeciesSV(types::registry& registry, bool forActiveMove) {
  return BuildSpecies<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
};  // namespace internal

types::entity Pokedex::buildSpecies(dex::Species species, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (species) {
        case Species::AMPHAROS: return buildSpeciesSV<Ampharos>(registry, forActiveMove);
        case Species::GARDEVOIR: return buildSpeciesSV<Gardevoir>(registry, forActiveMove);
        case Species::EMPOLEON: return buildSpeciesSV<Empoleon>(registry, forActiveMove);
        case Species::PANGORO: return buildSpeciesSV<Pangoro>(registry, forActiveMove);
        case Species::RIBOMBEE: return buildSpeciesSV<Ribombee>(registry, forActiveMove);
        case Species::DRAGAPULT: return buildSpeciesSV<Dragapult>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building a species that does not exist");
  return types::entity{};
}
};  // namespace pokesim