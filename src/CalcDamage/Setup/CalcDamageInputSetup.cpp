#include "CalcDamageInputSetup.hpp"

#include <Components/CalcDamage/Aliases.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
InputSetup::InputSetup(types::registry& registry, const Pokedex& pokedex, dex::Move move)
    : handle(registry, pokedex.buildActionMove(move, registry)) {
  handle.emplace<MoveName>(move);
}

void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  ENTT_ASSERT(
    !handle.try_get<Defenders>(),
    "Calc damage only supports one defender per move. Make a new move instead.");
  handle.emplace<Defenders>().val.push_back(entity);
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}

types::entity InputSetup::entity() const {
  return handle.entity();
}
}  // namespace pokesim::calc_damage