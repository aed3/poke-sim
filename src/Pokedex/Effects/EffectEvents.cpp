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
#include <Components/Tags/ItemPropertyTags.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {

void damageByHpDivisor(types::handle pokemonHandle, stat::Hp hp, types::stat hpDivisor) {
  applyDamage(pokemonHandle, hp.val / hpDivisor);
}

void applyBurnModifier(types::registry& registry, const CurrentActionMovesAsSource& moves) {
  for (types::entity move : moves.val) {
    if (registry.all_of<move::tags::Physical>(move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      registry.get<DamageRollModifiers>(move).burn = true;
    }
  }
}

void paralysisOnModifySpeed(stat::EffectiveSpe& effectiveSpe, types::stat speedDividend) {
  effectiveSpe.val = effectiveSpe.val * speedDividend / dex::latest::Paralysis::speedDivisor;
}

void paralysisOnBeforeMove(types::handle pokemonHandle, Battle battle, const CurrentActionMovesAsSource& moves) {
  types::registry& registry = *pokemonHandle.registry();
  for (types::entity move : moves.val) {
    setFailedActionMove(types::handle{registry, move}, battle);
  }
}

void choiceLockRemoveWithoutItem(types::handle pokemonHandle, const pokesim::ChoiceLock&) {
  pokemonHandle.remove<pokesim::ChoiceLock>();
}

void choiceLockRemoveWithItem(
  types::handle pokemonHandle, const ItemName& itemName, const pokesim::ChoiceLock&, const Pokedex& pokedex) {
  if (!pokedex.itemHas<item::tags::Choice>(itemName.name)) {
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
  simulation
    .viewForSelectedPokemon<applyBurnModifier, Tags<status::tags::Burn> /*, entt::exclude<ability::tags::Guts> */>();
}

void Burn::onResidual(Simulation& simulation) {
  static constexpr auto divisor = latest::Burn::onResidualHpDecreaseDivisor;
  simulation.viewForSelectedPokemon<damageByHpDivisor, Tags<status::tags::Burn>>(divisor);
}

void Paralysis::onModifySpe(Simulation& simulation) {
  constexpr auto speedDividend = dex::latest::Paralysis::speedDividend;
  simulation.viewForSelectedPokemon<
    paralysisOnModifySpeed,
    Tags<status::tags::Paralysis> /*, entt::exclude_t<ability::tags::QuickFeet>*/>(speedDividend);
}

void Paralysis::onBeforeMove(Simulation& simulation) {
  constexpr auto chance = dex::latest::Paralysis::onBeforeMoveChance;
  simulation.addToEntities<BaseEffectChance, tags::CurrentActionMoveSource, status::tags::Paralysis>(
    BaseEffectChance{chance});
  runRandomBinaryChance<BaseEffectChance, tags::CurrentActionMoveSource>(simulation, [](Simulation& sim) {
    sim.viewForSelectedPokemon<paralysisOnBeforeMove, Tags<tags::RandomEventCheckPassed>>();
  });
  simulation.removeFromEntities<BaseEffectChance, tags::CurrentActionMoveSource, status::tags::Paralysis>();
}

void ChoiceLock::onBeforeMove(Simulation& simulation) {
  simulation.viewForSelectedPokemon<choiceLockRemoveWithoutItem, Tags<>, entt::exclude_t<ItemName>>();
  simulation.viewForSelectedPokemon<choiceLockRemoveWithItem>(simulation.pokedex());
}

void ChoiceLock::onDisableMove(Simulation& simulation) {
  simulation.viewForSelectedPokemon<choiceLockRemoveWithoutItem, Tags<>, entt::exclude_t<ItemName>>();
  simulation.viewForSelectedPokemon<choiceLockRemoveWithItem>(simulation.pokedex());
  simulation.viewForSelectedPokemon<choiceLockOnDisableMove>();
}
}  // namespace pokesim::dex::events
