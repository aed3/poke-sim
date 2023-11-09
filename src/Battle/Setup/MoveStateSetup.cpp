#include "MoveStateSetup.hpp"

#include <Components/Names/MoveNames.hpp>
#include <Components/PP.hpp>
#include <entt/entity/entity.hpp>

namespace pokesim {
void MoveStateSetup::initBlank() {
  handle.emplace<MoveName>();
  handle.emplace<PP>();
  handle.emplace<MaxPP>();
}

void MoveStateSetup::setName(dex::Move moveName) {
  handle.emplace<MoveName>(moveName);
}

void MoveStateSetup::setPP(std::uint8_t pp) {
  handle.emplace<PP>(pp);
}

void MoveStateSetup::setMaxPP(std::uint8_t maxPP) {
  handle.emplace<MaxPP>(maxPP);
}
}  // namespace pokesim