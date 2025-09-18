#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::stat {
struct EffectiveSpe;
}  // namespace pokesim::stat

namespace pokesim::dex {
namespace internal {
struct ParalysisEvents {
  static void onModifySpe(stat::EffectiveSpe& effectiveSpe);
};
}  // namespace internal

template <GameMechanics>
struct Paralysis : internal::ParalysisEvents {
  static constexpr dex::Status name = dex::Status::PAR;

  static constexpr types::stat onModifySpaModifier = 2U;
  struct Strings {
    static constexpr std::string_view name = "Paralysis";
    static constexpr std::string_view smogonId = "par";
  };
};

namespace latest {
using Paralysis = dex::Paralysis<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex