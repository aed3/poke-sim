#pragma once

#include <Components/EVsIVs.hpp>
#include <Pokedex/Pokedex.hpp>
#include <entt/entity/registry.hpp>
#include <Battle/Setup/SideStateSetup.hpp>

namespace pokesim {
class Simulation {
 private:
  struct InputMove {
    dex::Move name = dex::NO_MOVE;
    std::uint8_t pp = 1;
    std::uint8_t maxPP = 1;
  };

  struct InputPokemon {
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

    std::vector<InputMove> moves{};
  };

  struct InputSide {
    std::vector<InputPokemon> team;
  };

  struct InputBattle {
    std::uint16_t turn = 0;
    std::uint32_t rngSeed = 0;
    float probability = 1;
    InputSide P1;
    InputSide P2;
  };

 private:
  using SideTeamSetupData = std::tuple<SideStateSetup, const InputSide*, std::vector<PokemonStateSetup>>;

  std::vector<entt::entity> createInitialMoves(const std::vector<InputMove>& moveDataList);
  PokemonStateSetup createInitialPokemon(const InputPokemon& pokemonData);
  std::pair<SideStateSetup, SideStateSetup> createInitialBattle(const InputBattle& battleData);

 public:
  entt::registry registry{};
  const Pokedex* pokedex = nullptr;
  const BattleFormat battleFormat = SINGLES_BATTLE_FORMAT;

  Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_) : pokedex(&pokedex_), battleFormat(battleFormat_) {}

  void createInitialStates(std::initializer_list<InputBattle> battleDataList);
};
}  // namespace pokesim