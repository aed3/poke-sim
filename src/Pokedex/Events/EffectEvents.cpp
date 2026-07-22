#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/ChoiceLock.hpp>
#include <Components/Damage.hpp>
#include <Components/DisabledMoveSlots.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/ItemPropertyTags.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Utilities/EntityFilter.hpp>
#include <entt/entity/registry.hpp>

#include "../Effects/headers.hpp"

namespace pokesim::dex {
namespace {
void damageByHpDivisor(types::handle pokemonHandle, stat::Hp hp, types::stat hpDivisor) {
  internal::applyDamage(pokemonHandle, hp.val / hpDivisor);
}

void applyBurnModifier(types::registry& registry, const CurrentActionMovesAsSource& moves) {
  for (types::entity move : moves.val) {
    if (registry.all_of<move::tags::Physical, tags::CurrentMoveHit>(
          move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      registry.get<DamageRollModifiers>(move).burn = true;
    }
  }
}

void paralysisOnModifySpeed(stat::EffectiveSpe& effectiveSpe, types::stat speedDivisor, types::stat speedDividend) {
  effectiveSpe.val = effectiveSpe.val * speedDividend / speedDivisor;
}

void paralysisOnBeforeMove(types::registry& registry, const CurrentActionMovesAsSource& moves) {
  for (types::entity move : moves.val) {
    registry.emplace<pokesim::tags::FailedCurrentMoveHit>(move);
  }
}

template <typename... Tags>
void choiceLockRemove(Simulation& simulation) {
  simulation.removeFromEntities<pokesim::ChoiceLock, Tags...>(entt::exclude<item::tags::Choice>);
}

void choiceLockOnDisableMove(
  types::handle handle, const pokesim::ChoiceLock& choiceLocked, const MoveSlots& moveSlots) {
  if (!handle.all_of<DisabledMoveSlots>()) {
    handle.emplace<DisabledMoveSlots>(types::moveSlots<bool>(moveSlots.val.size(), false));
  }

  handle.get<DisabledMoveSlots>().val[choiceLocked.val] = true;
}
}  // namespace

void Burn::onSetDamageRollModifiers(Simulation& simulation) {
  simulation.view<applyBurnModifier, Tags<status::tags::Burn> /*, entt::exclude<ability::tags::Guts> */>();
}

void Burn::onResidual(Simulation& simulation) {
  const auto divisor = simulation.pokedex().getStaticValue<Burn::onResidualHpDecreaseDivisor>();

  simulation.view<damageByHpDivisor, Tags<status::tags::Burn, tags::ActivePokemon>>(divisor);
}

void Paralysis::onModifySpe(Simulation& simulation) {
  const auto speedDivisor = simulation.pokedex().getStaticValue<Paralysis::speedDivisor>();
  const auto speedDividend = simulation.pokedex().getStaticValue<Paralysis::speedDividend>();

  simulation.view<
    paralysisOnModifySpeed,
    Tags<status::tags::Paralysis, tags::SpeStatUpdateRequired> /*, entt::exclude_t<ability::tags::QuickFeet>*/>(
    speedDivisor,
    speedDividend);
}

void Paralysis::onBeforeMove(Simulation& simulation) {
  const auto chance = simulation.pokedex().getStaticValue<Paralysis::onBeforeMoveChance>();

  simulation.view<
    pokesim::internal::setRandomBinaryChanceFromPercentChance,
    Tags<tags::CurrentActionMoveSource, status::tags::Paralysis>>(simulation, chance);

  pokesim::internal::randomBinaryChance(
    simulation,
    [](Simulation& sim) { sim.view<paralysisOnBeforeMove, Tags<pokesim::internal::tags::RandomEventCheckPassed>>(); },
    std::nullopt);
  simulation.view<internal::setFailedActionMove, Tags<pokesim::tags::FailedCurrentMoveHit>>();
  simulation.registry.clear<pokesim::tags::FailedCurrentMoveHit>();
}

void ChoiceLock::onBeforeMove(Simulation& simulation) {
  choiceLockRemove(simulation);
}

void ChoiceLock::onDisableMove(Simulation& simulation) {
  pokesim::internal::EntityFilter<internal::tags::DisableMove> filter{simulation};
  if (filter.hasNoneSelected()) {
    return;
  }

  choiceLockRemove<internal::tags::DisableMove>(simulation);
  filter.view<choiceLockOnDisableMove>();
}
}  // namespace pokesim::dex
