#pragma once

#ifdef POKESIM_USE_COMBINED_SOURCE_AND_HEADER_FILES
#include "../extras/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>
#include <entt/entt.hpp>

#ifndef POKESIM_ENABLE_TESTING
static_assert(false, "Rebuild PokeSim with the flag POKESIM_ENABLE_TESTING to enable testing.");
#endif

namespace pokesim {

inline MoveCreationInfo createMove(Pokedex& pokedex, dex::Move move) {
  entt::dense_set<dex::Move> moveSet{};
  moveSet.insert(move);
  pokedex.loadMoves(moveSet);
  types::pp pp = pokedex.getMoveData<Pp>(move).val;
  return MoveCreationInfo{move, pp, pp};
}

// TODO(aed3): Change how the `simple` parameter works once stat calculations are implement
inline PokemonCreationInfo createPredefinedPokemon(Pokedex& pokedex, dex::Species species, bool simple = false) {
  PokemonCreationInfo info;
  info.species = species;

  switch (species) {
    case dex::Species::EMPOLEON: {
      info.item = dex::Item::ASSAULT_VEST;
      info.ability = dex::Ability::COMPETITIVE;
      info.gender = dex::Gender::MALE;
      info.status = dex::Status::NO_STATUS;
      info.level = 99U;
      info.evs = {0U, 25U, 50U, 75U, 100U, 125U};
      info.stats = {275U, 181U, 191U, 242U, 229U, 154U};

      auto& moveA = info.moves.emplace_back(createMove(pokedex, dex::Move::FURY_ATTACK));
      moveA.pp -= 1U;

      info.moves.push_back(createMove(pokedex, dex::Move::KNOCK_OFF));
      break;
    }
    case dex::Species::AMPHAROS: {
      info.item = dex::Item::NO_ITEM;
      info.ability = dex::Ability::PLUS;
      info.gender = dex::Gender::FEMALE;
      info.level = 100U;
      info.nature = dex::Nature::HARDY;
      info.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
      info.stats = {295U, 165U, 190U, 255U, 210U, 145U};

      auto& move = info.moves.emplace_back(createMove(pokedex, dex::Move::THUNDERBOLT));
      move.pp -= 2U;
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

      auto& move = info.moves.emplace_back(createMove(pokedex, dex::Move::MOONBLAST));
      move.pp -= 4U;
      break;
    }
    case dex::Species::PANGORO: {
      info.ability = dex::Ability::IRON_FIST;
      info.gender = dex::Gender::MALE;
      info.item = dex::Item::LIFE_ORB;
      info.level = 95U;
      info.stats = {314U, 264U, 170U, 147U, 145U, 115U};
      info.evs = {125U, 100U, 75U, 50U, 25U, 0U};

      auto& move = info.moves.emplace_back(createMove(pokedex, dex::Move::KNOCK_OFF));
      move.pp -= 3U;
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

      auto& move = info.moves.emplace_back(createMove(pokedex, dex::Move::WILL_O_WISP));
      move.pp -= 2U;
      break;
    }
    case dex::Species::RIBOMBEE: {
      info.ability = dex::Ability::SWEET_VEIL;
      info.gender = dex::Gender::MALE;
      info.item = dex::Item::BRIGHT_POWDER;
      info.level = 91U;
      info.stats = {237U, 127U, 132U, 191U, 141U, 235U};
      info.ivs = {30U, 25U, 20U, 15U, 10U, 5U};

      auto& move = info.moves.emplace_back(createMove(pokedex, dex::Move::QUIVER_DANCE));
      move.pp -= 1U;
      break;
    }
    default: {
      CAPTURE(species);
      FAIL("This species does not have a predefined set.");
    }
  }

  if (simple) {
    info.item = dex::Item::NO_ITEM;
    info.status = dex::Status::NO_STATUS;
    info.moves = {createMove(pokedex, dex::Move::SPLASH)};
  }
  return info;
}

inline Simulation createSingleBattleSimulation(Pokedex& pokedex, BattleCreationInfo& battleCreationInfo) {
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);

  battleCreationInfo.p1 = {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(pokedex, dex::Species::AMPHAROS)}};
  battleCreationInfo.turn = 1U;

  pokedex.loadForBattleInfo({battleCreationInfo});
  return simulation;
}

inline Simulation createDoubleBattleSimulation(Pokedex& pokedex, BattleCreationInfo& battleCreationInfo) {
  Simulation simulation(pokedex, BattleFormat::DOUBLES_BATTLE_FORMAT);

  battleCreationInfo.p1 = {{
    createPredefinedPokemon(pokedex, dex::Species::GARDEVOIR),
    createPredefinedPokemon(pokedex, dex::Species::DRAGAPULT),
  }};
  battleCreationInfo.p2 = {{
    createPredefinedPokemon(pokedex, dex::Species::PANGORO),
    createPredefinedPokemon(pokedex, dex::Species::RIBOMBEE),
  }};

  battleCreationInfo.turn = 2U;
  battleCreationInfo.probability = 0.9F;
  battleCreationInfo.rngSeed = 0x12345678;

  pokedex.loadForBattleInfo({battleCreationInfo});
  return simulation;
}

struct TestChecks : debug::Checks {
  using debug::Checks::checkRemainingOutputs;

  bool isSpecificallyChecked(types::entity entity) const {
    types::entity original = debug::findCopyParent(currentEntitiesToInitial, *registry, entity);
    return specificallyChecked.contains(original);
  }

  template <typename Selector, typename... TypesToIgnore>
  void specificallyCheckEntities() const {
    for (types::entity entity : registry->view<Selector>()) {
      if (!isSpecificallyChecked(entity)) {
        continue;
      }

      checkEntityForChanges<TypesToIgnore...>(entity);
    }
  }

  template <typename Selector, typename... TypesToIgnore>
  void checkViewForChanges() const {
    for (types::entity entity : registry->view<Selector>()) {
      checkEntityForChanges<TypesToIgnore...>(entity);
    }
  }

  template <typename... TypesToIgnore>
  void checkEntityForChanges(types::entity entity) const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<TypesToIgnore...>();
    types::entity initialEntity = getInitialEntity(entity);
    debug::areEntitiesEqual(*registry, entity, registryOnInput, initialEntity, typesToIgnore);
  }

  template <typename... T>
  auto getInitialComponents(types::entity entity) const {
    return registryOnInput.get<T...>(getInitialEntity(entity));
  }

  void checkMovePpUsage(types::entity moveSlot) {
    checkEntityForChanges<Pp>(moveSlot);
    auto movePp = registry->get<Pp>(moveSlot);
    auto initialMovePp = getInitialComponents<Pp>(moveSlot);
    REQUIRE(movePp.val == initialMovePp.val - 1U);
  }

  TestChecks(const Simulation& _simulation, const types::entityVector& specificallyCheckedEntities = {})
      : debug::Checks(_simulation) {
    for (types::entity entity : specificallyCheckedEntities) {
      copyEntity(entity);
    }

    copyRemainingEntities();
  }
};
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
