#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Registry.hpp>

#include "../Abilities/headers.hpp"
#include "AbilityDexDataSetup.hpp"

// TODO(aed3): Make this and the individual ability files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildAbility {
 private:
 public:
  static types::entity build(types::registry& registry, GameMechanics gameMechanic) {
    dex::internal::AbilityDexDataSetup ability(registry);

    ability.setName(T::name(gameMechanic));

    return ability.entity();
  }
};

types::entity buildByGameMechanic(dex::Ability ability, types::registry& registry, GameMechanics gameMechanic) {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;  // NOLINT(google-build-using-namespace)
  switch (ability) {
    case Ability::COMPETITIVE: return BuildAbility<Competitive>::build(registry, gameMechanic);
    case Ability::DEFIANT:     return BuildAbility<Defiant>::build(registry, gameMechanic);
    case Ability::INFILTRATOR: return BuildAbility<Infiltrator>::build(registry, gameMechanic);
    case Ability::IRON_FIST:   return BuildAbility<IronFist>::build(registry, gameMechanic);
    case Ability::PLUS:        return BuildAbility<Plus>::build(registry, gameMechanic);
    case Ability::SWEET_VEIL:  return BuildAbility<SweetVeil>::build(registry, gameMechanic);
    case Ability::STATIC:      return BuildAbility<Static>::build(registry, gameMechanic);
    case Ability::TRACE:       return BuildAbility<Trace>::build(registry, gameMechanic);

    default: break;
  }
  POKESIM_REQUIRE_FAIL("Building an ability that is not yet supported.");
}
}  // namespace

types::entity Pokedex::buildAbility(dex::Ability ability, types::registry& registry) const {
  if (isGameMechanic(GameMechanics::SCARLET_VIOLET)) {
    return buildByGameMechanic(ability, registry, GameMechanics::SCARLET_VIOLET);
  }

  POKESIM_REQUIRE_FAIL("Building for a game that is not yet supported.");
  return types::entity{};
}
}  // namespace pokesim
