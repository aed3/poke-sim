#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Dark {
  static constexpr Type name(GameMechanics) { return Type::DARK; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::PSYCHIC: return TypeEffectiveness::IMMUNE;

      case Type::GHOST:
      case Type::DARK:  return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::BUG:
      case Type::FIGHTING:
      case Type::FAIRY:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::FIGHTING:
      case Type::DARK:
      case Type::FAIRY:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::PSYCHIC:
      case Type::GHOST:   return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Dark"; }
  };
};
}  // namespace pokesim::dex
