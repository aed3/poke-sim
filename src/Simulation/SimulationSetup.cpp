#include <AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Setup/PokemonStateSetup.hpp>
#include <Battle/Setup/headers.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/CalcDamage/AttackerDefender.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <SimulateTurn/Actions/Decisions.hpp>
#include <SimulateTurn/Actions/ResolveDecision.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <Types/Utilities/Variant.hpp>
#include <cstddef>
#include <entt/entity/registry.hpp>
#include <initializer_list>
#include <utility>
#include <vector>

#include "Simulation.hpp"

namespace pokesim {
Simulation::Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_)
    : battleFormat(battleFormat_), pokedex(pokedex_) {}

std::vector<types::entity> Simulation::createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList) {
  std::vector<types::entity> moveEntities{};
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
    std::vector<types::entity> moveEntities = createInitialMoves(pokemonData.moves);
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

  types::entity battleEntity = battleStateSetup.entity();
  types::entity p1Entity = p1SideSetup.entity();
  types::entity p2Entity = p2SideSetup.entity();

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
  types::handle battleHandle{registry, battleStateSetup.entity()};
  const Sides& sides = battleHandle.get<Sides>();

  resolveDecision({registry, sides.p1}, turnDecisionData.p1, battleHandle.get<ActionQueue>());
  resolveDecision({registry, sides.p2}, turnDecisionData.p2, battleHandle.get<ActionQueue>());
}

void Simulation::createCalcDamageInput(
  BattleStateSetup battleStateSetup, const CalcDamageInputInfo& damageCalcInputData) {
  ENTT_ASSERT(damageCalcInputData.attackerSlot != Slot::NONE, "A damage calculation must have a attacker");
  ENTT_ASSERT(damageCalcInputData.defenderSlot != Slot::NONE, "A damage calculation must have a defender");
  ENTT_ASSERT(damageCalcInputData.move != dex::Move::NO_MOVE, "A damage calculation must have a move");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToEntity(registry, sides, damageCalcInputData.attackerSlot);
  types::entity defenderEntity = slotToEntity(registry, sides, damageCalcInputData.defenderSlot);

  calc_damage::InputSetup inputSetup(registry);
  inputSetup.setAttacker(attackerEntity);
  inputSetup.setDefender(defenderEntity);
  inputSetup.setMove(damageCalcInputData.move);
  inputSetup.setBattle(battleStateSetup.entity());
}

void Simulation::createAnalyzeEffectInput(
  BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& analyzeEffectInputData) {
  ENTT_ASSERT(analyzeEffectInputData.attackerSlot != Slot::NONE, "An effect analysis must have a attacker");
  ENTT_ASSERT(analyzeEffectInputData.defenderSlot != Slot::NONE, "An effect analysis must have a defender");
  ENTT_ASSERT(!analyzeEffectInputData.effect.empty(), "An effect analysis must have an effect");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToEntity(registry, sides, analyzeEffectInputData.attackerSlot);
  types::entity defenderEntity = slotToEntity(registry, sides, analyzeEffectInputData.defenderSlot);

  analyze_effect::InputSetup inputSetup(registry);
  inputSetup.setAttacker(attackerEntity);
  inputSetup.setDefender(defenderEntity);
  inputSetup.setEffect(analyzeEffectInputData.effect);
  inputSetup.setBattle(battleStateSetup.entity());
}

void Simulation::createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList) {
  for (const BattleCreationInfo& battleData : battleDataList) {
    BattleStateSetup battleStateSetup(registry);
    auto [p1SideSetup, p2SideSetup] = createInitialBattle(battleStateSetup, battleData);

    createInitialSide(p1SideSetup, battleData.p1);
    createInitialSide(p2SideSetup, battleData.p2);

    if (!battleData.decisionsToSimulate.empty()) {
      ENTT_ASSERT(
        battleData.decisionsToSimulate.size() < std::numeric_limits<types::cloneIndex>::max(),
        "Cannot make more clones than their are entities");

      types::cloneIndex cloneCount = (types::cloneIndex)(battleData.decisionsToSimulate.size() - 1);
      if (cloneCount) {
        std::vector<BattleStateSetup> clones = battleStateSetup.clone(cloneCount);

        for (types::cloneIndex i = 0; i < cloneCount; i++) {
          createInitialTurnDecision(clones[i], battleData.decisionsToSimulate[i]);
          clones[i].setID(i);
        }
      }

      createInitialTurnDecision(battleStateSetup, *battleData.decisionsToSimulate.rbegin());
      battleStateSetup.setID(cloneCount);
    }

    for (const CalcDamageInputInfo& damageCalcInputData : battleData.damageCalculations) {
      createCalcDamageInput(battleStateSetup, damageCalcInputData);
    }

    for (const AnalyzeEffectInputInfo& analyzeEffectInputData : battleData.effectsToAnalyze) {
      createAnalyzeEffectInput(battleStateSetup, analyzeEffectInputData);
    }
  }
}
}  // namespace pokesim
