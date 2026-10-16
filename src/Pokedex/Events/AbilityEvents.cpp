#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Helpers/InternalHelpers.hpp>
#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <Battle/Pokemon/PokemonProperties.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/EffectStack.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/EventModifier.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/Pokedex.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Decisions.hpp>
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

template <typename BoostType, typename CheckedBoostType>
void competitiveDefiantAddToEffectStack(
  types::registry& registry, Battle battle, types::entity target, CheckedBoostType checkedBoost,
  types::boost boostByIfLowered, Ability ability) {
  if (checkedBoost && checkedBoost->val < Constants::PokemonStatBoost::DEFAULT) {
    registry.get_or_emplace<EffectStack>(battle.val)
      .val.push_back({BoostType{boostByIfLowered}, ability, target, target});
  }
}

template <typename BoostType>
void competitiveDefiantOnAfterEachBoost(
  types::handle handle, Battle battle, CurrentEffectAsTarget effect, types::boost boostByIfLowered, Ability ability) {
  types::registry& registry = *handle.registry();
  types::entity target = handle.entity();
  if (internal::isAlly(registry, target, registry.get<CurrentEffectSource>(effect.val).val)) {
    return;
  }

  auto [atkBoost, defBoost, spaBoost, spdBoost, speBoost, accBoost, evaBoost] =
    registry.try_get<AtkBoost, DefBoost, SpaBoost, SpdBoost, SpeBoost, AccBoost, EvaBoost>(effect.val);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, atkBoost, boostByIfLowered, ability);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, defBoost, boostByIfLowered, ability);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, spaBoost, boostByIfLowered, ability);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, spdBoost, boostByIfLowered, ability);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, speBoost, boostByIfLowered, ability);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, accBoost, boostByIfLowered, ability);
  competitiveDefiantAddToEffectStack<BoostType>(registry, battle, target, evaBoost, boostByIfLowered, ability);
}

void imposterOnSwitchIn(types::handle handle, Side side, const Simulation& simulation) {
  types::registry& registry = *handle.registry();
  FoeSide foeSide = registry.get<FoeSide>(side.val);
  const Team& foeTeam = registry.get<Team>(foeSide.val);
  types::entity transformTarget;

  if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
    transformTarget = foeTeam.val.front();
  }
  else {
    types::teamPositionIndex position = pokesim::internal::pokemonEntityToIndex(registry, side.val, handle.entity());
    if (position >= foeTeam.val.size()) {
      return;
    }
    transformTarget = foeTeam.val[position];
  }

  if (registry.all_of<pokesim::tags::ActivePokemon>(transformTarget)) {
    internal::transformInto(registry, handle.entity(), transformTarget, simulation.pokedex());
  }
}

template <typename CurrentActionMovesAsTargetType>
struct LongReachOnModifyMove {
  static void run(types::registry& registry, const CurrentActionMovesAsTargetType& moves) {
    for (types::entity move : moves) {
      registry.remove<move::tags::Contact>(move);
    }
  }
};

void plusOnModifySpa(types::handle, EventModifier&) {}

void pranksterOnImmunity(types::registry& registry, const CurrentActionMovesAsSource& moves) {
  for (types::entity move : moves) {
    SpeciesTypes targetTypes = registry.get<SpeciesTypes>(registry.get<CurrentActionTarget>(move).val);
    if (targetTypes.has(Type::DARK)) {
      registry.emplace<pokesim::tags::FailedCurrentMoveHit>(move);
    }
  }
}

bool pranksterChangePriority(
  types::handle handle, Battle battle, Side side, types::priority priorityBoost, const Pokedex& pokedex) {
  ActionQueue& actionQueue = handle.registry()->get<ActionQueue>(battle.val);
  Slot sourceSlot = internal::pokemonEntityToSlot(*handle.registry(), side.val, handle.entity());

  for (ActionQueueItem& item : actionQueue.val) {
    if (!item.decision.isMoveDecision() || item.decision.sourceSlot() != sourceSlot) {
      continue;
    }

    Move move = item.decision.move();
    if (pokedex.moveHasAll<move::tags::Status>(move)) {
      item.priority = types::priority(item.priority + priorityBoost);
      // if (!simulation.speedSortAfterEachMove) handle.emplace<tags::PranksterOnSpeedSort>();
      return true;
    }
  }

  return false;
}

void pranksterOnModifyPriority(
  types::handle handle, Battle battle, Side side, types::priority priorityBoost, const Pokedex& pokedex) {
  pranksterChangePriority(handle, battle, side, priorityBoost, pokedex);
}

void pranksterOnRemove(
  types::handle handle, Battle battle, Side side, types::priority priorityBoost, const Pokedex& pokedex) {
  if (pranksterChangePriority(handle, battle, side, (types::boost)-priorityBoost, pokedex)) {
    handle.registry()->get_or_emplace<simulate_turn::tags::SpeedSortNeeded>(battle.val);
  }
}

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

  pokesim::internal::setRandomBinaryChanceFromPercentChance({registry, battle.val}, battle, simulation, chanceOfStatic);

  types::entity effectSource = targetHandle.entity();
  types::entity effectTarget = source;
  registry.emplace<Paralysis>(battle.val);
  registry.emplace<CurrentEffectSource>(battle.val, effectSource);
  registry.emplace<CurrentEffectsAsSource>(effectSource).val.push_back(battle.val);
  registry.emplace<CurrentEffectTarget>(battle.val, effectTarget);
  registry.emplace<CurrentEffectAsTarget>(effectTarget, battle.val);
}
}  // namespace

void Analytic::onBasePower(Simulation& simulation) {
  const auto numerator = simulation.pokedex().getStaticValue<onBasePowerNumerator>();
  const auto denominator = simulation.pokedex().getStaticValue<onBasePowerDenominator>();

  simulation.view<analyticOnBasePowerSimulateTurn, Tags<Analytic, pokesim::tags::SimulateTurn>>(numerator, denominator);
  internal::currentActionMovesAsSourceView<
    AnalyticOnBasePowerCalcDamage,
    Tags<Analytic, pokesim::tags::CalculateDamage>>(simulation, numerator, denominator);
  internal::currentActionMovesAsSourceView<AnalyticOnBasePowerCalcDamage, Tags<Analytic, pokesim::tags::AnalyzeEffect>>(
    simulation,
    numerator,
    denominator);
}

void Competitive::onAfterEachBoost(Simulation& simulation) {
  const auto spaBoost = simulation.pokedex().getStaticValue<onAfterEachBoostSpaBoost>();
  simulation.view<competitiveDefiantOnAfterEachBoost<SpaBoost>, Tags<Competitive>>(spaBoost, Ability::COMPETITIVE);
}

void Defiant::onAfterEachBoost(Simulation& simulation) {
  const auto atkBoost = simulation.pokedex().getStaticValue<onAfterEachBoostAtkBoost>();
  simulation.view<competitiveDefiantOnAfterEachBoost<AtkBoost>, Tags<Defiant>>(atkBoost, Ability::DEFIANT);
}

void Imposter::onSwitchIn(Simulation& simulation) {
  simulation.view<imposterOnSwitchIn, Tags<Imposter, tags::CurrentActionTarget>>(simulation);
  internal::runStartAbilityEvent(simulation);
  simulation.removeFromEntities<internal::tags::StartAbility>();
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

void Prankster::onModifyPriority(Simulation& simulation) {
  const auto priorityBoost = simulation.pokedex().getStaticValue<onModifyPriorityIncrease>();

  simulation
    .view<pranksterOnModifyPriority, Tags<Prankster, pokesim::tags::SimulateTurn, pokesim::tags::ActivePokemon>>(
      priorityBoost,
      simulation.pokedex());
}

void Prankster::onImmunity(Simulation& simulation) {
  simulation.view<pranksterOnImmunity, Tags<Prankster>>();
  // if (!simulation.speedSortAfterEachMove) simulation.view<pranksterOnImmunity, Tags<tags::PranksterOnSpeedSort>>();
}

void Prankster::onRemove(Simulation& simulation) {
  // if (!simulation.speedSortAfterEachMove) return;
  const auto priorityBoost = simulation.pokedex().getStaticValue<onModifyPriorityIncrease>();

  simulation.view<pranksterOnRemove, Tags<Prankster, pokesim::internal::tags::RemoveAbility>>(
    priorityBoost,
    simulation.pokedex());
}

void Scrappy::onModifyMove(Simulation& simulation) {
  internal::currentActionMovesAsSourceView<ScrappyOnModifyMove, Tags<Scrappy>>(simulation);
}

void Static::onDamagingHit(Simulation& simulation) {
  const auto chanceOfStatic = simulation.pokedex().getStaticValue<onDamagingHitChance>();

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
  internal::clearCurrentEffect(simulation);
  simulation.removeFromEntities<Paralysis, pokesim::tags::Battle>();
}
}  // namespace pokesim::dex
