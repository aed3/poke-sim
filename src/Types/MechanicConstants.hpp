#pragma once

#include <Types/Random.hpp>
#include <array>
#include <cstdint>

namespace pokesim {
struct MechanicConstants {
  static constexpr std::uint8_t TYPES_PER_POKEMON = 2U;

  static constexpr std::array<types::percentChance, 4U> CRIT_CHANCE_DIVISORS{24U, 8U, 2U, 1U};
  static constexpr std::array<float, 7U> STAT_BOOST_STAGES{1.0F, 1.5F, 2.0F, 2.5F, 3.0F, 3.5F, 4.0F};

  // The 35%-35%-15%-15% out of 100 for 2-3-4-5 hits added so each index is the sum of the chance of its hit count and
  // the hit counts less than it so it works with the randomEventChances function
  static constexpr std::array<types::percentChance, 4U> PROGRESSIVE_MULTI_HIT_CHANCES{35U, 70U, 85U, 100U};

  // TODO(aed3): Should change based on simulation's game mechanics
  static constexpr float CRIT_MULTIPLIER = 1.5F;

  static constexpr std::uint8_t FIXED_POINT_SCALING_FACTOR = 12U;
  static constexpr std::uint16_t FIXED_POINT_SCALE = 1U << FIXED_POINT_SCALING_FACTOR;
  static constexpr std::uint16_t FIXED_POINT_HALF_SCALE = FIXED_POINT_SCALE / 2U;

  static constexpr std::uint8_t SIDE_COUNT = 2U;

  struct PokemonLevel {
    static constexpr std::uint8_t MAX = 100U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct PokemonBaseStat {
    static constexpr std::uint8_t MAX = 255U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct PokemonHpStat {
    static constexpr std::uint16_t MAX = 1428U;
    static constexpr std::uint16_t MIN = 1U;
  };

  struct PokemonCurrentHpStat {
    // TODO(aed3): Should this be doubled for dynamax or should that be a different value?
    static constexpr std::uint16_t MAX = 1428U;
    static constexpr std::uint16_t MIN = 0U;
  };

  struct PokemonStat {
    static constexpr std::uint16_t MAX = 633U;
    static constexpr std::uint16_t MIN = 1U;
  };

  struct PokemonEffectiveStat {
    static constexpr std::uint16_t MAX = 65535U;
    static constexpr std::uint16_t MIN = 1U;
  };

  struct PokemonEv {
    static constexpr std::uint8_t MAX = 255U;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct PokemonIv {
    static constexpr std::uint8_t MAX = 31U;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct PokemonStatBoost {
    static constexpr std::int8_t MAX = 6;
    static constexpr std::int8_t BASE = 0;
    static constexpr std::int8_t MIN = -6;
  };

  struct MoveMaxPp {
    static constexpr std::uint8_t MAX = 64U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct MovePp {
    static constexpr std::uint8_t MAX = MoveMaxPp::MAX;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct MoveBasePower {
    static constexpr std::uint8_t MAX = 255U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct MoveBaseAccuracy {
    static constexpr std::uint8_t MAX = 100U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct MoveHits {
    static constexpr std::uint8_t MAX = 10U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct MoveBaseEffectChance {
    static constexpr std::uint8_t MAX = 100U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct MovePriority {
    static constexpr std::int8_t MAX = 5;  // 8 is theoretically possible, but no existing move has more than 5
    static constexpr std::int8_t BASE = 0;
    static constexpr std::int8_t MIN = -7;
  };

  struct CritBoost {
    // 255 is theoretically possible, but the various effects that can increase crit chance can be added up to 7 at most
    // as of now
    static constexpr std::uint8_t MAX = 7U;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct Damage {
    static constexpr std::uint16_t MAX = 65535U;
    static constexpr std::uint16_t MIN = 1U;
  };

  struct DamageRollCount {
    static constexpr std::uint8_t MAX = 16U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct TypeEffectivenessShift {
    static constexpr std::int8_t MAX = 3;
    static constexpr std::int8_t BASE = 0;
    static constexpr std::int8_t MIN = -7;
  };

  struct TeamSize {
    static constexpr std::uint8_t MAX = 6U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct ActivePokemonSlotsPerSide {
    static constexpr std::uint8_t MAX = 2U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct ActivePokemon {
    static constexpr std::uint8_t MAX = ActivePokemonSlotsPerSide::MAX * SIDE_COUNT;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct MoveSlots {
    static constexpr std::uint8_t MAX = 4U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct Targets {
    static constexpr std::uint8_t MAX = 3U;
    static constexpr std::uint8_t MIN = 1U;
  };

  struct PercentChance {
    static constexpr std::uint8_t MAX = 100U;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct Probability {
    static constexpr float MAX = 1.0F;
    static constexpr float MIN = 0.0F;
  };

  static constexpr float PercentChanceToProbability = PercentChance::MAX / Probability::MAX;

  struct AnalyzeEffectMultiplier {
    static constexpr float MAX = PokemonHpStat::MAX;
    static constexpr float MIN = 0.0F;
  };

  struct ActionQueueLength {
    // TODO(aed3): 64 is a guess, so find out what the actual number is
    static constexpr std::uint8_t MAX = 64U;
    static constexpr std::uint8_t MIN = 0U;
  };

  struct TurnCount {
    // Technically 65535, but battles over 1000 turns aren't usually supported on Showdown
    static constexpr std::uint16_t MAX = 1000U;
    static constexpr std::uint16_t MIN = 0U;
  };
};
}  // namespace pokesim