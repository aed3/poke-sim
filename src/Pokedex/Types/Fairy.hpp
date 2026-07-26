#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Fairy {
  static constexpr Type name(GameMechanics) { return Type::FAIRY; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::DRAGON: return TypeEffectiveness::IMMUNE;

      case Type::BUG:
      case Type::FIGHTING:
      case Type::DARK:     return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::POISON:
      case Type::STEEL:  return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::FIRE:
      case Type::POISON:
      case Type::STEEL:  return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIGHTING:
      case Type::DRAGON:
      case Type::DARK:     return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Fairy"; }
  };
};
}  // namespace pokesim::dex
