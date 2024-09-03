#include "CalcDamageInputSetup.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
InputSetup::InputSetup(
  types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
  dex::Move move, const Pokedex& pokedex)
    : handle(registry, createActionMoveForTarget({registry, targetEntity}, battleEntity, sourceEntity, move, pokedex)) {
  handle.emplace<MoveName>(move);
  handle.emplace<pokesim::tags::CalculateDamage>();
  registry.emplace_or_replace<tags::Attacker>(sourceEntity);
  registry.emplace_or_replace<tags::Defender>(targetEntity);
}

types::entity InputSetup::entity() const {
  return handle.entity();
}
}  // namespace pokesim::calc_damage