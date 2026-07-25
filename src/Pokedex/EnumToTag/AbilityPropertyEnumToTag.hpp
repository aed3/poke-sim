#pragma once

#include <Components/Tags/AbilityPropertyTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/AbilityProperty.hpp>
#include <Types/Registry.hpp>
#include <utility>

namespace pokesim::dex {
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
void enumToTag(AbilityProperty ability, RunArgs&&... args) {
  if (ability & AbilityProperty::CANNOT_BREAK) {
    RunStruct<ability::tags::CannotBreak, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::CANNOT_SUPPRESS) {
    RunStruct<ability::tags::CannotSuppress, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::NO_ROLE_PLAY) {
    RunStruct<ability::tags::NoRolePlay, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::NO_SKILL_SWAP) {
    RunStruct<ability::tags::NoSkillSwap, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::NO_ENTRAINMENT) {
    RunStruct<ability::tags::NoEntrainment, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::NO_RECEIVER) {
    RunStruct<ability::tags::NoReceiver, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::NO_TRACE) {
    RunStruct<ability::tags::NoTrace, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (ability & AbilityProperty::NO_TRANSFORM) {
    RunStruct<ability::tags::NoTransform, T...>::run(std::forward<RunArgs>(args)...);
  }
}

// Assigns an ability's tag to a handle
void emplaceTagFromEnum(AbilityProperty property, types::handle handle);
void emplaceTagFromEnum(AbilityProperty property, types::registry& registry, types::entity entity);
}  // namespace pokesim::dex
