#pragma once

#include <Components/Tags/NatureTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Registry.hpp>
#include <utility>

namespace pokesim::dex {
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
auto enumToTag(Nature nature, RunArgs&&... args) {
  switch (nature) {
    case Nature::ADAMANT: return RunStruct<nature::tags::Adamant, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::BASHFUL: return RunStruct<nature::tags::Bashful, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::BOLD:    return RunStruct<nature::tags::Bold, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::BRAVE:   return RunStruct<nature::tags::Brave, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::CALM:    return RunStruct<nature::tags::Calm, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::CAREFUL: return RunStruct<nature::tags::Careful, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::DOCILE:  return RunStruct<nature::tags::Docile, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::GENTLE:  return RunStruct<nature::tags::Gentle, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::HARDY:   return RunStruct<nature::tags::Hardy, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::HASTY:   return RunStruct<nature::tags::Hasty, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::IMPISH:  return RunStruct<nature::tags::Impish, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::JOLLY:   return RunStruct<nature::tags::Jolly, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::LAX:     return RunStruct<nature::tags::Lax, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::LONELY:  return RunStruct<nature::tags::Lonely, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::MILD:    return RunStruct<nature::tags::Mild, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::MODEST:  return RunStruct<nature::tags::Modest, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::NAIVE:   return RunStruct<nature::tags::Naive, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::NAUGHTY: return RunStruct<nature::tags::Naughty, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::QUIET:   return RunStruct<nature::tags::Quiet, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::QUIRKY:  return RunStruct<nature::tags::Quirky, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::RASH:    return RunStruct<nature::tags::Rash, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::RELAXED: return RunStruct<nature::tags::Relaxed, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::SASSY:   return RunStruct<nature::tags::Sassy, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::SERIOUS: return RunStruct<nature::tags::Serious, T...>::run(std::forward<RunArgs>(args)...);
    case Nature::TIMID:   return RunStruct<nature::tags::Timid, T...>::run(std::forward<RunArgs>(args)...);

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

// Assigns a move's tag to a handle
void emplaceTagFromEnum(Nature nature, types::handle handle);
void emplaceTagFromEnum(Nature nature, types::registry& registry, types::entity entity);

// Checks if the handle has the move's tag
bool hasTag(Nature nature, types::handle handle);
bool hasTag(Nature nature, const types::registry& registry, types::entity entity);
}  // namespace pokesim::dex
