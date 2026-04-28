#include "Benchmarks.hpp"

namespace pokesim {
namespace {
struct VerticalSlice {
 private:
  static void createSingleBattleTeam(Pokedex& pokedex, BattleCreationInfo& battleCreationInfo) {
    auto& p1Info = battleCreationInfo.p1.team.emplace_back();
    p1Info.species = dex::Species::EMPOLEON;
    p1Info.item = dex::Item::ASSAULT_VEST;
    p1Info.ability = dex::Ability::COMPETITIVE;
    p1Info.gender = dex::Gender::MALE;
    p1Info.level = 99U;
    p1Info.evs = {0U, 25U, 50U, 75U, 100U, 125U};
    p1Info.stats = {275U, 181U, 191U, 242U, 229U, 154U};
    p1Info.moves = {{dex::Move::FURY_ATTACK}, {dex::Move::KNOCK_OFF}};

    auto& p2Info = battleCreationInfo.p2.team.emplace_back();
    p2Info.species = dex::Species::AMPHAROS;
    p2Info.ability = dex::Ability::PLUS;
    p2Info.gender = dex::Gender::FEMALE;
    p2Info.level = 100U;
    p2Info.nature = dex::Nature::HARDY;
    p2Info.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
    p2Info.stats = {295U, 165U, 190U, 255U, 210U, 145U};
    p2Info.moves = {{dex::Move::THUNDERBOLT}};

    pokedex.loadForBattleInfo({battleCreationInfo});

    for (MoveCreationInfo& move : p1Info.moves) {
      move.pp = move.maxPp = pokedex.getMoveData<Pp>(move.name).val;
    }

    for (MoveCreationInfo& move : p2Info.moves) {
      move.pp = move.maxPp = pokedex.getMoveData<Pp>(move.name).val;
    }
  }

 public:
  struct AssignSimulateTurnInputs : BenchmarkInputHolder {
    inline static const std::vector<std::string> TAGS = {"SimulateTurn", "VerticalSlice1"};
    static void run(types::rngState&, types::entityIndex inputCount, Simulation& simulation, Pokedex& pokedex) {
      BattleCreationInfo battleCreationInfo;
      createSingleBattleTeam(pokedex, battleCreationInfo);

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

      std::vector<BattleCreationInfo> battleInfoList{inputCount};

      for (types::entityIndex i = 0; i < inputCount; i++) {
        battleInfoList[i] = battleCreationInfo;
      }

      simulation.createInitialStates(battleInfoList);
      updateAllStats(simulation);
    }
  };
};
}  // namespace

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseMonteCarloOptions, VerticalSlice::AssignSimulateTurnInputs)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseRandomBranchingOptions, VerticalSlice::AssignSimulateTurnInputs)
}  // namespace pokesim
