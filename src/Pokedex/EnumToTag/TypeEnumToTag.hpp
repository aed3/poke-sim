#pragma once

#include <Components/Tags/TypeTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Type.hpp>
#include <utility>

namespace pokesim::type::tags {
/*
 * Runs a function with a certain type tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the type tags with
 * a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::Type::WATER, handle);
 * @endcode
 */
template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
auto enumToTag(dex::Type type, RunArgs&&... args) {
  switch (type) {
    case dex::Type::NORMAL:   return RunStruct<Normal, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::FIGHTING: return RunStruct<Fighting, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::FLYING:   return RunStruct<Flying, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::POISON:   return RunStruct<Poison, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::GROUND:   return RunStruct<Ground, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::ROCK:     return RunStruct<Rock, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::BUG:      return RunStruct<Bug, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::GHOST:    return RunStruct<Ghost, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::STEEL:    return RunStruct<Steel, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::FIRE:     return RunStruct<Fire, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::WATER:    return RunStruct<Water, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::GRASS:    return RunStruct<Grass, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::ELECTRIC: return RunStruct<Electric, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::PSYCHIC:  return RunStruct<Psychic, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::ICE:      return RunStruct<Ice, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::DRAGON:   return RunStruct<Dragon, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::DARK:     return RunStruct<Dark, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Type::FAIRY:    return RunStruct<Fairy, T...>::run(std::forward<RunArgs>(args)...);
    // dex::Type::NO_TYPE will pass through here often, but should do nothing
    case dex::Type::NO_TYPE: break;

    default: POKESIM_REQUIRE_FAIL("Using a tag for type that does not exist.");
  }

  using ReturnType = std::invoke_result_t<decltype(&RunStruct<void, T...>::run), RunArgs...>;
  if constexpr (std::is_void_v<ReturnType>) {
    return;
  }
  else {
    return ReturnType{};
  }
}
}  // namespace pokesim::type::tags
