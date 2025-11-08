#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Move.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::move::tags {
/*
 * Runs a function with a certain move tag based on the passed in enum.
 * The `RunStruct` type should be a struct that accepts one template parameter that will be one of the move tags with
 * a static `run` function.
 *
 * @code
 * template <typename Tag>
 * struct EmplaceTag {
 *   static void run(types::handle handle) { handle.emplace<Tag>(); }
 * };
 *
 * enumToTag<EmplaceTag>(dex::Move::FURY_ATTACK, handle);
 * @endcode
 */
template <template <typename> typename RunStruct, typename... RunFunctionArgs>
auto enumToTag(dex::Move move, RunFunctionArgs&&... args) {
  switch (move) {
    case dex::Move::FURY_ATTACK:  return RunStruct<FuryAttack>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Move::KNOCK_OFF:    return RunStruct<KnockOff>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Move::MOONBLAST:    return RunStruct<Moonblast>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Move::QUIVER_DANCE: return RunStruct<QuiverDance>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Move::THUNDERBOLT:  return RunStruct<Thunderbolt>::run(std::forward<RunFunctionArgs>(args)...);
    case dex::Move::WILL_O_WISP:  return RunStruct<WillOWisp>::run(std::forward<RunFunctionArgs>(args)...);

    default: {
      POKESIM_REQUIRE_FAIL("Using a tag for move that does not exist.");
    }
  }
}
}  // namespace pokesim::move::tags