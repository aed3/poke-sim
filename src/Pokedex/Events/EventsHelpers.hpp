#pragma once

#include <Battle/Helpers/IntegerModify.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Event.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
inline void updateDamageEventModifier(
  types::registry& registry, types::entity move, types::eventModifier numerator, types::eventModifier denominator) {
  auto& modifier = registry.get<internal::calc_damage::DamageRollModifiers>(move);
  modifier.damageEventModifier = internal::chainValueToModifier(modifier.damageEventModifier, numerator, denominator);
}

template <typename CurrentActionMovesAsType, typename... ExcludeComponents>
struct ModifyDamage {
  static void run(
    types::registry& registry, const CurrentActionMovesAsType& moves, types::eventModifier numerator,
    types::eventModifier denominator) {
    for (types::entity move : moves) {
      if (registry.any_of<move::tags::Status, ExcludeComponents...>(move)) {
        continue;
      }

      updateDamageEventModifier(registry, move, numerator, denominator);
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

template <typename Effect>
struct IncrementTurnsRemaining {
  template <typename, typename = void>
  struct turnsRemaining : std::false_type {};
  template <typename T>
  struct turnsRemaining<T, std::void_t<decltype(T::turnsRemaining)>> : std::true_type {};

  static void run(types::handle handle, Effect& effect) {
    bool remove = false;
    if constexpr (turnsRemaining<Effect>::value) {
      effect.turnsRemaining--;
      remove = effect.turnsRemaining == Constants::EffectDuration::MIN;
    }
    else {
      effect.val--;
      remove = effect.val == Constants::EffectDuration::MIN;
    }

    if (remove) {
      handle.remove<Effect>();
    }
  }
};

}  // namespace pokesim::internal
