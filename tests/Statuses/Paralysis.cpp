#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Paralysis: Can cause move failure", "[Simulation][SimulateTurn][Status][Paralysis]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  const types::registry& registry = simulation.registry;

  Simulation::BattleCreationInfo battleCreationInfo;
  battleCreationInfo.p1 = {{createPredefinedPokemon(dex::Species::EMPOLEON)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(dex::Species::RIBOMBEE)}};
  battleCreationInfo.p2.team[0].item = dex::Item::NO_ITEM;
  battleCreationInfo.turn = 1U;
  loadPokedexForBattleInfo(battleCreationInfo, pokedex);

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P2A};
  p1SlotDecision.moveChoice = dex::Move::KNOCK_OFF;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::QUIVER_DANCE;
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
    if (move.name == dex::Move::KNOCK_OFF || move.name == dex::Move::QUIVER_DANCE) {
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
    auto initialRngSeed = checks.getInitialComponent<RngSeed>(battle);
    REQUIRE(rngSeed.val == initialRngSeed.val);

    bool paralysisStoppedP1Move = probability.val == dex::latest::Paralysis::onBeforeMoveChance / 100.0F;
    bool p1Moved = probability.val == 1.0F - (dex::latest::Paralysis::onBeforeMoveChance / 100.0F);

    REQUIRE((paralysisStoppedP1Move || p1Moved));

    if (paralysisStoppedP1Move) {
      checks.checkEntityForChanges<>(p1Pokemon);
      checks.checkEntityForChanges<>(p1Move);

      checks.checkEntityForChanges<
        LastUsedMove,
        stat::EffectiveSpe,
        stat::EffectiveSpa,
        stat::EffectiveSpd,
        SpeBoost,
        SpaBoost,
        SpdBoost>(p2Pokemon);
    }

    if (p1Moved) {
      checks.checkEntityForChanges<LastUsedMove>(p1Pokemon);
      checks.checkEntityForChanges<Pp>(p1Move);

      checks.checkEntityForChanges<
        stat::CurrentHp,
        LastUsedMove,
        stat::EffectiveSpe,
        stat::EffectiveSpa,
        stat::EffectiveSpd,
        SpeBoost,
        SpaBoost,
        SpdBoost>(p2Pokemon);

      auto p1PokemonLastUsedMove = registry.get<LastUsedMove>(p1Pokemon);
      REQUIRE(p1PokemonLastUsedMove.val == p1Move);

      auto p1MovePp = registry.get<Pp>(p1Move);
      auto initialP1Move = checks.getInitialComponent<Pp>(p1Move);
      REQUIRE(p1MovePp.val == initialP1Move.val - 1U);

      auto p2PokemonHp = registry.get<stat::CurrentHp>(p2Pokemon);
      auto initialP2PokemonHp = checks.getInitialComponent<stat::CurrentHp>(p2Pokemon);
      REQUIRE(p2PokemonHp.val < initialP2PokemonHp.val);
    }

    auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
    REQUIRE(p2PokemonLastUsedMove.val == p2Move);

    checks.checkEntityForChanges<Pp>(p2Move);
    auto p2MovePp = registry.get<Pp>(p2Move);
    auto initialP2MovePp = checks.getInitialComponent<Pp>(p2Move);
    REQUIRE(p2MovePp.val == initialP2MovePp.val - 1U);

    foundProbabilities.insert(probability.val);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}
}  // namespace pokesim
