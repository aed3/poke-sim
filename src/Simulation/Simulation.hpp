#pragma once

#include <Battle/Setup/PokemonStateSetup.hpp>
#include <Components/EVsIVs.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/headers.hpp>
#include <cstdint>
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
    dex::Move name = dex::NO_MOVE;
    std::uint8_t pp = 1;
    std::uint8_t maxPP = 1;
  };

  struct PokemonCreationInfo {
    std::optional<std::uint16_t> id = std::nullopt;
    dex::Species species = dex::MISSING_NO;
    dex::Item item = dex::NO_ITEM;
    dex::Ability ability = dex::NO_ABILITY;
    dex::Gender gender = dex::NO_GENDER;
    dex::Status status = dex::NO_STATUS;
    std::uint8_t level = 1;

    dex::Nature nature = dex::NO_NATURE;
    EVs evs;
    IVs ivs;
    struct {
      std::uint16_t hp = 1;
      std::uint16_t atk = 1;
      std::uint16_t def = 1;
      std::uint16_t spa = 1;
      std::uint16_t spd = 1;
      std::uint16_t spe = 1;
    } stats;

    std::vector<MoveCreationInfo> moves{};
  };

  struct SideCreationInfo {
    std::vector<PokemonCreationInfo> team;
  };

  struct BattleCreationInfo {
    bool runWithSimulateTurn = false;
    bool runWithCalculateDamage = false;
    bool runWithAnalyzeEffect = false;
    std::uint16_t turn = 0;
    std::optional<std::uint32_t> rngSeed = std::nullopt;
    float probability = 1;
    SideCreationInfo P1;
    SideCreationInfo P2;
  };

 private:
  using SideTeamSetupData = std::tuple<SideStateSetup, const SideCreationInfo*, std::vector<PokemonStateSetup>>;

  /*_inline_*/ std::vector<entt::entity> createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList);
  /*_inline_*/ PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonData);
  /*_inline_*/ std::pair<SideStateSetup, SideStateSetup> createInitialBattle(const BattleCreationInfo& battleData);

 public:
  const BattleFormat battleFormat = SINGLES_BATTLE_FORMAT;
  const Pokedex& pokedex;
  entt::registry registry{};

  simulate_turn::Options simulateTurnOptions;
  calc_damage::Options calculateDamageOptions;
  analyze_effect::Options analyzeEffectOptions;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_) : battleFormat(battleFormat_), pokedex(pokedex_) {}

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