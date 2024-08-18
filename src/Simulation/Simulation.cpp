#include "Simulation.hpp"

#include <AnalyzeEffect/AnalyzeEffect.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/HitCount.hpp>
#include <Components/SimulationResults.hpp>
#include <SimulateTurn/SimulateTurn.hpp>

#include "SimulationOptions.hpp"
#include "SimulationResults.hpp"

namespace pokesim {
void Simulation::clearAllResults() {
  clearSimulateTurnResults();
  clearCalculateDamageResults();
  clearAnalyzeEffectResults();
}

void Simulation::clearSimulateTurnResults() {
  registry.clear<simulate_turn::TurnOutcomeBattles>();
}

void Simulation::clearCalculateDamageResults() {
  registry.clear<calc_damage::MaxDamage>();
  registry.clear<calc_damage::MaxDamage>();
  registry.clear<calc_damage::MinUsesUntilKo>();
  registry.clear<calc_damage::AttackerHpRecovered>();
  registry.clear<calc_damage::AttackerHpLost>();
  registry.clear<HitCount>();
}

void Simulation::clearAnalyzeEffectResults() {
  registry.clear<analyze_effect::EffectMultiplier>();
  registry.clear<analyze_effect::MultipliedDamage>();
  registry.clear<analyze_effect::MultipliedDamageRolls>();
  registry.clear<analyze_effect::MultipliedKoChance>();
}

simulate_turn::Results Simulation::simulateTurn(std::optional<simulate_turn::Options> options) {
  if (options.has_value()) {
    simulateTurnOptions = options.value();
  }

  simulate_turn::run(*this);

  return {*this};
}

calc_damage::Results Simulation::calculateDamage(std::optional<calc_damage::Options> options) {
  if (options.has_value()) {
    calculateDamageOptions = options.value();
  }

  updateAllStats(*this);
  calc_damage::run(*this);

  return {*this};
}

analyze_effect::Results Simulation::analyzeEffect(std::optional<analyze_effect::Options> options) {
  if (options.has_value()) {
    analyzeEffectOptions = options.value();
  }

  updateAllStats(*this);
  analyze_effect::run(*this);

  return {*this};
}

simulate_turn::Results Simulation::simulateTurn(
  std::initializer_list<BattleCreationInfo> battleDataList, std::optional<simulate_turn::Options> options) {
  createInitialStates(battleDataList);
  return simulateTurn(options);
}

calc_damage::Results Simulation::calculateDamage(
  std::initializer_list<BattleCreationInfo> battleDataList, std::optional<calc_damage::Options> options) {
  createInitialStates(battleDataList);
  return calculateDamage(options);
}

analyze_effect::Results Simulation::analyzeEffect(
  std::initializer_list<BattleCreationInfo> battleDataList, std::optional<analyze_effect::Options> options) {
  createInitialStates(battleDataList);
  return analyzeEffect(options);
}

void Simulation::run() {
  clearAllResults();
  simulateTurn();
  calculateDamage();
  analyzeEffect();
}

std::vector<types::entity> Simulation::selectedBattleEntities() {
  if (hasActiveSelection<tags::SelectedForViewBattle>()) {
    auto view = registry.view<tags::SelectedForViewBattle, Sides>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<Sides>();
  return {view.begin(), view.end()};
}
}  // namespace pokesim