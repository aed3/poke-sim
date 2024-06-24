#include "CalcDamageInputSetup.hpp"

#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CalcDamageTarget.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
InputSetup::InputSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {
  handle.emplace<tags::CalcDamageTarget>();
}

void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  handle.emplace<Defenders>().val.push_back(entity);
}

void InputSetup::setMove(dex::Move move, const Pokedex& pokedex) {
  types::entity moveEntity = pokedex.buildActionMove(move, *handle.registry());
  handle.emplace<Move>(moveEntity);
  handle.registry()->emplace<tags::Move>(moveEntity);
  handle.registry()->emplace<MoveName>(moveEntity, move);
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim::calc_damage