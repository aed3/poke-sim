#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Pokemon/PokemonProperties.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Pokedex/Effects/headers.hpp>
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
  types::handle targetHandle, CurrentActionMovesAsTarget moves, Battle battle, types::percentChance chanceOfStatic,
  const Simulation& simulation) {
  types::registry& registry = *targetHandle.registry();
  types::entity move = moves.val;

  if (!registry.all_of<pokesim::tags::CurrentMoveHit>(move)) {
    return;
  }
  types::entity source = registry.get<CurrentActionSource>(move).val;

  if (!internal::doesMoveMakeContact(registry, move, source)) {
    return;
  }

  pokesim::internal::setRandomBinaryChanceFromPercentChance({registry, move}, battle, simulation, chanceOfStatic);

  types::entity effectSource = targetHandle.entity();
  types::entity effectTarget = source;
  registry.emplace_or_replace<Paralysis>(move);
  registry.emplace<CurrentEffectSource>(move, effectSource);
  registry.emplace<CurrentEffectsAsSource>(effectSource, decltype(CurrentEffectsAsSource::val){move});
  registry.emplace<CurrentEffectTarget>(move, effectTarget);
  registry.emplace<CurrentEffectsAsTarget>(effectTarget, move);
}
}  // namespace

void Plus::onModifySpA(Simulation& simulation) {
  if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
    return;
  }
  simulation.view<plusOnModifySpa, Tags<Plus>>();
}

void Static::onDamagingHit(Simulation& simulation) {
  const auto chanceOfStatic = simulation.pokedex().getStaticValue<Static::onDamagingHitChance>();

  simulation.view<staticOnDamagingHit, Tags<Static>>(chanceOfStatic, simulation);

  internal::checkIfCanSetStatus(simulation);
  internal::removeRandomBinaryChanceComponents(simulation, entt::exclude_t<pokesim::tags::CanSetStatus>{});

  pokesim::internal::randomBinaryChance(
    simulation,
    [](Simulation& sim) {
      sim.removeFromEntities<tags::CanSetStatus, pokesim::internal::tags::RandomEventCheckFailed>();
    },
    std::nullopt);

  internal::setStatus(simulation);
  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}
}  // namespace pokesim::dex
