#include "Tests.hpp"

namespace pokesim {
Simulation::BattleCreationInfo createBaseBattleInfo(const Pokedex& pokedex) {
  Simulation::BattleCreationInfo battleCreationInfo{};

  // TODO(aed3): Make this function generally available
  auto createMove = [&pokedex](dex::Move move) {
    types::Pp pp = pokedex.getMoveData<Pp>(move).pp;
    return Simulation::MoveCreationInfo{move, pp, pp};
  };

  Simulation::PokemonCreationInfo p1aPokemonInfo{};
  p1aPokemonInfo.species = dex::Species::EMPOLEON;
  p1aPokemonInfo.ability = dex::Ability::DEFIANT;
  p1aPokemonInfo.gender = dex::Gender::MALE;
  p1aPokemonInfo.level = 100;
  p1aPokemonInfo.stats = {309, 208, 212, 258, 238, 156};
  p1aPokemonInfo.moves.push_back(createMove(dex::Move::FURY_ATTACK));

  Simulation::PokemonCreationInfo p2aPokemonInfo{};
  p2aPokemonInfo.species = dex::Species::AMPHAROS;
  p2aPokemonInfo.ability = dex::Ability::STATIC;
  p2aPokemonInfo.gender = dex::Gender::FEMALE;
  p2aPokemonInfo.level = 100;
  p2aPokemonInfo.stats = {321, 186, 206, 266, 216, 146};
  p1aPokemonInfo.moves.push_back(createMove(dex::Move::THUNDERBOLT));

  Simulation::PokemonCreationInfo p1bPokemonInfo{};
  p1bPokemonInfo.species = dex::Species::GARDEVOIR;
  p1bPokemonInfo.ability = dex::Ability::TRACE;
  p1bPokemonInfo.gender = dex::Gender::FEMALE;
  p1bPokemonInfo.level = 100;
  p1bPokemonInfo.stats = {277, 166, 166, 286, 266, 196};
  p1bPokemonInfo.moves.push_back(createMove(dex::Move::MOONBLAST));

  Simulation::PokemonCreationInfo p2bPokemonInfo{};
  p2bPokemonInfo.species = dex::Species::PANGORO;
  p2bPokemonInfo.ability = dex::Ability::IRON_FIST;
  p2bPokemonInfo.gender = dex::Gender::MALE;
  p2bPokemonInfo.level = 100;
  p2bPokemonInfo.stats = {331, 284, 192, 174, 178, 152};
  p2bPokemonInfo.moves.push_back(createMove(dex::Move::KNOCK_OFF));

  Simulation::PokemonCreationInfo p1cPokemonInfo{};
  p1cPokemonInfo.species = dex::Species::DRAGAPULT;
  p1cPokemonInfo.ability = dex::Ability::INFILTRATOR;
  p1cPokemonInfo.gender = dex::Gender::FEMALE;
  p1cPokemonInfo.level = 100;
  p1cPokemonInfo.stats = {217, 276, 186, 236, 186, 320};
  p1cPokemonInfo.moves.push_back(createMove(dex::Move::WILL_O_WISP));

  Simulation::PokemonCreationInfo p2cPokemonInfo{};
  p2cPokemonInfo.species = dex::Species::RIBOMBEE;
  p2cPokemonInfo.ability = dex::Ability::SWEET_VEIL;
  p2cPokemonInfo.gender = dex::Gender::MALE;
  p2cPokemonInfo.level = 100;
  p2cPokemonInfo.stats = {261, 146, 156, 226, 176, 284};
  p2cPokemonInfo.moves.push_back(createMove(dex::Move::QUIVER_DANCE));

  battleCreationInfo.p1 = {{p1aPokemonInfo, p1bPokemonInfo, p1cPokemonInfo}};
  battleCreationInfo.p2 = {{p2aPokemonInfo, p1bPokemonInfo, p2cPokemonInfo}};

  return battleCreationInfo;
}

entt::entity targetSlotToEntity(
  TargetSlot targetSlot, const types::TeamPositions<entt::entity>& p1Team,
  const types::TeamPositions<entt::entity>& p2Team) {
  switch (targetSlot) {
    case TargetSlot::P1A: {
      return p1Team[0];
    }
    case TargetSlot::P1B: {
      return p1Team[1];
    }
    case TargetSlot::P1C: {
      return p1Team[2];
    }
    case TargetSlot::P1D: {
      return p1Team[3];
    }
    case TargetSlot::P1E: {
      return p1Team[4];
    }
    case TargetSlot::P1F: {
      return p1Team[5];
    }
    case TargetSlot::P2A: {
      return p2Team[0];
    }
    case TargetSlot::P2B: {
      return p2Team[1];
    }
    case TargetSlot::P2C: {
      return p2Team[2];
    }
    case TargetSlot::P2D: {
      return p2Team[3];
    }
    case TargetSlot::P2E: {
      return p2Team[4];
    }
    case TargetSlot::P2F: {
      return p2Team[5];
    }
    default: {
      FAIL();
    }
  }

  return entt::entity{};
}

TEST_CASE("Simulation: Simulate Turn", "[Simulation][SimulateTurn][Setup]") {
  // TODO(aed3): Add in when action queue creation exists
}

TEST_CASE("Simulation: Calc Damage", "[Simulation][CalcDamage][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET_GAME_MECHANICS};
  entt::dense_set<dex::Move> moveSet{};
  for (dex::Move move :
       {dex::Move::FURY_ATTACK,
        dex::Move::THUNDERBOLT,
        dex::Move::MOONBLAST,
        dex::Move::KNOCK_OFF,
        dex::Move::WILL_O_WISP,
        dex::Move::QUIVER_DANCE}) {
    moveSet.insert(move);
  }

  pokedex.loadMoves(moveSet);

  Simulation::BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);
  battleInfo.damageCalculations = {
    {
      TargetSlot::P1A,
      TargetSlot::P1B,
      dex::Move::FURY_ATTACK,
    },
    {
      TargetSlot::P1B,
      TargetSlot::P1A,
      dex::Move::FURY_ATTACK,
    },
    {
      TargetSlot::P1B,
      TargetSlot::P1A,
      dex::Move::THUNDERBOLT,
    },
    {
      TargetSlot::P2B,
      TargetSlot::P2C,
      dex::Move::KNOCK_OFF,
    },
    {
      TargetSlot::P1B,
      TargetSlot::P2B,
      dex::Move::MOONBLAST,
    },
  };

  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  simulation.createInitialStates({battleInfo});

  entt::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  entt::entity battleEntity = battles[0];
  const auto& p1Team = registry.get<Team>(registry.get<Sides>(battleEntity).p1).team;
  const auto& p2Team = registry.get<Team>(registry.get<Sides>(battleEntity).p2).team;

  auto calculationsEntities = registry.group<calc_damage::Attacker, calc_damage::Defender, MoveName, Battle>();
  REQUIRE(calculationsEntities.size() == 5);

  for (const auto& calcDamageInfo : battleInfo.damageCalculations) {
    INFO(
      std::to_string((std::uint8_t)calcDamageInfo.attackerSlot) + "," +
      std::to_string((std::uint8_t)calcDamageInfo.defenderSlot) + "," +
      std::to_string((std::uint16_t)calcDamageInfo.move));

    int count =
      std::count_if(calculationsEntities.each().begin(), calculationsEntities.each().end(), [&](const auto& tuple) {
        const auto& [entity, attacker, defender, moveName, battle] = tuple;
        if (battle.battle != battleEntity) return false;
        if (moveName.name != calcDamageInfo.move) return false;
        if (attacker.attacker != targetSlotToEntity(calcDamageInfo.attackerSlot, p1Team, p2Team)) return false;
        if (defender.defender != targetSlotToEntity(calcDamageInfo.defenderSlot, p1Team, p2Team)) return false;
        return true;
      });

    REQUIRE(count == 1);
  }
}

TEST_CASE("Simulation: Analyze Effect", "[Simulation][AnalyzeEffect][Setup]") {
  // TODO(aed3): Add in when effect enums are added
}
}  // namespace pokesim