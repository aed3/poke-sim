#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Registry.hpp>
#include <type_traits>

#include "../Items/headers.hpp"
#include "ItemDexDataSetup.hpp"

// TODO(aed3): Make this and the individual item files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildItem {
 private:
 public:
  static types::entity build(types::registry& registry, bool /*forActiveMove*/) {
    dex::internal::ItemDexDataSetup item(registry);

    item.setName(T::name);

    return item.entity();
  }
};

template <template <GameMechanics> class T>
auto buildItemSV(types::registry& registry, bool forActiveMove) {
  return BuildItem<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
};  // namespace internal

types::entity Pokedex::buildItem(dex::Item item, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (item) {
        case Item::ASSAULT_VEST: return buildItemSV<AssaultVest>(registry, forActiveMove);
        case Item::BRIGHT_POWDER: return buildItemSV<BrightPowder>(registry, forActiveMove);
        case Item::CHOICE_SCARF: return buildItemSV<ChoiceScarf>(registry, forActiveMove);
        case Item::CHOICE_SPECS: return buildItemSV<ChoiceSpecs>(registry, forActiveMove);
        case Item::FOCUS_SASH: return buildItemSV<FocusSash>(registry, forActiveMove);
        case Item::LIFE_ORB: return buildItemSV<LifeOrb>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  POKESIM_ASSERT_FAIL("Building an item that does not exist.");
  return types::entity{};
}
};  // namespace pokesim