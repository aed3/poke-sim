#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Paralysis: Can cause move failure", "[Simulation][SimulateTurn][Effect][Status][Paralysis]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  const types::registry& registry = simulation.registry;

  BattleCreationInfo battleCreationInfo;
  battleCreationInfo.p1 = {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(pokedex, dex::Species::RIBOMBEE, true)}};
  battleCreationInfo.turn = 1U;
  battleCreationInfo.p1.team[0].status = dex::Status::PAR;
  pokedex.loadForBattleInfo({battleCreationInfo});

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P2A};
  p1SlotDecision.moveChoice = dex::Move::KNOCK_OFF;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::SPLASH;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};

  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
  simulation.createInitialStates({battleCreationInfo});
  auto& options = simulation.simulateTurnOptions;

  options.damageRollsConsidered = {
    DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
    DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
  };
  options.applyChangesToInputBattle = true;
  options.makeBranchesOnRandomEvents = true;

  types::entityVector specificallyCheckEntities;
  for (types::entity battle : registry.view<tags::Battle>()) {
    specificallyCheckEntities.push_back(battle);
  }
  for (types::entity pokemon : registry.view<tags::Pokemon>()) {
    specificallyCheckEntities.push_back(pokemon);
  }
  for (types::entity side : registry.view<tags::Side>()) {
    specificallyCheckEntities.push_back(side);
  }

  for (const auto& [entity, move] : registry.view<MoveName>().each()) {
    if (move.val == dex::Move::KNOCK_OFF || move.val == dex::Move::SPLASH) {
      specificallyCheckEntities.push_back(entity);
    }
  }

  updateAllStats(simulation);

  TestChecks checks{simulation, specificallyCheckEntities};
  auto result = simulation.simulateTurn();
  checks.checkRemainingOutputs();

  REQUIRE(result.turnOutcomeBattlesResults().size() == 1U);
  const auto& turnOutcomeBattles = std::get<1>(*result.turnOutcomeBattlesResults().each().begin()).val;
  REQUIRE(turnOutcomeBattles.size() == 2U);

  checks.checkViewForChanges<
    tags::Battle,
    Turn,
    Probability,
    simulate_turn::TurnOutcomeBattles,
    simulate_turn::tags::SpeedSortNeeded,
    ParentBattle,
    RootBattle>();

  checks.checkViewForChanges<tags::Side, SideDecision>();

  entt::dense_set<types::probability> foundProbabilities;
  for (types::entity battle : turnOutcomeBattles) {
    const auto& [turn, probability, rngSeed, rootBattle, sides] =
      registry.get<Turn, Probability, RngSeed, RootBattle, Sides>(battle);

    types::entity p1Side = sides.p1();
    types::entity p2Side = sides.p2();
    types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
    types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
    types::entity p1Move = registry.get<MoveSlots>(p1Pokemon).val[1];
    types::entity p2Move = registry.get<MoveSlots>(p2Pokemon).val[0];

    REQUIRE(turn.val == 2U);
    auto initialRngSeed = checks.getInitialComponents<RngSeed>(battle);
    REQUIRE(rngSeed.val == initialRngSeed.val);

    bool paralysisStoppedP1Move = probability.val == dex::latest::Paralysis::onBeforeMoveChance / 100.0F;
    bool p1Moved = probability.val == 1.0F - (dex::latest::Paralysis::onBeforeMoveChance / 100.0F);

    REQUIRE((paralysisStoppedP1Move || p1Moved));

    if (paralysisStoppedP1Move) {
      checks.checkEntityForChanges<>(p1Pokemon);
      checks.checkEntityForChanges<>(p1Move);

      checks.checkEntityForChanges<LastUsedMove>(p2Pokemon);
    }

    if (p1Moved) {
      checks.checkEntityForChanges<LastUsedMove>(p1Pokemon);
      checks.checkMovePpUsage(p1Move);

      checks.checkEntityForChanges<stat::CurrentHp, LastUsedMove>(p2Pokemon);

      auto p1PokemonLastUsedMove = registry.get<LastUsedMove>(p1Pokemon);
      REQUIRE(p1PokemonLastUsedMove.val == p1Move);

      auto p2PokemonHp = registry.get<stat::CurrentHp>(p2Pokemon);
      auto initialP2PokemonHp = checks.getInitialComponents<stat::CurrentHp>(p2Pokemon);
      REQUIRE(p2PokemonHp.val < initialP2PokemonHp.val);
    }

    auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
    REQUIRE(p2PokemonLastUsedMove.val == p2Move);

    checks.checkMovePpUsage(p2Move);
    foundProbabilities.insert(probability.val);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}
}  // namespace pokesim
