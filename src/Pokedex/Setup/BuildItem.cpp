#include <Components/Names/ItemNames.hpp>
#include <Components/Tags/ItemPropertyTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/EnumToTag/ItemEnumToTag.hpp>
#include <Pokedex/EnumToTag/ItemPropertyEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim {
namespace {
template <typename Item>
struct BuildItem {
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
    types::handle item{registry, registry.create()};

    item.emplace<ItemName>(Item::name(gameMechanic));

    if constexpr (has<Optional::properties, Item>::value) {
      dex::emplaceTagFromEnum(Item::properties(gameMechanic), item);
    }

    return item.entity();
  }
};
}  // namespace

types::entity Pokedex::buildItem(dex::Item item, types::registry& registry) const {
  if (isGameMechanic(GameMechanics::SCARLET_VIOLET)) {
    return dex::enumToTag<BuildItem>(item, registry, GameMechanics::SCARLET_VIOLET);
  }

  POKESIM_REQUIRE_FAIL("Building for a game that is not yet supported.");
  return types::entity{};
}
}  // namespace pokesim
