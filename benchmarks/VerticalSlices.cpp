#include "Benchmarks.hpp"

namespace pokesim {
namespace {
Simulation::MoveCreationInfo createMove(Pokedex& pokedex, dex::Move move) {
  entt::dense_set<dex::Move> moveSet{};
  moveSet.insert(move);
  pokedex.loadMoves(moveSet);
  types::pp pp = pokedex.getMoveData<Pp>(move).val;
  return Simulation::MoveCreationInfo{move, pp, pp};
}

void loadPokedexForBattleInfo(const Simulation::BattleCreationInfo& battleCreationInfo, Pokedex& pokedex) {
  entt::dense_set<dex::Move> moveSet{};
  entt::dense_set<dex::Species> speciesSet{};
  entt::dense_set<dex::Item> itemSet{};

  for (const auto& side : {battleCreationInfo.p1, battleCreationInfo.p2}) {
    for (const auto& pokemon : side.team) {
      for (const auto& moveSlot : pokemon.moves) {
        moveSet.insert(moveSlot.name);
      }
      speciesSet.insert(pokemon.species);
      if (pokemon.item != dex::Item::NO_ITEM) {
        itemSet.insert(pokemon.item);
      }
    }
  }

  pokedex.loadMoves(moveSet);
  pokedex.loadSpecies(speciesSet);
  pokedex.loadItems(itemSet);
}

Simulation createVerticalSlice1(BattleFormat battleFormat, DamageRollKind damageRollKind, types::entityIndex copies) {
  static Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  pokedex.unloadAllData();

  Simulation simulation{pokedex, battleFormat};
  Simulation::BattleCreationInfo battleCreationInfo;

  if (simulation.isBattleFormat(BattleFormat::SINGLES_BATTLE_FORMAT)) {
    auto& p1Info = battleCreationInfo.p1.team.emplace_back();
    p1Info.species = dex::Species::EMPOLEON;
    p1Info.item = dex::Item::ASSAULT_VEST;
    p1Info.ability = dex::Ability::COMPETITIVE;
    p1Info.gender = dex::Gender::MALE;
    p1Info.status = dex::Status::NO_STATUS;
    p1Info.level = 99U;
    p1Info.evs = {0U, 25U, 50U, 75U, 100U, 125U};
    p1Info.stats = {275U, 181U, 191U, 242U, 229U, 154U};
    p1Info.moves = {
      createMove(pokedex, dex::Move::FURY_ATTACK),
      createMove(pokedex, dex::Move::KNOCK_OFF),
    };
    auto& p2Info = battleCreationInfo.p2.team.emplace_back();
    p2Info.species = dex::Species::AMPHAROS;
    p2Info.item = dex::Item::NO_ITEM;
    p2Info.ability = dex::Ability::PLUS;
    p2Info.gender = dex::Gender::FEMALE;
    p2Info.level = 100U;
    p2Info.nature = dex::Nature::HARDY;
    p2Info.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
    p2Info.stats = {295U, 165U, 190U, 255U, 210U, 145U};

    p2Info.moves = {createMove(pokedex, dex::Move::THUNDERBOLT)};
  }

  loadPokedexForBattleInfo(battleCreationInfo, pokedex);
  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};

  p1SlotDecision.moveChoice = dex::Move::KNOCK_OFF;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::THUNDERBOLT;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};
  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};

  std::vector<Simulation::BattleCreationInfo> battleInfoList{copies};
  for (types::entityIndex i = 0; i < copies; i++) {
    battleInfoList[i] = battleCreationInfo;
  }

  simulation.simulateTurnOptions.makeBranchesOnRandomEvents = false;
  simulation.simulateTurnOptions.damageRollsConsidered.p1 = damageRollKind;
  simulation.simulateTurnOptions.damageRollsConsidered.p2 = damageRollKind;
  simulation.createInitialStates(battleInfoList);
  updateAllStats(simulation);

  return simulation;
}
}  // namespace

#define VERTICAL_SLICE_1_SINGLE_ST(copies)              \
  BENCHMARK_CASE(                                       \
    "Vertical Slice 1, Single, SimulateTurn, " #copies, \
    "[VerticalSlice][SingleBattle][SimulateTurn]",      \
    createVerticalSlice1(BattleFormat::SINGLES_BATTLE_FORMAT, DamageRollKind::ALL_DAMAGE_ROLLS, copies));

VERTICAL_SLICE_1_SINGLE_ST(1);
VERTICAL_SLICE_1_SINGLE_ST(2);
VERTICAL_SLICE_1_SINGLE_ST(4);
VERTICAL_SLICE_1_SINGLE_ST(8);
VERTICAL_SLICE_1_SINGLE_ST(16);
VERTICAL_SLICE_1_SINGLE_ST(32);
VERTICAL_SLICE_1_SINGLE_ST(64);
VERTICAL_SLICE_1_SINGLE_ST(128);
VERTICAL_SLICE_1_SINGLE_ST(256);
VERTICAL_SLICE_1_SINGLE_ST(512);
VERTICAL_SLICE_1_SINGLE_ST(1024);

}  // namespace pokesim
