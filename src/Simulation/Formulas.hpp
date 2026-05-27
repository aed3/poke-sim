#pragma once

#include <Components/EVsIVs.hpp>
#include <Config/Require.hpp>
#include <Types/Constants.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Stat.hpp>
#include <Types/Move.hpp>
#include <Types/NaturesTable.hpp>
#include <Types/Stats.hpp>

// TODO(aed3): Add argument checks to these functions. That will likely only make them true constexpr when asserts are
// turned off.

// NOLINTBEGIN(readability-magic-numbers)
namespace pokesim {
constexpr types::damage computeDamageRoll(types::damage damage, types::damageRollIndex damageRoll) {
  return (types::damage)(damage * ((100U - damageRoll) / 100.0F));
}

constexpr types::damage computeAverageDamageRoll(types::damage damage) {
  return (types::damage)(damage * (100U - (Constants::DamageRollCount::MAX - 1U) / 2.0F) / 100.0F);
}

constexpr types::damage computeMinDamageRoll(types::damage damage) {
  return computeDamageRoll(damage, Constants::DamageRollCount::MAX - 1U);
}

constexpr types::damage computeBaseDamage(
  types::power power, types::level level, types::stat attack, types::stat defense) {
  return ((((2U * level / 5U + 2U) * power * attack) / defense) / 50U) + 2U;
}

constexpr types::stat computeStatFromBaseStat(
  dex::Stat statName, types::baseStat baseStat, types::level level, dex::Nature nature, const Evs& evs,
  const Ivs& ivs) {
  types::ev ev = Constants::PokemonEv::DEFAULT;
  types::iv iv = Constants::PokemonIv::DEFAULT;

  switch (statName) {
    case dex::Stat::HP: {
      ev = evs.hp;
      iv = ivs.hp;
      break;
    }
    case dex::Stat::ATK: {
      ev = evs.atk;
      iv = ivs.atk;
      break;
    }
    case dex::Stat::DEF: {
      ev = evs.def;
      iv = ivs.def;
      break;
    }
    case dex::Stat::SPA: {
      ev = evs.spa;
      iv = ivs.spa;
      break;
    }
    case dex::Stat::SPD: {
      ev = evs.spd;
      iv = ivs.spd;
      break;
    }
    case dex::Stat::SPE: {
      ev = evs.spe;
      iv = ivs.spe;
      break;
    }
    default: POKESIM_REQUIRE_FAIL("Using a stat that does not have EVs and/or IVs.");
  }

  if (statName == dex::Stat::HP) {
    return ((2U * baseStat + iv + (ev / 4U) + 100U) * level / 100U) + 10U;
  }

  types::stat stat = ((2U * baseStat + iv + (ev / 4U)) * level / 100U) + 5U;

  if (NaturesTable::plus(nature) == statName) {
    stat = (stat * 110U) / 100U;
  }
  else if (NaturesTable::minus(nature) == statName) {
    stat = (stat * 90U) / 100U;
  }
  return stat;
}
}  // namespace pokesim
// NOLINTEND(readability-magic-numbers)
