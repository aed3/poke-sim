#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Grass {
  static constexpr Type name(GameMechanics) { return Type::GRASS; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::GRASS:
      case Type::WATER:
      case Type::ELECTRIC:
      case Type::GROUND:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIRE:
      case Type::BUG:
      case Type::FLYING:
      case Type::POISON:
      case Type::ICE:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::GRASS:
      case Type::FIRE:
      case Type::BUG:
      case Type::FLYING:
      case Type::POISON:
      case Type::DRAGON:
      case Type::STEEL:  return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::WATER:
      case Type::GROUND:
      case Type::ROCK:   return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Grass"; }
  };
};
}  // namespace pokesim::dex
