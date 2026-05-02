#include "Benchmarks.hpp"

namespace pokesim {
namespace {
struct VerticalSlice {
 private:
  static void setAllMovePp(const Pokedex& pokedex, BattleCreationInfo& battleCreationInfo) {
    for (auto& side : battleCreationInfo.sides) {
      for (auto& pokemon : side.team) {
        for (auto& move : pokemon.moves) {
          move.pp = move.maxPp = pokedex.getMoveData<Pp>(move.name).val;
        }
      }
    }
  }

  static BattleCreationInfo createSingleBattleTeam(Pokedex& pokedex) {
    BattleCreationInfo battleCreationInfo;
    auto& p1Info = battleCreationInfo.sides.p1().team.emplace_back();
    p1Info.species = dex::Species::EMPOLEON;
    p1Info.item = dex::Item::ASSAULT_VEST;
    p1Info.ability = dex::Ability::COMPETITIVE;
    p1Info.gender = dex::Gender::MALE;
    p1Info.level = 99U;
    p1Info.evs = {0U, 25U, 50U, 75U, 100U, 125U};
    p1Info.stats = {275U, 181U, 191U, 242U, 229U, 154U};
    p1Info.moves = {{dex::Move::FURY_ATTACK}, {dex::Move::KNOCK_OFF}};

    auto& p2Info = battleCreationInfo.sides.p2().team.emplace_back();
    p2Info.species = dex::Species::AMPHAROS;
    p2Info.ability = dex::Ability::PLUS;
    p2Info.gender = dex::Gender::FEMALE;
    p2Info.level = 100U;
    p2Info.nature = dex::Nature::HARDY;
    p2Info.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
    p2Info.stats = {295U, 165U, 190U, 255U, 210U, 145U};
    p2Info.moves = {{dex::Move::THUNDERBOLT}};

    pokedex.loadForBattleInfo({battleCreationInfo});
    setAllMovePp(pokedex, battleCreationInfo);

    return battleCreationInfo;
  }

  static BattleCreationInfo createDoubleBattleTeam(Pokedex& pokedex) {
    BattleCreationInfo battleCreationInfo;
    auto& p1Team = battleCreationInfo.sides.p1().team;
    auto& p2Team = battleCreationInfo.sides.p2().team;

    auto& p1AInfo = p1Team.emplace_back();
    p1AInfo.species = dex::Species::GARDEVOIR;
    p1AInfo.ability = dex::Ability::TRACE;
    p1AInfo.gender = dex::Gender::FEMALE;
    p1AInfo.item = dex::Item::CHOICE_SCARF;
    p1AInfo.status = dex::Status::BRN;
    p1AInfo.level = 90U;
    p1AInfo.stats = {222U, 127U, 132U, 246U, 234U, 176U};
    p1AInfo.evs = {0U, 25U, 50U, 75U, 100U, 125U};
    p1AInfo.moves = {{dex::Move::MOONBLAST}};

    auto& p2AInfo = p2Team.emplace_back();
    p2AInfo.species = dex::Species::PANGORO;
    p2AInfo.ability = dex::Ability::IRON_FIST;
    p2AInfo.gender = dex::Gender::MALE;
    p2AInfo.item = dex::Item::LIFE_ORB;
    p2AInfo.level = 95U;
    p2AInfo.stats = {314U, 264U, 170U, 147U, 145U, 115U};
    p2AInfo.evs = {125U, 100U, 75U, 50U, 25U, 0U};
    p2AInfo.moves = {{dex::Move::KNOCK_OFF}};

    auto& p1BInfo = p1Team.emplace_back();
    p1BInfo.species = dex::Species::DRAGAPULT;
    p1BInfo.ability = dex::Ability::INFILTRATOR;
    p1BInfo.gender = dex::Gender::FEMALE;
    p1BInfo.item = dex::Item::FOCUS_SASH;
    p1BInfo.level = 100U;
    p1BInfo.nature = dex::Nature::HASTY;
    p1BInfo.stats = {291U, 255U, 153U, 225U, 180U, 350U};
    p1BInfo.ivs = {5U, 10U, 15U, 20U, 25U, 30U};
    p1BInfo.moves = {{dex::Move::WILL_O_WISP}};

    auto& p2BInfo = p2Team.emplace_back();
    p2BInfo.species = dex::Species::RIBOMBEE;
    p2BInfo.ability = dex::Ability::SWEET_VEIL;
    p2BInfo.gender = dex::Gender::MALE;
    p2BInfo.item = dex::Item::BRIGHT_POWDER;
    p2BInfo.level = 91U;
    p2BInfo.stats = {237U, 127U, 132U, 191U, 141U, 235U};
    p2BInfo.ivs = {30U, 25U, 20U, 15U, 10U, 5U};
    p2BInfo.moves = {{dex::Move::QUIVER_DANCE}};

    pokedex.loadForBattleInfo({battleCreationInfo});
    setAllMovePp(pokedex, battleCreationInfo);

    return battleCreationInfo;
  }

 public:
  struct AssignSimulateTurnSingleBattleInputs : BenchmarkInputHolder {
    inline static const std::vector<std::string> TAGS = {"SimulateTurn", "VerticalSlice1"};
    static void run(types::rngState&, types::entityIndex inputCount, Simulation& simulation, Pokedex& pokedex) {
      static BattleCreationInfo battleCreationInfo = createSingleBattleTeam(pokedex);
      pokedex.loadForBattleInfo({battleCreationInfo});

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

      simulation.createInitialStates({inputCount, battleCreationInfo});
      updateAllStats(simulation);
    }
  };

  struct AssignSimulateTurnDoubleBattleInputs : BenchmarkInputHolder {
    inline static const std::vector<std::string> TAGS = {"SimulateTurn", "VerticalSlice1"};
    static void run(types::rngState&, types::entityIndex inputCount, Simulation& simulation, Pokedex& pokedex) {
      static BattleCreationInfo battleCreationInfo = createDoubleBattleTeam(pokedex);
      pokedex.loadForBattleInfo({battleCreationInfo});

      battleCreationInfo.runWithSimulateTurn = true;
      SideDecision p1Decision{PlayerSideId::P1};
      SideDecision p2Decision{PlayerSideId::P2};
      SlotDecision p1ASlotDecision{Slot::P1A, Slot::P2B};
      SlotDecision p1BSlotDecision{Slot::P1B, Slot::P2A};
      SlotDecision p2ASlotDecision{Slot::P2A, Slot::P1B};
      SlotDecision p2BSlotDecision{Slot::P2B, Slot::P2B};

      p1ASlotDecision.moveChoice = dex::Move::MOONBLAST;
      p1BSlotDecision.moveChoice = dex::Move::WILL_O_WISP;
      p1Decision.decisions = types::sideSlots<SlotDecision>{p1ASlotDecision, p1BSlotDecision};
      p2ASlotDecision.moveChoice = dex::Move::KNOCK_OFF;
      p2BSlotDecision.moveChoice = dex::Move::QUIVER_DANCE;
      p2Decision.decisions = types::sideSlots<SlotDecision>{p2ASlotDecision, p2BSlotDecision};
      battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};

      simulation.createInitialStates({inputCount, battleCreationInfo});
      updateAllStats(simulation);
    }
  };

  struct AssignCalcDamageSingleBattleInputs : BenchmarkInputHolder {
    inline static const std::vector<std::string> TAGS = {"CalcDamage", "VerticalSlice1"};
    static void run(types::rngState&, types::entityIndex inputCount, Simulation& simulation, Pokedex& pokedex) {
      static BattleCreationInfo battleCreationInfo = createSingleBattleTeam(pokedex);
      pokedex.loadForBattleInfo({battleCreationInfo});

      battleCreationInfo.runWithCalculateDamage = true;
      battleCreationInfo.damageCalculations = {
        {Slot::P2A, Slot::P1A, {dex::Move::THUNDERBOLT}},
      };

      simulation.createInitialStates({inputCount, battleCreationInfo});
      updateAllStats(simulation);
    }
  };

  struct AssignAnalyzeEffectSingleBattleInputs : BenchmarkInputHolder {
    inline static const std::vector<std::string> TAGS = {"AnalyzeEffect", "VerticalSlice1"};
    static void run(types::rngState&, types::entityIndex inputCount, Simulation& simulation, Pokedex& pokedex) {
      static BattleCreationInfo battleCreationInfo = createSingleBattleTeam(pokedex);
      pokedex.loadForBattleInfo({battleCreationInfo});

      battleCreationInfo.runWithAnalyzeEffect = true;
      battleCreationInfo.effectsToAnalyze = {
        {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      };

      simulation.createInitialStates({inputCount, battleCreationInfo});
      updateAllStats(simulation);
    }
  };
};
}  // namespace

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseMonteCarloOptions,
  VerticalSlice::AssignSimulateTurnSingleBattleInputs)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseRandomBranchingOptions,
  VerticalSlice::AssignSimulateTurnSingleBattleInputs)

BENCHMARK_CASE(
  CreatePokedex, CreateDoubleBattleSimulation, ChooseMonteCarloOptions,
  VerticalSlice::AssignSimulateTurnDoubleBattleInputs)
BENCHMARK_CASE(
  CreatePokedex, CreateDoubleBattleSimulation, ChooseRandomBranchingOptions,
  VerticalSlice::AssignSimulateTurnDoubleBattleInputs)

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseCalcDamageOptions,
  VerticalSlice::AssignCalcDamageSingleBattleInputs)

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseAnalyzeEffectOptions,
  VerticalSlice::AssignAnalyzeEffectSingleBattleInputs)

}  // namespace pokesim
