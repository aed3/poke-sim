#include "MoveStateSetup.hpp"

#include <Components/Names/MoveNames.hpp>
#include <Components/PP.hpp>
#include <Types/Move.hpp>

namespace pokesim {
void MoveStateSetup::initBlank() {
  handle.emplace<MoveName>();
  handle.emplace<Pp>();
  handle.emplace<MaxPp>();
}

void MoveStateSetup::setName(dex::Move moveName) {
  handle.emplace<MoveName>(moveName);
}

void MoveStateSetup::setPP(types::Pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveStateSetup::setMaxPP(types::Pp maxPp) {
  handle.emplace<MaxPp>(maxPp);
}
}  // namespace pokesim