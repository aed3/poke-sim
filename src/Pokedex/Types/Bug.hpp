#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Bug {
  static constexpr Type name(GameMechanics) { return Type::BUG; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::GRASS:
      case Type::GROUND:
      case Type::FIGHTING: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::FIRE:
      case Type::FLYING:
      case Type::ROCK:   return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::FIRE:
      case Type::FLYING:
      case Type::POISON:
      case Type::FIGHTING:
      case Type::GHOST:
      case Type::STEEL:
      case Type::FAIRY:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::PSYCHIC:
      case Type::DARK:    return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Bug"; }
  };
};
}  // namespace pokesim::dex
