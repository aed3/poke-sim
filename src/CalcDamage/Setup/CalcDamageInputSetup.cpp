#include "CalcDamageInputSetup.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
InputSetup::InputSetup(types::registry& _registry) : registry(&_registry) {}

void InputSetup::setup(
  types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, dex::Move move,
  const Pokedex& pokedex) {
  moveEntity = createActionMoveForTarget({*registry, targetEntity}, battleEntity, sourceEntity, move, pokedex);
  types::handle handle{*registry, moveEntity};

  handle.emplace<MoveName>(move);
  registry->emplace_or_replace<tags::Attacker>(sourceEntity);
  registry->emplace_or_replace<tags::Defender>(targetEntity);
}

types::entity InputSetup::entity() const {
  POKESIM_REQUIRE(moveEntity != entt::null, "Getting move entity before proper setup.");
  return moveEntity;
}
}  // namespace pokesim::calc_damage