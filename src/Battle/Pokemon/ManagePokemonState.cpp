#include "ManagePokemonState.hpp"

#include <Battle/Setup/EmplaceTagFromEnum.hpp>
#include <Components/Boosts.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/LastUsedMove.hpp>
#include <Components/Names/StatNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/PP.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Components/Tags/TypeTags.hpp>
#include <Pokedex/EnumToTag/StatusEnumToTag.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <cmath>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
namespace {
template <typename EffectiveStat, typename BoostType>
void applyBoostToEffectiveStat(EffectiveStat& effectiveStat, BoostType boost) {
  applyStatBoost(effectiveStat.val, boost.val);
}

template <typename BoostType, typename StatUpdateRequired>
void boostStat(types::registry& registry, CurrentEffectTarget target, BoostType& boost) {
  BoostType& currentBoost = registry.get_or_emplace<BoostType>(target.val);
  currentBoost.val += boost.val;

  if (boost.val) {
    registry.emplace_or_replace<StatUpdateRequired>(target.val);
  }
}

template <typename BoostType>
void clampBoost(types::registry& registry, CurrentEffectTarget target, BoostType& boost) {
  BoostType* currentBoost = registry.try_get<BoostType>(target.val);
  if (!currentBoost) {
    return;
  }

  types::boost combinedBoost = currentBoost->val + boost.val;
  combinedBoost = std::max(combinedBoost, MechanicConstants::PokemonStatBoost::MIN);
  combinedBoost = std::min(combinedBoost, MechanicConstants::PokemonStatBoost::MAX);
  boost.val = combinedBoost - boost.val;
}

template <typename BoostType, typename StatUpdateRequired>
void boost(Simulation& simulation) {
  simulation.view<boostStat<BoostType, StatUpdateRequired>>();
  runAfterEachBoostEvent<BoostType>(simulation);
}

template <typename Type>
void checkTypeStatusImmunity(types::handle handle, CurrentEffectTarget target) {
  if (handle.registry()->all_of<Type>(target.val)) {
    handle.remove<tags::CanSetStatus>();
  }
}

template <typename StatusType>
struct CheckIfStatusIsSettable {
  static void run(Simulation& simulation) {
    simulation.addToEntities<tags::CanSetStatus, StatusType, CurrentEffectSource, CurrentEffectTarget>();
    simulation.view<checkIfTargetHasStatus, Tags<StatusType>>();
    if constexpr (std::is_same_v<StatusType, status::tags::Burn>) {
      simulation.view<checkTypeStatusImmunity<type::tags::Fire>, Tags<StatusType>>();
    }
    if constexpr (std::is_same_v<StatusType, status::tags::Freeze>) {
      simulation.view<checkTypeStatusImmunity<type::tags::Ice>, Tags<StatusType>>();
    }
    if constexpr (std::is_same_v<StatusType, status::tags::Paralysis>) {  // And simulation is using a mechanic where
                                                                          // electric types cannot be paralyzed
      simulation.view<checkTypeStatusImmunity<type::tags::Electric>, Tags<StatusType>>();
    }

    if constexpr (std::is_same_v<StatusType, status::tags::Poison> || std::is_same_v<StatusType, status::tags::Toxic>) {
      simulation.view<checkTypeStatusImmunity<type::tags::Poison>, Tags<StatusType>>();
      simulation.view<checkTypeStatusImmunity<type::tags::Steel>, Tags<StatusType>>();
    }

    runStatusImmunityEvent<StatusType>(simulation);
  }

  static void checkIfTargetHasStatus(types::handle handle, CurrentEffectTarget target) {
    if (handle.registry()->all_of<StatusName>(target.val)) {
      handle.remove<tags::CanSetStatus>();
    }
  };
};

template <typename StatusType>
struct RemoveNotSettableStatus {
  static void run(Simulation& simulation) {
    simulation.removeFromEntities<StatusType, CurrentEffectSource, CurrentEffectTarget>(
      entt::exclude<tags::CanSetStatus>);
  }
};

template <typename StatusType>
void setStatus(types::registry& registry, CurrentEffectTarget target, dex::Status status) {
  registry.emplace<StatusName>(target.val, status);
  registry.emplace<StatusType>(target.val);
}
}  // namespace

void checkIfStatusIsSettable(Simulation& simulation) {
  status::tags::forEach<CheckIfStatusIsSettable>(simulation);
}

void trySetStatus(Simulation& simulation) {
  checkIfStatusIsSettable(simulation);
  status::tags::forEach<RemoveNotSettableStatus>(simulation);
  simulation.registry.clear<tags::CanSetStatus>();

  simulation.view<setStatus<status::tags::Burn>, Tags<status::tags::Burn>>(dex::Status::BRN);
  simulation.view<setStatus<status::tags::Freeze>, Tags<status::tags::Freeze>>(dex::Status::FRZ);
  simulation.view<setStatus<status::tags::Paralysis>, Tags<status::tags::Paralysis>>(dex::Status::PAR);
  simulation.view<setStatus<status::tags::Poison>, Tags<status::tags::Poison>>(dex::Status::PSN);
  simulation.view<setStatus<status::tags::Sleep>, Tags<status::tags::Sleep>>(dex::Status::SLP);
  simulation.view<setStatus<status::tags::Toxic>, Tags<status::tags::Toxic>>(dex::Status::TOX);

  runStartSleep(simulation);
  runStartFreeze(simulation);

  runAfterSetStatusEvent(simulation);
}

void clearStatus(types::handle pokemonHandle) {
  pokemonHandle.remove<
    StatusName,
    status::tags::Burn,
    status::tags::Freeze,
    status::tags::Paralysis,
    status::tags::Poison,
    status::tags::Sleep,
    status::tags::Toxic>();
}

void deductPp(Pp& pp) {
  if (pp.val) {
    pp.val -= 1U;
  }
}

void setLastMoveUsed(types::registry& registry, CurrentActionSource source, const CurrentActionMoveSlot& move) {
  registry.emplace<LastUsedMove>(source.val, move.val);
}

void resetEffectiveAtk(types::handle handle, stat::Atk atk) {
  handle.emplace_or_replace<stat::EffectiveAtk>(atk.val);
}

void resetEffectiveDef(types::handle handle, stat::Def def) {
  handle.emplace_or_replace<stat::EffectiveDef>(def.val);
}

void resetEffectiveSpa(types::handle handle, stat::Spa spa) {
  handle.emplace_or_replace<stat::EffectiveSpa>(spa.val);
}

void resetEffectiveSpd(types::handle handle, stat::Spd spd) {
  handle.emplace_or_replace<stat::EffectiveSpd>(spd.val);
}

void resetEffectiveSpe(types::handle handle, stat::Spe spe) {
  handle.emplace_or_replace<stat::EffectiveSpe>(spe.val);
}

void applyDamageToHp(types::registry& registry, const Damage& damage, CurrentActionTarget target) {
  stat::CurrentHp& hp = registry.get<stat::CurrentHp>(target.val);
  if (damage.val < hp.val) {
    hp.val -= damage.val;
  }
  else {
    hp.val = 0U;
    // Faint
  }
}

void applyStatBoost(types::stat& stat, types::boost boost) {
  if (boost >= 0) {
    stat = types::stat(stat * MechanicConstants::STAT_BOOST_STAGES[boost]);
  }
  else {
    stat = types::stat(stat / MechanicConstants::STAT_BOOST_STAGES[-boost]);
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

  boost<AtkBoost, tags::AtkStatUpdateRequired>(simulation);
  boost<DefBoost, tags::DefStatUpdateRequired>(simulation);
  boost<SpaBoost, tags::SpaStatUpdateRequired>(simulation);
  boost<SpdBoost, tags::SpdStatUpdateRequired>(simulation);
  boost<SpeBoost, tags::SpeStatUpdateRequired>(simulation);
  runAfterBoostEvent(simulation);
}

void updateAllStats(Simulation& simulation) {
  updateAtk(simulation);
  updateDef(simulation);
  updateSpa(simulation);
  updateSpd(simulation);
  updateSpe(simulation);
}

void updateAtk(Simulation& simulation) {
  internal::SelectForPokemonView<tags::AtkStatUpdateRequired> selectedAtkUpdateRequired{simulation};
  if (selectedAtkUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveAtk>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveAtk, AtkBoost>>();
  runModifyAtk(simulation);

  simulation.registry.clear<tags::AtkStatUpdateRequired>();
}

void updateDef(Simulation& simulation) {
  internal::SelectForPokemonView<tags::DefStatUpdateRequired> selectedDefUpdateRequired{simulation};
  if (selectedDefUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveDef>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveDef, DefBoost>>();
  runModifyDef(simulation);

  simulation.registry.clear<tags::DefStatUpdateRequired>();
}

void updateSpa(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpaStatUpdateRequired> selectedSpaUpdateRequired{simulation};
  if (selectedSpaUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpa>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveSpa, SpaBoost>>();
  runModifySpa(simulation);

  simulation.registry.clear<tags::SpaStatUpdateRequired>();
}

void updateSpd(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpdStatUpdateRequired> selectedSpdUpdateRequired{simulation};
  if (selectedSpdUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpd>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveSpd, SpdBoost>>();
  runModifySpd(simulation);

  simulation.registry.clear<tags::SpdStatUpdateRequired>();
}

void updateSpe(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpeStatUpdateRequired> selectedSpeUpdateRequired{simulation};
  if (selectedSpeUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpe>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveSpe, SpeBoost>>();
  runModifySpe(simulation);
  // trick room

  simulation.registry.clear<tags::SpeStatUpdateRequired>();
}
}  // namespace pokesim