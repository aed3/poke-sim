#pragma once

#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Stat.hpp>
#include <array>
#include <type_traits>

namespace pokesim {
namespace internal {
struct NaturePlusMinus {
 private:
  using Stat = pokesim::dex::Stat;
  using Nature = pokesim::dex::Nature;
  using statEnumType = std::underlying_type_t<Stat>;

  static constexpr statEnumType NATURED_STATS = 5U;
  static constexpr std::array<Stat, NATURED_STATS> statIndexMapping = {
    Stat::ATK,
    Stat::DEF,
    Stat::SPA,
    Stat::SPD,
    Stat::SPE,
  };

  static constexpr std::array<std::array<Nature, NATURED_STATS>, NATURED_STATS> table = {{
    {Nature::HARDY, Nature::LONELY, Nature::ADAMANT, Nature::NAUGHTY, Nature::BRAVE},
    {Nature::BOLD, Nature::DOCILE, Nature::IMPISH, Nature::LAX, Nature::RELAXED},
    {Nature::MODEST, Nature::MILD, Nature::BASHFUL, Nature::RASH, Nature::QUIET},
    {Nature::CALM, Nature::GENTLE, Nature::CAREFUL, Nature::QUIRKY, Nature::SASSY},
    {Nature::TIMID, Nature::HASTY, Nature::JOLLY, Nature::NAIVE, Nature::SERIOUS},
  }};

 public:
  Stat plus = Stat::NONE;
  Stat minus = Stat::NONE;

  constexpr NaturePlusMinus() {}
  constexpr NaturePlusMinus(Nature nature) {
    for (statEnumType plusIndex = 0U; plusIndex < NATURED_STATS; plusIndex++) {
      for (statEnumType minusIndex = 0U; minusIndex < NATURED_STATS; minusIndex++) {
        if (nature != table.at(plusIndex).at(minusIndex)) {
          continue;
        }

        Stat plusStat = statIndexMapping.at(plusIndex);
        Stat minusStat = statIndexMapping.at(minusIndex);
        if (plusIndex != minusIndex) {
          plus = plusStat;
          minus = minusStat;
        }
        return;
      }
    }
  }
  constexpr NaturePlusMinus(std::underlying_type_t<Nature> nature) : NaturePlusMinus((Nature)nature) {}
};
}  // namespace internal

struct NaturesTable {
 private:
  using natureEnumType = std::underlying_type_t<dex::Nature>;

  static constexpr std::array<internal::NaturePlusMinus, dex::TOTAL_NATURE_COUNT + 1U> list = {
    0U,  1U,  2U,  3U,  4U,  5U,  6U,  7U,  8U,  9U,  10U, 11U, 12U,
    13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U,
  };

 public:
  static constexpr dex::Stat plus(dex::Nature nature) { return list.at((natureEnumType)nature).plus; }
  static constexpr dex::Stat minus(dex::Nature nature) { return list.at((natureEnumType)nature).minus; }
};
}  // namespace pokesim
