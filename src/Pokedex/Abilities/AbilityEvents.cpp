#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {
void plusOnModifySpa(types::handle, EventModifier&) {}

void staticOnDamagingHit(
  types::handle targetHandle, const CurrentActionMovesAsTarget& moves, types::percentChance chanceOfStatic) {
  types::registry& registry = *targetHandle.registry();
  for (types::entity move : moves.val) {
    if (!registry.all_of<move::tags::Contact>(move)) {
      continue;
    }
    types::entity source = registry.get<CurrentActionSource>(move).val;
    /*
    if (registry.all_of<item::tags::ProtectivePads>(source)) {
      continue;
    }
    */

    types::entity effectSource = targetHandle.entity();
    types::entity effectTarget = source;
    registry.emplace<BaseEffectChance>(move, chanceOfStatic);
    registry.emplace_or_replace<status::tags::Paralysis>(move);
    registry.emplace<CurrentEffectSource>(move, effectSource);
    registry.emplace<CurrentEffectsAsSource>(effectSource, types::entityVector{move});
    registry.emplace<CurrentEffectTarget>(move, effectTarget);
    registry.emplace<CurrentEffectsAsTarget>(effectTarget, types::entityVector{move});
  }
}
}  // namespace

void Plus::onModifySpA(Simulation& simulation) {
  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    return;
  }
  simulation.viewForSelectedPokemon<plusOnModifySpa, Tags<ability::tags::Plus>>();
}

void Static::onDamagingHit(Simulation& simulation) {
  static constexpr auto chanceOfStatic = dex::latest::Static::onDamagingHitChance;
  simulation.viewForSelectedPokemon<staticOnDamagingHit, Tags<ability::tags::Static>>(chanceOfStatic);
  checkIfCanSetStatus(simulation);
  runRandomBinaryChance<BaseEffectChance, tags::CanSetStatus>(simulation, [](Simulation& sim) {
    sim.removeFromEntities<tags::CanSetStatus, tags::SelectedForViewMove, tags::RandomEventCheckFailed>();
  });
  setStatus(simulation);
  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}
}  // namespace pokesim::dex::events