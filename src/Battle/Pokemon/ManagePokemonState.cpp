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
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
namespace {
template <typename EffectiveStat, typename BoostType>
void applyBoostToEffectiveStat(EffectiveStat& effectiveStat, BoostType boost) {
  applyStatBoost(effectiveStat.val, boost.val);
}
}  // namespace

void setStatus(types::handle pokemonHandle, dex::Status status) {
  clearStatus(pokemonHandle);
  pokemonHandle.emplace<StatusName>(status);
  status::tags::emplaceTagFromEnum(status, pokemonHandle);
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

void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMoveSlot& move) {
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

void applyDamageToHp(types::registry& registry, const Damage& damage, CurrentActionTargets& targets) {
  stat::CurrentHp& hp = registry.get<stat::CurrentHp>(targets.only());
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