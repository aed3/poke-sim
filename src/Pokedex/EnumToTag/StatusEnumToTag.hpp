#pragma once

#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Registry.hpp>
#include <utility>

namespace pokesim::dex {
/*
 * Runs a function with a certain status tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the status tags with
 * a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::Status::BRN, handle);
 * @endcode
 */
template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
auto enumToTag(Status status, RunArgs&&... args) {
  switch (status) {
    case Status::BRN: return RunStruct<status::tags::Burn, T...>::run(std::forward<RunArgs>(args)...);
    case Status::FRZ: return RunStruct<status::tags::Freeze, T...>::run(std::forward<RunArgs>(args)...);
    case Status::PAR: return RunStruct<status::tags::Paralysis, T...>::run(std::forward<RunArgs>(args)...);
    case Status::PSN: return RunStruct<status::tags::Poison, T...>::run(std::forward<RunArgs>(args)...);
    case Status::SLP: return RunStruct<status::tags::Sleep, T...>::run(std::forward<RunArgs>(args)...);
    case Status::TOX: return RunStruct<status::tags::Toxic, T...>::run(std::forward<RunArgs>(args)...);

    default: {
      POKESIM_REQUIRE_FAIL("Using a tag for status that does not exist.");
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

template <template <typename, typename...> typename RunStruct, typename... T, typename... RunArgs>
void forEachStatus(RunArgs&&... args) {
  enumToTag<RunStruct, T...>(Status::BRN, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(Status::FRZ, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(Status::PAR, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(Status::PSN, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(Status::SLP, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(Status::TOX, std::forward<RunArgs>(args)...);
}

// Assigns a status' tag to a handle
void emplaceTagFromEnum(Status status, types::handle handle);
void emplaceTagFromEnum(Status status, types::registry& registry, types::entity entity);

// Checks if the handle has the status' tag
bool hasTag(Status status, types::handle handle);
bool hasTag(Status status, const types::registry& registry, types::entity entity);
}  // namespace pokesim::dex
