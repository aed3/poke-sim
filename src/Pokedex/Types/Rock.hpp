#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Rock {
  static constexpr Type name(GameMechanics) { return Type::ROCK; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::NORMAL:
      case Type::FIRE:
      case Type::FLYING:
      case Type::POISON: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::WATER:
      case Type::GROUND:
      case Type::FIGHTING:
      case Type::STEEL:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::GROUND:
      case Type::FIGHTING:
      case Type::STEEL:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIRE:
      case Type::BUG:
      case Type::FLYING:
      case Type::ICE:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Rock"; }
  };
};
}  // namespace pokesim::dex
