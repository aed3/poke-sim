#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Ground {
  static constexpr Type name(GameMechanics) { return Type::GROUND; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::ELECTRIC: return TypeEffectiveness::IMMUNE;

      case Type::POISON:
      case Type::ROCK:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::WATER:
      case Type::ICE:   return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::FLYING: return TypeEffectiveness::IMMUNE;

      case Type::GRASS:
      case Type::BUG:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIRE:
      case Type::ELECTRIC:
      case Type::POISON:
      case Type::ROCK:
      case Type::STEEL:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Ground"; }
  };
};
}  // namespace pokesim::dex
