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

namespace pokesim::internal::calc_damage {
InputSetup::InputSetup(types::registry& registry, types::entity moveEntity) : handle(registry, moveEntity) {}

void InputSetup::setup(
  types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, pokesim::dex::Move move) {
  types::registry& registry = *handle.registry();

  setupActionMoveBuild(registry, battleEntity, sourceEntity, targetEntity, entity(), move);

  handle.emplace<MoveName>(move);
  handle.emplace<pokesim::tags::CurrentMoveHit>();
  handle.emplace<pokesim::tags::CalculateDamage>();
  registry.emplace_or_replace<pokesim::calc_damage::tags::Attacker>(sourceEntity);
  registry.emplace_or_replace<pokesim::calc_damage::tags::Defender>(targetEntity);
}
}  // namespace pokesim::internal::calc_damage
