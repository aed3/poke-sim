#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Fighting {
  static constexpr Type name(GameMechanics) { return Type::FIGHTING; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::BUG:
      case Type::ROCK:
      case Type::DARK: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FLYING:
      case Type::PSYCHIC:
      case Type::FAIRY:   return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::GHOST: return TypeEffectiveness::IMMUNE;

      case Type::BUG:
      case Type::FLYING:
      case Type::POISON:
      case Type::PSYCHIC:
      case Type::FAIRY:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::NORMAL:
      case Type::ROCK:
      case Type::ICE:
      case Type::DARK:
      case Type::STEEL:  return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Fighting"; }
  };
};
}  // namespace pokesim::dex
