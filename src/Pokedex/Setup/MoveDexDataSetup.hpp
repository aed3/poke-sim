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

  void setName(Move move);
  void setType(Type type);
  void setAccuracy(types::baseAccuracy accuracy);
  void setBasePower(types::basePower basePower);

  void setCategoryPhysical();
  void setCategorySpecial();
  void setCategoryStatus();

  void setBasePp(types::pp pp);
  void setPriority(types::priority priority);
  void setMultiHit(types::moveHits minHits, types::moveHits maxHits);

  void setPrimaryEffect(types::entity entity);
  void setSecondaryEffect(types::entity entity);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(Pokedex* pokedex) : DexDataSetup(pokedex) {}
  types::entity entity() const { return handle; }

  void setChance(types::baseEffectChance chance);
  void setEffectsSelf();
  void setEffectsTarget();

  template <typename BoostType>
  void setBoost(types::boost boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  }
};
}  // namespace pokesim::dex::internal
