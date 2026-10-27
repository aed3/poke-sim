#pragma once

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/Damage.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Event.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
template <typename CurrentActionMovesAsType, typename... ExcludeComponents>
struct ModifyDamage {
  static void run(
    types::registry& registry, const CurrentActionMovesAsType& moves, types::eventModifier numerator,
    types::eventModifier denominator) {
    for (types::entity move : moves) {
      if (registry.any_of<move::tags::Status, ExcludeComponents...>(move)) {
        continue;
      }

      DamageRollModifiers& modifier = registry.get<DamageRollModifiers>(move);
      modifier.modifyDamageEvent = internal::chainValueToModifier(modifier.modifyDamageEvent, numerator, denominator);
    }
  }
};

template <typename CurrentActionMovesAsType>
struct SetMoveModifier {
  static void run(
    types::handle handle, const CurrentActionMovesAsType& moves, types::eventModifier numerator,
    types::eventModifier denominator) {
    for (types::entity move : moves) {
      EventModifier* eventModifier = handle.registry()->try_get<EventModifier>(move);

      if (eventModifier) {
        internal::chainComponentToModifier(*eventModifier, numerator, denominator);
      }
    }
  }
};
}  // namespace pokesim::internal
