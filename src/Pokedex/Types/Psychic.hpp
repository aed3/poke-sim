#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Psychic {
  static constexpr Type name(GameMechanics) { return Type::PSYCHIC; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::FIGHTING:
      case Type::PSYCHIC:  return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::BUG:
      case Type::GHOST:
      case Type::DARK:  return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::DARK: return TypeEffectiveness::IMMUNE;

      case Type::PSYCHIC:
      case Type::STEEL:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::POISON:
      case Type::FIGHTING: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Psychic"; }
  };
};
}  // namespace pokesim::dex
