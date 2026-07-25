#include <Components/Names/AbilityNames.hpp>
#include <Config/Require.hpp>
#include <Pokedex/EnumToTag/AbilityEnumToTag.hpp>
#include <Pokedex/EnumToTag/AbilityPropertyEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim {
namespace {
template <typename Ability>
struct BuildAbility {
 private:
  enum class Optional : std::uint8_t {
    properties,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::properties, Type, void_t<Type::properties>> : std::true_type {};

 public:
  static types::entity run(types::registry& registry, GameMechanics gameMechanic) {
    types::handle ability{registry, registry.create()};

    ability.emplace<AbilityName>(Ability::name(gameMechanic));

    if constexpr (has<Optional::properties, Ability>::value) {
      dex::emplaceTagFromEnum(Ability::properties(gameMechanic), ability);
    }

    return ability.entity();
  }
};
}  // namespace

types::entity Pokedex::buildAbility(dex::Ability ability, types::registry& registry) const {
  if (isGameMechanic(GameMechanics::SCARLET_VIOLET)) {
    return dex::enumToTag<BuildAbility>(ability, registry, GameMechanics::SCARLET_VIOLET);
  }

  POKESIM_REQUIRE_FAIL("Building for a game that is not yet supported.");
  return types::entity{};
}
}  // namespace pokesim
