#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct Paralysis {
  static void onModifySpe(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct Paralysis : events::Paralysis {
  static constexpr dex::Status name = dex::Status::PAR;

  static constexpr types::stat speedDividend = 50U;
  static constexpr types::stat speedDivisor = 100U;
  struct Strings {
    static constexpr std::string_view name = "Paralysis";
    static constexpr std::string_view smogonId = "par";
  };
};

namespace latest {
using Paralysis = dex::Paralysis<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex