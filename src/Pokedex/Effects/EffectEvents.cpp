#include <Components/Damage.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {
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

void Paralysis::onModifySpe(Simulation& simulation) {
  constexpr auto speedDividend = dex::latest::Paralysis::speedDividend;
  simulation.viewForSelectedPokemon<
    paralysisOnModifySpeed,
    Tags<status::tags::Paralysis> /*, entt::exclude_t<ability::tags::QuickFeet>*/>(speedDividend);
}

void ChoiceLock::onDisableMove(Simulation& simulation) {
  simulation.viewForSelectedPokemon<choiceLockOnDisableMove>();
}
}  // namespace pokesim::dex::events