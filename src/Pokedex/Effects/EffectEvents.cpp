#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Config/Require.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex {
void internal::ChoiceLockEvents::onDisableMove(
  types::registry& registry, const pokesim::ChoiceLock& choiceLocked, const MoveSlots& moveSlots) {
  POKESIM_REQUIRE(
    std::find(moveSlots.val.begin(), moveSlots.val.end(), choiceLocked.val) != moveSlots.val.end(),
    "Should skip if the move is no longer present, but when does that happen?");

  for (types::entity entity : moveSlots.val) {
    if (entity != choiceLocked.val) {
      registry.emplace<move::tags::Disabled>(entity);
    }
  }
}
}  // namespace pokesim::dex