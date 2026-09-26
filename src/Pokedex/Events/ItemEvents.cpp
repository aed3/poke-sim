#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Helpers/InternalHelpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Pokemon/PokemonProperties.hpp>
#include <Components/CalcDamage/ModifyingEventRanTags.hpp>
#include <Components/Current.hpp>
#include <Components/Damage.hpp>
#include <Components/Effects/AddedFlinchChance.hpp>
#include <Components/Effects/ChoiceLock.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Species/Ditto.hpp>
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

template <typename CurrentActionMovesAsTargetType>
struct SetMoveTargetModifier {
  static void run(
    types::handle handle, const CurrentActionMovesAsTargetType& moves, types::eventModifier numerator,
    types::eventModifier denominator) {
    for (types::entity move : moves) {
      EventModifier* eventModifier = handle.registry()->try_get<EventModifier>(move);

      if (eventModifier) {
        internal::chainComponentToModifier(*eventModifier, numerator, denominator);
      }
    }
  }
};

template <typename CurrentActionMovesAsSourceType>
struct SourceModifyDamage {
  static void run(
    types::registry& registry, const CurrentActionMovesAsSourceType& moves, types::eventModifier numerator,
    types::eventModifier denominator) {
    for (types::entity move : moves) {
      if (registry.all_of<move::tags::Status>(move)) {
        continue;
      }

      DamageRollModifiers& modifier = registry.get<DamageRollModifiers>(move);
      modifier.modifyDamageEvent = internal::chainValueToModifier(modifier.modifyDamageEvent, numerator, denominator);
    }
  }
};

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
    if (registry.any_of<move::tags::Status, pokesim::tags::Flinch, AddedFlinchChance>(move)) {
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

void quickPowderOnModifySpe(
  SpeciesName species, EventModifier& eventModifier, types::effectMultiplier speedMultiplier) {
  if (species.val != Species::DITTO) {
    return;
  }

  internal::chainComponentToModifier(eventModifier, speedMultiplier);
}
}  // namespace

void AssaultVest::onModifySpd(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<AssaultVest::onModifySpdModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<AssaultVest>>(modifier, 1U);
}

void AssaultVest::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpdStatUpdateRequired, internal::tags::EndItem, AssaultVest>();
}

void BrightPowder::onModifyAccuracy(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<BrightPowder::onModifyAccuracyNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<BrightPowder::onModifyAccuracyDenominator>();

  internal::currentActionMovesAsTargetView<SetMoveTargetModifier, Tags<BrightPowder>>(
    simulation,
    numerator,
    denominator);
}

void ChoiceScarf::onModifySpe(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<ChoiceScarf::onModifySpeModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<ChoiceScarf>>(modifier, 1U);
}

void ChoiceScarf::onSourceModifyMove(Simulation& simulation) {
  simulation.view<
    setChoiceLock,
    Tags<ChoiceScarf, tags::CurrentActionSource, tags::SimulateTurn>,
    entt::exclude_t<ChoiceLock>>();
}

void ChoiceScarf::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpeStatUpdateRequired, internal::tags::EndItem, ChoiceScarf>();
}

void ChoiceSpecs::onModifySpa(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<ChoiceSpecs::onModifySpaModifier>();

  simulation.view<internal::chainComponentToModifier<types::effectMultiplier>, Tags<ChoiceSpecs>>(modifier, 1U);
}

void ChoiceSpecs::onSourceModifyMove(Simulation& simulation) {
  simulation.view<
    setChoiceLock,
    Tags<ChoiceSpecs, tags::CurrentActionSource, tags::SimulateTurn>,
    entt::exclude_t<ChoiceLock>>();
}

void ChoiceSpecs::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpaStatUpdateRequired, internal::tags::EndItem, ChoiceSpecs>();
}

void FocusSash::onAfterModifyDamage(Simulation& simulation) {
  const auto hpToKeep = simulation.pokedex().getStaticValue<FocusSash::onAfterModifyDamageHpToKeep>();

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
  const auto percentChance = simulation.pokedex().getStaticValue<KingsRock::addedFlinchChance>();

  simulation.view<kingsRockOnModifyMove, Tags<KingsRock>>(percentChance);
}

void LifeOrb::onModifyDamage(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<LifeOrb::onModifyDamageNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<LifeOrb::onModifyDamageDenominator>();

  internal::currentActionMovesAsSourceView<SourceModifyDamage, Tags<LifeOrb>>(simulation, numerator, denominator);
}

void LifeOrb::onAfterMoveUsed(Simulation& simulation) {
  const auto divisor = simulation.pokedex().getStaticValue<LifeOrb::onAfterMoveUsedHpDecreaseDivisor>();

  simulation.view<lifeOrbOnAfterMove, Tags<LifeOrb>>(divisor);
}

void RockyHelmet::onDamagingHit(Simulation& simulation) {
  const auto divisor = simulation.pokedex().getStaticValue<RockyHelmet::onDamagingHitHpDecreaseDivisor>();

  simulation.view<rockyHelmetOnDamagingHit, Tags<RockyHelmet>>(divisor);
}

void QuickPowder::onModifySpe(Simulation& simulation) {
  const auto modifier = simulation.pokedex().getStaticValue<onModifySpeModifier>();

  simulation.view<quickPowderOnModifySpe, Tags<QuickPowder> /*, entt::exclude_t<pokesim::tags::Transformed>*/>(
    modifier);
}
}  // namespace pokesim::dex
