#include "Tests.hpp"

namespace pokesim {
namespace {
Simulation::BattleCreationInfo createBaseBattleInfo(Pokedex& pokedex) {
  Simulation::BattleCreationInfo battleCreationInfo;

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
    types::pp pp = pokedex.getMoveData<Pp>(move).val;
    return Simulation::MoveCreationInfo{move, pp, pp};
  };

  Simulation::PokemonCreationInfo p1aPokemonInfo{};
  p1aPokemonInfo.species = dex::Species::EMPOLEON;
  p1aPokemonInfo.ability = dex::Ability::DEFIANT;
  p1aPokemonInfo.gender = dex::Gender::MALE;
  p1aPokemonInfo.level = 100U;
  p1aPokemonInfo.stats = {309U, 208U, 212U, 258U, 238U, 156U};
  p1aPokemonInfo.moves.push_back(createMove(dex::Move::FURY_ATTACK));

  Simulation::PokemonCreationInfo p2aPokemonInfo{};
  p2aPokemonInfo.species = dex::Species::AMPHAROS;
  p2aPokemonInfo.ability = dex::Ability::STATIC;
  p2aPokemonInfo.gender = dex::Gender::FEMALE;
  p2aPokemonInfo.level = 100U;
  p2aPokemonInfo.stats = {321U, 186U, 206U, 266U, 216U, 146U};
  p2aPokemonInfo.moves.push_back(createMove(dex::Move::THUNDERBOLT));

  Simulation::PokemonCreationInfo p1bPokemonInfo{};
  p1bPokemonInfo.species = dex::Species::GARDEVOIR;
  p1bPokemonInfo.ability = dex::Ability::TRACE;
  p1bPokemonInfo.gender = dex::Gender::FEMALE;
  p1bPokemonInfo.level = 100U;
  p1bPokemonInfo.stats = {277U, 166U, 166U, 286U, 266U, 196U};
  p1bPokemonInfo.moves.push_back(createMove(dex::Move::MOONBLAST));

  Simulation::PokemonCreationInfo p2bPokemonInfo{};
  p2bPokemonInfo.species = dex::Species::PANGORO;
  p2bPokemonInfo.ability = dex::Ability::IRON_FIST;
  p2bPokemonInfo.gender = dex::Gender::MALE;
  p2bPokemonInfo.level = 100U;
  p2bPokemonInfo.stats = {331U, 284U, 192U, 174U, 178U, 152U};
  p2bPokemonInfo.moves.push_back(createMove(dex::Move::KNOCK_OFF));

  Simulation::PokemonCreationInfo p1cPokemonInfo{};
  p1cPokemonInfo.species = dex::Species::DRAGAPULT;
  p1cPokemonInfo.ability = dex::Ability::INFILTRATOR;
  p1cPokemonInfo.gender = dex::Gender::FEMALE;
  p1cPokemonInfo.level = 100U;
  p1cPokemonInfo.stats = {217U, 276U, 186U, 236U, 186U, 320U};
  p1cPokemonInfo.moves.push_back(createMove(dex::Move::WILL_O_WISP));

  Simulation::PokemonCreationInfo p2cPokemonInfo{};
  p2cPokemonInfo.species = dex::Species::RIBOMBEE;
  p2cPokemonInfo.ability = dex::Ability::SWEET_VEIL;
  p2cPokemonInfo.gender = dex::Gender::MALE;
  p2cPokemonInfo.level = 100U;
  p2cPokemonInfo.stats = {261U, 146U, 156U, 226U, 176U, 284U};
  p2cPokemonInfo.moves.push_back(createMove(dex::Move::QUIVER_DANCE));

  battleCreationInfo.p1 = {{p1aPokemonInfo, p1bPokemonInfo, p1cPokemonInfo}};
  battleCreationInfo.p2 = {{p2aPokemonInfo, p1bPokemonInfo, p2cPokemonInfo}};

  entt::dense_set<dex::Species> speciesSet{};
  speciesSet.insert(p1aPokemonInfo.species);
  speciesSet.insert(p1bPokemonInfo.species);
  speciesSet.insert(p1cPokemonInfo.species);
  speciesSet.insert(p2aPokemonInfo.species);
  speciesSet.insert(p2bPokemonInfo.species);
  speciesSet.insert(p2cPokemonInfo.species);
  pokedex.loadSpecies(speciesSet);

  return battleCreationInfo;
}
}  // namespace

TEST_CASE("Simulation Setup: Simulate Turn", "[Simulation][SimulateTurn][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);
  battleInfo.runWithSimulateTurn = true;

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

    types::registry& registry = simulation.registry;
    auto group = registry.group<ActionQueue, Id, Sides>();
    REQUIRE(group.size() == battleInfo.decisionsToSimulate.size());

    for (std::size_t i = 0U; i < battleInfo.decisionsToSimulate.size(); i++) {
      const auto sides = registry.get<Sides>(group[i]);
      const types::handle p1Handle = {registry, sides.p1()};
      const types::handle p2Handle = {registry, sides.p2()};

      simulate_turn::resolveDecision(p1Handle, p1Handle.get<SideDecision>());
      simulate_turn::resolveDecision(p2Handle, p2Handle.get<SideDecision>());

      const auto& queue = registry.get<ActionQueue>(group[i]).val;
      const auto id = registry.get<Id>(group[i]).val;
      REQUIRE(queue.size() == 2U);

      auto p1DecisionEntity = *std::find_if(queue.begin(), queue.end(), [&](types::entity entity) {
        return registry.get<SourceSlotName>(entity).name == Slot::P1A;
      });

      auto p2DecisionEntity = *std::find_if(queue.begin(), queue.end(), [&](types::entity entity) {
        return registry.get<SourceSlotName>(entity).name == Slot::P2A;
      });

      auto checkDecision = [&](
                             types::entity decisionEntity,
                             const pokesim::SlotDecision& decision,
                             const Simulation::PokemonCreationInfo& pokemon) {
        if (decision.moveChoice) {
          const auto [target, move, speedSort] = registry.get<TargetSlotName, action::Move, SpeedSort>(decisionEntity);

          REQUIRE(target.name == decision.targetSlot);
          REQUIRE(move.name == decision.moveChoice);
          REQUIRE(speedSort.speed == pokemon.stats.spe);
          REQUIRE(speedSort.order == ActionOrder::MOVE);
          REQUIRE(speedSort.priority == 0);
          REQUIRE(speedSort.fractionalPriority == false);
        }
        else {
          REQUIRE(registry.all_of<action::tags::Switch, TargetSlotName, SpeedSort>(decisionEntity));
          const auto [target, speedSort] = registry.get<TargetSlotName, SpeedSort>(decisionEntity);

          REQUIRE(target.name == decision.targetSlot);
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
    for (std::uint8_t i = 0U; i < 3U; i++) {
      for (std::uint8_t j = 0U; j < 3U; j++) {
        auto newP1Decision = p1Decision;
        auto newP2Decision = p2Decision;
        auto& newP1SlotDecision = newP1Decision.decisions.get<types::slotDecisions>()[0];
        auto& newP2SlotDecision = newP2Decision.decisions.get<types::slotDecisions>()[0];

        switch (i) {
          case 0U: {
            newP1SlotDecision.moveChoice = p1SlotDecision.moveChoice;
            newP1SlotDecision.targetSlot = Slot::P2A;
            break;
          }
          case 1U: {
            newP1SlotDecision.moveChoice = std::nullopt;
            newP1SlotDecision.targetSlot = Slot::P1B;
            break;
          }
          case 2U: {
            newP1SlotDecision.moveChoice = std::nullopt;
            newP1SlotDecision.targetSlot = Slot::P1C;
            break;
          }
          default: break;
        }

        switch (j) {
          case 0U: {
            newP2SlotDecision.moveChoice = p2SlotDecision.moveChoice;
            newP2SlotDecision.targetSlot = Slot::P1A;
            break;
          }
          case 1U: {
            newP2SlotDecision.moveChoice = std::nullopt;
            newP2SlotDecision.targetSlot = Slot::P2B;
            break;
          }
          case 2U: {
            newP2SlotDecision.moveChoice = std::nullopt;
            newP2SlotDecision.targetSlot = Slot::P2C;
            break;
          }
          default: break;
        }

        battleInfo.decisionsToSimulate.push_back({newP1Decision, newP2Decision});
      }
    }

    check();
  }
}

TEST_CASE("Simulation Setup: Calc Damage", "[Simulation][CalculateDamage][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);

  battleInfo.damageCalculations = {
    {
      Slot::P1A,
      Slot::P1B,
      {dex::Move::FURY_ATTACK, dex::Move::KNOCK_OFF},
    },
    {
      Slot::P1B,
      Slot::P1A,
      {dex::Move::FURY_ATTACK},
    },
    {
      Slot::P1B,
      Slot::P1A,
      {dex::Move::THUNDERBOLT},
    },
    {
      Slot::P2B,
      Slot::P2C,
      {dex::Move::KNOCK_OFF},
    },
    {
      Slot::P2B,
      Slot::P1B,
      {dex::Move::MOONBLAST},
    },
  };

  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  simulation.createInitialStates({battleInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1U);

  auto view = registry.view<tags::CalculateDamage>();
  REQUIRE(view.size() == 7U);
}

TEST_CASE("Simulation Setup: Analyze Effect", "[Simulation][AnalyzeEffect][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);

  battleInfo.effectsToAnalyze = {
    {Slot::P1A, Slot::P1B, Slot::P1A, {dex::Move::FURY_ATTACK, dex::Move::KNOCK_OFF}, dex::Status::BRN},
    {Slot::P1B, Slot::P1A, Slot::P1A, {dex::Move::FURY_ATTACK}, std::nullopt, {{dex::Stat::DEF, 2}}},
    {
      Slot::P1B,
      Slot::P1A,
      Slot::P1B,
      {dex::Move::THUNDERBOLT},
      std::nullopt,
      {{dex::Stat::SPA, -3}},
    },
    {Slot::P2B, Slot::P2C, Slot::P2B, {dex::Move::KNOCK_OFF}, dex::Status::BRN, {{dex::Stat::ATK, 2}}},
    {Slot::P2B, Slot::P1B, Slot::P2B, {dex::Move::MOONBLAST}, dex::Status::TOX, {{dex::Stat::SPE, 6}}},
  };

  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  simulation.createInitialStates({battleInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1U);

  auto view = registry.view<analyze_effect::tags::Input>();
  REQUIRE(view.size() == 6U);
}
}  // namespace pokesim