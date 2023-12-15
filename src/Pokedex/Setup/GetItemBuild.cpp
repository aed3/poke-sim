#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Item.hpp>
#include <entt/entity/fwd.hpp>
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
  static entt::entity build(Pokedex* pokedex) {
    dex::internal::ItemDexDataSetup item(pokedex);

    item.setName(T::name);

    return item.entity();
  }
};

template <template <GameMechanics> class T>
auto buildItemSV(Pokedex* pokedex) {
  return internal::BuildItem<T<GameMechanics::SCARLET_VIOLET>>::build(pokedex);
}
};  // namespace internal

entt::entity Pokedex::buildItem(dex::Item item) {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (item) {
        case Item::ASSAULT_VEST: return buildItemSV<AssaultVest>(this);
        case Item::BRIGHT_POWDER: return buildItemSV<BrightPowder>(this);
        case Item::CHOICE_SCARF: return buildItemSV<ChoiceScarf>(this);
        case Item::CHOICE_SPECS: return buildItemSV<ChoiceSpecs>(this);
        case Item::FOCUS_SASH: return buildItemSV<FocusSash>(this);
        case Item::LIFE_ORB: return buildItemSV<LifeOrb>(this);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building an item that does not exist");
  return entt::entity{};
}
};  // namespace pokesim