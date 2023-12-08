#include "CalcDamageInputSetup.hpp"

#include <Components/CalcDamage/AttackerDefender.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Types/Enums/Move.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
void InputSetup::setAttacker(entt::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(entt::entity entity) {
  handle.emplace<Defender>(entity);
}

void InputSetup::setMove(dex::Move move) {
  handle.emplace<MoveName>(move);
}

void InputSetup::setBattle(entt::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim::calc_damage