#include "CalcDamage.hpp"

#include <Components/BasePower.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CalcDamage/DamageCalcVariables.hpp>
#include <Components/Damage.hpp>
#include <Components/Level.hpp>
#include <Components/Stats.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Damage.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <cmath>
#include <type_traits>

namespace pokesim::calc_damage {
void run(Simulation& simulation) {
  getDamage(simulation);

  clearRunVariables(simulation);
}

void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<tags::Crit>();
}

void modifyDamageWithTypes(Simulation& /*simulation*/) {}

void getDamageRole(Simulation& /*simulation*/) {}

void internal::assignCritChanceDivisor(types::handle moveHandle, const CritBoost& critBoost) {
  std::size_t index = std::min((std::size_t)critBoost.val, internal::CRIT_CHANCE_DIVISORS.size() - 1);
  moveHandle.emplace<CritChanceDivisor>(internal::CRIT_CHANCE_DIVISORS[index]);
}

void internal::setSourceLevel(types::handle moveHandle, const Attacker& attacker) {
  moveHandle.emplace<AttackingLevel>(moveHandle.registry()->get<Level>(attacker.val).val);
}

template <typename Category>
void internal::setUsedAttackStat(types::handle moveHandle, const Attacker& attacker) {
  types::stat attackingStat = 1;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    attackingStat = moveHandle.registry()->get<stat::EffectiveAtk>(attacker.val).val;
  }
  else {
    attackingStat = moveHandle.registry()->get<stat::EffectiveSpa>(attacker.val).val;
  }
  moveHandle.emplace<AttackingStat>(attackingStat);
}

template <typename Category>
void internal::setUsedDefenseStat(types::handle moveHandle, const Defenders& defenders) {
  types::stat defendingStat = 1;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    defendingStat = moveHandle.registry()->get<stat::EffectiveDef>(defenders.val[0]).val;
  }
  else {
    defendingStat = moveHandle.registry()->get<stat::EffectiveSpd>(defenders.val[0]).val;
  }
  moveHandle.emplace<DefendingStat>(defendingStat);
}

void internal::calculateBaseDamage(
  types::handle moveHandle, const BasePower& basePower, const AttackingLevel& level, const AttackingStat& attack,
  const DefendingStat& defense) {
  types::damage damage = (((2 * level.val / 5 + 2) * basePower.val * attack.val) / defense.val) / 50;
  moveHandle.emplace<Damage>(damage);
}

void setIfMoveCrits(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  simulation.addToEntities<CritBoost, pokesim::tags::SelectedForViewMove>();
  runModifyCritBoostEvent(simulation);
  simulation.viewForSelectedMoves<internal::assignCritChanceDivisor>();
  simulation.registry.clear<CritBoost>();

  setReciprocalRandomBinaryChoice<CritChanceDivisor, pokesim::tags::SelectedForViewMove>(simulation);
  reciprocalRandomBinaryChance(simulation, [](Simulation& sim) {
    sim.addToEntities<tags::Crit, pokesim::tags::RandomEventCheckPassed>();
  });

  clearRandomChanceResult(simulation);
  simulation.registry.clear<CritChanceDivisor>();
}

void getDamage(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  setIfMoveCrits(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);
  simulation.viewForSelectedMoves<internal::setSourceLevel>();

  simulation.viewForSelectedMoves<internal::setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<internal::setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  simulation.viewForSelectedMoves<internal::setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<internal::setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  simulation.viewForSelectedMoves<internal::calculateBaseDamage>();

  getDamageRole(simulation);

  modifyDamageWithTypes(simulation);
}
}  // namespace pokesim::calc_damage