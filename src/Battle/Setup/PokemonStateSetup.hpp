#pragma once

#include <Components/Boosts.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Gender.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
// Tool to set properties of a Pokemon's state to an entity.
struct PokemonStateSetup : internal::StateSetupBase {
  PokemonStateSetup(types::registry& registry) : PokemonStateSetup(registry, registry.create()) {}
  PokemonStateSetup(types::registry& registry, types::entity entity);

  operator types::entity() const { return entity(); }

  /**
   * @brief Applies the defaults to the required properties for a Pokemon state.
   *
   * Some of the required properties are a blank `SpeciesName`, `Side`, and `Battle` component along with an
   * automatically set Id.
   */
  void initBlank();

  void setAutoID();
  void setID(types::stateId id);
  void setSpecies(dex::Species speciesName);

  void setSide(types::entity entity);
  void setBattle(types::entity entity);

  void setHp(types::stat hp);
  void setTypes(SpeciesTypes types);
  void setLevel(types::level level);
  void setGender(dex::Gender gender);
  void setAbility(dex::Ability ability);
  void setItem(dex::Item item);
  void setMoves(const std::vector<types::entity>& moveSlots);

  void setPostion(types::teamPositionIndex position);
  void setStatus(dex::Status status);

  void setNature(dex::Nature nature);
  void setEVs(types::ev hp, types::ev atk, types::ev def, types::ev spa, types::ev spd, types::ev spe);
  void setEVs(const Evs& evs);
  void setIVs(types::iv hp, types::iv atk, types::iv def, types::iv spa, types::iv spd, types::iv spe);
  void setIVs(const Ivs& ivs);

  template <typename BoostType>
  void setBoost(types::boost boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  };

  template <typename StatType>
  void setStat(types::stat stat) {
    static_assert(
      std::is_same<stat::Hp, StatType>() || std::is_same<stat::Atk, StatType>() ||
      std::is_same<stat::Def, StatType>() || std::is_same<stat::Spa, StatType>() ||
      std::is_same<stat::Spd, StatType>() || std::is_same<stat::Spe, StatType>());
    handle.emplace<StatType>(stat);
  };
};
}  // namespace pokesim