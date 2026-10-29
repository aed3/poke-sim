#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Helpers/InternalHelpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Pokemon/PokemonProperties.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Components/Current.hpp>
#include <Components/Damage.hpp>
#include <Components/Effects/AddedFlinchChance.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Pokedex/Effects/ChoiceLock.hpp>
#include <Pokedex/Effects/Flinch.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Species/Ditto.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Registry.hpp>
#include <cmath>
#include <entt/entity/registry.hpp>

#include "../Items/headers.hpp"
#include "EventsHelpers.hpp"

namespace pokesim::dex {
namespace {
void setChoiceLock(types::handle pokemonHandle, Battle battle) {
  CurrentActionMoveSlot moveSlot = pokemonHandle.registry()->get<CurrentActionMoveSlot>(battle.val);
  pokemonHandle.emplace<ChoiceLock>(moveSlot.val);
}

template <typename SimulationTag>
struct FocusSashOnAfterModifyDamage {
  static void run(Simulation& simulation, types::damage hpToKeep) {
    simulation.view<modifyDamage, Tags<FocusSash, SimulationTag, tags::CanUseItem>>(hpToKeep);
  }

  static void modifyDamage(
    types::handle handle, const CurrentActionMovesAsTarget& moves, stat::CurrentHp currentHp, stat::Hp hp,
    types::damage hpToKeep) {
    if (currentHp.val != hp.val) {
      return;
    }

    types::registry& registry = *handle.registry();
    for (types::entity move : moves) {
      if constexpr (std::is_same_v<tags::SimulateTurn, SimulationTag>) {
        Damage& damage = registry.get<Damage>(move);
        if (damage.val < hp.val) {
          continue;
        }

        damage.val = hp.val - hpToKeep;
        handle.emplace<internal::calc_damage::tags::RanAfterModifyDamage>();
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

void kingsRockOnModifyMove(
  types::registry& registry, const CurrentActionMovesAsSource& moves, types::percentChance addedFlinchChance) {
  for (types::entity move : moves) {
    if (registry.any_of<move::tags::Status, Flinch, AddedFlinchChance>(move)) {
      continue;
    }

    registry.emplace<AddedFlinchChance>(move, addedFlinchChance);
  }
}

void lifeOrbOnAfterMove(
  types::handle handle, const CurrentActionMovesAsSource& moves, stat::Hp hp, types::stat hpDivisor) {
  bool onlyStatusMoves = true;
  types::registry& registry = *handle.registry();
  for (types::entity move : moves) {
    onlyStatusMoves &= registry.all_of<move::tags::Status>(move);
  }

  if (!onlyStatusMoves) {
    internal::applyDamage(handle, hp.val / hpDivisor);
  }
}

void metronomeOnTryMove(
  types::registry& registry, Battle battle, MetronomeItem::Effect& metronomeEffect, types::moveHits maxSuccessiveHits) {
  Move currentMove = registry.get<MoveName>(registry.get<CurrentAction>(battle.val).action).val;
  if (metronomeEffect.lastMove == currentMove) {
    metronomeEffect.successiveHits = std::min(maxSuccessiveHits, types::moveHits(metronomeEffect.successiveHits + 1U));
  }
  else {
    metronomeEffect.successiveHits = 0U;
  }
  metronomeEffect.lastMove = currentMove;
}

void metronomeOnMoveFail(MetronomeItem::Effect& metronomeEffect) {
  metronomeEffect.successiveHits = 0U;
}

struct MetronomeOnModifyDamage {
  static void simulateTurn(
    types::registry& registry, MetronomeItem::Effect metronomeEffect, const CurrentActionMovesAsSource& moves,
    const Pokedex& pokedex, types::eventModifier denominator) {
    types::eventModifier numerator =
      pokedex.getStaticValue<MetronomeItem::Effect::onModifyDamageModifiers>(metronomeEffect.successiveHits);
    if (numerator == denominator) {
      return;
    }

    for (types::entity move : moves) {
      if (registry.any_of<move::tags::Status>(move)) {
        continue;
      }

      internal::updateDamageEventModifier(registry, move, numerator, denominator);
    }
  }

  template <typename CurrentActionMovesAsSourceType>
  struct CalcDamage {
    static void run(
      types::registry& registry, MetronomeItem::Effect metronomeEffect, const CurrentActionMovesAsSourceType& moves,
      const Pokedex& pokedex, types::eventModifier denominator) {
      types::eventModifier numerator =
        pokedex.getStaticValue<MetronomeItem::Effect::onModifyDamageModifiers>(metronomeEffect.successiveHits);
      if (numerator == denominator) {
        return;
      }

      for (types::entity move : moves) {
        if (metronomeEffect.lastMove != registry.get<MoveName>(move).val) {
          continue;
        }

        internal::updateDamageEventModifier(registry, move, numerator, denominator);
      }
    }
  };

  template <typename CurrentActionMovesAsSourceType>
  struct CalcDamageForSuccessiveHits {
    static void run(
      types::registry& registry, MetronomeItem::Effect metronomeEffect, const CurrentActionMovesAsSourceType& moves,
      const Pokedex& pokedex, types::eventModifier denominator, types::moveHits maxSuccessiveHits) {
      for (types::entity move : moves) {
        if (metronomeEffect.lastMove != registry.get<MoveName>(move).val) {
          continue;
        }

        auto* totalDamageDoneOverUses = registry.try_get<internal::calc_damage::TotalDamageDoneOverUses>(move);
        if (totalDamageDoneOverUses) {
          metronomeEffect.successiveHits += std::min((types::moveUses)maxSuccessiveHits, totalDamageDoneOverUses->uses);
          if (metronomeEffect.successiveHits < maxSuccessiveHits) {
            registry.get_or_emplace<internal::calc_damage::tags::NextSuccessiveUseMayChangeDamage>(move);
          }
        }

        types::eventModifier numerator =
          pokedex.getStaticValue<MetronomeItem::Effect::onModifyDamageModifiers>(metronomeEffect.successiveHits);
        internal::updateDamageEventModifier(registry, move, numerator, denominator);
      }
    }
  };

  template <typename SimulationTag>
  static void runForSimulation(Simulation& simulation, types::eventModifier denominator) {
    const Pokedex& pokedex = simulation.pokedex();
    if constexpr (std::is_same_v<SimulationTag, tags::SimulateTurn>) {
      simulation.view<simulateTurn, Tags<tags::SimulateTurn>>(pokedex, denominator);
    }
    else {
      bool successiveUseEffectsInKoChanceCalculation =
        std::is_same_v<SimulationTag, tags::CalculateDamage>
          ? simulation.calculateDamageOptions.getSuccessiveUseEffectsInKoChanceCalculation()
          : simulation.analyzeEffectOptions.getSuccessiveUseEffectsInKoChanceCalculation();

      if (successiveUseEffectsInKoChanceCalculation) {
        internal::currentActionMovesAsSourceView<CalcDamageForSuccessiveHits, Tags<SimulationTag>>(
          simulation,
          pokedex,
          denominator,
          pokedex.getStaticValue<MetronomeItem::Effect::onModifyDamageMaxSuccessiveHits>());
      }
      else {
        internal::currentActionMovesAsSourceView<CalcDamage, Tags<SimulationTag>>(simulation, pokedex, denominator);
      }
    }
  }
};

void rockyHelmetOnDamagingHit(types::handle handle, CurrentActionMovesAsTarget moves, types::stat hpDivisor) {
  types::registry& registry = *handle.registry();
  types::entity move = moves.val;
  types::entity source = registry.get<CurrentActionSource>(moves.val).val;
  if (!internal::doesMoveMakeContact(registry, move, source)) {
    return;
  }

  stat::Hp hp = registry.get<stat::Hp>(source);
  internal::applyDamage({registry, source}, hp.val / hpDivisor);
}

void quickClawOnFractionalPrioritySetChance(
  types::handle handle, Battle battle, Side side, const Simulation& simulation) {
  ActionQueue& actionQueue = handle.registry()->get<ActionQueue>(battle.val);
  Slot sourceSlot = internal::pokemonEntityToSlot(*handle.registry(), side.val, handle.entity());
  const auto chance = simulation.pokedex().getStaticValue<QuickClaw::onFractionalPriorityChance>();

  for (ActionQueueItem& item : actionQueue.val) {
    if (!item.decision.isMoveDecision() || item.decision.sourceSlot() != sourceSlot) {
      continue;
    }

    // if (pokedex.moveHasAll<move::tags::Status>(item.decision.getMove()) && handle.all_of<MyceliumMight>())
    // continue;
    internal::setRandomBinaryChanceFromPercentChance(handle, battle, simulation, chance);
    return;
  }
}

void quickClawOnFractionalPriority(types::handle handle, Battle battle, Side side, const Pokedex& pokedex) {
  ActionQueue& actionQueue = handle.registry()->get<ActionQueue>(battle.val);
  Slot sourceSlot = internal::pokemonEntityToSlot(*handle.registry(), side.val, handle.entity());
  const auto fractionalPriority = pokedex.getStaticValue<QuickClaw::onFractionalPriorityPriority>();

  for (ActionQueueItem& item : actionQueue.val) {
    if (!item.decision.isMoveDecision() || item.decision.sourceSlot() != sourceSlot) {
      continue;
    }

    // if (pokedex.moveHasAll<move::tags::Status>(item.decision.getMove()) && handle.all_of<MyceliumMight>())
    // continue;
    item.fractionalPriority = fractionalPriority;
  }
}

void quickPowderOnModifySpe(
  SpeciesName species, EventModifier& eventModifier, types::effectMultiplier speedMultiplier) {
  if (species.val != Species::DITTO) {
    return;
  }

  internal::chainComponentToModifier(eventModifier, speedMultiplier);
}
}  // namespace

void AssaultVest::onModifySpd(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<onModifySpdModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<AssaultVest>>(modifier, 1U);
}

void AssaultVest::onRemove(Simulation& simulation) {
  simulation.addToEntities<tags::SpdStatUpdateRequired, internal::tags::RemoveItem, AssaultVest>();
}

void BrightPowder::onModifyAccuracy(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<onModifyAccuracyNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<onModifyAccuracyDenominator>();

  simulation.view<internal::SetMoveModifier<CurrentActionMovesAsTarget>::run, Tags<BrightPowder>>(
    numerator,
    denominator);
}

void ChoiceScarf::onModifySpe(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<onModifySpeModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<ChoiceScarf>>(modifier, 1U);
}

void ChoiceScarf::onSourceModifyMove(Simulation& simulation) {
  simulation.view<
    setChoiceLock,
    Tags<ChoiceScarf, tags::CurrentActionSource, tags::SimulateTurn>,
    entt::exclude_t<ChoiceLock>>();
}

void ChoiceScarf::onRemove(Simulation& simulation) {
  simulation.addToEntities<tags::SpeStatUpdateRequired, internal::tags::RemoveItem, ChoiceScarf>();
}

void ChoiceSpecs::onModifySpa(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<onModifySpaModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<ChoiceSpecs>>(modifier, 1U);
}

void ChoiceSpecs::onSourceModifyMove(Simulation& simulation) {
  simulation.view<
    setChoiceLock,
    Tags<ChoiceSpecs, tags::CurrentActionSource, tags::SimulateTurn>,
    entt::exclude_t<ChoiceLock>>();
}

void ChoiceSpecs::onRemove(Simulation& simulation) {
  simulation.addToEntities<tags::SpaStatUpdateRequired, internal::tags::RemoveItem, ChoiceSpecs>();
}

void FocusSash::onAfterModifyDamage(Simulation& simulation) {
  const auto hpToKeep = simulation.pokedex().getStaticValue<onAfterModifyDamageHpToKeep>();

  simulation.addToEntities<tags::CanUseItem, tags::CurrentActionTarget, FocusSash>();
  internal::checkIfCanUseItem(simulation);

  Simulation::forEachSimulationTag<FocusSashOnAfterModifyDamage>(simulation, hpToKeep);

  simulation.removeFromEntities<tags::CanUseItem>();
}

void FocusSash::onDamage(Simulation& simulation) {
  simulation.addToEntities<tags::CanUseItem, internal::calc_damage::tags::RanAfterModifyDamage, FocusSash>();
  internal::tryUseItem(simulation);
}

void KingsRock::onModifyMove(Simulation& simulation) {
  const auto percentChance = simulation.pokedex().getStaticValue<addedFlinchChance>();

  simulation.view<kingsRockOnModifyMove, Tags<KingsRock>>(percentChance);
}

void LifeOrb::onModifyDamage(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<onModifyDamageNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<onModifyDamageDenominator>();

  internal::currentActionMovesAsSourceView<internal::ModifyDamage, Tags<LifeOrb>>(simulation, numerator, denominator);
}

void LifeOrb::onAfterMoveUsed(Simulation& simulation) {
  const auto divisor = simulation.pokedex().getStaticValue<onAfterMoveUsedHpDecreaseDivisor>();

  simulation.view<lifeOrbOnAfterMove, Tags<LifeOrb>>(divisor);
}

void LumBerry::onAfterSetStatus(Simulation& simulation) {
  simulation.addToEntities<tags::CanEatItem, tags::HasStatus, LumBerry, CurrentEffectAsTarget>();
  // simulation.addToEntities<tags::CanEatItem, Confusion::Effect, LumBerry, CurrentEffectAsTarget>();
  internal::tryEatItem(simulation);
}

void LumBerry::onEat(Simulation& simulation) {
  simulation.view<internal::clearStatus, Tags<tags::CanEatItem, LumBerry>>();
  // simulation.removeFromEntities<Confusion::Effect, tags::CanEatItem, LumBerry>();
}

void LumBerry::onUpdate(Simulation& simulation) {
  simulation.addToEntities<tags::CanEatItem, tags::HasStatus, LumBerry, internal::tags::RunUpdate>();
  // simulation.removeFromEntities<tags::CanEatItem, Confusion::Effect, LumBerry, internal::tags::RunUpdate>();
  internal::tryEatItem(simulation);
}

void MetronomeItem::Effect::onTryMove(Simulation& simulation) {
  const auto maxSuccessiveHits = simulation.pokedex().getStaticValue<onModifyDamageMaxSuccessiveHits>();

  simulation.addToEntitiesWithExclude<MetronomeItem, MainSource, tags::CurrentActionSource>(
    entt::exclude<MetronomeItem>);
  simulation.view<metronomeOnTryMove, Tags<tags::CurrentActionSource>>(maxSuccessiveHits);
}

void MetronomeItem::Effect::onMoveFail(Simulation& simulation) {
  simulation.view<metronomeOnMoveFail, Tags<tags::FailedCurrentActionSource>>();
}

void MetronomeItem::Effect::onModifyDamage(Simulation& simulation) {
  const auto denominator = simulation.pokedex().getStaticValue<onModifyDamageDenominator>();
  MetronomeOnModifyDamage::runForSimulation<tags::SimulateTurn>(simulation, denominator);
  MetronomeOnModifyDamage::runForSimulation<tags::CalculateDamage>(simulation, denominator);
  MetronomeOnModifyDamage::runForSimulation<tags::AnalyzeEffect>(simulation, denominator);
}

void RockyHelmet::onDamagingHit(Simulation& simulation) {
  const auto divisor = simulation.pokedex().getStaticValue<onDamagingHitHpDecreaseDivisor>();

  simulation.view<rockyHelmetOnDamagingHit, Tags<RockyHelmet>>(divisor);
}

void QuickClaw::onFractionalPriority(Simulation& simulation) {
  simulation.view<quickClawOnFractionalPrioritySetChance, Tags<QuickClaw, tags::SimulateTurn, tags::ActivePokemon>>(
    simulation);

  pokesim::internal::randomBinaryChance(
    simulation,
    [](Simulation& sim) {
      sim.view<quickClawOnFractionalPriority, Tags<pokesim::internal::tags::RandomEventCheckPassed>>(sim.pokedex());
    },
    std::nullopt);
}

void QuickPowder::onModifySpe(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<onModifySpeModifier>();

  simulation.view<quickPowderOnModifySpe, Tags<QuickPowder> /*, entt::exclude_t<tags::Transformed>*/>(modifier);
}
}  // namespace pokesim::dex
