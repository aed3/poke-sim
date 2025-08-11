#pragma once

#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Status.hpp>
// TODO(aed3): Make this auto generated

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
template <template <typename> typename RunStruct, typename... RunFunctionArgs>
auto enumToTag(dex::Status status, RunFunctionArgs&&... args) {
  switch (status) {
    case dex::Status::BRN: return RunStruct<Burn>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Status::FRZ: return RunStruct<Freeze>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Status::PAR: return RunStruct<Paralysis>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Status::PSN: return RunStruct<Poison>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Status::SLP: return RunStruct<Sleep>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Status::TOX: return RunStruct<Toxic>::run(std::forward<RunFunctionArgs>(args)...);

    default: {
      POKESIM_REQUIRE(false, "Adding tag for status that does not exist.");
    }
  }
}
}  // namespace pokesim::status::tags