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
  types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, dex::Move move) {
  types::registry& registry = *handle.registry();

  setupActionMoveBuild(registry, battleEntity, sourceEntity, targetEntity, entity(), move);

  handle.emplace<MoveName>(move);
  handle.emplace<pokesim::tags::CurrentMoveHit>();
  handle.emplace<pokesim::tags::CalculateDamage>();
  registry.emplace_or_replace<tags::Attacker>(sourceEntity);
  registry.emplace_or_replace<tags::Defender>(targetEntity);
}
}  // namespace pokesim::calc_damage
