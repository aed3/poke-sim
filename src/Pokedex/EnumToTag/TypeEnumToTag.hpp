#pragma once

#include <Components/Tags/TypeTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Type.hpp>

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
template <template <typename> typename RunStruct, typename... RunFunctionArgs>
void enumToTag(dex::Type type, RunFunctionArgs&&... args) {
  switch (type) {
    case dex::Type::NORMAL:   return RunStruct<Normal>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::FIGHTING: return RunStruct<Fighting>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::FLYING:   return RunStruct<Flying>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::POISON:   return RunStruct<Poison>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::GROUND:   return RunStruct<Ground>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::ROCK:     return RunStruct<Rock>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::BUG:      return RunStruct<Bug>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::GHOST:    return RunStruct<Ghost>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::STEEL:    return RunStruct<Steel>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::FIRE:     return RunStruct<Fire>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::WATER:    return RunStruct<Water>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::GRASS:    return RunStruct<Grass>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::ELECTRIC: return RunStruct<Electric>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::PSYCHIC:  return RunStruct<Psychic>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::ICE:      return RunStruct<Ice>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::DRAGON:   return RunStruct<Dragon>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::DARK:     return RunStruct<Dark>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Type::FAIRY:    return RunStruct<Fairy>::run(std::forward<RunFunctionArgs>(args)...);

    // dex::Type::NO_TYPE will pass through here often, but should do nothing
    case dex::Type::NO_TYPE: return;

    default: {
      POKESIM_REQUIRE_FAIL("Using a tag for type that does not exist.");
    }
  }
}
}  // namespace pokesim::type::tags