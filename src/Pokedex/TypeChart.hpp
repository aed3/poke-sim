#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <array>
#include <cstdint>
#include <initializer_list>

namespace pokesim {
// The extra array element is for NO_TYPE
using TypeChartBase = std::array<
  std::array<pokesim::TypeEffectiveness, pokesim::dex::TOTAL_TYPE_COUNT + 1U>, pokesim::dex::TOTAL_TYPE_COUNT + 1U>;

struct TypeChart : private TypeChartBase {
 private:
  using constructorType = std::initializer_list<
    std::pair<pokesim::dex::Type, std::initializer_list<std::pair<pokesim::dex::Type, pokesim::TypeEffectiveness>>>>;

 public:
  constexpr TypeChart(const constructorType partialChart) : TypeChartBase() {
    for (auto& ratios : *this) {
      for (auto& effectiveness : ratios) {
        effectiveness = pokesim::TypeEffectiveness::NEUTRAL;
      }
    }

    for (const auto& [defending, ratios] : partialChart) {
      for (const auto& [attacking, effectiveness] : ratios) {
        at((std::uint8_t)attacking).at((std::uint8_t)defending) = effectiveness;
      }
    }
  }

  constexpr TypeChart(GameMechanics gameMechanics) : TypeChart(TypeChart::pickForMechanics(gameMechanics)) {}

  constexpr pokesim::TypeEffectiveness effectiveness(pokesim::dex::Type attacking, pokesim::dex::Type defending) const {
    return at((std::uint8_t)attacking).at((std::uint8_t)defending);
  }

 private:
  static constexpr TypeChart pickForMechanics(GameMechanics mechanics) {
    switch (mechanics) {
      default:
        // X_Y
        return TypeChart{
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
    }
  }
};
}  // namespace pokesim