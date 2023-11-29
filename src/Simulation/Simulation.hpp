#pragma once

#include <Battle/Actions/Decisions.hpp>
#include <Components/EVsIVs.hpp>
#include <Types/headers.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

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
class Simulation {
 public:
  struct MoveCreationInfo {
    dex::Move name = dex::Move::NO_MOVE;
    types::Pp pp = 1;
    types::Pp maxPp = 1;
  };

  struct PokemonCreationInfo {
    std::optional<types::StateId> id = std::nullopt;
    dex::Species species = dex::Species::MISSING_NO;
    dex::Item item = dex::Item::NO_ITEM;
    dex::Ability ability = dex::Ability::NO_ABILITY;
    dex::Gender gender = dex::Gender::NO_GENDER;
    dex::Status status = dex::Status::NO_STATUS;
    types::Level level = 1;

    dex::Nature nature = dex::Nature::NO_NATURE;
    Evs evs;
    Ivs ivs;
    struct {
      types::Stat hp = 1;
      types::Stat atk = 1;
      types::Stat def = 1;
      types::Stat spa = 1;
      types::Stat spd = 1;
      types::Stat spe = 1;
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

  struct BattleCreationInfo {
    bool runWithSimulateTurn = false;
    bool runWithCalculateDamage = false;
    bool runWithAnalyzeEffect = false;
    types::BattleTurn turn = 0;
    std::optional<types::StateRngSeed> rngSeed = std::nullopt;
    types::StateProbability probability = 1;

    SideCreationInfo p1;
    SideCreationInfo p2;

    std::vector<TurnDecisionInfo> decisionsToSimulate;
  };

 private:
  /*_inline_*/ std::vector<entt::entity> createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList);
  /*_inline_*/ PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonData);
  /*_inline_*/ void createInitialSide(SideStateSetup sideSetup, const SideCreationInfo& sideData);
  /*_inline_*/ void createInitialTurnDecision(
    BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionData);
  /*_inline_*/ std::tuple<SideStateSetup, SideStateSetup> createInitialBattle(
    BattleStateSetup battleStateSetup, const BattleCreationInfo& battleData);

 public:
  const BattleFormat battleFormat = BattleFormat::SINGLES_BATTLE_FORMAT;
  const Pokedex& pokedex;
  entt::registry registry{};

  simulate_turn::Options simulateTurnOptions;
  calc_damage::Options calculateDamageOptions;
  analyze_effect::Options analyzeEffectOptions;

  /*_inline_*/ Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_);

  // Load information about any number of battle states into the simulation's registry.
  /*_inline_*/ void createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList);

  /*_inline_*/ void run();

  /*_inline_*/ simulate_turn::Results simulateTurn(std::optional<simulate_turn::Options> options = std::nullopt);
  /*_inline_*/ calc_damage::Results calculateDamage(std::optional<calc_damage::Options> options = std::nullopt);
  /*_inline_*/ analyze_effect::Results analyzeEffect(std::optional<analyze_effect::Options> options = std::nullopt);

  /*_inline_*/ simulate_turn::Results simulateTurn(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<simulate_turn::Options> options = std::nullopt);

  /*_inline_*/ calc_damage::Results calculateDamage(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<calc_damage::Options> options = std::nullopt);

  /*_inline_*/ analyze_effect::Results analyzeEffect(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<analyze_effect::Options> options = std::nullopt);

  /*_inline_*/ void clearAllResults();
  /*_inline_*/ void clearSimulateTurnResults();
  /*_inline_*/ void clearCalculateDamageResults();
  /*_inline_*/ void clearAnalyzeEffectResults();
};
}  // namespace pokesim