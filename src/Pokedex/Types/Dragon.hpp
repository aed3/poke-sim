#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Dragon {
  static constexpr Type name(GameMechanics) { return Type::DRAGON; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::GRASS:
      case Type::WATER:
      case Type::FIRE:
      case Type::ELECTRIC: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::ICE:
      case Type::DRAGON:
      case Type::FAIRY:  return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::FAIRY: return TypeEffectiveness::IMMUNE;

      case Type::STEEL: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::DRAGON: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Dragon"; }
  };
};
}  // namespace pokesim::dex
