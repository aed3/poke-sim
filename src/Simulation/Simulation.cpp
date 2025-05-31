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
  registry.clear<calc_damage::UsesUntilKo, calc_damage::AttackerHpRecovered, calc_damage::AttackerHpLost>();
}

void Simulation::clearAnalyzeEffectResults() {
  registry.clear<
    analyze_effect::EffectMultiplier,
    analyze_effect::MultipliedDamageRolls,
    analyze_effect::MultipliedUsesUntilKo>();
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
  const std::vector<BattleCreationInfo>& battleInfoList, std::optional<simulate_turn::Options> options) {
  createInitialStates(battleInfoList);
  return simulateTurn(options);
}

calc_damage::Results Simulation::calculateDamage(
  const std::vector<BattleCreationInfo>& battleInfoList, std::optional<calc_damage::Options> options) {
  createInitialStates(battleInfoList);
  return calculateDamage(options);
}

analyze_effect::Results Simulation::analyzeEffect(
  const std::vector<BattleCreationInfo>& battleInfoList, std::optional<analyze_effect::Options> options) {
  createInitialStates(battleInfoList);
  return analyzeEffect(options);
}

void Simulation::run() {
  clearAllResults();
  simulateTurn();
  calculateDamage();
  analyzeEffect();
}

types::entityVector Simulation::selectedBattleEntities() const {
  if (hasActiveSelection<tags::SelectedForViewBattle>()) {
    auto view = registry.view<tags::SelectedForViewBattle, tags::Battle>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<tags::Battle>();
  return {view.begin(), view.end()};
}

types::entityVector Simulation::selectedMoveEntities() const {
  if (hasActiveSelection<tags::SelectedForViewMove>()) {
    auto view = registry.view<tags::SelectedForViewMove, tags::CurrentActionMove>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<tags::CurrentActionMove>();
  return {view.begin(), view.end()};
}

types::entityVector Simulation::selectedPokemonEntities() const {
  if (hasActiveSelection<tags::SelectedForViewPokemon>()) {
    auto view = registry.view<tags::SelectedForViewPokemon, tags::Pokemon>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<tags::Pokemon>();
  return {view.begin(), view.end()};
}
}  // namespace pokesim