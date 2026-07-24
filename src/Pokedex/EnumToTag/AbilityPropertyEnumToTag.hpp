#pragma once

#include <Components/Tags/AbilityPropertyTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/AbilityProperty.hpp>
#include <utility>

namespace pokesim::ability::tags {
/*
 * Runs a function with a certain ability property tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the ability property
 * tags with a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::AbilityProperty::CANNOT_BREAK, handle);
 * @endcode
 */
template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
void enumToTag(dex::AbilityProperty ability, RunArgs&&... args) {
  if (ability & dex::AbilityProperty::CANNOT_BREAK) {
    RunStruct<CannotBreak, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::CANNOT_SUPPRESS) {
    RunStruct<CannotSuppress, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::NO_ROLE_PLAY) {
    RunStruct<NoRolePlay, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::NO_SKILL_SWAP) {
    RunStruct<NoSkillSwap, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::NO_ENTRAINMENT) {
    RunStruct<NoEntrainment, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::NO_RECEIVER) {
    RunStruct<NoReceiver, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::NO_TRACE) {
    RunStruct<NoTrace, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & dex::AbilityProperty::NO_TRANSFORM) {
    RunStruct<NoTransform, T...>::run(std::forward<RunArgs>(args)...);
  }
}
}  // namespace pokesim::ability::tags
