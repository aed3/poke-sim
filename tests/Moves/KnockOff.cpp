#include "../Tests.hpp"
#include "Catch2/catch_amalgamated.hpp"

namespace pokesim {
TEST_CASE("Knock Off: Remove Most Items", "[Simulation][SimulateTurn][Move][KnockOff]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  const types::registry& registry = simulation.registry;

  Simulation::BattleCreationInfo battleCreationInfo;
  battleCreationInfo.p1 = {{createPredefinedPokemon(pokedex, dex::Species::EMPOLEON)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(pokedex, dex::Species::RIBOMBEE, true)}};
  battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
  battleCreationInfo.p2.team[0].item = dex::Item::ASSAULT_VEST;
  battleCreationInfo.turn = 1U;
  loadPokedexForBattleInfo(battleCreationInfo, pokedex);

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
  REQUIRE(turnOutcomeBattles.size() == 1U);

  checks.checkViewForChanges<
    tags::Battle,
    Turn,
    simulate_turn::TurnOutcomeBattles,
    simulate_turn::tags::SpeedSortNeeded,
    ParentBattle,
    RootBattle>();

  checks.checkViewForChanges<tags::Side, SideDecision>();

  types::entity battle = turnOutcomeBattles[0];
  const auto& [turn, rootBattle, sides] = registry.get<Turn, RootBattle, Sides>(battle);

  types::entity p1Side = sides.p1();
  types::entity p2Side = sides.p2();
  types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
  types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
  types::entity p1Move = registry.get<MoveSlots>(p1Pokemon).val[1];
  types::entity p2Move = registry.get<MoveSlots>(p2Pokemon).val[0];

  checks.checkEntityForChanges<LastUsedMove>(p1Pokemon);
  checks.checkEntityForChanges<stat::CurrentHp, LastUsedMove, ItemName, item::tags::AssaultVest, stat::EffectiveSpd>(
    p2Pokemon);

  auto p1PokemonLastUsedMove = registry.get<LastUsedMove>(p1Pokemon);
  REQUIRE(p1PokemonLastUsedMove.val == p1Move);

  auto p2PokemonLastUsedMove = registry.get<LastUsedMove>(p2Pokemon);
  REQUIRE(p2PokemonLastUsedMove.val == p2Move);

  auto p2PokemonHp = registry.get<stat::CurrentHp>(p2Pokemon);
  auto initialP2PokemonHp = checks.getInitialComponents<stat::CurrentHp>(p2Pokemon);
  REQUIRE(p2PokemonHp.val < initialP2PokemonHp.val);

  checks.checkMovePpUsage(p1Move);
  checks.checkMovePpUsage(p2Move);

  REQUIRE_FALSE(registry.all_of<ItemName>(p2Pokemon));
  REQUIRE_FALSE(registry.all_of<item::tags::AssaultVest>(p2Pokemon));

  auto p2Spd = registry.get<stat::EffectiveSpd>(p2Pokemon);
  auto initialP2Spd = checks.getInitialComponents<stat::EffectiveSpd>(p2Pokemon);
  REQUIRE(p2Spd.val < initialP2Spd.val);
};
}  // namespace pokesim
