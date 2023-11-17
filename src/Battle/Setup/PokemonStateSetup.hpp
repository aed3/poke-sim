#pragma once

#include <Components/Boosts.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/Stats.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Gender.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
// Tool to set properties of a Pokemon's state to an entity.
struct PokemonStateSetup : internal::StateSetupBase {
  PokemonStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  PokemonStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /**
   * @brief Applies the defaults to the required properties for a Pokemon state.
   *
   * Some of the required properties are a blank `SpeciesName`, `Side`, and `Battle` component along with an
   * automatically set Id.
   */
  /*_inline_*/ void initBlank();

  /*_inline_*/ void setAutoID();
  /*_inline_*/ void setID(types::StateId id);
  /*_inline_*/ void setSpecies(dex::Species speciesName);

  /*_inline_*/ void setSide(entt::entity entity);
  /*_inline_*/ void setBattle(entt::entity entity);

  /*_inline_*/ void setLevel(types::Level level);
  /*_inline_*/ void setGender(dex::Gender gender);
  /*_inline_*/ void setAbility(dex::Ability ability);
  /*_inline_*/ void setItem(dex::Item item);
  /*_inline_*/ void setMoves(const std::vector<entt::entity>& moveSlots);

  /*_inline_*/ void setPostion(types::TeamSlotPosition position);
  /*_inline_*/ void setStatus(dex::Status status);

  /*_inline_*/ void setNature(dex::Nature nature);
  /*_inline_*/ void setEVs(types::Ev hp, types::Ev atk, types::Ev def, types::Ev spa, types::Ev spd, types::Ev spe);
  /*_inline_*/ void setEVs(const Evs& evs);
  /*_inline_*/ void setIVs(types::Iv hp, types::Iv atk, types::Iv def, types::Iv spa, types::Iv spd, types::Iv spe);
  /*_inline_*/ void setIVs(const Ivs& ivs);

  template <typename BoostType>
  /*_inline_*/ void setBoost(types::Boost boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  };

  template <typename StatType>
  /*_inline_*/ void setStat(types::Stat stat) {
    static_assert(
      std::is_same<stat::Hp, StatType>() || std::is_same<stat::Atk, StatType>() ||
      std::is_same<stat::Def, StatType>() || std::is_same<stat::Spa, StatType>() ||
      std::is_same<stat::Spd, StatType>() || std::is_same<stat::Spe, StatType>());
    handle.emplace<StatType>(stat);
  };
};
}  // namespace pokesim