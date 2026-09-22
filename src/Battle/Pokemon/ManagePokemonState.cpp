#include "ManagePokemonState.hpp"

#include <Components/Boosts.hpp>
#include <Components/Current.hpp>
#include <Components/Damage.hpp>
#include <Components/Effects/ChoiceLock.hpp>
#include <Components/Effects/Trapper.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/FaintQueue.hpp>
#include <Components/LastUsedMove.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/StatNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/ItemPropertyTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Pokedex/Effects/headers.hpp>
#include <Pokedex/EnumToTag/AbilityEnumToTag.hpp>
#include <Pokedex/EnumToTag/ItemEnumToTag.hpp>
#include <Pokedex/EnumToTag/StatusEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Types/headers.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/EntityFilter.hpp>
#include <cmath>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
namespace {
template <typename ItemTag, typename SelectionTag>
struct RemoveItem {
  static void run(Simulation& simulation) { simulation.removeFromEntities<ItemTag, SelectionTag>(); }
};

template <typename SelectionTag>
void removeItemComponents(Simulation& simulation) {
  dex::forEachItem<RemoveItem, SelectionTag>(simulation.pokedex(), simulation);
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

  if (boost.val) {
    registry.emplace_or_replace<StatUpdateRequired>(target.val);
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

template <typename BoostType, typename StatUpdateRequired>
void boost(Simulation& simulation) {
  simulation.view<boostStat<BoostType, StatUpdateRequired>>();
  runAfterEachBoostEvent<BoostType>(simulation);
}

template <typename StatusType>
struct CheckIfStatusIsSettable {
  static void run(Simulation& simulation) {
    simulation.addToEntities<pokesim::tags::CanSetStatus, StatusType, CurrentEffectSource, CurrentEffectTarget>();
    simulation.view<checkIfTargetHasStatus, Tags<StatusType>>();
    simulation.view<checkTypeStatusImmunity, Tags<StatusType>>(simulation.pokedex());
    runStatusImmunityEvent<StatusType>(simulation);
  }

  static void checkIfTargetHasStatus(types::handle handle, CurrentEffectTarget target) {
    if (handle.registry()->all_of<pokesim::tags::HasStatus>(target.val)) {
      handle.remove<pokesim::tags::CanSetStatus>();
    }
  }

  static void checkTypeStatusImmunity(types::handle handle, CurrentEffectTarget target, const Pokedex& pokedex) {
    SpeciesTypes types = handle.registry()->get<SpeciesTypes>(target.val);
    for (dex::Type type : types.val) {
      if (pokedex.getStaticValue<StatusType::isTypeImmune>(type)) {
        handle.remove<pokesim::tags::CanSetStatus>();
        return;
      }
    }
  }
};

template <typename StatusType>
struct RemoveNotSettableStatus {
  static void run(Simulation& simulation) {
    simulation.removeFromEntities<StatusType, CurrentEffectSource, CurrentEffectTarget>(
      entt::exclude<pokesim::tags::CanSetStatus>);
  }
};

template <typename StatusType>
struct SetEffectTargetStatus {
  static constexpr pokesim::dex::Status status = StatusType::name();
  static void run(Simulation& simulation) { simulation.view<setEffectTargetStatus, Tags<StatusType>>(); }

  static void setEffectTargetStatus(types::registry& registry, CurrentEffectTarget target) {
    setStatus(status, registry, target.val);

    if constexpr (status == pokesim::dex::Status::PAR) {
      registry.emplace<pokesim::tags::SpeStatUpdateRequired>(target.val);
    }
  }
};

bool areTypesTrappedImmune(SpeciesTypes types, const Pokedex& pokedex) {
  for (pokesim::dex::Type type : types.val) {
    if (pokedex.getStaticValue<pokesim::dex::Trapped::isTypeImmune>(type)) {
      return true;
    }
  }

  return false;
}

void trapAndSetTrapper(
  types::registry& registry, CurrentEffectSource source, CurrentEffectTarget target, const Pokedex& pokedex) {
  if (areTypesTrappedImmune(registry.get<SpeciesTypes>(target.val), pokedex)) {
    return;
  }

  registry.emplace<pokesim::tags::Trapped>(target.val);
  registry.emplace<Trapper>(target.val, source.val);
}

void setSpeedSortNeeded(types::registry& registry, Battle battle) {
  registry.emplace_or_replace<pokesim::simulate_turn::tags::SpeedSortNeeded>(battle.val);
}
}  // namespace

void setItem(pokesim::dex::Item item, const Pokedex& pokedex, types::registry& registry, types::entity entity) {
  registry.emplace<pokesim::tags::HasItem>(entity);
  dex::emplaceTagFromEnum(item, registry, entity);

  if (pokedex.itemHas<item::tags::Choice>(item)) {
    registry.emplace<item::tags::Choice>(entity);
  }
  if (pokedex.itemHas<item::tags::Berry>(item)) {
    registry.emplace<item::tags::Berry>(entity);
  }
}

void setAbility(pokesim::dex::Ability ability, const Pokedex&, types::registry& registry, types::entity entity) {
  registry.emplace<pokesim::tags::HasAbility>(entity);
  dex::emplaceTagFromEnum(ability, registry, entity);
}

void setStatus(pokesim::dex::Status status, types::registry& registry, types::entity entity) {
  registry.emplace<pokesim::tags::HasStatus>(entity);
  dex::emplaceTagFromEnum(status, registry, entity);
}

void checkIfCanUseItem(Simulation&) {}

void useItem(Simulation& simulation) {
  runAfterUseItemEvent(simulation);
  removeItemComponents<pokesim::tags::CanUseItem>(simulation);
}

void tryUseItem(Simulation& simulation) {
  checkIfCanUseItem(simulation);
  useItem(simulation);
}

void checkIfCanRemoveItem(Simulation& simulation) {
  runTryTakeItemEvent(simulation);
}

void removeItem(Simulation& simulation) {
  simulation.addToEntities<tags::EndItem, pokesim::tags::CanRemoveItem>();
  runEndItemEvent(simulation);
  simulation.removeFromEntities<tags::EndItem>();

  removeItemComponents<pokesim::tags::CanRemoveItem>(simulation);
}

void tryRemoveItem(Simulation& simulation) {
  checkIfCanRemoveItem(simulation);
  removeItem(simulation);
}

void checkIfCanSetStatus(Simulation& simulation) {
  pokesim::dex::forEachStatus<CheckIfStatusIsSettable>(simulation);
}

void setStatus(Simulation& simulation) {
  pokesim::dex::forEachStatus<RemoveNotSettableStatus>(simulation);
  simulation.removeFromEntities<pokesim::tags::CanSetStatus>();

  pokesim::dex::forEachStatus<SetEffectTargetStatus>(simulation);

  runStartSleep(simulation);
  runStartFreeze(simulation);

  runAfterSetStatusEvent(simulation);
}

void trySetStatus(Simulation& simulation) {
  checkIfCanSetStatus(simulation);
  setStatus(simulation);
}

void clearStatus(types::handle pokemonHandle) {
  pokemonHandle.remove<
    pokesim::tags::HasStatus,
    pokesim::dex::Burn,
    pokesim::dex::Freeze,
    pokesim::dex::Paralysis,
    pokesim::dex::Poison,
    pokesim::dex::Sleep,
    pokesim::dex::Toxic>();
}

void clearVolatiles(types::handle pokemonHandle) {
  pokemonHandle.remove<AtkBoost, DefBoost, SpaBoost, SpdBoost, SpeBoost>();
  pokemonHandle.remove<LastUsedMove>();

  // TODO(aed3): Make autogenerated
  pokemonHandle.remove<ChoiceLock, Trapper, pokesim::tags::Flinch, pokesim::tags::Switching, pokesim::tags::Trapped>();
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
  runTryBoostEvent(simulation);

  boost<AtkBoost, pokesim::tags::AtkStatUpdateRequired>(simulation);
  boost<DefBoost, pokesim::tags::DefStatUpdateRequired>(simulation);
  boost<SpaBoost, pokesim::tags::SpaStatUpdateRequired>(simulation);
  boost<SpdBoost, pokesim::tags::SpdStatUpdateRequired>(simulation);
  boost<SpeBoost, pokesim::tags::SpeStatUpdateRequired>(simulation);
  runAfterBoostEvent(simulation);
}

void tryTrap(Simulation& simulation) {
  simulation.view<trapAndSetTrapper, Tags<tags::TryTrap>>(simulation.pokedex());
}

void trap(types::handle handle, SpeciesTypes types, const Pokedex& pokedex) {
  if (areTypesTrappedImmune(types, pokedex)) {
    return;
  }

  handle.emplace<pokesim::tags::Trapped>();
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
