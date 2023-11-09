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

namespace pokesim {
struct SideStateSetup;
struct SimulateTurnOptions {};
struct CalculateDamageOptions {};
struct AnalyzeEffectOptions {};
struct SimulateTurnResults {};
struct CalculateDamageResults {};
struct AnalyzeEffectResults {};

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
    std::uint16_t turn = 0;
    std::optional<std::uint32_t> rngSeed = std::nullopt;
    float probability = 1;
    SideCreationInfo P1;
    SideCreationInfo P2;
    std::optional<SimulateTurnOptions> simulateTurnOptions = std::nullopt;
    std::optional<CalculateDamageOptions> calculateDamageOptions = std::nullopt;
    std::optional<AnalyzeEffectOptions> analyzeEffectOptions = std::nullopt;
  };

 private:
  using SideTeamSetupData = std::tuple<SideStateSetup, const SideCreationInfo*, std::vector<PokemonStateSetup>>;

  /*_inline_*/ std::vector<entt::entity> createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList);
  /*_inline_*/ PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonData);
  /*_inline_*/ std::pair<SideStateSetup, SideStateSetup> createInitialBattle(const BattleCreationInfo& battleData);

 public:
  entt::registry registry{};
  const Pokedex* pokedex = nullptr;
  const BattleFormat battleFormat = SINGLES_BATTLE_FORMAT;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_) : pokedex(&pokedex_), battleFormat(battleFormat_) {}

  // Load information about any number of battle states into the simulation's registry.
  /*_inline_*/ void createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList);
  template <auto CallbackFunction>
  /*_inline_*/ void setDecisionCallback();

  /*_inline_*/ void run();

  /*_inline_*/ SimulateTurnResults simulateTurn(std::optional<SimulateTurnOptions> options = std::nullopt);
  /*_inline_*/ CalculateDamageResults calculateDamage(std::optional<CalculateDamageOptions> options = std::nullopt);
  /*_inline_*/ AnalyzeEffectResults analyzeEffect(std::optional<AnalyzeEffectOptions> options = std::nullopt);

  /*_inline_*/ SimulateTurnOptions simulateTurn(
    std::initializer_list<BattleCreationInfo> battleDataList, std::optional<SimulateTurnOptions> options = std::nullopt) {
    createInitialStates(battleDataList);
    // remove other options and set these if there are none
    // simulateTurn(options);
    return {};
  }
  /*_inline_*/ CalculateDamageOptions calculateDamage(
    std::initializer_list<BattleCreationInfo> battleDataList, std::optional<CalculateDamageOptions> options = std::nullopt) {
    createInitialStates(battleDataList);
    // remove other options and set these if there are none
    // calculateDamage(options);
    return {};
  }
  /*_inline_*/ AnalyzeEffectOptions analyzeEffect(
    std::initializer_list<BattleCreationInfo> battleDataList, std::optional<AnalyzeEffectOptions> options = std::nullopt) {
    createInitialStates(battleDataList);
    // remove other options and set these if there are none
    // analyzeEffect(options);
    return {};
  }
};
}  // namespace pokesim