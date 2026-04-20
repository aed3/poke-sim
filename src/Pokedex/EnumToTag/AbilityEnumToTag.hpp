#pragma once

#include <Components/Tags/AbilityTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Ability.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::ability::tags {
/*
 * Runs a function with a certain ability tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the ability tags with
 * a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::Ability::DEFIANT, handle);
 * @endcode
 */
template <template <typename> typename RunStruct, typename... RunFunctionArgs>
auto enumToTag(dex::Ability ability, RunFunctionArgs&&... args) {
  switch (ability) {
    case dex::Ability::DEFIANT:     return RunStruct<Defiant>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Ability::INFILTRATOR: return RunStruct<Infiltrator>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Ability::IRON_FIST:   return RunStruct<IronFist>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Ability::PLUS:        return RunStruct<Plus>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Ability::STATIC:      return RunStruct<Static>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Ability::SWEET_VEIL:  return RunStruct<SweetVeil>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Ability::TRACE:       return RunStruct<Trace>::run(std::forward<RunFunctionArgs>(args)...);

    default: {
      POKESIM_REQUIRE_FAIL("Using a tag for ability that does not exist.");
    }
  }
}
}  // namespace pokesim::ability::tags