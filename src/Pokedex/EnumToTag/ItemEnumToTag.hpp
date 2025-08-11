#pragma once

#include <Components/Tags/ItemTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Item.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::item::tags {
/*
 * Runs a function with a certain item tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the item tags with
 * a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::Item::ASSAULT_VEST, handle);
 * @endcode
 */
template <template <typename> typename RunStruct, typename... RunFunctionArgs>
auto enumToTag(dex::Item item, RunFunctionArgs&&... args) {
  switch (item) {
    case dex::Item::ASSAULT_VEST:  return RunStruct<AssaultVest>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Item::BRIGHT_POWDER: return RunStruct<BrightPowder>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Item::CHOICE_SCARF:  return RunStruct<ChoiceScarf>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Item::CHOICE_SPECS:  return RunStruct<ChoiceSpecs>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Item::FOCUS_SASH:    return RunStruct<FocusSash>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Item::LIFE_ORB:      return RunStruct<LifeOrb>::run(std::forward<RunFunctionArgs>(args)...);

    default: {
      POKESIM_REQUIRE(false, "Adding tag for item that does not exist.");
    }
  }
}
}  // namespace pokesim::item::tags