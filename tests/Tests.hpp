#pragma once

#ifdef POKESIM_USE_SINGLE_HEADER_FILE
#include "../include/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>
#include <entt/entt.hpp>

#ifndef POKESIM_TESTING
static_assert(false, "Rebuild PokeSim with the flag POKESIM_ENABLE_TESTING to enable testing.");
#endif

namespace pokesim {
inline Simulation createSingleBattleSimulation(Simulation::BattleCreationInfo& battleCreationInfo) {
  static Pokedex* pokedex = nullptr;
  if (!pokedex) {
    pokedex = new Pokedex(GameMechanics::SCARLET_VIOLET);
    entt::dense_set<dex::Move> moveSet{};
    for (dex::Move move : {dex::Move::FURY_ATTACK, dex::Move::THUNDERBOLT, dex::Move::KNOCK_OFF}) moveSet.insert(move);
    pokedex->loadMoves(moveSet);

    entt::dense_set<dex::Species> speciesSet{};
    for (dex::Species species : {dex::Species::EMPOLEON, dex::Species::AMPHAROS}) speciesSet.insert(species);
    pokedex->loadSpecies(speciesSet);
  }

  Simulation simulation(*pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  {
    Simulation::PokemonCreationInfo p1PokemonInfo{};
    p1PokemonInfo.species = dex::Species::EMPOLEON;
    p1PokemonInfo.item = dex::Item::ASSAULT_VEST;
    p1PokemonInfo.ability = dex::Ability::DEFIANT;
    p1PokemonInfo.gender = dex::Gender::MALE;
    p1PokemonInfo.status = dex::Status::PAR;
    p1PokemonInfo.level = 99U;
    p1PokemonInfo.evs = {0U, 25U, 50U, 75U, 100U, 125U};
    p1PokemonInfo.stats = {275U, 181U, 191U, 242U, 229U, 154U};

    Simulation::MoveCreationInfo p1MoveInfoA{};
    p1MoveInfoA.name = dex::Move::FURY_ATTACK;
    p1MoveInfoA.maxPp = pokedex->getMoveData<Pp>(dex::Move::FURY_ATTACK).val;
    p1MoveInfoA.pp = p1MoveInfoA.maxPp - 1U;
    p1PokemonInfo.moves.push_back(p1MoveInfoA);
    Simulation::MoveCreationInfo p1MoveInfoB{};
    p1MoveInfoB.name = dex::Move::KNOCK_OFF;
    p1MoveInfoB.maxPp = pokedex->getMoveData<Pp>(dex::Move::KNOCK_OFF).val;
    p1MoveInfoB.pp = p1MoveInfoB.maxPp;
    p1PokemonInfo.moves.push_back(p1MoveInfoB);

    Simulation::PokemonCreationInfo p2PokemonInfo{};
    p2PokemonInfo.species = dex::Species::AMPHAROS;
    p2PokemonInfo.item = dex::Item::CHOICE_SPECS;
    p2PokemonInfo.ability = dex::Ability::STATIC;
    p2PokemonInfo.gender = dex::Gender::FEMALE;
    p2PokemonInfo.level = 100U;
    p2PokemonInfo.nature = dex::Nature::MODEST;
    p2PokemonInfo.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
    p2PokemonInfo.stats = {295U, 148U, 190U, 280U, 210U, 145U};

    Simulation::MoveCreationInfo p2MoveInfo{};
    p2MoveInfo.name = dex::Move::THUNDERBOLT;
    p2MoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::THUNDERBOLT).val;
    p2MoveInfo.pp = p2MoveInfo.maxPp - 2U;
    p2PokemonInfo.moves.push_back(p2MoveInfo);

    battleCreationInfo.p1 = {{p1PokemonInfo}};
    battleCreationInfo.p2 = {{p2PokemonInfo}};
  }

  battleCreationInfo.turn = 1U;

  return simulation;
}

inline Simulation createDoubleBattleSimulation(Simulation::BattleCreationInfo& battleCreationInfo) {
  static Pokedex* pokedex = nullptr;
  if (!pokedex) {
    pokedex = new Pokedex(GameMechanics::SCARLET_VIOLET);
    entt::dense_set<dex::Move> moveSet{};
    for (dex::Move move :
         {dex::Move::MOONBLAST, dex::Move::KNOCK_OFF, dex::Move::WILL_O_WISP, dex::Move::QUIVER_DANCE}) {
      moveSet.insert(move);
    }
    pokedex->loadMoves(moveSet);

    entt::dense_set<dex::Species> speciesSet{};
    for (dex::Species species :
         {dex::Species::GARDEVOIR, dex::Species::PANGORO, dex::Species::DRAGAPULT, dex::Species::RIBOMBEE})
      speciesSet.insert(species);
    pokedex->loadSpecies(speciesSet);
  }

  Simulation simulation(*pokedex, BattleFormat::DOUBLES_BATTLE_FORMAT);

  {
    Simulation::PokemonCreationInfo p1aPokemonInfo{};
    p1aPokemonInfo.species = dex::Species::GARDEVOIR;
    p1aPokemonInfo.ability = dex::Ability::TRACE;
    p1aPokemonInfo.gender = dex::Gender::FEMALE;
    p1aPokemonInfo.item = dex::Item::CHOICE_SCARF;
    p1aPokemonInfo.status = dex::Status::BRN;
    p1aPokemonInfo.level = 90U;
    p1aPokemonInfo.stats = {222U, 127U, 132U, 246U, 234U, 176U};
    p1aPokemonInfo.evs = {0U, 25U, 50U, 75U, 100U, 125U};

    Simulation::MoveCreationInfo p1aMoveInfo{};
    p1aMoveInfo.name = dex::Move::MOONBLAST;
    p1aMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::MOONBLAST).val;
    p1aMoveInfo.pp = p1aMoveInfo.maxPp - 4U;
    p1aPokemonInfo.moves.push_back(p1aMoveInfo);

    Simulation::PokemonCreationInfo p2aPokemonInfo{};
    p2aPokemonInfo.species = dex::Species::PANGORO;
    p2aPokemonInfo.ability = dex::Ability::IRON_FIST;
    p2aPokemonInfo.gender = dex::Gender::MALE;
    p2aPokemonInfo.item = dex::Item::LIFE_ORB;
    p2aPokemonInfo.level = 95U;
    p2aPokemonInfo.stats = {314U, 264U, 170U, 147U, 145U, 115U};
    p2aPokemonInfo.evs = {125U, 100U, 75U, 50U, 25U, 0U};

    Simulation::MoveCreationInfo p2aMoveInfo{};
    p2aMoveInfo.name = dex::Move::KNOCK_OFF;
    p2aMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::KNOCK_OFF).val;
    p2aMoveInfo.pp = p2aMoveInfo.maxPp - 3U;
    p2aPokemonInfo.moves.push_back(p2aMoveInfo);

    Simulation::PokemonCreationInfo p1bPokemonInfo{};
    p1bPokemonInfo.species = dex::Species::DRAGAPULT;
    p1bPokemonInfo.ability = dex::Ability::INFILTRATOR;
    p1bPokemonInfo.gender = dex::Gender::FEMALE;
    p1bPokemonInfo.item = dex::Item::FOCUS_SASH;
    p1bPokemonInfo.level = 100U;
    p1bPokemonInfo.nature = dex::Nature::HASTY;
    p1bPokemonInfo.stats = {291U, 255U, 153U, 225U, 180U, 350U};
    p1bPokemonInfo.ivs = {5U, 10U, 15U, 20U, 25U, 30U};

    Simulation::MoveCreationInfo p1bMoveInfo{};
    p1bMoveInfo.name = dex::Move::WILL_O_WISP;
    p1bMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::WILL_O_WISP).val;
    p1bMoveInfo.pp = p1bMoveInfo.maxPp - 2U;
    p1bPokemonInfo.moves.push_back(p1bMoveInfo);

    Simulation::PokemonCreationInfo p2bPokemonInfo{};
    p2bPokemonInfo.species = dex::Species::RIBOMBEE;
    p2bPokemonInfo.ability = dex::Ability::SWEET_VEIL;
    p2bPokemonInfo.gender = dex::Gender::MALE;
    p2bPokemonInfo.item = dex::Item::BRIGHT_POWDER;
    p2bPokemonInfo.level = 91U;
    p2bPokemonInfo.stats = {237U, 127U, 132U, 191U, 141U, 235U};
    p2bPokemonInfo.ivs = {30U, 25U, 20U, 15U, 10U, 5U};

    Simulation::MoveCreationInfo p2bMoveInfo{};
    p2bMoveInfo.name = dex::Move::QUIVER_DANCE;
    p2bMoveInfo.maxPp = pokedex->getMoveData<Pp>(dex::Move::QUIVER_DANCE).val;
    p2bMoveInfo.pp = p2bMoveInfo.maxPp - 1U;
    p2bPokemonInfo.moves.push_back(p2bMoveInfo);

    battleCreationInfo.p1 = {{p1aPokemonInfo, p1bPokemonInfo}};
    battleCreationInfo.p2 = {{p2aPokemonInfo, p2bPokemonInfo}};
  }

  battleCreationInfo.turn = 2U;
  battleCreationInfo.probability = 0.9F;
  battleCreationInfo.rngSeed = 0x12345678;

  return simulation;
}
}  // namespace pokesim

namespace Catch {
template <>
struct StringMaker<pokesim::DamageRollKind> {
  static void append(std::string& fullName, const std::string& part) {
    if (!fullName.empty()) {
      fullName += " | " + part;
    }
    else {
      fullName = part;
    }
  }

  static std::string convert(const pokesim::DamageRollKind& value) {
    std::string fullName;
    if ((int)value & (int)pokesim::DamageRollKind::AVERAGE_DAMAGE) {
      append(fullName, "AVERAGE_DAMAGE");
    }
    if ((int)value & (int)pokesim::DamageRollKind::MIN_DAMAGE) {
      append(fullName, "MIN_DAMAGE");
    }
    if ((int)value & (int)pokesim::DamageRollKind::MAX_DAMAGE) {
      append(fullName, "MAX_DAMAGE");
    }
    if ((int)value & (int)pokesim::DamageRollKind::GUARANTEED_CRIT_CHANCE) {
      append(fullName, "GUARANTEED_CRIT_CHANCE");
    }
    if ((int)value & (int)pokesim::DamageRollKind::ALL_DAMAGE_ROLLS) {
      append(fullName, "ALL_DAMAGE_ROLLS");
    }

    return fullName;
  }
};

template <>
struct StringMaker<std::nullopt_t> {
  static std::string convert(const std::nullopt_t&) { return "nullopt"; }
};
}  // namespace Catch