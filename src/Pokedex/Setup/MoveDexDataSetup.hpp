#pragma once

#include <Components/Boosts.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>
#include <type_traits>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct MoveDexDataSetup : DexDataSetup {
  MoveDexDataSetup(Pokedex* pokedex) : DexDataSetup(pokedex) {}

  /*_inline_*/ void setName(Move move);
  /*_inline_*/ void setType(Type type);
  /*_inline_*/ void setAccuracy(types::baseAccuracy accuracy);
  /*_inline_*/ void setBasePower(types::basePower basePower);

  /*_inline_*/ void setCategoryPhysical();
  /*_inline_*/ void setCategorySpecial();
  /*_inline_*/ void setCategoryStatus();

  /*_inline_*/ void setBasePp(types::pp pp);
  /*_inline_*/ void setPriority(types::priority priority);
  /*_inline_*/ void setMultiHit(types::moveHits minHits, types::moveHits maxHits);

  /*_inline_*/ void setPrimaryEffect(types::entity entity);
  /*_inline_*/ void setSecondaryEffect(types::entity entity);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(Pokedex* pokedex) : DexDataSetup(pokedex) {}
  types::entity entity() const { return handle; }

  /*_inline_*/ void setChance(types::baseEffectChance chance);
  /*_inline_*/ void setEffectsSelf();
  /*_inline_*/ void setEffectsTarget();

  template <typename BoostType>
  /*_inline_*/ void setBoost(types::boost boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  }
};
}  // namespace pokesim::dex::internal
