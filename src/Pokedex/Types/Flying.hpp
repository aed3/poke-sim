#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <string_view>

namespace pokesim::dex {
struct Flying {
  static constexpr Type name(GameMechanics) { return Type::FLYING; }

  static constexpr TypeEffectiveness defending(GameMechanics, Type attacking) {
    switch (attacking) {
      case Type::GROUND: return TypeEffectiveness::IMMUNE;

      case Type::GRASS:
      case Type::BUG:
      case Type::FIGHTING: return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::ELECTRIC:
      case Type::ROCK:
      case Type::ICE:      return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  static constexpr TypeEffectiveness attacking(GameMechanics, Type defending) {
    switch (defending) {
      case Type::ELECTRIC:
      case Type::ROCK:
      case Type::STEEL:    return TypeEffectiveness::NOT_VERY_EFFECTIVE;

      case Type::GRASS:
      case Type::BUG:
      case Type::FIGHTING: return TypeEffectiveness::SUPER_EFFECTIVE;

      default: return TypeEffectiveness::NEUTRAL;
    }
  }

  struct Strings {
    static constexpr std::string_view name() { return "Flying"; }
  };
};
}  // namespace pokesim::dex
