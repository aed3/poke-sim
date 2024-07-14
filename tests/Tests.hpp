#pragma once

#ifdef USE_SINGLE_HEADER
#include "../include/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>
#include <entt/entt.hpp>

namespace pokesim {
inline Simulation createSingleBattleSimulation(
  Simulation::BattleCreationInfo& battleCreationInfo, bool addTsDecisions) {
  Pokedex* pokedex = new Pokedex(GameMechanics::SCARLET_VIOLET);
  entt::dense_set<dex::Move> moveSet{};
  for (dex::Move move : {dex::Move::FURY_ATTACK, dex::Move::THUNDERBOLT}) moveSet.insert(move);

  pokedex->loadMoves(moveSet);

  Simulation simulation(*pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  {
    Simulation::PokemonCreationInfo p1PokemonInfo{};
    p1PokemonInfo.species = dex::Species::EMPOLEON;
    p1PokemonInfo.item = dex::Item::ASSAULT_VEST;
    p1PokemonInfo.ability = dex::Ability::DEFIANT;
    p1PokemonInfo.gender = dex::Gender::MALE;
    p1PokemonInfo.status = dex::Status::PAR;
    p1PokemonInfo.level = 99;
    p1PokemonInfo.evs = {0, 50, 100, 150, 200, 255};
    p1PokemonInfo.stats = {309, 208, 212, 258, 238, 156};

    Simulation::MoveCreationInfo p1MoveInfo{};
    p1MoveInfo.name = dex::Move::FURY_ATTACK;
    p1MoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::FURY_ATTACK).val;
    p1MoveInfo.pp = p1MoveInfo.maxPp - 1;
    p1PokemonInfo.moves.push_back(p1MoveInfo);

    Simulation::PokemonCreationInfo p2PokemonInfo{};
    p2PokemonInfo.species = dex::Species::AMPHAROS;
    p2PokemonInfo.item = dex::Item::CHOICE_SPECS;
    p2PokemonInfo.ability = dex::Ability::STATIC;
    p2PokemonInfo.gender = dex::Gender::FEMALE;
    p2PokemonInfo.level = 100;
    p2PokemonInfo.nature = dex::Nature::MODEST;
    p2PokemonInfo.ivs = {5, 10, 15, 20, 25, 30};
    p2PokemonInfo.stats = {321, 186, 206, 266, 216, 146};

    Simulation::MoveCreationInfo p2MoveInfo{};
    p2MoveInfo.name = dex::Move::THUNDERBOLT;
    p2MoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::THUNDERBOLT).val;
    p2MoveInfo.pp = p2MoveInfo.maxPp - 2;
    p2PokemonInfo.moves.push_back(p2MoveInfo);

    battleCreationInfo.p1 = {{p1PokemonInfo}};
    battleCreationInfo.p2 = {{p2PokemonInfo}};

    entt::dense_set<dex::Species> speciesSet{};
    speciesSet.insert(p1PokemonInfo.species);
    speciesSet.insert(p2PokemonInfo.species);
    pokedex->loadSpecies(speciesSet);

    if (addTsDecisions) {
      SideDecision p1Decision{PlayerSideId::P1};
      SideDecision p2Decision{PlayerSideId::P2};
      SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
      SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};

      p1SlotDecision.moveChoice = dex::Move::FURY_ATTACK;
      p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
      p2SlotDecision.moveChoice = dex::Move::THUNDERBOLT;
      p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};
      battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
    }
  }

  return simulation;
}

inline Simulation createDoubleBattleSimulation(
  Simulation::BattleCreationInfo& battleCreationInfo, bool /*addTsDecisions*/) {
  Pokedex* pokedex = new Pokedex(GameMechanics::SCARLET_VIOLET);
  entt::dense_set<dex::Move> moveSet{};
  for (dex::Move move : {dex::Move::MOONBLAST, dex::Move::KNOCK_OFF, dex::Move::WILL_O_WISP, dex::Move::QUIVER_DANCE}) {
    moveSet.insert(move);
  }

  pokedex->loadMoves(moveSet);

  Simulation simulation(*pokedex, BattleFormat::DOUBLES_BATTLE_FORMAT);

  {
    Simulation::PokemonCreationInfo p1aPokemonInfo{};
    p1aPokemonInfo.species = dex::Species::GARDEVOIR;
    p1aPokemonInfo.ability = dex::Ability::TRACE;
    p1aPokemonInfo.gender = dex::Gender::FEMALE;
    p1aPokemonInfo.item = dex::Item::CHOICE_SCARF;
    p1aPokemonInfo.status = dex::Status::BRN;
    p1aPokemonInfo.level = 90;
    p1aPokemonInfo.stats = {277, 166, 166, 286, 266, 196};
    p1aPokemonInfo.evs = {0, 50, 100, 150, 200, 255};

    Simulation::MoveCreationInfo p1aMoveInfo{};
    p1aMoveInfo.name = dex::Move::MOONBLAST;
    p1aMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::MOONBLAST).val;
    p1aMoveInfo.pp = p1aMoveInfo.maxPp - 4;
    p1aPokemonInfo.moves.push_back(p1aMoveInfo);

    Simulation::PokemonCreationInfo p2aPokemonInfo{};
    p2aPokemonInfo.species = dex::Species::PANGORO;
    p2aPokemonInfo.ability = dex::Ability::IRON_FIST;
    p2aPokemonInfo.gender = dex::Gender::MALE;
    p2aPokemonInfo.item = dex::Item::LIFE_ORB;
    p2aPokemonInfo.level = 95;
    p2aPokemonInfo.stats = {331, 284, 192, 174, 178, 152};
    p2aPokemonInfo.evs = {255, 200, 150, 100, 50, 0};

    Simulation::MoveCreationInfo p2aMoveInfo{};
    p2aMoveInfo.name = dex::Move::KNOCK_OFF;
    p2aMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::KNOCK_OFF).val;
    p2aMoveInfo.pp = p2aMoveInfo.maxPp - 3;
    p2aPokemonInfo.moves.push_back(p2aMoveInfo);

    Simulation::PokemonCreationInfo p1bPokemonInfo{};
    p1bPokemonInfo.species = dex::Species::DRAGAPULT;
    p1bPokemonInfo.ability = dex::Ability::INFILTRATOR;
    p1bPokemonInfo.gender = dex::Gender::FEMALE;
    p1bPokemonInfo.item = dex::Item::FOCUS_SASH;
    p1bPokemonInfo.level = 100;
    p1bPokemonInfo.nature = dex::Nature::HASTY;
    p1bPokemonInfo.stats = {217, 276, 186, 236, 186, 320};
    p1bPokemonInfo.ivs = {5, 10, 15, 20, 25, 30};

    Simulation::MoveCreationInfo p1bMoveInfo{};
    p1bMoveInfo.name = dex::Move::WILL_O_WISP;
    p1bMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::WILL_O_WISP).val;
    p1bMoveInfo.pp = p1bMoveInfo.maxPp - 2;
    p1bPokemonInfo.moves.push_back(p1bMoveInfo);

    Simulation::PokemonCreationInfo p2bPokemonInfo{};
    p2bPokemonInfo.species = dex::Species::RIBOMBEE;
    p2bPokemonInfo.ability = dex::Ability::SWEET_VEIL;
    p2bPokemonInfo.gender = dex::Gender::MALE;
    p2bPokemonInfo.item = dex::Item::BRIGHT_POWDER;
    p2bPokemonInfo.level = 91;
    p2bPokemonInfo.stats = {261, 146, 156, 226, 176, 284};
    p2bPokemonInfo.ivs = {30, 25, 20, 15, 10, 5};

    Simulation::MoveCreationInfo p2bMoveInfo{};
    p2bMoveInfo.name = dex::Move::QUIVER_DANCE;
    p2bMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::QUIVER_DANCE).val;
    p2bMoveInfo.pp = p2bMoveInfo.maxPp - 1;
    p2bPokemonInfo.moves.push_back(p2bMoveInfo);

    battleCreationInfo.p1 = {{p1aPokemonInfo, p1bPokemonInfo}};
    battleCreationInfo.p2 = {{p2aPokemonInfo, p2bPokemonInfo}};

    entt::dense_set<dex::Species> speciesSet{};
    speciesSet.insert(p1aPokemonInfo.species);
    speciesSet.insert(p1bPokemonInfo.species);
    speciesSet.insert(p2aPokemonInfo.species);
    speciesSet.insert(p2bPokemonInfo.species);
    pokedex->loadSpecies(speciesSet);
  }

  battleCreationInfo.turn = 2;
  battleCreationInfo.probability = 0.9F;
  battleCreationInfo.rngSeed = 0x12345678;

  return simulation;
}
}  // namespace pokesim
