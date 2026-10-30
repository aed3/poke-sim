#include "ManagePokemonState.hpp"

#include <Battle/ManageBattleState.hpp>
#include <Components/Boosts.hpp>
#include <Components/Current.hpp>
#include <Components/Damage.hpp>
#include <Components/DisabledMoveSlots.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/FaintQueue.hpp>
#include <Components/LastUsedMove.hpp>
#include <Components/Level.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/AbilityNames.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/NatureNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Names/StatNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Pokedex/BaseStats.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/ItemPropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Components/TransformedFrom.hpp>
#include <Pokedex/Effects/ChoiceLock.hpp>
#include <Pokedex/Effects/Trapped.hpp>
#include <Pokedex/Effects/Trapper.hpp>
#include <Pokedex/EnumToTag/AbilityEnumToTag.hpp>
#include <Pokedex/EnumToTag/ItemEnumToTag.hpp>
#include <Pokedex/EnumToTag/StatusEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Simulation/Formulas.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/EntityFilter.hpp>
#include <cmath>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

#include "ManagePokemonTags.hpp"

namespace pokesim::internal {
namespace {
template <typename ItemTag, typename SelectionTag>
struct RemoveItem {
  template <typename = ItemTag, typename = void>
  struct hasEffect : std::false_type {};
  template <typename T>
  struct hasEffect<T, std::void_t<decltype(T::Effect::name)>> : std::true_type {};

  static void run(Simulation& simulation) {
    simulation.removeFromEntities<ItemTag, SelectionTag>();
    if constexpr (hasEffect<ItemTag>::value) {
      simulation.removeFromEntities<typename ItemTag::Effect, SelectionTag>();
    }
  }
};

template <typename Tag>
struct RemoveIfHas {
  static bool run(types::handle handle) {
    if (handle.all_of<Tag>()) {
      handle.remove<Tag>();
      return true;
    }
    return false;
  }
};

template <typename SelectionTag>
void removeItemComponents(Simulation& simulation) {
  dex::forEachItem<RemoveItem, SelectionTag>(simulation.pokedex(), simulation);
  simulation.view<checkIfRemoveFromActiveWithUpdateEvents, Tags<SelectionTag>>();
  auto view = simulation.registry.view<SelectionTag>();
  simulation.registry.remove<pokesim::tags::HasItem, item::tags::Choice, item::tags::Berry>(view.begin(), view.end());
  simulation.removeFromEntities<SelectionTag>();
}

void resetEffectiveAtk(stat::Atk atk, stat::EffectiveAtk& effectiveAtk) {
  effectiveAtk.val = atk.val;
}

void resetEffectiveDef(stat::Def def, stat::EffectiveDef& effectiveDef) {
  effectiveDef.val = def.val;
}

void resetEffectiveSpa(stat::Spa spa, stat::EffectiveSpa& effectiveSpa) {
  effectiveSpa.val = spa.val;
}

void resetEffectiveSpd(stat::Spd spd, stat::EffectiveSpd& effectiveSpd) {
  effectiveSpd.val = spd.val;
}

void resetEffectiveSpe(stat::Spe spe, stat::EffectiveSpe& effectiveSpe) {
  effectiveSpe.val = spe.val;
}

template <typename EffectiveStat, typename BoostType>
void applyBoostToEffectiveStat(EffectiveStat& effectiveStat, BoostType boost) {
  internal::applyStatBoost(effectiveStat.val, boost.val);
}

template <typename BoostType, typename StatUpdateRequired>
void boostStat(types::registry& registry, CurrentEffectTarget target, BoostType& boost) {
  BoostType& currentBoost = registry.get_or_emplace<BoostType>(target.val);
  currentBoost.val += boost.val;

  if constexpr (!std::is_same_v<StatUpdateRequired, void>) {
    if (boost.val) {
      registry.get_or_emplace<StatUpdateRequired>(target.val);
    }
  }
  if (currentBoost.val == Constants::PokemonStatBoost::DEFAULT) {
    registry.remove<BoostType>(target.val);
  }
}

template <typename BoostType>
void clampBoost(types::registry& registry, CurrentEffectTarget target, BoostType& boost) {
  BoostType* currentBoost = registry.try_get<BoostType>(target.val);
  if (!currentBoost) {
    return;
  }

  using BoostLimits = Constants::PokemonStatBoost;

  types::boost combinedBoost = currentBoost->val + boost.val;
  if (combinedBoost > BoostLimits::MAX) {
    boost.val = BoostLimits::MAX - currentBoost->val;
  }
  else if (combinedBoost < BoostLimits::MIN) {
    boost.val = BoostLimits::MIN - currentBoost->val;
  }
}

template <typename StatusType>
struct CheckIfStatusIsSettable {
  static void run(Simulation& simulation) {
    simulation.addToEntities<pokesim::tags::EffectCanSetStatus, StatusType, CurrentEffectSource, CurrentEffectTarget>();
    simulation.view<checkIfTargetHasStatus, Tags<StatusType>>();
    simulation.view<checkTypeStatusImmunity, Tags<StatusType>>(simulation.pokedex());
    runStatusImmunityEvent<StatusType>(simulation);
  }

  static void checkIfTargetHasStatus(types::handle handle, CurrentEffectTarget target) {
    if (handle.registry()->all_of<pokesim::tags::HasStatus>(target.val)) {
      handle.remove<pokesim::tags::EffectCanSetStatus>();
    }
  }

  static void checkTypeStatusImmunity(types::handle handle, CurrentEffectTarget target, const Pokedex& pokedex) {
    SpeciesTypes types = handle.registry()->get<SpeciesTypes>(target.val);
    for (dex::Type type : types.val) {
      if (pokedex.getStaticValue<StatusType::isTypeImmune>(type)) {
        handle.remove<pokesim::tags::EffectCanSetStatus>();
        return;
      }
    }
  }
};

template <typename StatusType>
struct RemoveNotSettableStatus {
  static void run(Simulation& simulation) {
    simulation.removeFromEntities<StatusType, CurrentEffectSource, CurrentEffectTarget>(
      entt::exclude<pokesim::tags::EffectCanSetStatus>);
  }
};

template <typename StatusType>
struct SetEffectTargetStatus {
  static constexpr dex::Status status = StatusType::name();
  static void run(Simulation& simulation) { simulation.view<setEffectTargetStatus, Tags<StatusType>>(); }

  static void setEffectTargetStatus(types::registry& registry, CurrentEffectTarget target) {
    setStatus(registry, status, target.val);

    if constexpr (status == dex::Status::PAR) {
      registry.emplace<pokesim::tags::SpeStatUpdateRequired>(target.val);
    }
  }
};

bool areTypesTrappedImmune(SpeciesTypes types, const Pokedex& pokedex) {
  for (dex::Type type : types.val) {
    if (pokedex.getStaticValue<dex::Trapped::isTypeImmune>(type)) {
      return true;
    }
  }

  return false;
}

void revertTransform(
  types::handle handle, SpeciesName& species, MoveSlots& moves, SpeciesTypes& types, stat::Atk& atk, stat::Def& def,
  stat::Spa& spa, stat::Spd& spd, stat::Spe& spe, Level level, Evs evs, Ivs ivs, const TransformedFrom& transformedFrom,
  const Pokedex& pokedex) {
  species.val = transformedFrom.species;
  moves.val = transformedFrom.moves;

  pokedex.forEachLoadedAbility([&handle](dex::Ability ability) { dex::enumToTag<RemoveIfHas>(ability, handle); });
  dex::emplaceTagFromEnum(transformedFrom.ability, handle);

  types = pokedex.getSpeciesData<SpeciesTypes>(species.val);
  BaseStats baseStats = pokedex.getSpeciesData<BaseStats>(species.val);
  dex::Nature nature = handle.all_of<NatureName>() ? handle.get<NatureName>().val : dex::Nature::NO_NATURE;

  atk.val = computeStatFromBaseStat(dex::Stat::ATK, baseStats.atk, level.val, nature, evs.atk, ivs.atk);
  def.val = computeStatFromBaseStat(dex::Stat::DEF, baseStats.def, level.val, nature, evs.def, ivs.def);
  spa.val = computeStatFromBaseStat(dex::Stat::SPA, baseStats.spa, level.val, nature, evs.spa, ivs.spa);
  spd.val = computeStatFromBaseStat(dex::Stat::SPD, baseStats.spd, level.val, nature, evs.spd, ivs.spd);
  spe.val = computeStatFromBaseStat(dex::Stat::SPE, baseStats.spe, level.val, nature, evs.spe, ivs.spe);

  handle.emplace<pokesim::tags::AtkStatUpdateRequired>();
  handle.emplace<pokesim::tags::DefStatUpdateRequired>();
  handle.emplace<pokesim::tags::SpaStatUpdateRequired>();
  handle.emplace<pokesim::tags::SpdStatUpdateRequired>();
  handle.emplace<pokesim::tags::SpeStatUpdateRequired>();
}

void trapAndSetTrapper(
  types::registry& registry, CurrentEffectSource source, CurrentEffectTarget target, const Pokedex& pokedex) {
  if (areTypesTrappedImmune(registry.get<SpeciesTypes>(target.val), pokedex)) {
    return;
  }

  registry.emplace<dex::Trapped>(target.val);
  registry.emplace<dex::Trapper>(target.val, source.val);
}

void setSpeedSortNeeded(types::registry& registry, Battle battle) {
  registry.get_or_emplace<pokesim::simulate_turn::tags::SpeedSortNeeded>(battle.val);
}
}  // namespace

void setItem(types::registry& registry, dex::Item item, types::entity entity, const Pokedex& pokedex) {
  registry.emplace<pokesim::tags::HasItem>(entity);
  dex::emplaceTagFromEnum(item, registry, entity);

  if (pokedex.itemHas<item::tags::Choice>(item)) {
    registry.emplace<item::tags::Choice>(entity);
  }
  if (pokedex.itemHas<item::tags::Berry>(item)) {
    registry.emplace<item::tags::Berry>(entity);
  }
}

void setAbility(types::registry& registry, dex::Ability ability, types::entity entity) {
  registry.emplace<pokesim::tags::HasAbility>(entity);
  dex::emplaceTagFromEnum(ability, registry, entity);
}

void setStatus(types::registry& registry, dex::Status status, types::entity entity) {
  registry.emplace<pokesim::tags::HasStatus>(entity);
  dex::emplaceTagFromEnum(status, registry, entity);
}

void checkIfCanUseItem(Simulation&) {}
void checkIfCanEatItem(Simulation& simulation) {
  checkIfCanUseItem(simulation);
}

void useItem(Simulation& simulation) {
  runAfterUseItemEvent(simulation);
  removeItemComponents<pokesim::tags::CanUseItem>(simulation);
}

void tryUseItem(Simulation& simulation) {
  checkIfCanUseItem(simulation);
  useItem(simulation);
}

void eatItem(Simulation& simulation) {
  runEatItemEvent(simulation);
  runAfterEatItemEvent(simulation);
  runAfterUseItemEvent(simulation);
  removeItemComponents<pokesim::tags::CanEatItem>(simulation);
}

void tryEatItem(Simulation& simulation) {
  checkIfCanEatItem(simulation);
  eatItem(simulation);
}

void checkIfCanRemoveItem(Simulation& simulation) {
  runTryTakeItemEvent(simulation);
}

void removeItem(Simulation& simulation) {
  simulation.addToEntities<tags::RemoveItem, pokesim::tags::CanRemoveItem>();
  runRemoveItemEvent(simulation);
  simulation.removeFromEntities<tags::RemoveItem>();

  removeItemComponents<pokesim::tags::CanRemoveItem>(simulation);
}

void tryRemoveItem(Simulation& simulation) {
  checkIfCanRemoveItem(simulation);
  removeItem(simulation);
}

void checkIfCanSetStatus(Simulation& simulation) {
  dex::forEachStatus<CheckIfStatusIsSettable>(simulation);
}

void setStatus(Simulation& simulation) {
  dex::forEachStatus<RemoveNotSettableStatus>(simulation);
  dex::forEachStatus<SetEffectTargetStatus>(simulation);

  runStartSleep(simulation);
  runStartFreeze(simulation);

  runAfterSetStatusEvent(simulation);
  simulation.removeFromEntities<pokesim::tags::EffectCanSetStatus>();
}

void trySetStatus(Simulation& simulation) {
  checkIfCanSetStatus(simulation);
  setStatus(simulation);
}

void clearStatus(types::handle pokemonHandle) {
  pokemonHandle
    .remove<pokesim::tags::HasStatus, dex::Burn, dex::Freeze, dex::Paralysis, dex::Poison, dex::Sleep, dex::Toxic>();
}

void clearVolatiles(Simulation& simulation) {
  simulation.view<revertTransform, Tags<tags::ClearVolatiles>>(simulation.pokedex());
  clearVolatileTags(simulation);
}

void deductPp(MoveSlots& moveSlots, LastUsedMove lastUsedMove) {
  MoveSlot& moveSlot = moveSlots.val[lastUsedMove.val];
  if (moveSlot.pp >= Constants::PP_USE_DEDUCTION + Constants::MovePp::MIN) {
    moveSlot.pp -= Constants::PP_USE_DEDUCTION;
  }
  else {
    moveSlot.pp = Constants::MovePp::MIN;
  }
}

void setLastMoveUsed(types::registry& registry, CurrentAction& source, CurrentActionMoveSlot move) {
  registry.emplace_or_replace<LastUsedMove>(source.source, move.val);
}

void faint(types::handle handle, Battle battle) {
  types::registry& registry = *handle.registry();
  FaintQueue& faintQueue = registry.get_or_emplace<FaintQueue>(battle.val);
  types::entity entity = handle.entity();
  for (types::entity queued : faintQueue.val) {
    if (queued == entity) {
      return;
    }
  }

  faintQueue.val.push_back(entity);
}

void applyDamage(types::handle handle, types::damage damage) {
  stat::CurrentHp& hp = handle.get<stat::CurrentHp>();
  if (damage < hp.val) {
    hp.val -= damage;
  }
  else {
    hp.val = Constants::PokemonCurrentHpStat::MIN;
    faint(handle, handle.get<Battle>());
  }
}

void applyStatBoost(types::stat& stat, types::boost boost) {
  if (boost >= 0) {
    stat = types::stat(stat * Constants::STAT_BOOST_STAGES[boost]);
  }
  else {
    stat = types::stat(stat / Constants::STAT_BOOST_STAGES[-boost]);
  }
}

void tryBoost(Simulation& simulation) {
  runChangeBoostEvent(simulation);
  simulation.view<clampBoost<AtkBoost>>();
  simulation.view<clampBoost<DefBoost>>();
  simulation.view<clampBoost<SpaBoost>>();
  simulation.view<clampBoost<SpdBoost>>();
  simulation.view<clampBoost<SpeBoost>>();
  simulation.view<clampBoost<AccBoost>>();
  simulation.view<clampBoost<EvaBoost>>();
  runTryBoostEvent(simulation);

  simulation.view<boostStat<AtkBoost, pokesim::tags::AtkStatUpdateRequired>>();
  simulation.view<boostStat<DefBoost, pokesim::tags::DefStatUpdateRequired>>();
  simulation.view<boostStat<SpaBoost, pokesim::tags::SpaStatUpdateRequired>>();
  simulation.view<boostStat<SpdBoost, pokesim::tags::SpdStatUpdateRequired>>();
  simulation.view<boostStat<SpeBoost, pokesim::tags::SpeStatUpdateRequired>>();
  simulation.view<boostStat<AccBoost, void>>();
  simulation.view<boostStat<EvaBoost, void>>();
  runAfterEachBoostEvent(simulation);
  runAfterBoostEvent(simulation);
}

void tryTrap(Simulation& simulation) {
  simulation.view<trapAndSetTrapper, Tags<tags::TryTrap>>(simulation.pokedex());
}

void trap(types::handle handle, SpeciesTypes types, const Pokedex& pokedex) {
  if (areTypesTrappedImmune(types, pokedex)) {
    return;
  }

  handle.emplace<dex::Trapped>();
}

void transformInto(types::registry& registry, types::entity source, types::entity target, const Pokedex& pokedex) {
  types::handle sourceHandle{registry, source};
  types::handle targetHandle{registry, target};
  if (targetHandle.all_of<TransformedFrom>()) {
    return;
  }

  auto [sourceSpecies, sourceTypes, sourceMoves, sourceAtk, sourceDef, sourceSpa, sourceSpd, sourceSpe] =
    sourceHandle.get<SpeciesName, SpeciesTypes, MoveSlots, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>();
  auto [targetSpecies, targetTypes, targetMoves, targetAtk, targetDef, targetSpa, targetSpd, targetSpe] =
    targetHandle.get<SpeciesName, SpeciesTypes, MoveSlots, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>();

  dex::Ability sourceAbility = dex::Ability::NO_ABILITY;
  dex::Ability targetAbility = dex::Ability::NO_ABILITY;
  pokedex.forEachLoadedAbility([&sourceHandle, &targetHandle, &sourceAbility, &targetAbility](dex::Ability ability) {
    if (dex::enumToTag<RemoveIfHas>(ability, sourceHandle)) {
      sourceAbility = ability;
    }
    if (dex::hasTag(ability, targetHandle)) {
      targetAbility = ability;
    }
  });

  POKESIM_REQUIRE(sourceAbility != dex::Ability::NO_ABILITY, "Every Pokemon should have an ability (for now).");
  POKESIM_REQUIRE(targetAbility != dex::Ability::NO_ABILITY, "Every Pokemon should have an ability (for now).");

  sourceHandle.emplace<TransformedFrom>(TransformedFrom{sourceSpecies.val, sourceAbility, sourceMoves.val});

  sourceSpecies = targetSpecies;
  sourceTypes = targetTypes;
  sourceAtk = targetAtk;
  sourceDef = targetDef;
  sourceSpa = targetSpa;
  sourceSpd = targetSpd;
  sourceSpe = targetSpe;
  dex::emplaceTagFromEnum(targetAbility, sourceHandle);

  sourceMoves.val.pop_count(sourceMoves.val.size());
  for (MoveSlot targetMoveSlot : targetMoves.val) {
    sourceMoves.val.push_back({
      targetMoveSlot.move,
      std::min(targetMoveSlot.maxPp, Constants::MoveMaxPp::COPIED_WITH_TRANSFORM),
      std::min(targetMoveSlot.maxPp, Constants::MoveMaxPp::COPIED_WITH_TRANSFORM),
    });
  }

  if (targetHandle.all_of<AtkBoost>()) sourceHandle.emplace_or_replace<AtkBoost>(targetHandle.get<AtkBoost>());
  if (targetHandle.all_of<DefBoost>()) sourceHandle.emplace_or_replace<DefBoost>(targetHandle.get<DefBoost>());
  if (targetHandle.all_of<SpaBoost>()) sourceHandle.emplace_or_replace<SpaBoost>(targetHandle.get<SpaBoost>());
  if (targetHandle.all_of<SpdBoost>()) sourceHandle.emplace_or_replace<SpdBoost>(targetHandle.get<SpdBoost>());
  if (targetHandle.all_of<SpeBoost>()) sourceHandle.emplace_or_replace<SpeBoost>(targetHandle.get<SpeBoost>());
  if (targetHandle.all_of<AccBoost>()) sourceHandle.emplace_or_replace<AccBoost>(targetHandle.get<AccBoost>());
  if (targetHandle.all_of<EvaBoost>()) sourceHandle.emplace_or_replace<EvaBoost>(targetHandle.get<EvaBoost>());

  sourceHandle.emplace<pokesim::tags::AtkStatUpdateRequired>();
  sourceHandle.emplace<pokesim::tags::DefStatUpdateRequired>();
  sourceHandle.emplace<pokesim::tags::SpaStatUpdateRequired>();
  sourceHandle.emplace<pokesim::tags::SpdStatUpdateRequired>();
  sourceHandle.emplace<pokesim::tags::SpeStatUpdateRequired>();

  sourceHandle.remove<dex::ChoiceLock, DisabledMoveSlots, LastUsedMove>();

  // Technically, EndAbility should be called as well for the source's original ability, but since neither Ditto nor Mew
  // have abilities that need that, I'll let it go for now.
  if (sourceAbility != targetAbility) {
    sourceHandle.emplace<pokesim::internal::tags::StartAbility>();
  }
}

void updateAllStats(Simulation& simulation) {
  updateAtk(simulation, false);
  updateDef(simulation, false);
  updateSpa(simulation, false);
  updateSpd(simulation, false);
  updateSpe(simulation, false);
}

void updateAtk(Simulation& simulation, bool ignoreBoosts) {
  internal::EntityFilter<pokesim::tags::AtkStatUpdateRequired> filter{simulation};
  if (filter.hasNoneSelected()) return;

  filter.view<resetEffectiveAtk>();

  if (!ignoreBoosts) {
    filter.view<applyBoostToEffectiveStat<stat::EffectiveAtk, AtkBoost>>();
  }
  runModifyAtk(simulation);

  filter.clearSelectionTags();
}

void updateDef(Simulation& simulation, bool ignoreBoosts) {
  internal::EntityFilter<pokesim::tags::DefStatUpdateRequired> filter{simulation};
  if (filter.hasNoneSelected()) return;

  filter.view<resetEffectiveDef>();

  if (!ignoreBoosts) {
    filter.view<applyBoostToEffectiveStat<stat::EffectiveDef, DefBoost>>();
  }
  runModifyDef(simulation);

  filter.clearSelectionTags();
}

void updateSpa(Simulation& simulation, bool ignoreBoosts) {
  internal::EntityFilter<pokesim::tags::SpaStatUpdateRequired> filter{simulation};
  if (filter.hasNoneSelected()) return;

  filter.view<resetEffectiveSpa>();

  if (!ignoreBoosts) {
    filter.view<applyBoostToEffectiveStat<stat::EffectiveSpa, SpaBoost>>();
  }
  runModifySpa(simulation);

  filter.clearSelectionTags();
}

void updateSpd(Simulation& simulation, bool ignoreBoosts) {
  internal::EntityFilter<pokesim::tags::SpdStatUpdateRequired> filter{simulation};
  if (filter.hasNoneSelected()) return;

  filter.view<resetEffectiveSpd>();

  if (!ignoreBoosts) {
    filter.view<applyBoostToEffectiveStat<stat::EffectiveSpd, SpdBoost>>();
  }
  runModifySpd(simulation);

  filter.clearSelectionTags();
}

void updateSpe(Simulation& simulation, bool ignoreBoosts) {
  internal::EntityFilter<pokesim::tags::SpeStatUpdateRequired> filter{simulation};
  if (filter.hasNoneSelected()) return;

  filter.view<resetEffectiveSpe>();

  if (!ignoreBoosts) {
    filter.view<applyBoostToEffectiveStat<stat::EffectiveSpe, SpeBoost>>();
  }
  runModifySpe(simulation);
  // trick room

  filter.view<setSpeedSortNeeded, Tags<pokesim::tags::SimulateTurn>>();
  filter.clearSelectionTags();
}
}  // namespace pokesim::internal
