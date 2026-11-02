#pragma once

#include <Types/Constants.hpp>
#include <Types/Effect.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Type.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace shared_name_effect {
struct BadPoison;
}

struct BadPoison {
  static constexpr Status name(GameMechanics = {}) { return Status::TOX; }

  static constexpr bool isTypeImmune(GameMechanics, Type type) { return type == Type::POISON || type == Type::STEEL; }

  struct Strings {
    static constexpr std::string_view name() { return "Bad Poison"; }
    static constexpr std::string_view smogonId() { return "tox"; }
  };

  using Effect = shared_name_effect::BadPoison;
};

namespace shared_name_effect {
struct BadPoison {
  static constexpr Status name(GameMechanics = {}) { return Status::TOX; }
  using MainSource = pokesim::dex::BadPoison;

  static void onStart(Simulation& simulation);
  static void onSwitchIn(Simulation& simulation);
  static void onResidual(Simulation& simulation);

  types::badPoisonStages val = Constants::BadPoisonStages::DEFAULT;
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
