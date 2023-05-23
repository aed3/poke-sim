#include "MoveSetup.hpp"

#include <Components/Names/Move.hpp>
#include <Components/PP.hpp>

namespace pokesim {
void MoveSetup::initBlank() {
  handle.emplace<MoveName>();
  handle.emplace<PP>();
  handle.emplace<MaxPP>();
}

void MoveSetup::setName(dex::Move moveName) {
  handle.emplace<MoveName>(moveName);
}

void MoveSetup::setPP(std::uint8_t pp) {
  handle.emplace<PP>(pp);
}

void MoveSetup::setMaxPP(std::uint8_t maxPP) {
  handle.emplace<MaxPP>(maxPP);
}
}  // namespace pokesim