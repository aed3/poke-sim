#include "ManagePokemonState.hpp"

#include <Components/Damage.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/LastUsedMove.hpp>
#include <Components/PP.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void deductPp(Pp& pp) {
  if (pp.val) {
    pp.val -= 1;
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
  stat::CurrentHp& hp = registry.get<stat::CurrentHp>(targets.val[0]);
  if (damage.val > hp.val) {
    hp.val = 0;
    // Faint
  }
  else {
    hp.val -= damage.val;
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

  // apply boosts
  runModifyAtk(simulation);

  selectedAtkUpdateRequired.deselect();
  simulation.registry.clear<tags::AtkStatUpdateRequired>();
}

void updateDef(Simulation& simulation) {
  internal::SelectForPokemonView<tags::DefStatUpdateRequired> selectedDefUpdateRequired{simulation};
  if (selectedDefUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveDef>();

  // apply boosts
  runModifyDef(simulation);

  selectedDefUpdateRequired.deselect();
  simulation.registry.clear<tags::DefStatUpdateRequired>();
}

void updateSpa(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpaStatUpdateRequired> selectedSpaUpdateRequired{simulation};
  if (selectedSpaUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpa>();

  // apply boosts
  runModifySpa(simulation);

  selectedSpaUpdateRequired.deselect();
  simulation.registry.clear<tags::SpaStatUpdateRequired>();
}

void updateSpd(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpdStatUpdateRequired> selectedSpdUpdateRequired{simulation};
  if (selectedSpdUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpd>();

  // apply boosts
  runModifySpd(simulation);

  selectedSpdUpdateRequired.deselect();
  simulation.registry.clear<tags::SpdStatUpdateRequired>();
}

void updateSpe(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpeStatUpdateRequired> selectedSpeUpdateRequired{simulation};
  if (selectedSpeUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpe>();

  // apply boosts
  runModifySpe(simulation);
  // trick room

  selectedSpeUpdateRequired.deselect();
  simulation.registry.clear<tags::SpeStatUpdateRequired>();
}
}  // namespace pokesim