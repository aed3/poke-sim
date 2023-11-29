#include <Battle/Actions/Action.hpp>
#include <Battle/Actions/Decisions.hpp>
#include <Battle/Setup/PokemonStateSetup.hpp>
#include <Battle/Setup/headers.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <cstddef>
#include <entt/entity/registry.hpp>
#include <initializer_list>
#include <utility>
#include <vector>

#include "Simulation.hpp"

namespace pokesim {
Simulation::Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_)
    : battleFormat(battleFormat_), pokedex(pokedex_) {}

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
  if (pokemonData.gender != dex::Gender::NO_GENDER) pokemonSetup.setGender(pokemonData.gender);
  if (pokemonData.ability != dex::Ability::NO_ABILITY) pokemonSetup.setAbility(pokemonData.ability);
  if (pokemonData.item != dex::Item::NO_ITEM) pokemonSetup.setItem(pokemonData.item);
  if (pokemonData.nature != dex::Nature::NO_NATURE) pokemonSetup.setNature(pokemonData.nature);
  if (pokemonData.status != dex::Status::NO_STATUS) pokemonSetup.setStatus(pokemonData.status);

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

void Simulation::createInitialSide(SideStateSetup sideSetup, const SideCreationInfo& sideData) {
  std::vector<PokemonStateSetup> pokemonSetupList;
  pokemonSetupList.reserve(sideData.team.size());

  for (const PokemonCreationInfo& pokemonData : sideData.team) {
    PokemonStateSetup pokemonSetup = createInitialPokemon(pokemonData);
    std::vector<entt::entity> moveEntities = createInitialMoves(pokemonData.moves);
    pokemonSetup.setMoves(moveEntities);

    pokemonSetupList.push_back(pokemonSetup);
  }

  sideSetup.setTeam(pokemonSetupList);
}

std::tuple<SideStateSetup, SideStateSetup> Simulation::createInitialBattle(
  BattleStateSetup battleStateSetup, const BattleCreationInfo& battleData) {
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

  battleStateSetup.setSide(PlayerSideId::P1, p1Entity);
  battleStateSetup.setSide(PlayerSideId::P2, p2Entity);

  p1SideSetup.setOpponent(p2Entity);
  p2SideSetup.setOpponent(p1Entity);

  p1SideSetup.setBattle(battleEntity);
  p2SideSetup.setBattle(battleEntity);

  return {p1SideSetup, p2SideSetup};
}

void Simulation::createInitialTurnDecision(
  BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionData) {
  entt::handle battleHandle{registry, battleStateSetup.entity()};
  const Sides& sides = battleHandle.get<Sides>();

  resolveDecision({registry, sides.p1}, turnDecisionData.p1, registry.get<ActionQueue>(sides.p1));
  resolveDecision({registry, sides.p2}, turnDecisionData.p2, registry.get<ActionQueue>(sides.p2));

  moveSideActionsToBattleActions(battleHandle, sides, battleHandle.get<ActionQueue>());
}

void Simulation::createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList) {
  for (const BattleCreationInfo& battleData : battleDataList) {
    BattleStateSetup battleStateSetup(registry);
    auto [p1SideSetup, p2SideSetup] = createInitialBattle(battleStateSetup, battleData);

    createInitialSide(p1SideSetup, battleData.p1);
    createInitialSide(p2SideSetup, battleData.p2);

    if (!battleData.decisionsToSimulate.empty()) {
      createInitialTurnDecision(battleStateSetup, battleData.decisionsToSimulate[0]);

      for (std::size_t i = 1; i < battleData.decisionsToSimulate.size(); i++) {
        createInitialTurnDecision(battleStateSetup.clone(), battleData.decisionsToSimulate[i]);
      }
    }
  }
}
}  // namespace pokesim
