#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Normal {
  static constexpr Type name(GameMechanics) { return Type::NORMAL; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::GHOST: return TypeEffectiveness::IMMUNE;

      case Type::FIGHTING: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::GHOST: return TypeEffectiveness::IMMUNE;

      case Type::ROCK:
      case Type::STEEL: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Normal"; }
  };
};
}  // namespace pokesim::dex
