#include "CalcDamageInputSetup.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
InputSetup::InputSetup(types::registry& registry, types::entity moveEntity) : handle(registry, moveEntity) {}

void InputSetup::setup(
  types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, dex::Move move,
  const Pokedex& pokedex) {
  types::registry& registry = *handle.registry();

  createActionMoveForTarget({registry, targetEntity}, battleEntity, sourceEntity, move, pokedex, entity());

  handle.emplace<MoveName>(move);
  handle.emplace<pokesim::tags::CurrentMoveHit>();
  registry.emplace_or_replace<tags::Attacker>(sourceEntity);
  registry.emplace_or_replace<tags::Defender>(targetEntity);
}
}  // namespace pokesim::calc_damage
