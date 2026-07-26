#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Electric {
  static constexpr Type name(GameMechanics) { return Type::ELECTRIC; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::ELECTRIC:
      case Type::FLYING:
      case Type::STEEL:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GROUND: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::GROUND: return TypeEffectiveness::IMMUNE;

      case Type::GRASS:
      case Type::ELECTRIC:
      case Type::DRAGON:   return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::WATER:
      case Type::FLYING: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Electric"; }
  };
};
}  // namespace pokesim::dex
