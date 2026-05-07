#pragma once

#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Stat.hpp>
#include <array>
#include <type_traits>

namespace pokesim {
namespace internal {
struct NaturePlusMinus {
 private:
  using statEnumType = std::underlying_type_t<dex::Stat>;

  static constexpr statEnumType NATURED_STATS = 5U;
  static constexpr std::array<dex::Stat, NATURED_STATS> statIndexMapping = {
    dex::Stat::ATK,
    dex::Stat::DEF,
    dex::Stat::SPA,
    dex::Stat::SPD,
    dex::Stat::SPE,
  };

  static constexpr std::array<std::array<dex::Nature, NATURED_STATS>, NATURED_STATS> table = {{
    {dex::Nature::HARDY, dex::Nature::LONELY, dex::Nature::ADAMANT, dex::Nature::NAUGHTY, dex::Nature::BRAVE},
    {dex::Nature::BOLD, dex::Nature::DOCILE, dex::Nature::IMPISH, dex::Nature::LAX, dex::Nature::RELAXED},
    {dex::Nature::MODEST, dex::Nature::MILD, dex::Nature::BASHFUL, dex::Nature::RASH, dex::Nature::QUIET},
    {dex::Nature::CALM, dex::Nature::GENTLE, dex::Nature::CAREFUL, dex::Nature::QUIRKY, dex::Nature::SASSY},
    {dex::Nature::TIMID, dex::Nature::HASTY, dex::Nature::JOLLY, dex::Nature::NAIVE, dex::Nature::SERIOUS},
  }};

 public:
  dex::Stat plus = dex::Stat::NONE;
  dex::Stat minus = dex::Stat::NONE;

  constexpr NaturePlusMinus() {}
  constexpr NaturePlusMinus(dex::Nature nature) {
    for (statEnumType plusIndex = 0U; plusIndex < NATURED_STATS; plusIndex++) {
      for (statEnumType minusIndex = 0U; minusIndex < NATURED_STATS; minusIndex++) {
        if (nature != table.at(plusIndex).at(minusIndex)) {
          continue;
        }

        dex::Stat plusStat = statIndexMapping.at(plusIndex);
        dex::Stat minusStat = statIndexMapping.at(minusIndex);
        if (plusIndex != minusIndex) {
          plus = plusStat;
          minus = minusStat;
        }
        return;
      }
    }
  }
  constexpr NaturePlusMinus(std::underlying_type_t<dex::Nature> nature) : NaturePlusMinus((dex::Nature)nature) {}
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
