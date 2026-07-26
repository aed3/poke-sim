#pragma once

#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Registry.hpp>
#include <utility>

#include "../Types/headers.hpp"

namespace pokesim::dex {
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
auto enumToTag(Type type, RunArgs&&... args) {
  switch (type) {
    case Type::NORMAL:   return RunStruct<dex::Normal, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FIGHTING: return RunStruct<dex::Fighting, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FLYING:   return RunStruct<dex::Flying, T...>::run(std::forward<RunArgs>(args)...);
    case Type::POISON:   return RunStruct<dex::Poison, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GROUND:   return RunStruct<dex::Ground, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ROCK:     return RunStruct<dex::Rock, T...>::run(std::forward<RunArgs>(args)...);
    case Type::BUG:      return RunStruct<dex::Bug, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GHOST:    return RunStruct<dex::Ghost, T...>::run(std::forward<RunArgs>(args)...);
    case Type::STEEL:    return RunStruct<dex::Steel, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FIRE:     return RunStruct<dex::Fire, T...>::run(std::forward<RunArgs>(args)...);
    case Type::WATER:    return RunStruct<dex::Water, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GRASS:    return RunStruct<dex::Grass, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ELECTRIC: return RunStruct<dex::Electric, T...>::run(std::forward<RunArgs>(args)...);
    case Type::PSYCHIC:  return RunStruct<dex::Psychic, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ICE:      return RunStruct<dex::Ice, T...>::run(std::forward<RunArgs>(args)...);
    case Type::DRAGON:   return RunStruct<dex::Dragon, T...>::run(std::forward<RunArgs>(args)...);
    case Type::DARK:     return RunStruct<dex::Dark, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FAIRY:    return RunStruct<dex::Fairy, T...>::run(std::forward<RunArgs>(args)...);
    // Type::NO_TYPE will pass through here often, but should do nothing
    case Type::NO_TYPE: break;

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

// Assigns a type's tag to a handle
void emplaceTagFromEnum(Type type, types::handle handle);
void emplaceTagFromEnum(Type type, types::registry& registry, types::entity entity);

// Checks if the handle has the type's tag
bool hasTag(Type type, types::handle handle);
bool hasTag(Type type, const types::registry& registry, types::entity entity);
}  // namespace pokesim::dex
