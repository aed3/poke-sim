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
inline void loadPokedexForBattleInfo(const Simulation::BattleCreationInfo& battleCreationInfo, Pokedex& pokedex) {
  entt::dense_set<dex::Move> moveSet{};
  entt::dense_set<dex::Species> speciesSet{};

  for (const auto& side : {battleCreationInfo.p1, battleCreationInfo.p2}) {
    for (const auto& pokemon : side.team) {
      for (const auto& moveSlot : pokemon.moves) {
        moveSet.insert(moveSlot.name);
      }
      speciesSet.insert(pokemon.species);
    }
  }

  pokedex.loadMoves(moveSet);
  pokedex.loadSpecies(speciesSet);
}

inline Simulation::PokemonCreationInfo createPredefinedPokemon(dex::Species species) {
  Simulation::PokemonCreationInfo info;
  info.species = species;

  switch (species) {
    case dex::Species::EMPOLEON: {
      info.item = dex::Item::ASSAULT_VEST;
      info.ability = dex::Ability::DEFIANT;
      info.gender = dex::Gender::MALE;
      info.status = dex::Status::PAR;
      info.level = 99U;
      info.evs = {0U, 25U, 50U, 75U, 100U, 125U};
      info.stats = {275U, 181U, 191U, 242U, 229U, 154U};

      auto& moveA = info.moves.emplace_back();
      moveA.name = dex::Move::FURY_ATTACK;
      moveA.maxPp = dex::FuryAttack<GameMechanics::SCARLET_VIOLET>::basePp;
      moveA.pp = moveA.maxPp - 1U;

      auto& moveB = info.moves.emplace_back();
      moveB.name = dex::Move::KNOCK_OFF;
      moveB.maxPp = dex::KnockOff<GameMechanics::SCARLET_VIOLET>::basePp;
      moveB.pp = moveB.maxPp;
      break;
    }
    case dex::Species::AMPHAROS: {
      info.item = dex::Item::CHOICE_SPECS;
      info.ability = dex::Ability::PLUS;
      info.gender = dex::Gender::FEMALE;
      info.level = 100U;
      info.nature = dex::Nature::MODEST;
      info.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
      info.stats = {295U, 148U, 190U, 280U, 210U, 145U};

      auto& move = info.moves.emplace_back();
      move.name = dex::Move::THUNDERBOLT;
      move.maxPp = dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>::basePp;
      move.pp = move.maxPp - 2U;
      break;
    }
    case dex::Species::GARDEVOIR: {
      info.ability = dex::Ability::TRACE;
      info.gender = dex::Gender::FEMALE;
      info.item = dex::Item::CHOICE_SCARF;
      info.status = dex::Status::BRN;
      info.level = 90U;
      info.stats = {222U, 127U, 132U, 246U, 234U, 176U};
      info.evs = {0U, 25U, 50U, 75U, 100U, 125U};

      auto& move = info.moves.emplace_back();
      move.name = dex::Move::MOONBLAST;
      move.maxPp = dex::Moonblast<GameMechanics::SCARLET_VIOLET>::basePp;
      move.pp = move.maxPp - 4U;
      break;
    }
    case dex::Species::PANGORO: {
      info.ability = dex::Ability::IRON_FIST;
      info.gender = dex::Gender::MALE;
      info.item = dex::Item::LIFE_ORB;
      info.level = 95U;
      info.stats = {314U, 264U, 170U, 147U, 145U, 115U};
      info.evs = {125U, 100U, 75U, 50U, 25U, 0U};

      auto& move = info.moves.emplace_back();
      move.name = dex::Move::KNOCK_OFF;
      move.maxPp = dex::KnockOff<GameMechanics::SCARLET_VIOLET>::basePp;
      move.pp = move.maxPp - 3U;
      break;
    }
    case dex::Species::DRAGAPULT: {
      info.ability = dex::Ability::INFILTRATOR;
      info.gender = dex::Gender::FEMALE;
      info.item = dex::Item::FOCUS_SASH;
      info.level = 100U;
      info.nature = dex::Nature::HASTY;
      info.stats = {291U, 255U, 153U, 225U, 180U, 350U};
      info.ivs = {5U, 10U, 15U, 20U, 25U, 30U};

      auto& move = info.moves.emplace_back();
      move.name = dex::Move::WILL_O_WISP;
      move.maxPp = dex::WillOWisp<GameMechanics::SCARLET_VIOLET>::basePp;
      move.pp = move.maxPp - 2U;
      break;
    }
    case dex::Species::RIBOMBEE: {
      info.ability = dex::Ability::SWEET_VEIL;
      info.gender = dex::Gender::MALE;
      info.item = dex::Item::BRIGHT_POWDER;
      info.level = 91U;
      info.stats = {237U, 127U, 132U, 191U, 141U, 235U};
      info.ivs = {30U, 25U, 20U, 15U, 10U, 5U};

      auto& move = info.moves.emplace_back();
      move.name = dex::Move::QUIVER_DANCE;
      move.maxPp = dex::QuiverDance<GameMechanics::SCARLET_VIOLET>::basePp;
      move.pp = move.maxPp - 1U;
      break;
    }
    default: {
      CAPTURE(species);
      FAIL("This species does not have a predefined set.");
    }
  }

  return info;
}

inline Simulation createSingleBattleSimulation(Pokedex& pokedex, Simulation::BattleCreationInfo& battleCreationInfo) {
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);

  battleCreationInfo.p1 = {{createPredefinedPokemon(dex::Species::EMPOLEON)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(dex::Species::AMPHAROS)}};
  battleCreationInfo.turn = 1U;

  loadPokedexForBattleInfo(battleCreationInfo, pokedex);
  return simulation;
}

inline Simulation createDoubleBattleSimulation(Pokedex& pokedex, Simulation::BattleCreationInfo& battleCreationInfo) {
  Simulation simulation(pokedex, BattleFormat::DOUBLES_BATTLE_FORMAT);

  battleCreationInfo.p1 = {{
    createPredefinedPokemon(dex::Species::GARDEVOIR),
    createPredefinedPokemon(dex::Species::DRAGAPULT),
  }};
  battleCreationInfo.p2 = {{
    createPredefinedPokemon(dex::Species::PANGORO),
    createPredefinedPokemon(dex::Species::RIBOMBEE),
  }};

  battleCreationInfo.turn = 2U;
  battleCreationInfo.probability = 0.9F;
  battleCreationInfo.rngSeed = 0x12345678;

  loadPokedexForBattleInfo(battleCreationInfo, pokedex);
  return simulation;
}
}  // namespace pokesim

namespace Catch {
template <>
struct StringMaker<pokesim::DamageRollKind> {
  static void append(std::string& fullName, const std::string& part) {
    if (fullName.empty()) {
      fullName = part;
    }
    else {
      fullName += " | " + part;
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