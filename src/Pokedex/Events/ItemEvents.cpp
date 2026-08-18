#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/ChoiceLock.hpp>
#include <Components/Current.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <cmath>
#include <entt/entity/registry.hpp>

#include "../Items/headers.hpp"

namespace pokesim::dex {
namespace {
void setChoiceLock(types::handle pokemonHandle, Battle battle) {
  CurrentActionMoveSlot moveSlot = pokemonHandle.registry()->get<CurrentActionMoveSlot>(battle.val);
  pokemonHandle.emplace<pokesim::ChoiceLock>(moveSlot.val);
}

template <typename Numerator>
void setMoveTargetModifier(
  types::registry& registry, const CurrentActionMovesAsTarget& moves, Numerator numerator,
  types::eventModifier denominator) {
  for (types::entity move : moves.val) {
    EventModifier* eventModifier = registry.try_get<EventModifier>(move);
    if (eventModifier) {
      internal::chainComponentToModifier(*eventModifier, numerator, denominator);
    }
  }
}

template <typename Numerator>
void sourceModifyDamage(
  types::registry& registry, const CurrentActionMovesAsSource& moves, Numerator numerator,
  types::eventModifier denominator) {
  for (types::entity move : moves.val) {
    DamageRollModifiers* modifier = registry.try_get<DamageRollModifiers>(move);
    if (modifier) {
      modifier->modifyDamageEvent = internal::chainValueToModifier(modifier->modifyDamageEvent, numerator, denominator);
    }
  }
}

template <typename SimulationTag>
struct FocusSashOnAfterModifyDamage {
  static void run(Simulation& simulation, types::damage hpToKeep) {
    simulation.view<modifyDamage, Tags<dex::FocusSash, SimulationTag, tags::CanUseItem>>(hpToKeep);
  }

  static void modifyDamage(
    types::handle pokemonHandle, const CurrentActionMovesAsTarget& moves, stat::CurrentHp currentHp, stat::Hp hp,
    types::damage hpToKeep) {
    if (currentHp.val != hp.val) {
      return;
    }

    types::registry& registry = *pokemonHandle.registry();
    for (types::entity move : moves.val) {
      if (!registry.all_of<pokesim::tags::CurrentMoveHit>(move)) {
        continue;
      }

      if constexpr (std::is_same_v<tags::SimulateTurn, SimulationTag>) {
        Damage& damage = registry.get<Damage>(move);
        if (damage.val < hp.val) {
          continue;
        }

        damage.val = hp.val - hpToKeep;
        pokemonHandle.emplace<internal::calc_damage::tags::RanAfterModifyDamage>();
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
    if (registry.all_of<pokesim::tags::CurrentActionMove>(move)) {
      onlyStatusMoves &= registry.all_of<move::tags::Status>(move);
    }
  }

  if (!onlyStatusMoves) {
    internal::applyDamage(pokemonHandle, hp.val / hpDivisor);
  }
}
}  // namespace

void AssaultVest::onModifySpd(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<AssaultVest::onModifySpdModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<dex::AssaultVest>>(modifier, 1U);
}

void AssaultVest::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpdStatUpdateRequired, internal::tags::EndItem, dex::AssaultVest>();
}

void BrightPowder::onModifyAccuracy(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<BrightPowder::onModifyAccuracyNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<BrightPowder::onModifyAccuracyDenominator>();

  simulation.view<setMoveTargetModifier<types::eventModifier>, Tags<dex::BrightPowder>>(numerator, denominator);
}

void ChoiceScarf::onModifySpe(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<ChoiceScarf::onModifySpeModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<dex::ChoiceScarf>>(modifier, 1U);
}

void ChoiceScarf::onSourceModifyMove(Simulation& simulation) {
  simulation.view<setChoiceLock, Tags<dex::ChoiceScarf, tags::CurrentActionSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceScarf::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpeStatUpdateRequired, internal::tags::EndItem, dex::ChoiceScarf>();
}

void ChoiceSpecs::onModifySpa(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<ChoiceSpecs::onModifySpaModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<dex::ChoiceSpecs>>(modifier, 1U);
}

void ChoiceSpecs::onSourceModifyMove(Simulation& simulation) {
  simulation.view<setChoiceLock, Tags<dex::ChoiceSpecs, tags::CurrentActionSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceSpecs::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpaStatUpdateRequired, internal::tags::EndItem, dex::ChoiceSpecs>();
}

void FocusSash::onAfterModifyDamage(Simulation& simulation) {
  const auto hpToKeep = simulation.pokedex().getStaticValue<FocusSash::onAfterModifyDamageHpToKeep>();

  simulation.addToEntities<tags::CanUseItem, tags::CurrentActionTarget, dex::FocusSash>();
  internal::checkIfCanUseItem(simulation);

  Simulation::forEachSimulationTag<FocusSashOnAfterModifyDamage>(simulation, hpToKeep);

  simulation.registry.clear<tags::CanUseItem>();
}

void FocusSash::onDamage(Simulation& simulation) {
  simulation.addToEntities<tags::CanUseItem, internal::calc_damage::tags::RanAfterModifyDamage, dex::FocusSash>();
  internal::tryUseItem(simulation);
}

void LifeOrb::onModifyDamage(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<LifeOrb::onModifyDamageNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<LifeOrb::onModifyDamageDenominator>();

  simulation.view<sourceModifyDamage<types::eventModifier>, Tags<dex::LifeOrb>>(numerator, denominator);
}

void LifeOrb::onAfterMoveUsed(Simulation& simulation) {
  const auto divisor = simulation.pokedex().getStaticValue<LifeOrb::onAfterMoveUsedHpDecreaseDivisor>();

  simulation.view<lifeOrbOnAfterMove, Tags<dex::LifeOrb>>(divisor);
}
}  // namespace pokesim::dex
