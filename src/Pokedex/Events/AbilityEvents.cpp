#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Helpers/InternalHelpers.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Pokemon/PokemonProperties.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
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
void analyticOnBasePowerSimulateTurn(
  types::registry& registry, const CurrentActionMovesAsSource moves, Battle battle, types::eventModifier numerator,
  types::eventModifier denominator) {
  const ActionQueue& actionQueue = registry.get<ActionQueue>(battle.val);

  bool boosted = true;
  for (const ActionQueueItem& item : actionQueue.val) {
    if (item.order != ActionOrder::MOVE) {
      continue;
    }

    types::entity upcomingAttacker =
      slotToPokemonEntity(registry, registry.get<Sides>(battle.val), item.decision.sourceSlot());
    if (registry.all_of<pokesim::tags::ActivePokemon>(upcomingAttacker)) {
      boosted = false;
      break;
    }
  }

  if (boosted) {
    for (types::entity move : moves) {
      EventModifier* eventModifier = registry.try_get<EventModifier>(move);
      if (eventModifier) {
        internal::chainComponentToModifier(*eventModifier, numerator, denominator);
      }
    }
  }
}

template <typename CurrentActionMovesAsSourceType>
struct AnalyticOnBasePowerCalcDamage {
  static void run(
    types::handle handle, const CurrentActionMovesAsSourceType& moves, types::eventModifier numerator,
    types::eventModifier denominator) {
    types::registry& registry = *handle.registry();
    for (types::entity move : moves) {
      auto [eventModifier, defender] = registry.get<EventModifier, calc_damage::Defender>(move);

      if (handle.get<stat::EffectiveSpe>().val <= registry.get<stat::EffectiveSpe>(defender.val).val) {
        internal::chainComponentToModifier(eventModifier, numerator, denominator);
      }
    }
  }
};

template <typename CurrentActionMovesAsTargetType>
struct LongReachOnModifyMove {
  static void run(types::registry& registry, const CurrentActionMovesAsTargetType& moves) {
    for (types::entity move : moves) {
      registry.remove<move::tags::Contact>(move);
    }
  }
};

void plusOnModifySpa(types::handle, EventModifier&) {}

template <typename CurrentActionMovesAsTargetType>
struct ScrappyOnModifyMove {
  static void run(types::registry& registry, const CurrentActionMovesAsTargetType& moves) {
    for (types::entity move : moves) {
      TypeName typeName = registry.get<TypeName>(move);
      if (typeName.val == Type::NORMAL || typeName.val == Type::FIGHTING) {
        registry.emplace<move::tags::IgnoreImmunities>(move);
      }
    }
  }
};

void staticOnDamagingHit(
  types::handle targetHandle, CurrentActionMovesAsTarget moves, Battle battle, types::percentChance chanceOfStatic,
  const Simulation& simulation) {
  types::registry& registry = *targetHandle.registry();
  types::entity move = moves.val;

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

void Analytic::onBasePower(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<Analytic::onBasePowerNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<Analytic::onBasePowerDenominator>();

  simulation.view<analyticOnBasePowerSimulateTurn, Tags<Analytic, pokesim::tags::SimulateTurn>>(numerator, denominator);
  internal::currentActionMovesAsSourceView<
    AnalyticOnBasePowerCalcDamage,
    Tags<Analytic, pokesim::tags::CalculateDamage>>(simulation, numerator, denominator);
  internal::currentActionMovesAsSourceView<AnalyticOnBasePowerCalcDamage, Tags<Analytic, pokesim::tags::AnalyzeEffect>>(
    simulation,
    numerator,
    denominator);
}

void LongReach::onModifyMove(Simulation& simulation) {
  internal::currentActionMovesAsSourceView<LongReachOnModifyMove, Tags<LongReach>>(simulation);
}

void Plus::onModifySpA(Simulation& simulation) {
  if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
    return;
  }
  simulation.view<plusOnModifySpa, Tags<Plus>>();
}

void Scrappy::onModifyMove(Simulation& simulation) {
  internal::currentActionMovesAsSourceView<ScrappyOnModifyMove, Tags<Scrappy>>(simulation);
}

void Static::onDamagingHit(Simulation& simulation) {
  const auto chanceOfStatic = simulation.pokedex().getStaticValue<Static::onDamagingHitChance>();

  simulation.view<staticOnDamagingHit, Tags<Static>>(chanceOfStatic, simulation);

  internal::checkIfCanSetStatus(simulation);
  internal::removeRandomBinaryChanceComponents(simulation, entt::exclude<pokesim::tags::CanSetStatus>);

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
