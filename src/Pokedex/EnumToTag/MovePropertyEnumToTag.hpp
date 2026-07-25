#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Registry.hpp>
#include <utility>

namespace pokesim::dex {
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
void enumToTag(MoveProperty item, RunArgs&&... args) {
  if (item & MoveProperty::CONTACT) {
    RunStruct<move::tags::Contact, T...>::run(std::forward<RunArgs>(args)...);
  }

  if (item & MoveProperty::VARIABLE_HIT_COUNT) {
    RunStruct<move::tags::VariableHitCount, T...>::run(std::forward<RunArgs>(args)...);
  }
}

// Assigns a move's tag to a handle
void emplaceTagFromEnum(MoveProperty property, types::handle handle);
void emplaceTagFromEnum(MoveProperty property, types::registry& registry, types::entity entity);
}  // namespace pokesim::dex
