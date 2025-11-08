#pragma once

#include <Components/BaseEffectChance.hpp>
#include <Components/Boosts.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/AddedTargets.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>
#include <type_traits>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct MoveDexDataSetup : DexDataSetup {
  MoveDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(Move move);
  void setNameTag(Move move);
  void setType(Type type);
  void setAccuracy(types::baseAccuracy accuracy);
  void setBasePower(types::basePower basePower);

  void setCategoryPhysical();
  void setCategorySpecial();
  void setCategoryStatus();

  void setBasePp(types::pp pp);
  void setPriority(types::priority priority);
  void setHitCount(types::moveHits hitCount);

  void addAddedTargets(AddedTargetOptions addedTargets);

  void setEffectTargetsMoveSource();
  void setEffectTargetsMoveTarget();

  template <typename EffectType, typename... EffectValues>
  void setPrimaryEffect(const EffectValues&... effectValues) {
    POKESIM_REQUIRE(
      !handle.all_of<move::effect::tags::Secondary>(),
      "Moves can only have primary or secondary effects, not both.");
    handle.emplace_or_replace<move::effect::tags::Primary>();
    handle.emplace<EffectType>(effectValues...);
  }

  template <typename EffectType, typename... EffectValues>
  void setSecondaryEffect(types::percentChance chance, const EffectValues&... effectValues) {
    POKESIM_REQUIRE(
      !handle.all_of<move::effect::tags::Primary>(),
      "Moves can only have secondary or primary effects, not both.");
    handle.emplace_or_replace<move::effect::tags::Secondary>();
    handle.emplace<EffectType>(effectValues...);
    handle.emplace<BaseEffectChance>(chance);
  }
};
}  // namespace pokesim::dex::internal
