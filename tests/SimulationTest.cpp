#include "Tests.hpp"

namespace pokesim {
Simulation::BattleCreationInfo createBaseBattleInfo(Pokedex& pokedex) {
  Simulation::BattleCreationInfo battleCreationInfo{};

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

  // TODO(aed3): Make this function generally available
  auto createMove = [&pokedex](dex::Move move) {
    types::pp pp = pokedex.getMoveData<Pp>(move).pp;
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
  p2aPokemonInfo.moves.push_back(createMove(dex::Move::THUNDERBOLT));

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

types::entity targetSlotToEntity(
  Slot targetSlot, const types::teamPositions<types::entity>& p1Team,
  const types::teamPositions<types::entity>& p2Team) {
  switch (targetSlot) {
    case Slot::P1A: {
      return p1Team[0];
    }
    case Slot::P1B: {
      return p1Team[1];
    }
    case Slot::P1C: {
      return p1Team[2];
    }
    case Slot::P1D: {
      return p1Team[3];
    }
    case Slot::P1E: {
      return p1Team[4];
    }
    case Slot::P1F: {
      return p1Team[5];
    }
    case Slot::P2A: {
      return p2Team[0];
    }
    case Slot::P2B: {
      return p2Team[1];
    }
    case Slot::P2C: {
      return p2Team[2];
    }
    case Slot::P2D: {
      return p2Team[3];
    }
    case Slot::P2E: {
      return p2Team[4];
    }
    case Slot::P2F: {
      return p2Team[5];
    }
    default: {
      FAIL();
    }
  }

  return types::entity{};
}

TEST_CASE("Simulation: Simulate Turn", "[Simulation][SimulateTurn][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);

  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};

  p1SlotDecision.moveChoice = dex::Move::FURY_ATTACK;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::THUNDERBOLT;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};

  auto check = [&]() {
    Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
    simulation.createInitialStates({battleInfo});

    simulation.run();

    types::registry& registry = simulation.registry;
    auto group = registry.group<ActionQueue, Id, Sides>();
    REQUIRE(group.size() == battleInfo.decisionsToSimulate.size());

    for (std::size_t i = 0; i < battleInfo.decisionsToSimulate.size(); i++) {
      const auto sides = registry.get<Sides>(group[i]);
      const types::handle p1Handle = {registry, sides.p1};
      const types::handle p2Handle = {registry, sides.p2};

      resolveDecision(p1Handle, p1Handle.get<SideDecision>());
      resolveDecision(p2Handle, p2Handle.get<SideDecision>());

      const auto& queue = registry.get<ActionQueue>(group[i]).actionQueue;
      const auto id = registry.get<Id>(group[i]).id;
      REQUIRE(queue.size() == 2);

      auto p1DecisionEntity = *std::find_if(queue.begin(), queue.end(), [&](types::entity entity) {
        return registry.get<SourceSlotName>(entity).sourceSlot == Slot::P1A;
      });

      auto p2DecisionEntity = *std::find_if(queue.begin(), queue.end(), [&](types::entity entity) {
        return registry.get<SourceSlotName>(entity).sourceSlot == Slot::P2A;
      });

      auto checkDecision = [&](
                             types::entity decisionEntity,
                             const pokesim::SlotDecision& decision,
                             const Simulation::PokemonCreationInfo& pokemon) {
        if (decision.moveChoice) {
          const auto [target, move, speedSort] = registry.get<TargetSlotName, action::Move, SpeedSort>(decisionEntity);

          REQUIRE(target.targetSlot == decision.targetSlot);
          REQUIRE(move.name == decision.moveChoice);
          REQUIRE(speedSort.speed == pokemon.stats.spe);
          REQUIRE(speedSort.order == ActionOrder::MOVE);
          REQUIRE(speedSort.priority == 0);
          REQUIRE(speedSort.fractionalPriority == false);
        }
        else {
          REQUIRE(registry.all_of<action::tags::Switch, TargetSlotName, SpeedSort>(decisionEntity));
          const auto [target, speedSort] = registry.get<TargetSlotName, SpeedSort>(decisionEntity);

          REQUIRE(target.targetSlot == decision.targetSlot);
          REQUIRE(speedSort.speed == pokemon.stats.spe);
          REQUIRE(speedSort.order == ActionOrder::SWITCH);
          REQUIRE(speedSort.priority == 0);
          REQUIRE(speedSort.fractionalPriority == false);
        }
      };

      {
        INFO("P1 Action");
        const auto& decision = battleInfo.decisionsToSimulate[id].p1.decisions.get<types::slotDecisions>()[0];
        checkDecision(p1DecisionEntity, decision, battleInfo.p1.team[0]);
      }

      {
        INFO("P2 Action");
        const auto& decision = battleInfo.decisionsToSimulate[id].p2.decisions.get<types::slotDecisions>()[0];
        checkDecision(p2DecisionEntity, decision, battleInfo.p2.team[0]);
      }
    }
  };

  SECTION("One Decision") {
    battleInfo.decisionsToSimulate = {{p1Decision, p2Decision}};

    check();
  }

  SECTION("Multiple Decisions") {
    battleInfo.decisionsToSimulate = {{p1Decision, p2Decision}, {p1Decision, p2Decision}};

    check();
  }

  SECTION("Multiple Different Decisions") {
    for (std::uint8_t i = 0; i < 3; i++) {
      for (std::uint8_t j = 0; j < 3; j++) {
        auto newP1Decision = p1Decision;
        auto newP2Decision = p2Decision;
        auto& newP1SlotDecision = newP1Decision.decisions.get<types::slotDecisions>()[0];
        auto& newP2SlotDecision = newP2Decision.decisions.get<types::slotDecisions>()[0];

        switch (i) {
          case 0: {
            newP1SlotDecision.moveChoice = p1SlotDecision.moveChoice;
            newP1SlotDecision.targetSlot = Slot::P2A;
            break;
          }
          case 1: {
            newP1SlotDecision.moveChoice = std::nullopt;
            newP1SlotDecision.targetSlot = Slot::P1B;
            break;
          }
          case 2: {
            newP1SlotDecision.moveChoice = std::nullopt;
            newP1SlotDecision.targetSlot = Slot::P1C;
            break;
          }
        }

        switch (j) {
          case 0: {
            newP2SlotDecision.moveChoice = p2SlotDecision.moveChoice;
            newP2SlotDecision.targetSlot = Slot::P1A;
            break;
          }
          case 1: {
            newP2SlotDecision.moveChoice = std::nullopt;
            newP2SlotDecision.targetSlot = Slot::P2B;
            break;
          }
          case 2: {
            newP2SlotDecision.moveChoice = std::nullopt;
            newP2SlotDecision.targetSlot = Slot::P2C;
            break;
          }
        }

        battleInfo.decisionsToSimulate.push_back({newP1Decision, newP2Decision});
      }
    }

    check();
  }
}

TEST_CASE("Simulation: Calc Damage", "[Simulation][CalcDamage][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);

  battleInfo.damageCalculations = {
    {
      Slot::P1A,
      Slot::P1B,
      dex::Move::FURY_ATTACK,
    },
    {
      Slot::P1B,
      Slot::P1A,
      dex::Move::FURY_ATTACK,
    },
    {
      Slot::P1B,
      Slot::P1A,
      dex::Move::THUNDERBOLT,
    },
    {
      Slot::P2B,
      Slot::P2C,
      dex::Move::KNOCK_OFF,
    },
    {
      Slot::P1B,
      Slot::P2B,
      dex::Move::MOONBLAST,
    },
  };

  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  simulation.createInitialStates({battleInfo});

  types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  types::entity battleEntity = battles[0];
  const auto& p1Team = registry.get<Team>(registry.get<Sides>(battleEntity).p1).team;
  const auto& p2Team = registry.get<Team>(registry.get<Sides>(battleEntity).p2).team;

  auto calculationsEntitiesGroup = registry.group<calc_damage::Attacker, calc_damage::Defender, MoveName, Battle>();
  REQUIRE(calculationsEntitiesGroup.size() == 5);

  const auto calculationsEntities = calculationsEntitiesGroup.each();
  for (const auto& calcDamageInfo : battleInfo.damageCalculations) {
    INFO(
      std::to_string((std::uint8_t)calcDamageInfo.attackerSlot) + "," +
      std::to_string((std::uint8_t)calcDamageInfo.defenderSlot) + "," +
      std::to_string((std::uint16_t)calcDamageInfo.move));

    bool found = std::any_of(calculationsEntities.begin(), calculationsEntities.end(), [&](const auto& tuple) {
      const auto& [entity, attacker, defender, moveName, battle] = tuple;
      if (battle.battle != battleEntity) return false;
      if (moveName.name != calcDamageInfo.move) return false;
      if (attacker.attacker != targetSlotToEntity(calcDamageInfo.attackerSlot, p1Team, p2Team)) return false;
      if (defender.defender != targetSlotToEntity(calcDamageInfo.defenderSlot, p1Team, p2Team)) return false;
      return true;
    });

    REQUIRE(found);
  }
}

TEST_CASE("Simulation: Analyze Effect", "[Simulation][AnalyzeEffect][Setup]") {
  // TODO(aed3): Add in when effect enums are added
}
}  // namespace pokesim