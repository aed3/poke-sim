#pragma once

#include <Components/Boosts.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/Stats.hpp>
#include <Types/Ability.hpp>
#include <Types/Gender.hpp>
#include <Types/Item.hpp>
#include <Types/Nature.hpp>
#include <Types/Species.hpp>
#include <Types/Status.hpp>

#include "StateSetupBase.hpp"

namespace pokesim {
struct PokemonStateSetup : internal::StateSetupBase {
  PokemonStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  PokemonStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline_*/ void setAutoID();
  /*_inline_*/ void setID(std::uint16_t id);
  /*_inline_*/ void setSpecies(dex::Species speciesName);

  /*_inline_*/ void setSide(entt::entity entity);
  /*_inline_*/ void setBattle(entt::entity entity);

  /*_inline_*/ void setLevel(std::uint8_t level);
  /*_inline_*/ void setGender(dex::Gender gender);
  /*_inline_*/ void setAbility(dex::Ability ability);
  /*_inline_*/ void setItem(dex::Item item);
  /*_inline_*/ void setMoves(const std::vector<entt::entity>& moveSlots);

  /*_inline_*/ void setPostion(std::uint8_t position);
  /*_inline_*/ void setStatus(dex::Status status);

  /*_inline_*/ void setNature(dex::Nature nature);
  /*_inline_*/ void setEVs(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);
  /*_inline_*/ void setEVs(const EVs& evs);
  /*_inline_*/ void setIVs(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);
  /*_inline_*/ void setIVs(const IVs& ivs);

  template <typename BoostType>
  /*_inline_*/ void setBoost(std::int8_t boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  };

  template <typename StatType>
  /*_inline_*/ void setStat(std::uint16_t stat) {
    static_assert(
      std::is_same<stat::HP, StatType>() || std::is_same<stat::Atk, StatType>() ||
      std::is_same<stat::Def, StatType>() || std::is_same<stat::Spa, StatType>() ||
      std::is_same<stat::Spd, StatType>() || std::is_same<stat::Spe, StatType>());
    handle.emplace<StatType>(stat);
  };
};
}  // namespace pokesim