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
    case Type::NORMAL:   return RunStruct<NormalType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FIGHTING: return RunStruct<FightingType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FLYING:   return RunStruct<FlyingType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::POISON:   return RunStruct<PoisonType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GROUND:   return RunStruct<GroundType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ROCK:     return RunStruct<RockType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::BUG:      return RunStruct<BugType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GHOST:    return RunStruct<GhostType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::STEEL:    return RunStruct<SteelType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FIRE:     return RunStruct<FireType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::WATER:    return RunStruct<WaterType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GRASS:    return RunStruct<GrassType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ELECTRIC: return RunStruct<ElectricType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::PSYCHIC:  return RunStruct<PsychicType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ICE:      return RunStruct<IceType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::DRAGON:   return RunStruct<DragonType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::DARK:     return RunStruct<DarkType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FAIRY:    return RunStruct<FairyType, T...>::run(std::forward<RunArgs>(args)...);
    // Type::NO_TYPE will pass through here often, but should do nothing
    case Type::NO_TYPE: break;

    default: POKESIM_REQUIRE_FAIL("Using a tag for type that does not exist.");
  }

  using ReturnType = std::invoke_result_t<decltype(&RunStruct<NormalType, T...>::run), RunArgs...>;
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
