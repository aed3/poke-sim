#pragma once

#include <Components/Tags/NatureTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Nature.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::nature::tags {
/*
 * Runs a function with a certain nature tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the nature tags with
 * a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::Nature::ADAMANT, handle);
 * @endcode
 */
template <template <typename> typename RunStruct, typename... RunFunctionArgs>
auto enumToTag(dex::Nature nature, RunFunctionArgs&&... args) {
  switch (nature) {
    case dex::Nature::ADAMANT: return RunStruct<Adamant>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::BASHFUL: return RunStruct<Bashful>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::BOLD:    return RunStruct<Bold>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::BRAVE:   return RunStruct<Brave>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::CALM:    return RunStruct<Calm>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::CAREFUL: return RunStruct<Careful>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::DOCILE:  return RunStruct<Docile>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::GENTLE:  return RunStruct<Gentle>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::HARDY:   return RunStruct<Hardy>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::HASTY:   return RunStruct<Hasty>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::IMPISH:  return RunStruct<Impish>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::JOLLY:   return RunStruct<Jolly>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::LAX:     return RunStruct<Lax>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::LONELY:  return RunStruct<Lonely>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::MILD:    return RunStruct<Mild>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::MODEST:  return RunStruct<Modest>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::NAIVE:   return RunStruct<Naive>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::NAUGHTY: return RunStruct<Naughty>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::QUIET:   return RunStruct<Quiet>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::QUIRKY:  return RunStruct<Quirky>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::RASH:    return RunStruct<Rash>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::RELAXED: return RunStruct<Relaxed>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::SASSY:   return RunStruct<Sassy>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::SERIOUS: return RunStruct<Serious>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Nature::TIMID:   return RunStruct<Timid>::run(std::forward<RunFunctionArgs>(args)...);

    default: {
      POKESIM_REQUIRE(false, "Adding tag for nature that does not exist.");
    }
  }
}
}  // namespace pokesim::nature::tags