#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Ice {
  static constexpr Type name(GameMechanics) { return Type::ICE; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::ICE: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIRE:
      case Type::FIGHTING:
      case Type::ROCK:
      case Type::STEEL:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::WATER:
      case Type::FIRE:
      case Type::ICE:
      case Type::STEEL: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::FLYING:
      case Type::GROUND:
      case Type::DRAGON: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Ice"; }
  };
};
}  // namespace pokesim::dex
