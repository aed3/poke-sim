#pragma once

#include <Components/Tags/NatureTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Nature.hpp>
#include <utility>

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
template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
auto enumToTag(dex::Nature nature, RunArgs&&... args) {
  switch (nature) {
    case dex::Nature::ADAMANT: return RunStruct<Adamant, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::BASHFUL: return RunStruct<Bashful, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::BOLD:    return RunStruct<Bold, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::BRAVE:   return RunStruct<Brave, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::CALM:    return RunStruct<Calm, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::CAREFUL: return RunStruct<Careful, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::DOCILE:  return RunStruct<Docile, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::GENTLE:  return RunStruct<Gentle, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::HARDY:   return RunStruct<Hardy, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::HASTY:   return RunStruct<Hasty, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::IMPISH:  return RunStruct<Impish, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::JOLLY:   return RunStruct<Jolly, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::LAX:     return RunStruct<Lax, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::LONELY:  return RunStruct<Lonely, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::MILD:    return RunStruct<Mild, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::MODEST:  return RunStruct<Modest, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::NAIVE:   return RunStruct<Naive, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::NAUGHTY: return RunStruct<Naughty, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::QUIET:   return RunStruct<Quiet, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::QUIRKY:  return RunStruct<Quirky, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::RASH:    return RunStruct<Rash, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::RELAXED: return RunStruct<Relaxed, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::SASSY:   return RunStruct<Sassy, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::SERIOUS: return RunStruct<Serious, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Nature::TIMID:   return RunStruct<Timid, T...>::run(std::forward<RunArgs>(args)...);

    default: {
      POKESIM_REQUIRE_FAIL("Using a tag for nature that does not exist.");
      using ReturnType = std::invoke_result_t<decltype(&RunStruct<void, T...>::run), RunArgs...>;
      if constexpr (std::is_void_v<ReturnType>) {
        return;
      }
      else {
        return ReturnType{};
      }
    }
  }
}
}  // namespace pokesim::nature::tags
