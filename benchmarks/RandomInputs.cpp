#include "Benchmarks.hpp"

namespace pokesim {
namespace {
struct Random {
 private:
  static constexpr std::array<dex::Species, 6U> availableSpecies{
    dex::Species::AMPHAROS,
    dex::Species::GARDEVOIR,
    dex::Species::EMPOLEON,
    dex::Species::PANGORO,
    dex::Species::RIBOMBEE,
    dex::Species::DRAGAPULT,
  };

  static constexpr std::array<dex::Move, 7U> availableMoves{
    dex::Move::FURY_ATTACK,
    dex::Move::KNOCK_OFF,
    dex::Move::MOONBLAST,
    dex::Move::QUIVER_DANCE,
    dex::Move::SPLASH,
    dex::Move::THUNDERBOLT,
    dex::Move::WILL_O_WISP,
  };

  static constexpr std::array<dex::Ability, 12U> availableAbilities{
    dex::Ability::CLEAR_BODY,
    dex::Ability::COMPETITIVE,
    dex::Ability::DEFIANT,
    dex::Ability::HONEY_GATHER,
    dex::Ability::INFILTRATOR,
    dex::Ability::IRON_FIST,
    dex::Ability::PLUS,
    dex::Ability::STATIC,
    dex::Ability::SWEET_VEIL,
    dex::Ability::SYNCHRONIZE,
    dex::Ability::TORRENT,
    dex::Ability::TRACE,
  };

  static constexpr std::array<dex::Item, 7U> availableItems{
    dex::Item::NO_ITEM,
    dex::Item::ASSAULT_VEST,
    dex::Item::BRIGHT_POWDER,
    dex::Item::CHOICE_SCARF,
    dex::Item::CHOICE_SPECS,
    dex::Item::FOCUS_SASH,
    dex::Item::LIFE_ORB,
  };

  template <typename T>
  static auto pickFromList(const T& list, types::rngState rngState) {
    return list[internal::nextBoundedRandomValue(rngState, (types::rngResult)list.size())];
  }

  template <typename MinMaxStruct>
  static auto pickFromBounds(types::rngState& rngState) {
    return internal::nextBoundedRandomValue(rngState, MinMaxStruct::MAX + 1U, MinMaxStruct::MIN);
  }

  static std::vector<dex::Move> pickUniqueMoveList(types::rngState& rngState, std::size_t listLength) {
    std::vector<dex::Move> movesPicked{listLength};
    std::vector<dex::Move> unusedMoves{availableMoves.begin(), availableMoves.end()};
    for (std::size_t i = 0; i < movesPicked.size(); i++) {
      dex::Move move =
        unusedMoves[internal::nextBoundedRandomValue(rngState, (types::rngResult)(unusedMoves.size() - i))];
      movesPicked[i] = move;
      auto end = std::remove(unusedMoves.begin(), unusedMoves.end(), move);
      (void)end;
    }

    return movesPicked;
  }

  static PokemonCreationInfo createRandomPokemon(types::rngState& rngState, Pokedex& pokedex) {
    PokemonCreationInfo info;
    info.species = pickFromList(availableSpecies, rngState);
    info.level = pickFromBounds<MechanicConstants::PokemonLevel>(rngState);
    info.item = pickFromList(availableItems, rngState);
    info.ability = pickFromList(availableAbilities, rngState);
    info.gender = (dex::Gender)internal::nextBoundedRandomValue(rngState, dex::TOTAL_GENDER_COUNT);
    info.nature = (dex::Nature)internal::nextBoundedRandomValue(rngState, dex::TOTAL_NATURE_COUNT + 1U);

    info.evs.hp = pickFromBounds<MechanicConstants::PokemonEv>(rngState);
    info.evs.atk = pickFromBounds<MechanicConstants::PokemonEv>(rngState);
    info.evs.def = pickFromBounds<MechanicConstants::PokemonEv>(rngState);
    info.evs.spa = pickFromBounds<MechanicConstants::PokemonEv>(rngState);
    info.evs.spd = pickFromBounds<MechanicConstants::PokemonEv>(rngState);
    info.evs.spe = pickFromBounds<MechanicConstants::PokemonEv>(rngState);

    info.ivs.hp = pickFromBounds<MechanicConstants::PokemonIv>(rngState);
    info.ivs.atk = pickFromBounds<MechanicConstants::PokemonIv>(rngState);
    info.ivs.def = pickFromBounds<MechanicConstants::PokemonIv>(rngState);
    info.ivs.spa = pickFromBounds<MechanicConstants::PokemonIv>(rngState);
    info.ivs.spd = pickFromBounds<MechanicConstants::PokemonIv>(rngState);
    info.ivs.spe = pickFromBounds<MechanicConstants::PokemonIv>(rngState);

    auto moves = pickUniqueMoveList(rngState, MechanicConstants::MoveSlots::MAX);
    for (dex::Move move : moves) {
      info.moves.push_back({move});
    }

    entt::dense_set<dex::Species> species;
    species.insert(info.species);
    pokedex.loadSpecies(species);

    info.currentHp = internal::nextBoundedRandomValue(
      rngState,
      computeStatFromBaseStat(
        dex::Stat::HP,
        pokedex.getSpeciesData<BaseStats>(info.species).hp,
        info.level.value(),
        info.nature.value(),
        {info.evs.hp.value()},
        {info.ivs.hp.value()}),
      MechanicConstants::PokemonCurrentHpStat::MIN + 1U);

    info.status = (dex::Status)internal::nextBoundedRandomValue(rngState, dex::TOTAL_STATUS_COUNT + 1U);

    return info;
  }

  static SideCreationInfo createRandomSide(types::rngState& rngState, Pokedex& pokedex) {
    SideCreationInfo side;
    for (types::teamPositionIndex i = 0U; i < MechanicConstants::TeamSize::MAX; i++) {
      side.team.emplace_back(createRandomPokemon(rngState, pokedex));
    }
    return side;
  }

  static BattleCreationInfo createRandomBattle(types::rngState& rngState, Pokedex& pokedex) {
    BattleCreationInfo battle;

    battle.sides = {
      createRandomSide(rngState, pokedex),
      createRandomSide(rngState, pokedex),
    };

    battle.turn = (pickFromBounds<MechanicConstants::TurnCount>(rngState) / 2U) + 1U;
    battle.rngSeed = rngState;
    battle.probability = pickFromBounds<MechanicConstants::PercentChance>(rngState) /
                         (types::probability)MechanicConstants::PercentChance::MAX;

    return battle;
  }

  static TurnDecisionInfo createRandomTurnDecision(
    types::rngState& rngState, const BattleCreationInfo& battle, const Simulation& simulation) {
    SideDecision p1Decision{PlayerSideId::P1};
    SideDecision p2Decision{PlayerSideId::P2};
    if (simulation.isBattleFormat(BattleFormat::SINGLES)) {
      SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
      SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};
      p1SlotDecision.moveChoice = pickFromList(battle.sides.p1().team[0].moves, rngState).name;
      p2SlotDecision.moveChoice = pickFromList(battle.sides.p2().team[0].moves, rngState).name;

      p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
      p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};
    }
    else {
      SlotDecision p1ASlotDecision{Slot::P1A, pickFromList(std::vector{Slot::P1B, Slot::P2A, Slot::P2B}, rngState)};
      SlotDecision p1BSlotDecision{Slot::P1B, pickFromList(std::vector{Slot::P1A, Slot::P2A, Slot::P2B}, rngState)};
      SlotDecision p2ASlotDecision{Slot::P2A, pickFromList(std::vector{Slot::P2B, Slot::P1A, Slot::P1B}, rngState)};
      SlotDecision p2BSlotDecision{Slot::P2B, pickFromList(std::vector{Slot::P2A, Slot::P1A, Slot::P1B}, rngState)};
      p1ASlotDecision.moveChoice = pickFromList(battle.sides.p1().team[0].moves, rngState).name;
      p1BSlotDecision.moveChoice = pickFromList(battle.sides.p1().team[1].moves, rngState).name;
      p2ASlotDecision.moveChoice = pickFromList(battle.sides.p2().team[0].moves, rngState).name;
      p2BSlotDecision.moveChoice = pickFromList(battle.sides.p2().team[1].moves, rngState).name;

      p1Decision.decisions = types::slotDecisions{p1ASlotDecision, p1BSlotDecision};
      p2Decision.decisions = types::slotDecisions{p2ASlotDecision, p2BSlotDecision};
    }

    TurnDecisionInfo turnDecision{p1Decision, p2Decision};

    for (auto& sideDecision : turnDecision) {
      for (auto& slotDecision : sideDecision.decisions.get<types::slotDecisions>()) {
        if (simulation.pokedex().moveHas<move::tags::Self>(slotDecision.moveChoice.value())) {
          slotDecision.targetSlot = slotDecision.sourceSlot;
        }
      }
    }

    return turnDecision;
  }

  static CalcDamageInputInfo createRandomCalcDamageInput(types::rngState& rngState) {
    CalcDamageInputInfo inputInfo{
      (Slot)internal::nextBoundedRandomValue(rngState, TOTAL_SLOT_COUNT + 1U, 1U),
      (Slot)internal::nextBoundedRandomValue(rngState, TOTAL_SLOT_COUNT + 1U, 1U),
    };

    std::size_t movesCalculated =
      internal::nextBoundedRandomValue(rngState, (types::rngResult)availableMoves.size(), 1U);
    inputInfo.moves = pickUniqueMoveList(rngState, movesCalculated);

    return inputInfo;
  }

  static AnalyzeEffectInputInfo createRandomAnalyzeEffectInput(types::rngState& rngState) {
    AnalyzeEffectInputInfo inputInfo{
      (Slot)internal::nextBoundedRandomValue(rngState, TOTAL_SLOT_COUNT + 1U, 1U),
      (Slot)internal::nextBoundedRandomValue(rngState, TOTAL_SLOT_COUNT + 1U, 1U),
    };

    inputInfo.effectTarget =
      internal::nextBoundedRandomValue(rngState, 2U) == 0U ? inputInfo.attackerSlot : inputInfo.defenderSlot;

    types::boost boost = (types::boost)internal::nextBoundedRandomValue(
      rngState,
      (types::rngResult)(MechanicConstants::PokemonStatBoost::MAX - MechanicConstants::PokemonStatBoost::MIN + 1U));
    boost -= MechanicConstants::PokemonStatBoost::MIN;

    inputInfo.boostEffect = {{
      (dex::Stat)(1 << internal::nextBoundedRandomValue(rngState, dex::TOTAL_STAT_COUNT, 1U)),
      boost,
    }};

    std::size_t movesCalculated =
      internal::nextBoundedRandomValue(rngState, (types::rngResult)availableMoves.size(), 1U);
    inputInfo.moves = pickUniqueMoveList(rngState, movesCalculated);

    return inputInfo;
  }

  template <typename SimulationTag, bool oneRandomBattle, bool oneRandomInput>
  struct Assign : BenchmarkInputHolder {
   private:
    static constexpr auto getFirstTag() {
      if constexpr (std::is_same_v<SimulationTag, tags::SimulateTurn>) {
        return "SimulateTurn";
      }
      else if constexpr (std::is_same_v<SimulationTag, tags::CalculateDamage>) {
        return "CalcDamage";
      }
      else {
        return "AnalyzeEffect";
      }
    }

    static auto& getInputList(BattleCreationInfo& battleCreationInfo) {
      if constexpr (std::is_same_v<SimulationTag, tags::SimulateTurn>) {
        battleCreationInfo.runWithSimulateTurn = true;
        return battleCreationInfo.decisionsToSimulate;
      }
      else if constexpr (std::is_same_v<SimulationTag, tags::CalculateDamage>) {
        battleCreationInfo.runWithCalculateDamage = true;
        return battleCreationInfo.damageCalculations;
      }
      else {
        battleCreationInfo.runWithAnalyzeEffect = true;
        return battleCreationInfo.effectsToAnalyze;
      }
    }

    static auto getRandomInput(
      types::rngState& rngState, const BattleCreationInfo& battle, const Simulation& simulation) {
      if constexpr (std::is_same_v<SimulationTag, tags::SimulateTurn>) {
        return createRandomTurnDecision(rngState, battle, simulation);
      }
      else if constexpr (std::is_same_v<SimulationTag, tags::CalculateDamage>) {
        return createRandomCalcDamageInput(rngState);
      }
      else {
        return createRandomAnalyzeEffectInput(rngState);
      }
    }

   public:
    inline static const std::vector<std::string> TAGS = {
      getFirstTag(),
      "RandomInputs",
      oneRandomBattle ? "OneRandomBattle" : "ManyRandomBattles",
      oneRandomInput ? "OneRandomInput" : "ManyRandomInputs",
    };

    static void run(
      types::rngState& rngState, types::entityIndex inputCount, Simulation& simulation, Pokedex& pokedex) {
      static_assert(
        oneRandomBattle || !oneRandomInput,
        "Cannot have many battles and one input as each battle would require a different input.");
      std::vector<BattleCreationInfo> battleInfoList;

      if constexpr (oneRandomBattle) {
        battleInfoList.push_back(createRandomBattle(rngState, pokedex));
        BattleCreationInfo& battle = battleInfoList[0];
        pokedex.loadForBattleInfo(battleInfoList);

        if constexpr (oneRandomInput) {
          getInputList(battle).push_back(getRandomInput(rngState, battle, simulation));
          battleInfoList = {inputCount, battleInfoList[0]};
        }
        else {
          for (types::entityIndex i = 0; i < inputCount; i++) {
            getInputList(battle).push_back(getRandomInput(rngState, battle, simulation));
          }
        }
      }
      else {
        battleInfoList.resize(inputCount);
        for (types::entityIndex i = 0; i < inputCount; i++) {
          battleInfoList[i] = createRandomBattle(rngState, pokedex);
          BattleCreationInfo& battle = battleInfoList[i];

          pokedex.loadForBattleInfo({battle});
          getInputList(battle).push_back(getRandomInput(rngState, battle, simulation));
        }
      }

      simulation.createInitialStates(battleInfoList);
      updateAllStats(simulation);
    }
  };

 public:
  struct AssignSimulateTurnOneBattleOneInput : Assign<tags::SimulateTurn, true, true> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  };
  struct AssignSimulateTurnOneBattleManyInputs : Assign<tags::SimulateTurn, true, false> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  };
  struct AssignSimulateTurnManyBattlesManyInputs : Assign<tags::SimulateTurn, false, false> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  };

  struct AssignCalcDamageOneBattleOneInput : Assign<tags::CalculateDamage, true, true> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  };
  struct AssignCalcDamageOneBattleManyInputs : Assign<tags::CalculateDamage, true, false> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  };
  struct AssignCalcDamageManyBattlesManyInputs : Assign<tags::CalculateDamage, false, false> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  };

  struct AssignAnalyzeEffectOneBattleOneInput : Assign<tags::AnalyzeEffect, true, true> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 13U;
  };
  struct AssignAnalyzeEffectOneBattleManyInputs : Assign<tags::AnalyzeEffect, true, false> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 13U;
  };
  struct AssignAnalyzeEffectManyBattlesManyInputs : Assign<tags::AnalyzeEffect, false, false> {
    static constexpr types::entityIndex MAX_INPUTS = 1U << 13U;
  };
};
}  // namespace

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseMonteCarloOptions, Random::AssignSimulateTurnOneBattleOneInput)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseMonteCarloOptions, Random::AssignSimulateTurnOneBattleManyInputs)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseMonteCarloOptions, Random::AssignSimulateTurnManyBattlesManyInputs)

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseCalcDamageOptions, Random::AssignCalcDamageOneBattleOneInput)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseCalcDamageOptions, Random::AssignCalcDamageOneBattleManyInputs)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseCalcDamageOptions, Random::AssignCalcDamageManyBattlesManyInputs)

BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseAnalyzeEffectOptions, Random::AssignAnalyzeEffectOneBattleOneInput)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseAnalyzeEffectOptions,
  Random::AssignAnalyzeEffectOneBattleManyInputs)
BENCHMARK_CASE(
  CreatePokedex, CreateSingleBattleSimulation, ChooseAnalyzeEffectOptions,
  Random::AssignAnalyzeEffectManyBattlesManyInputs)
}  // namespace pokesim
