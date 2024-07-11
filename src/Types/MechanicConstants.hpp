#pragma once

#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <Types/Random.hpp>
#include <Utilities/TypeChart.hpp>
#include <array>
#include <cmath>
#include <cstdint>

namespace pokesim {
struct MechanicConstants {
  static constexpr std::array<types::percentChance, 4U> CRIT_CHANCE_DIVISORS{24U, 8U, 2U, 1U};
  static constexpr std::uint8_t MAX_DAMAGE_ROLL_COUNT = 16U;

  static constexpr std::uint8_t MAX_TEAM_SIZE = 6U;
  static constexpr std::uint8_t MAX_ACTIVE_POKEMON_SLOTS = 2U;
  static constexpr std::uint8_t MAX_MOVE_SLOTS = 4U;
  static constexpr std::uint8_t MAX_TARGETS = 3U;

  static constexpr float CRIT_MULTIPLIER = 1.5F;
  static constexpr float STAB_MULTIPLIER = 1.5F;

  static constexpr std::uint8_t FIXED_POINT_SCALING_FACTOR = 12U;
  static constexpr std::uint16_t FIXED_POINT_SCALE = 1U << FIXED_POINT_SCALING_FACTOR;
  static constexpr std::uint16_t FIXED_POINT_HALF_SCALE = FIXED_POINT_SCALE / 2U;

  static constexpr internal::TypeChart TYPE_CHART{
    {dex::Type::NORMAL,
     {
       {dex::Type::GHOST, TypeEffectiveness::IMMUNE},

       {dex::Type::FIGHTING, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::GRASS,
     {
       {dex::Type::WATER, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ELECTRIC, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::GROUND, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::FIRE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ICE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::POISON, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FLYING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::WATER,
     {
       {dex::Type::FIRE, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::WATER, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ICE, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::ELECTRIC, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::FIRE,
     {
       {dex::Type::FIRE, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ICE, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FAIRY, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::WATER, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GROUND, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::BUG,
     {
       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::GROUND, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::FIRE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FLYING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::FLYING,
     {
       {dex::Type::GROUND, TypeEffectiveness::IMMUNE},

       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::ELECTRIC, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ICE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::POISON,
     {
       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::POISON, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FAIRY, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::GROUND, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::PSYCHIC, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::ELECTRIC,
     {
       {dex::Type::ELECTRIC, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FLYING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::GROUND, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::GROUND,
     {
       {dex::Type::ELECTRIC, TypeEffectiveness::IMMUNE},

       {dex::Type::POISON, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::WATER, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ICE, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::FIGHTING,
     {
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::DARK, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::FLYING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::PSYCHIC, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FAIRY, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::PSYCHIC,
     {
       {dex::Type::FIGHTING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::PSYCHIC, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::BUG, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GHOST, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::DARK, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::ROCK,
     {
       {dex::Type::NORMAL, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::POISON, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FLYING, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::WATER, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GROUND, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::ICE,
     {
       {dex::Type::ICE, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::FIRE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::GHOST,
     {
       {dex::Type::NORMAL, TypeEffectiveness::IMMUNE},
       {dex::Type::FIGHTING, TypeEffectiveness::IMMUNE},

       {dex::Type::POISON, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::GHOST, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::DARK, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::DRAGON,
     {
       {dex::Type::FIRE, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::WATER, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ELECTRIC, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::ICE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::DRAGON, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FAIRY, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::DARK,
     {
       {dex::Type::PSYCHIC, TypeEffectiveness::IMMUNE},

       {dex::Type::GHOST, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::DARK, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::FIGHTING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FAIRY, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::STEEL,
     {
       {dex::Type::POISON, TypeEffectiveness::IMMUNE},

       {dex::Type::NORMAL, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::GRASS, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ICE, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FLYING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::PSYCHIC, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::ROCK, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::DRAGON, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::FAIRY, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::FIRE, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::FIGHTING, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::GROUND, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
    {dex::Type::FAIRY,
     {
       {dex::Type::DRAGON, TypeEffectiveness::IMMUNE},

       {dex::Type::FIGHTING, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::BUG, TypeEffectiveness::NOT_VERY_EFFECTIVE},
       {dex::Type::DARK, TypeEffectiveness::NOT_VERY_EFFECTIVE},

       {dex::Type::POISON, TypeEffectiveness::SUPER_EFFECTIVE},
       {dex::Type::STEEL, TypeEffectiveness::SUPER_EFFECTIVE},
     }},
  };
};
}  // namespace pokesim