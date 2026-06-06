#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/ItemPropertyTags.hpp>
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
  applyDamage(pokemonHandle, hp.val / hpDivisor);
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

void choiceLockRemoveWithoutItem(types::handle pokemonHandle, const pokesim::ChoiceLock&) {
  pokemonHandle.remove<pokesim::ChoiceLock>();
}

void choiceLockRemoveWithItem(
  types::handle pokemonHandle, const ItemName& itemName, const pokesim::ChoiceLock&, const Pokedex& pokedex) {
  if (!pokedex.itemHas<item::tags::Choice>(itemName.val)) {
    pokemonHandle.remove<pokesim::ChoiceLock>();
  }
}

void choiceLockOnDisableMove(
  types::registry& registry, const pokesim::ChoiceLock& choiceLocked, const MoveSlots& moveSlots) {
  POKESIM_REQUIRE(
    std::find(moveSlots.val.begin(), moveSlots.val.end(), choiceLocked.val) != moveSlots.val.end(),
    "Should skip if the move is no longer present, but when does that happen?");

  for (types::entity entity : moveSlots.val) {
    if (entity != choiceLocked.val) {
      registry.emplace<move::tags::Disabled>(entity);
    }
  }
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
  simulation.view<setFailedActionMove, Tags<pokesim::tags::FailedCurrentMoveHit>>();
  simulation.registry.clear<tags::FailedCurrentMoveHit>();
}

void ChoiceLock::onBeforeMove(Simulation& simulation) {
  simulation.view<choiceLockRemoveWithoutItem, Tags<>, entt::exclude_t<ItemName>>();
  simulation.view<choiceLockRemoveWithItem>(simulation.pokedex());
}

void ChoiceLock::onDisableMove(Simulation& simulation) {
  internal::EntityFilter<tags::DisableMove> filter{simulation};
  if (filter.hasNoneSelected()) {
    return;
  }

  filter.view<choiceLockRemoveWithoutItem, Tags<>, entt::exclude_t<ItemName>>();
  filter.view<choiceLockRemoveWithItem>(simulation.pokedex());
  filter.view<choiceLockOnDisableMove>();
}
}  // namespace pokesim::dex
