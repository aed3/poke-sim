#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Steel {
  static constexpr Type name(GameMechanics) { return Type::STEEL; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::POISON: return TypeEffectiveness::IMMUNE;

      case Type::NORMAL:
      case Type::GRASS:
      case Type::BUG:
      case Type::FLYING:
      case Type::PSYCHIC:
      case Type::ROCK:
      case Type::ICE:
      case Type::DRAGON:
      case Type::STEEL:
      case Type::FAIRY:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIRE:
      case Type::GROUND:
      case Type::FIGHTING: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::WATER:
      case Type::FIRE:
      case Type::ELECTRIC:
      case Type::STEEL:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::ROCK:
      case Type::ICE:
      case Type::FAIRY: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Steel"; }
  };
};
}  // namespace pokesim::dex
