#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "../Abilities/headers.hpp"

namespace pokesim::dex {
namespace {
void plusOnModifySpa(types::handle, EventModifier&) {}

void staticOnDamagingHit(
  types::handle targetHandle, const CurrentActionMovesAsTarget& moves, Battle battle,
  types::percentChance chanceOfStatic, const Simulation& simulation) {
  types::registry& registry = *targetHandle.registry();
  for (types::entity move : moves.val) {
    if (!registry.all_of<pokesim::tags::CurrentMoveHit>(move)) {
      continue;
    }
    if (!registry.all_of<move::tags::Contact>(move)) {
      continue;
    }

    types::entity source = registry.get<CurrentActionSource>(move).val;
    /*
    if (registry.all_of<item::tags::ProtectivePads>(source)) {
      continue;
    }
    */

    pokesim::internal::setRandomBinaryChanceFromPercentChance({registry, move}, battle, simulation, chanceOfStatic);

    types::entity effectSource = targetHandle.entity();
    types::entity effectTarget = source;
    registry.emplace_or_replace<status::tags::Paralysis>(move);
    registry.emplace<CurrentEffectSource>(move, effectSource);
    registry.emplace<CurrentEffectsAsSource>(effectSource, types::entityVector{move});
    registry.emplace<CurrentEffectTarget>(move, effectTarget);
    registry.emplace<CurrentEffectsAsTarget>(effectTarget, types::entityVector{move});
  }
}
}  // namespace

void Plus::onModifySpA(Simulation& simulation) {
  if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
    return;
  }
  simulation.view<plusOnModifySpa, Tags<ability::tags::Plus>>();
}

void Static::onDamagingHit(Simulation& simulation) {
  const auto chanceOfStatic = simulation.pokedex().getStaticValue<dex::Static::onDamagingHitChance>();

  simulation.view<staticOnDamagingHit, Tags<ability::tags::Static>>(chanceOfStatic, simulation);

  // TODO(aed3): This is now inefficient since the random chance will happen for move sources that cannot have their
  // status changed.
  checkIfCanSetStatus(simulation);
  pokesim::internal::randomBinaryChance(
    simulation,
    [](Simulation& sim) {
      sim.removeFromEntities<tags::CanSetStatus, pokesim::internal::tags::RandomEventCheckFailed>();
    },
    std::nullopt);

  setStatus(simulation);
  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}
}  // namespace pokesim::dex
