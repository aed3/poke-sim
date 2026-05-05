#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Registry.hpp>

#include "../Items/headers.hpp"
#include "ItemDexDataSetup.hpp"

// TODO(aed3): Make this and the individual item files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildItem {
 private:
 public:
  static types::entity build(types::registry& registry, GameMechanics gameMechanic) {
    dex::internal::ItemDexDataSetup item(registry);

    item.setName(T::name(gameMechanic));

    item.setProperties(T::itemTags);

    return item.entity();
  }
};
types::entity buildByGameMechanic(dex::Item item, types::registry& registry, GameMechanics gameMechanic) {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;  // NOLINT(google-build-using-namespace)
  switch (item) {
    case Item::ASSAULT_VEST:  return BuildItem<AssaultVest>::build(registry, gameMechanic);
    case Item::BRIGHT_POWDER: return BuildItem<BrightPowder>::build(registry, gameMechanic);
    case Item::CHOICE_SCARF:  return BuildItem<ChoiceScarf>::build(registry, gameMechanic);
    case Item::CHOICE_SPECS:  return BuildItem<ChoiceSpecs>::build(registry, gameMechanic);
    case Item::FOCUS_SASH:    return BuildItem<FocusSash>::build(registry, gameMechanic);
    case Item::LIFE_ORB:      return BuildItem<LifeOrb>::build(registry, gameMechanic);

    default: break;
  }
  POKESIM_REQUIRE_FAIL("Building an item that is not yet supported.");
}
}  // namespace

types::entity Pokedex::buildItem(dex::Item item, types::registry& registry) const {
  if (isGameMechanic(GameMechanics::SCARLET_VIOLET)) {
    return buildByGameMechanic(item, registry, GameMechanics::SCARLET_VIOLET);
  }

  POKESIM_REQUIRE_FAIL("Building for a game that is not yet supported.");
  return types::entity{};
}
}  // namespace pokesim
