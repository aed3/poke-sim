#pragma once

#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Status.hpp>
#include <utility>

namespace pokesim::status::tags {
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
auto enumToTag(dex::Status status, RunArgs&&... args) {
  switch (status) {
    case dex::Status::BRN: return RunStruct<Burn, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Status::FRZ: return RunStruct<Freeze, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Status::PAR: return RunStruct<Paralysis, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Status::PSN: return RunStruct<Poison, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Status::SLP: return RunStruct<Sleep, T...>::run(std::forward<RunArgs>(args)...);
    case dex::Status::TOX: return RunStruct<Toxic, T...>::run(std::forward<RunArgs>(args)...);

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
void forEach(RunArgs&&... args) {
  enumToTag<RunStruct, T...>(dex::Status::BRN, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(dex::Status::FRZ, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(dex::Status::PAR, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(dex::Status::PSN, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(dex::Status::SLP, std::forward<RunArgs>(args)...);
  enumToTag<RunStruct, T...>(dex::Status::TOX, std::forward<RunArgs>(args)...);
}
}  // namespace pokesim::status::tags
