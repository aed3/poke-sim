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
    case Type::NORMAL:   return RunStruct<dex::NormalType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FIGHTING: return RunStruct<dex::FightingType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FLYING:   return RunStruct<dex::FlyingType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::POISON:   return RunStruct<dex::PoisonType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GROUND:   return RunStruct<dex::GroundType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ROCK:     return RunStruct<dex::RockType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::BUG:      return RunStruct<dex::BugType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GHOST:    return RunStruct<dex::GhostType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::STEEL:    return RunStruct<dex::SteelType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FIRE:     return RunStruct<dex::FireType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::WATER:    return RunStruct<dex::WaterType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::GRASS:    return RunStruct<dex::GrassType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ELECTRIC: return RunStruct<dex::ElectricType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::PSYCHIC:  return RunStruct<dex::PsychicType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::ICE:      return RunStruct<dex::IceType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::DRAGON:   return RunStruct<dex::DragonType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::DARK:     return RunStruct<dex::DarkType, T...>::run(std::forward<RunArgs>(args)...);
    case Type::FAIRY:    return RunStruct<dex::FairyType, T...>::run(std::forward<RunArgs>(args)...);
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
