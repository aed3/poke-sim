#pragma once

#include <Components/Decisions.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Types/Entity.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/headers.hpp>
#include <entt/entity/registry.hpp>
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
struct InputSetup;
}  // namespace calc_damage
namespace analyze_effect {
struct Results;
struct InputSetup;
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
 public:
  struct MoveCreationInfo {
    dex::Move name = dex::Move::NO_MOVE;
    types::pp pp = MechanicConstants::MovePp::MIN;
    types::pp maxPp = MechanicConstants::MoveMaxPp::MIN;
  };

  struct PokemonCreationInfo {
    std::optional<types::stateId> id = std::nullopt;
    std::optional<types::stat> hp = std::nullopt;
    dex::Species species = dex::Species::NO_SPECIES;
    std::optional<SpeciesTypes> types = std::nullopt;
    dex::Item item = dex::Item::NO_ITEM;
    dex::Ability ability = dex::Ability::NO_ABILITY;
    dex::Gender gender = dex::Gender::NO_GENDER;
    dex::Status status = dex::Status::NO_STATUS;
    types::level level = MechanicConstants::PokemonLevel::MIN;

    bool fainted = false;

    dex::Nature nature = dex::Nature::NO_NATURE;
    Evs evs;
    Ivs ivs;
    struct {
      types::stat hp = MechanicConstants::PokemonHpStat::MIN;
      types::stat atk = MechanicConstants::PokemonStat::MIN;
      types::stat def = MechanicConstants::PokemonStat::MIN;
      types::stat spa = MechanicConstants::PokemonStat::MIN;
      types::stat spd = MechanicConstants::PokemonStat::MIN;
      types::stat spe = MechanicConstants::PokemonStat::MIN;
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
   private:
    struct BoostInfo {
      dex::Stat stat = dex::Stat::ATK;
      types::boost boost = MechanicConstants::PokemonStatBoost::BASE;
    };

   public:
    Slot attackerSlot = Slot::NONE;
    Slot defenderSlot = Slot::NONE;
    Slot effectTarget = Slot::NONE;
    std::vector<dex::Move> moves;
    std::optional<types::effectEnum> effect = std::nullopt;
    std::optional<BoostInfo> boostEffect = std::nullopt;
  };

  struct BattleCreationInfo {
    bool runWithSimulateTurn = false;
    bool runWithCalculateDamage = false;
    bool runWithAnalyzeEffect = false;
    types::battleTurn turn = MechanicConstants::TurnCount::MIN;
    std::optional<types::rngState> rngSeed = std::nullopt;
    types::probability probability = MechanicConstants::Probability::MAX;

    SideCreationInfo p1;
    SideCreationInfo p2;

    std::vector<TurnDecisionInfo> decisionsToSimulate;
    std::vector<CalcDamageInputInfo> damageCalculations;
    std::vector<AnalyzeEffectInputInfo> effectsToAnalyze;
  };

 private:
  types::entityVector createInitialMoves(const std::vector<MoveCreationInfo>& moveInfoList);
  PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonInfo);
  void createInitialSide(
    SideStateSetup sideSetup, const SideCreationInfo& sideInfo, const BattleCreationInfo& battleInfo);

  void createInitialTurnDecision(BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionInfo);
  void createCalcDamageInput(
    BattleStateSetup battleStateSetup, calc_damage::InputSetup& inputSetup, const CalcDamageInputInfo& inputInfo);
  void createAnalyzeEffectInput(
    BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& inputInfo,
    debug::SimulationSetupChecks& debugChecks);

  std::tuple<SideStateSetup, SideStateSetup> createInitialBattle(
    BattleStateSetup battleStateSetup, const BattleCreationInfo& battleInfo);

  BattleFormat constantBattleFormat = BattleFormat::SINGLES_BATTLE_FORMAT;
  const Pokedex* const pokedexPointer;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

 public:
  simulate_turn::Options simulateTurnOptions;
  calc_damage::Options calculateDamageOptions;
  analyze_effect::Options analyzeEffectOptions;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_);
  Simulation(Simulation&& other) noexcept;
  ~Simulation();

  const Pokedex& pokedex() const;
  constexpr BattleFormat battleFormat() const { return constantBattleFormat; }

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
};
}  // namespace pokesim