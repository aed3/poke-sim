#pragma once

#include <Components/Tags/SimulationTags.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <entt/entity/registry.hpp>
#include <optional>
#include <tuple>
#include <utility>

#include "BattleCreationInfo.hpp"
#include "RegistryContainer.hpp"
#include "SimulationOptions.hpp"

namespace pokesim {
struct SideStateSetup;
struct PokemonStateSetup;
struct BattleStateSetup;
class Pokedex;

namespace simulate_turn {
struct Results;
}
namespace calc_damage {
struct Results;
}  // namespace calc_damage
namespace analyze_effect {
struct Results;
}  // namespace analyze_effect
namespace debug {
struct SimulationSetupChecks;
}

/**
 * @brief The entry point for creating and running simulations.
 *
 * @details Each `Simulation` instance will only simulate for either single or double battles. This class is optimized
 * for running multiple simulations of the same battle, where each battle state has completed the same number of turns.
 */
class Simulation : public internal::RegistryContainer {
 private:
  types::entityVector createInitialMoves(const std::vector<MoveCreationInfo>& moveInfoList);
  PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonInfo);
  void createInitialSide(
    SideStateSetup sideSetup, const SideCreationInfo& sideInfo, const BattleCreationInfo& battleInfo);

  void createInitialTurnDecision(BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionInfo);
  void createCalcDamageInput(
    BattleStateSetup battleStateSetup, const CalcDamageInputInfo& inputInfo, debug::SimulationSetupChecks& debugChecks);
  void createAnalyzeEffectInput(
    BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& inputInfo,
    debug::SimulationSetupChecks& debugChecks);

  std::tuple<SideStateSetup, SideStateSetup> createInitialBattle(
    BattleStateSetup battleStateSetup, const BattleCreationInfo& battleInfo);

 private:
  struct Constants {
    Constants(const Pokedex& pokedex_, BattleFormat battleFormat_)
        : battleFormatValue(battleFormat_), pokedexValue(&pokedex_) {}
    constexpr bool isBattleFormat(BattleFormat checkedFormat) const { return checkedFormat == battleFormatValue; }
    constexpr const Pokedex& pokedex() const { return *pokedexValue; }

   private:
    BattleFormat battleFormatValue;
    const Pokedex* pokedexValue;
  } constants;
  Simulation(const Constants& other);

 public:
  simulate_turn::Options simulateTurnOptions;
  calc_damage::Options calculateDamageOptions;
  analyze_effect::Options analyzeEffectOptions;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_);
  Simulation(Simulation&& other) noexcept;
  ~Simulation();

  const Pokedex& pokedex() const;
  constexpr bool isBattleFormat(BattleFormat checkedFormat) { return constants.isBattleFormat(checkedFormat); }

  // Load information about any number of battle states into the simulation's registry.
  void createInitialStates(const std::vector<BattleCreationInfo>& battleInfoList);

  void run();

  simulate_turn::Results simulateTurn(std::optional<simulate_turn::Options> options = std::nullopt);
  calc_damage::Results calculateDamage(std::optional<calc_damage::Options> options = std::nullopt);
  analyze_effect::Results analyzeEffect(std::optional<analyze_effect::Options> options = std::nullopt);

  simulate_turn::Results simulateTurn(
    const std::vector<BattleCreationInfo>& battleInfoList,
    std::optional<simulate_turn::Options> options = std::nullopt);

  calc_damage::Results calculateDamage(
    const std::vector<BattleCreationInfo>& battleInfoList, std::optional<calc_damage::Options> options = std::nullopt);

  analyze_effect::Results analyzeEffect(
    const std::vector<BattleCreationInfo>& battleInfoList,
    std::optional<analyze_effect::Options> options = std::nullopt);

  void clearAllResults();
  void clearSimulateTurnResults();
  void clearCalculateDamageResults();
  void clearAnalyzeEffectResults();

  types::entityVector selectedBattleEntities() const;
  types::entityVector selectedMoveEntities() const;
  types::entityVector selectedPokemonEntities() const;

  template <template <typename> typename RunStruct, typename... RunFunctionArgs>
  static void forEachSimulationTag(RunFunctionArgs&&... args) {
    RunStruct<tags::SimulateTurn>::run(std::forward<RunFunctionArgs>(args)...);
    RunStruct<tags::CalculateDamage>::run(std::forward<RunFunctionArgs>(args)...);
    RunStruct<tags::AnalyzeEffect>::run(std::forward<RunFunctionArgs>(args)...);
  }
};
}  // namespace pokesim
