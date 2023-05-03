#include <Pokedex/Pokedex.hpp>

#include "../Moves/headers.hpp"

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Pokedex::getMoveBuild(dex::Move move))(Pokedex&) {
  // Tidy check ignored because "using namespace" is in function
  using namespace dex::build;  // NOLINT(google-build-using-namespace)

  switch (move) {
    case FuryAttack::name: return FuryAttack::build;
    case Thunderbolt::name: return Thunderbolt::build;
    case WillOWisp::name: return WillOWisp::build;
    case KnockOff::name: return KnockOff::build;
    case QuiverDance::name: return QuiverDance::build;
    case Moonblast::name: return Moonblast::build;
    default: {
      ENTT_ASSERT(false, "Building a move that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim