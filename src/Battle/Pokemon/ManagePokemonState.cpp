#include "ManagePokemonState.hpp"

#include <Battle/Setup/EmplaceTagFromEnum.hpp>
#include <Components/Boosts.hpp>
#include <Components/ChoiceLock.hpp>
#include <Components/Current.hpp>
#include <Components/Damage.hpp>
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
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RunEventTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Components/Tags/TypeTags.hpp>
#include <Pokedex/EnumToTag/ItemEnumToTag.hpp>
#include <Pokedex/EnumToTag/StatusEnumToTag.hpp>
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
template <typename ItemTag>
struct RemoveItem {
  static void run(types::handle handle) { handle.remove<ItemTag>(); }
};

void removeItemTags(types::handle handle, ItemName item) {
  pokesim::item::tags::enumToTag<RemoveItem>(item.val, handle);
}

template <typename SelectionTag>
void removeItemComponents(Simulation& simulation) {
  simulation.view<removeItemTags, Tags<SelectionTag>>();
  simulation.removeFromEntities<ItemName, SelectionTag>();
  simulation.registry.clear<SelectionTag>();
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

template <typename Type>
void checkTypeStatusImmunity(types::handle handle, CurrentEffectTarget target) {
  if (handle.registry()->all_of<Type>(target.val)) {
    handle.remove<pokesim::tags::CanSetStatus>();
  }
}

template <typename StatusType>
struct CheckIfStatusIsSettable {
  static void run(Simulation& simulation) {
    simulation.addToEntities<pokesim::tags::CanSetStatus, StatusType, CurrentEffectSource, CurrentEffectTarget>();
    simulation.view<checkIfTargetHasStatus, Tags<StatusType>>();
    if constexpr (std::is_same_v<StatusType, pokesim::status::tags::Burn>) {
      simulation.view<checkTypeStatusImmunity<pokesim::type::tags::Fire>, Tags<StatusType>>();
    }
    if constexpr (std::is_same_v<StatusType, pokesim::status::tags::Freeze>) {
      simulation.view<checkTypeStatusImmunity<pokesim::type::tags::Ice>, Tags<StatusType>>();
    }
    if constexpr (std::is_same_v<StatusType, pokesim::status::tags::Paralysis>) {  // And simulation is using a mechanic
                                                                                   // where
                                                                                   // electric types cannot be paralyzed
      simulation.view<checkTypeStatusImmunity<pokesim::type::tags::Electric>, Tags<StatusType>>();
    }

    if constexpr (
      std::is_same_v<StatusType, pokesim::status::tags::Poison> ||
      std::is_same_v<StatusType, pokesim::status::tags::Toxic>) {
      simulation.view<checkTypeStatusImmunity<pokesim::type::tags::Poison>, Tags<StatusType>>();
      simulation.view<checkTypeStatusImmunity<pokesim::type::tags::Steel>, Tags<StatusType>>();
    }

    runStatusImmunityEvent<StatusType>(simulation);
  }

  static void checkIfTargetHasStatus(types::handle handle, CurrentEffectTarget target) {
    if (handle.registry()->all_of<StatusName>(target.val)) {
      handle.remove<pokesim::tags::CanSetStatus>();
    }
  };
};

template <typename StatusType>
struct RemoveNotSettableStatus {
  static void run(Simulation& simulation) {
    simulation.removeFromEntities<StatusType, CurrentEffectSource, CurrentEffectTarget>(
      entt::exclude<pokesim::tags::CanSetStatus>);
  }
};

template <typename StatusType>
void setStatus(types::registry& registry, CurrentEffectTarget target, dex::Status status) {
  registry.emplace<StatusName>(target.val, status);
  registry.emplace<StatusType>(target.val);
  if constexpr (std::is_same_v<StatusType, pokesim::status::tags::Paralysis>) {
    registry.emplace<pokesim::tags::SpeStatUpdateRequired>(target.val);
  }
}

void setSpeedSortNeeded(types::registry& registry, Battle battle) {
  registry.emplace_or_replace<pokesim::simulate_turn::tags::SpeedSortNeeded>(battle.val);
}
}  // namespace

void checkIfCanUseItem(Simulation& simulation) {
  simulation.removeFromEntities<pokesim::tags::CanUseItem>(entt::exclude<ItemName>);
}

void useItem(Simulation& simulation) {
  runAfterUseItemEvent(simulation);
  removeItemComponents<pokesim::tags::CanUseItem>(simulation);
}

void tryUseItem(Simulation& simulation) {
  checkIfCanUseItem(simulation);
  useItem(simulation);
}

void checkIfCanRemoveItem(Simulation& simulation) {
  simulation.removeFromEntities<pokesim::tags::CanRemoveItem>(entt::exclude<ItemName>);
  runTryTakeItemEvent(simulation);
}

void removeItem(Simulation& simulation) {
  simulation.addToEntities<tags::EndItem, pokesim::tags::CanRemoveItem>();
  runEndItemEvent(simulation);
  simulation.registry.clear<tags::EndItem>();

  removeItemComponents<pokesim::tags::CanRemoveItem>(simulation);
}

void tryRemoveItem(Simulation& simulation) {
  checkIfCanRemoveItem(simulation);
  removeItem(simulation);
}

void checkIfCanSetStatus(Simulation& simulation) {
  pokesim::status::tags::forEach<CheckIfStatusIsSettable>(simulation);
}

void setStatus(Simulation& simulation) {
  pokesim::status::tags::forEach<RemoveNotSettableStatus>(simulation);
  simulation.registry.clear<pokesim::tags::CanSetStatus>();

  simulation.view<setStatus<pokesim::status::tags::Burn>, Tags<pokesim::status::tags::Burn>>(dex::Status::BRN);
  simulation.view<setStatus<pokesim::status::tags::Freeze>, Tags<pokesim::status::tags::Freeze>>(dex::Status::FRZ);
  simulation.view<setStatus<pokesim::status::tags::Paralysis>, Tags<pokesim::status::tags::Paralysis>>(
    dex::Status::PAR);
  simulation.view<setStatus<pokesim::status::tags::Poison>, Tags<pokesim::status::tags::Poison>>(dex::Status::PSN);
  simulation.view<setStatus<pokesim::status::tags::Sleep>, Tags<pokesim::status::tags::Sleep>>(dex::Status::SLP);
  simulation.view<setStatus<pokesim::status::tags::Toxic>, Tags<pokesim::status::tags::Toxic>>(dex::Status::TOX);

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
    StatusName,
    pokesim::status::tags::Burn,
    pokesim::status::tags::Freeze,
    pokesim::status::tags::Paralysis,
    pokesim::status::tags::Poison,
    pokesim::status::tags::Sleep,
    pokesim::status::tags::Toxic>();
}

void clearVolatiles(types::handle pokemonHandle) {
  pokemonHandle.remove<AtkBoost, DefBoost, SpaBoost, SpdBoost, SpeBoost>();
  pokemonHandle.remove<LastUsedMove>();

  // TODO(aed3): Make autogenerated
  pokemonHandle.remove<ChoiceLock>();
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

void setLastMoveUsed(types::registry& registry, CurrentActionSource source, const CurrentActionMoveSlot& move) {
  registry.emplace<LastUsedMove>(source.val, move.val);
}

void faint(types::handle pokemonHandle, Battle battle) {
  types::registry& registry = *pokemonHandle.registry();
  FaintQueue& faintQueue = registry.get_or_emplace<FaintQueue>(battle.val);
  faintQueue.val.push_back(pokemonHandle.entity());
}

void applyDamage(types::handle pokemonHandle, types::damage damage) {
  stat::CurrentHp& hp = pokemonHandle.get<stat::CurrentHp>();
  if (damage < hp.val) {
    hp.val -= damage;
  }
  else {
    hp.val = Constants::PokemonCurrentHpStat::MIN;
    faint(pokemonHandle, pokemonHandle.get<Battle>());
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
