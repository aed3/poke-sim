#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <utility>

namespace pokesim::move::tags {
/*
 * Runs a function with a certain move property tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the move property
 * tags with a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::MoveProperty::CONTACT, handle);
 * @endcode
 */
template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
void enumToTag(dex::MoveProperty item, RunArgs&&... args) {
  if (item & dex::MoveProperty::CONTACT) {
    RunStruct<Contact, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & dex::MoveProperty::VARIABLE_HIT_COUNT) {
    RunStruct<VariableHitCount, T...>::run(std::forward<RunArgs>(args)...);
  }
}
}  // namespace pokesim::move::tags
