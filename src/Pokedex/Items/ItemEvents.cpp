#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/ChoiceLock.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <cmath>
#include <entt/entity/registry.hpp>

#include "headers.hpp"

namespace pokesim::dex::events {
namespace {
void setChoiceLock(types::handle pokemonHandle, Battle battle) {
  types::entity moveSlot = pokemonHandle.registry()->get<CurrentActionMoveSlot>(battle.val).val;
  pokemonHandle.emplace<pokesim::ChoiceLock>(moveSlot);
}

template <typename Numerator>
void setMoveTargetModifier(
  types::registry& registry, const CurrentActionMovesAsTarget& moves, Numerator numerator,
  types::eventModifier denominator) {
  for (types::entity move : moves.val) {
    EventModifier* eventModifier = registry.try_get<EventModifier>(move);
    if (eventModifier) {
      chainComponentToModifier(*eventModifier, numerator, denominator);
    }
  }
}

template <typename Numerator>
void sourceModifyDamage(
  types::registry& registry, const CurrentActionMovesAsSource& moves, Numerator numerator,
  types::eventModifier denominator) {
  for (types::entity move : moves.val) {
    DamageRollModifiers& modifier = registry.get<DamageRollModifiers>(move);
    modifier.modifyDamageEvent = chainValueToModifier(modifier.modifyDamageEvent, numerator, denominator);
  }
}

template <typename SimulationTag>
struct FocusSashOnAfterModifyDamage {
  static void run(Simulation& simulation, types::damage hpToKeep) {
    simulation.viewForSelectedPokemon<modifyDamage, Tags<item::tags::FocusSash, SimulationTag, tags::CanUseItem>>(
      hpToKeep);
  }

  static void modifyDamage(
    types::handle pokemonHandle, const CurrentActionMovesAsTarget& moves, stat::CurrentHp currentHp, stat::Hp hp,
    types::damage hpToKeep) {
    if (currentHp.val != hp.val) {
      return;
    }

    types::registry& registry = *pokemonHandle.registry();
    for (types::entity move : moves.val) {
      if constexpr (std::is_same_v<tags::SimulateTurn, SimulationTag>) {
        Damage& damage = registry.get<Damage>(move);
        if (damage.val < hp.val) {
          continue;
        }

        damage.val = hp.val - hpToKeep;
        pokemonHandle.emplace<calc_damage::tags::RanAfterModifyDamage>();
      }
      else {
        DamageRolls& damageRolls = registry.get<DamageRolls>(move);
        for (Damage& roll : damageRolls.val) {
          if (roll.val < hp.val) {
            break;
          }

          roll.val = hp.val - hpToKeep;
        }
      }
    }
  }
};

void lifeOrbOnAfterMove(
  types::handle pokemonHandle, const CurrentActionMovesAsSource& moves, stat::Hp hp, types::stat hpDivisor) {
  bool onlyStatusMoves = true;
  types::registry& registry = *pokemonHandle.registry();
  for (types::entity move : moves.val) {
    onlyStatusMoves &= registry.all_of<move::tags::Status>(move);
  }

  if (!onlyStatusMoves) {
    applyDamage(pokemonHandle, hp.val / hpDivisor);
  }
}
}  // namespace

void AssaultVest::onModifySpd(Simulation& simulation) {
  static constexpr auto modifier = latest::AssaultVest::onModifySpdModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::AssaultVest>>(
    modifier,
    1U);
}

void BrightPowder::onModifyAccuracy(Simulation& simulation) {
  static constexpr auto numerator = latest::BrightPowder::onModifyAccuracyNumerator;
  static constexpr auto denominator = latest::BrightPowder::onModifyAccuracyDenominator;
  simulation.viewForSelectedPokemon<setMoveTargetModifier<types::eventModifier>, Tags<item::tags::BrightPowder>>(
    numerator,
    denominator);
}

void ChoiceScarf::onModifySpe(Simulation& simulation) {
  static constexpr auto modifier = latest::ChoiceScarf::onModifySpeModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::ChoiceScarf>>(
    modifier,
    1U);
}

void ChoiceScarf::onSourceModifyMove(Simulation& simulation) {
  simulation
    .view<setChoiceLock, Tags<item::tags::ChoiceScarf, tags::CurrentActionMoveSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceScarf::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpeStatUpdateRequired, tags::SelectedForViewPokemon, item::tags::ChoiceScarf>();
}

void ChoiceSpecs::onModifySpa(Simulation& simulation) {
  static constexpr auto modifier = latest::ChoiceSpecs::onModifySpaModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::ChoiceSpecs>>(
    modifier,
    1U);
}

void ChoiceSpecs::onSourceModifyMove(Simulation& simulation) {
  simulation
    .view<setChoiceLock, Tags<item::tags::ChoiceScarf, tags::CurrentActionMoveSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceSpecs::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpaStatUpdateRequired, tags::SelectedForViewPokemon, item::tags::ChoiceSpecs>();
}

void FocusSash::onAfterModifyDamage(Simulation& simulation) {
  static constexpr auto hpToKeep = latest::FocusSash::onAfterModifyDamageHpToKeep;
  simulation.addToEntities<tags::CanUseItem, tags::CurrentActionMoveTarget, item::tags::FocusSash>();
  checkIfCanUseItem(simulation);

  Simulation::forEachSimulationTag<FocusSashOnAfterModifyDamage>(simulation, hpToKeep);

  simulation.registry.clear<tags::CanUseItem>();
}

void FocusSash::onDamage(Simulation& simulation) {
  simulation.addToEntities<tags::CanUseItem, calc_damage::tags::RanAfterModifyDamage, item::tags::FocusSash>();
  tryUseItem(simulation);
}

void LifeOrb::onModifyDamage(Simulation& simulation) {
  static constexpr auto numerator = latest::LifeOrb::onModifyDamageNumerator;
  static constexpr auto denominator = latest::LifeOrb::onModifyDamageDenominator;
  simulation.viewForSelectedPokemon<sourceModifyDamage<types::eventModifier>, Tags<item::tags::LifeOrb>>(
    numerator,
    denominator);
}

void LifeOrb::onAfterMoveUsed(Simulation& simulation) {
  static constexpr auto divisor = latest::LifeOrb::onAfterMoveUsedHpDecreaseDivisor;
  simulation.viewForSelectedPokemon<lifeOrbOnAfterMove, Tags<item::tags::LifeOrb>>(divisor);
}
}  // namespace pokesim::dex::events