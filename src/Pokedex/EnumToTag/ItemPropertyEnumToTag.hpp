#pragma once

#include <Components/Tags/ItemPropertyTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/ItemProperty.hpp>
#include <utility>

namespace pokesim::item::tags {
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
void enumToTag(dex::ItemProperty item, RunArgs&&... args) {
  if (item & dex::ItemProperty::BERRY) {
    RunStruct<Berry, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & dex::ItemProperty::CHOICE) {
    RunStruct<Choice, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & dex::ItemProperty::GEM) {
    RunStruct<Gem, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & dex::ItemProperty::IGNORES_KLUTZ) {
    RunStruct<IgnoresKlutz, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & dex::ItemProperty::POKEBALL) {
    RunStruct<Pokeball, T...>::run(std::forward<RunArgs>(args)...);
  }
}
}  // namespace pokesim::item::tags
