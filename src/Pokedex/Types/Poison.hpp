#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Poison {
  static constexpr Type name(GameMechanics) { return Type::POISON; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::GRASS:
      case Type::BUG:
      case Type::POISON:
      case Type::FIGHTING:
      case Type::FAIRY:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GROUND:
      case Type::PSYCHIC: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::STEEL: return TypeEffectiveness::IMMUNE;

      case Type::POISON:
      case Type::GROUND:
      case Type::ROCK:
      case Type::GHOST:  return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::FAIRY: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Poison"; }
  };
};
}  // namespace pokesim::dex
