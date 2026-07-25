#pragma once

#include <Components/Tags/ItemPropertyTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ItemProperty.hpp>
#include <Types/Registry.hpp>
#include <utility>

namespace pokesim::dex {
/*
 * Runs a function with a certain item property tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the item property
 * tags with a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::ItemProperty::BERRY, handle);
 * @endcode
 */
template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
void enumToTag(ItemProperty item, RunArgs&&... args) {
  if (item & ItemProperty::BERRY) {
    RunStruct<item::tags::Berry, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & ItemProperty::CHOICE) {
    RunStruct<item::tags::Choice, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & ItemProperty::GEM) {
    RunStruct<item::tags::Gem, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & ItemProperty::IGNORES_KLUTZ) {
    RunStruct<item::tags::IgnoresKlutz, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & ItemProperty::POKEBALL) {
    RunStruct<item::tags::Pokeball, T...>::run(std::forward<RunArgs>(args)...);
  }
}

// Assigns an item's tag to a handle
void emplaceTagFromEnum(ItemProperty property, types::handle handle);
void emplaceTagFromEnum(ItemProperty property, types::registry& registry, types::entity entity);
}  // namespace pokesim::dex
