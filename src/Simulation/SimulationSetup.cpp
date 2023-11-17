#include <Battle/Setup/headers.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <algorithm>
#include <cstddef>

#include "Simulation.hpp"
#include "SimulationOptions.hpp"

namespace pokesim {
std::vector<entt::entity> Simulation::createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList) {
  std::vector<entt::entity> moveEntities{};
  moveEntities.reserve(moveDataList.size());

  for (const MoveCreationInfo& moveData : moveDataList) {
    MoveStateSetup moveSetup(registry);
    moveSetup.setName(moveData.name);
    moveSetup.setPP(moveData.pp);
    moveSetup.setMaxPP(moveData.maxPp);
    moveEntities.push_back(moveSetup.entity());
  }

  return moveEntities;
}

PokemonStateSetup Simulation::createInitialPokemon(const PokemonCreationInfo& pokemonData) {
  PokemonStateSetup pokemonSetup(registry);
  if (pokemonData.id.has_value()) {
    pokemonSetup.setID(pokemonData.id.value());
  }
  else {
    pokemonSetup.setAutoID();
  }

  pokemonSetup.setSpecies(pokemonData.species);
  pokemonSetup.setLevel(pokemonData.level);
  if (pokemonData.gender) pokemonSetup.setGender(pokemonData.gender);
  if (pokemonData.ability) pokemonSetup.setAbility(pokemonData.ability);
  if (pokemonData.item) pokemonSetup.setItem(pokemonData.item);
  if (pokemonData.nature) pokemonSetup.setNature(pokemonData.nature);
  if (pokemonData.status) pokemonSetup.setStatus(pokemonData.status);

  pokemonSetup.setEVs(pokemonData.evs);
  pokemonSetup.setIVs(pokemonData.ivs);
  pokemonSetup.setStat<stat::Hp>(pokemonData.stats.hp);
  pokemonSetup.setStat<stat::Atk>(pokemonData.stats.atk);
  pokemonSetup.setStat<stat::Def>(pokemonData.stats.def);
  pokemonSetup.setStat<stat::Spa>(pokemonData.stats.spa);
  pokemonSetup.setStat<stat::Spd>(pokemonData.stats.spd);
  pokemonSetup.setStat<stat::Spe>(pokemonData.stats.spe);

  return pokemonSetup;
}

std::pair<SideStateSetup, SideStateSetup> Simulation::createInitialBattle(const BattleCreationInfo& battleData) {
  BattleStateSetup battleStateSetup(registry);
  battleStateSetup.setAutoID();
  battleStateSetup.setTurn(battleData.turn);
  battleStateSetup.setRNGSeed(battleData.rngSeed);
  battleStateSetup.setProbability(battleData.probability);

  if (battleData.runWithSimulateTurn) {
    battleStateSetup.setProperty<tags::SimulateTurn>();
  }
  if (battleData.runWithCalculateDamage) {
    battleStateSetup.setProperty<tags::CalculateDamage>();
  }
  if (battleData.runWithAnalyzeEffect) {
    battleStateSetup.setProperty<tags::AnalyzeEffect>();
  }

  SideStateSetup p1SideSetup(registry);
  SideStateSetup p2SideSetup(registry);

  entt::entity battleEntity = battleStateSetup.entity();
  entt::entity p1Entity = p1SideSetup.entity();
  entt::entity p2Entity = p2SideSetup.entity();

  battleStateSetup.setSide(Side::P1, p1Entity);
  battleStateSetup.setSide(Side::P2, p2Entity);

  p1SideSetup.setOpponent(p2Entity);
  p2SideSetup.setOpponent(p1Entity);

  p1SideSetup.setBattle(battleEntity);
  p2SideSetup.setBattle(battleEntity);

  return {p1SideSetup, p2SideSetup};
}

void Simulation::createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList) {
  std::vector<SideTeamSetupData> sideTeamSetupData{};
  sideTeamSetupData.reserve(battleDataList.size() * 2);

  for (const BattleCreationInfo& battleData : battleDataList) {
    auto [p1SideSetup, p2SideSetup] = createInitialBattle(battleData);

    sideTeamSetupData.push_back({p1SideSetup, &battleData.P1, {}});
    sideTeamSetupData.push_back({p2SideSetup, &battleData.P2, {}});
  }

  for (auto& [sideSetup, sideData, pokemonSetupList] : sideTeamSetupData) {
    pokemonSetupList.reserve(sideData->team.size());
    for (const PokemonCreationInfo& pokemonData : sideData->team) {
      pokemonSetupList.push_back(createInitialPokemon(pokemonData));
    }
  }

  for (auto& [sideSetup, sideData, pokemonSetupList] : sideTeamSetupData) {
    sideSetup.setTeam(pokemonSetupList);
  }

  for (auto& [sideSetup, sideData, pokemonSetupList] : sideTeamSetupData) {
    for (std::size_t i = 0; i < pokemonSetupList.size(); i++) {
      std::vector<entt::entity> moveEntities = createInitialMoves(sideData->team[i].moves);
      pokemonSetupList[i].setMoves(moveEntities);
    }
  }
}
}  // namespace pokesim
