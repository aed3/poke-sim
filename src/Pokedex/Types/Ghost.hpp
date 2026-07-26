#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Ghost {
  static constexpr Type name(GameMechanics) { return Type::GHOST; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::NORMAL:
      case Type::FIGHTING: return TypeEffectiveness::IMMUNE;

      case Type::BUG:
      case Type::POISON: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GHOST:
      case Type::DARK:  return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::NORMAL: return TypeEffectiveness::IMMUNE;

      case Type::DARK: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::PSYCHIC:
      case Type::GHOST:   return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Ghost"; }
  };
};
}  // namespace pokesim::dex
