#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Pokedex/Abilities.hpp>
#include <Components/Pokedex/BaseStats.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Config/Require.hpp>
#include <Pokedex/EnumToTag/SpeciesEnumToTag.hpp>
#include <Pokedex/EnumToTag/TypeEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>
#include <cstdint>
#include <entt/entity/handle.hpp>
#include <type_traits>

namespace pokesim {
namespace {
template <typename Species>
struct BuildSpecies {
 private:
  enum class Optional : std::uint8_t {
    primaryAbility,
    secondaryAbility,
    hiddenAbility,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename = Species, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::primaryAbility, Type, void_t<Type::primaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::secondaryAbility, Type, void_t<Type::secondaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::hiddenAbility, Type, void_t<Type::hiddenAbility>> : std::true_type {};

 public:
  static types::entity run(types::registry& registry, GameMechanics gameMechanic) {
    types::handle species{registry, registry.create()};

    species.emplace<SpeciesName>(Species::name(gameMechanic));
    SpeciesTypes types = Species::type(gameMechanic);
    species.emplace<BaseStats>(
      Species::hp(gameMechanic),
      Species::atk(gameMechanic),
      Species::def(gameMechanic),
      Species::spa(gameMechanic),
      Species::spd(gameMechanic),
      Species::spe(gameMechanic));
    species.emplace<SpeciesTypes>(types);
    dex::emplaceTagFromEnum(types.type1(), species);
    dex::emplaceTagFromEnum(types.type2(), species);

    if constexpr (has<Optional::primaryAbility>::value) {
      species.emplace<PrimaryAbility>(Species::primaryAbility(gameMechanic));
    }
    if constexpr (has<Optional::secondaryAbility>::value) {
      species.emplace<SecondaryAbility>(Species::secondaryAbility(gameMechanic));
    }
    if constexpr (has<Optional::hiddenAbility>::value) {
      species.emplace<HiddenAbility>(Species::hiddenAbility(gameMechanic));
    }

    return species.entity();
  }
};
}  // namespace

types::entity Pokedex::buildSpecies(dex::Species species, types::registry& registry) const {
  if (isGameMechanic(GameMechanics::SCARLET_VIOLET)) {
    return dex::enumToTag<BuildSpecies>(species, registry, GameMechanics::SCARLET_VIOLET);
  }

  POKESIM_REQUIRE_FAIL("Building for a game that is not yet supported.");
  return types::entity{};
}
}  // namespace pokesim
