#pragma once

#include <Components/Decisions.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Types/Entity.hpp>
#include <Types/headers.hpp>
#include <entt/entity/registry.hpp>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

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
}
namespace analyze_effect {
struct Results;
}

/**
 * @brief The entry point for creating and running simulations.
 *
 * @details Each `Simulation` instance will only simulate for either single or double battles. This class is optimized
 * for running multiple simulations of the same battle, where each battle state has completed the same number of turns.
 */
class Simulation : public internal::RegistryContainer {
 public:
  struct MoveCreationInfo {
    dex::Move name = dex::Move::NO_MOVE;
    types::pp pp = 1;
    types::pp maxPp = 1;
  };

  struct PokemonCreationInfo {
    std::optional<types::stateId> id = std::nullopt;
    std::optional<types::stat> hp = std::nullopt;
    dex::Species species = dex::Species::MISSING_NO;
    std::optional<SpeciesTypes> types = std::nullopt;
    dex::Item item = dex::Item::NO_ITEM;
    dex::Ability ability = dex::Ability::NO_ABILITY;
    dex::Gender gender = dex::Gender::NO_GENDER;
    dex::Status status = dex::Status::NO_STATUS;
    types::level level = 1;

    dex::Nature nature = dex::Nature::NO_NATURE;
    Evs evs;
    Ivs ivs;
    struct {
      types::stat hp = 1;
      types::stat atk = 1;
      types::stat def = 1;
      types::stat spa = 1;
      types::stat spd = 1;
      types::stat spe = 1;
    } stats;

    std::vector<MoveCreationInfo> moves{};
  };

  struct SideCreationInfo {
    std::vector<PokemonCreationInfo> team;
  };

  struct TurnDecisionInfo {
    SideDecision p1;
    SideDecision p2;
  };

  struct CalcDamageInputInfo {
    Slot attackerSlot = Slot::NONE;
    Slot defenderSlot = Slot::NONE;
    dex::Move move = dex::Move::NO_MOVE;
  };

  struct AnalyzeEffectInputInfo {
    Slot attackerSlot = Slot::NONE;
    Slot defenderSlot = Slot::NONE;
    types::effectEnum effect;
  };

  struct BattleCreationInfo {
    bool runWithSimulateTurn = false;
    bool runWithCalculateDamage = false;
    bool runWithAnalyzeEffect = false;
    types::battleTurn turn = 0;
    std::optional<types::rngState> rngSeed = std::nullopt;
    types::probability probability = 1;

    SideCreationInfo p1;
    SideCreationInfo p2;

    std::vector<TurnDecisionInfo> decisionsToSimulate;
    std::vector<CalcDamageInputInfo> damageCalculations;
    std::vector<AnalyzeEffectInputInfo> effectsToAnalyze;
  };

 private:
  std::vector<types::entity> createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList);
  PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonData, const BattleCreationInfo& battleData);
  void createInitialSide(
    SideStateSetup sideSetup, const SideCreationInfo& sideData, const BattleCreationInfo& battleData);

  void createInitialTurnDecision(BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionData);
  void createCalcDamageInput(BattleStateSetup battleStateSetup, const CalcDamageInputInfo& damageCalcInputData);
  void createAnalyzeEffectInput(
    BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& analyzeEffectInputData);

  std::tuple<SideStateSetup, SideStateSetup> createInitialBattle(
    BattleStateSetup battleStateSetup, const BattleCreationInfo& battleData);

 public:
  const BattleFormat battleFormat = BattleFormat::SINGLES_BATTLE_FORMAT;
  const Pokedex& pokedex;

  simulate_turn::Options simulateTurnOptions;
  calc_damage::Options calculateDamageOptions;
  analyze_effect::Options analyzeEffectOptions;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_);

  // Load information about any number of battle states into the simulation's registry.
  void createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList);

  void run();

  simulate_turn::Results simulateTurn(std::optional<simulate_turn::Options> options = std::nullopt);
  calc_damage::Results calculateDamage(std::optional<calc_damage::Options> options = std::nullopt);
  analyze_effect::Results analyzeEffect(std::optional<analyze_effect::Options> options = std::nullopt);

  simulate_turn::Results simulateTurn(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<simulate_turn::Options> options = std::nullopt);

  calc_damage::Results calculateDamage(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<calc_damage::Options> options = std::nullopt);

  analyze_effect::Results analyzeEffect(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<analyze_effect::Options> options = std::nullopt);

  void clearAllResults();
  void clearSimulateTurnResults();
  void clearCalculateDamageResults();
  void clearAnalyzeEffectResults();

  std::vector<types::entity> selectedBattleEntities();
};
}  // namespace pokesim