#pragma once

#include <Components/Boosts.hpp>
#include <Types/Constants.hpp>
#include <Types/Effect.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/FixedMemoryVector.hpp>

namespace pokesim {
struct EffectStackItem {
  types::effectEnum::extended<AtkBoost, DefBoost, SpaBoost, SpdBoost, SpeBoost, AccBoost, EvaBoost> effect;
  types::effectSource effectSource{};
  types::entity source{};
  types::entity target{};

  constexpr bool operator==(const EffectStackItem& other) const {
    return source == other.source && target == other.target && effect == other.effect;
  }
};

struct EffectStack {
  types::fixedMemoryVector<EffectStackItem, Constants::EffectStackLength::MAX> val{};
};
}  // namespace pokesim
