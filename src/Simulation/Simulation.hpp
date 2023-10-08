#pragma once

#include <Battle/Setup/SideStateSetup.hpp>
#include <Components/EVsIVs.hpp>
#include <Pokedex/Pokedex.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
class Simulation {
 public:
  struct MoveCreationInfo {
    dex::Move name = dex::NO_MOVE;
    std::uint8_t pp = 1;
    std::uint8_t maxPP = 1;
  };

  struct PokemonCreationInfo {
    std::uint16_t id = 0;
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
    std::uint32_t rngSeed = 0;
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
  entt::registry registry{};
  const Pokedex* pokedex = nullptr;
  const BattleFormat battleFormat = SINGLES_BATTLE_FORMAT;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_) : pokedex(&pokedex_), battleFormat(battleFormat_) {}

  /*_inline_*/ void createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList);
};
}  // namespace pokesim