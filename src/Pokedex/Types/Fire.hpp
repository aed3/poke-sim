#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Fire {
  static constexpr Type name(GameMechanics) { return Type::FIRE; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::WATER:
      case Type::GROUND:
      case Type::ROCK:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::FIRE:
      case Type::BUG:
      case Type::ICE:
      case Type::STEEL:
      case Type::FAIRY: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::WATER:
      case Type::FIRE:
      case Type::ROCK:
      case Type::DRAGON: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::BUG:
      case Type::ICE:
      case Type::STEEL: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Fire"; }
  };
};
}  // namespace pokesim::dex
