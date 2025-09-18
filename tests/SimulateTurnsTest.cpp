#include "Tests.hpp"

namespace pokesim {

namespace {
using SpeedSortList = std::vector<SpeedSort>;

void runSpeedSortTest(
  const SpeedSortList& speedSortList, const SpeedSortList& idealSortedList,
  const SpeedTieIndexes& idealSpeedTies = {}) {
  types::registry registry;
  ActionQueue initialQueue;

  for (const SpeedSort& speedSort : speedSortList) {
    types::entity entity = registry.create();
    registry.emplace<SpeedSort>(entity, speedSort);
    initialQueue.val.push_back(entity);
  }

  types::handle handle{registry, registry.create()};
  ActionQueue sortedQueue = initialQueue;

  simulate_turn::speedSort(handle, sortedQueue);

  REQUIRE(initialQueue.val.size() == sortedQueue.val.size());
  for (types::entity entity : initialQueue.val) {
    bool entityFound = false;
    for (types::entity sortedEntity : sortedQueue.val) {
      if (sortedEntity == entity) {
        entityFound = true;
        break;
      }
    }

    REQUIRE(entityFound);
  }

  for (std::size_t i = 0U; i < idealSortedList.size(); i++) {
    INFO(std::to_string(i));
    const SpeedSort& idealSpeedSort = idealSortedList[i];
    const SpeedSort& trueSpeedSort = registry.get<SpeedSort>(sortedQueue.val[i]);

    REQUIRE(trueSpeedSort.order == idealSpeedSort.order);
    REQUIRE(trueSpeedSort.priority == idealSpeedSort.priority);
    REQUIRE(trueSpeedSort.fractionalPriority == idealSpeedSort.fractionalPriority);
    REQUIRE(trueSpeedSort.speed == idealSpeedSort.speed);
  }

  if (idealSpeedTies.val.empty()) {
    REQUIRE_FALSE(handle.all_of<SpeedTieIndexes>());
    return;
  }

  REQUIRE(handle.all_of<SpeedTieIndexes>());
  const SpeedTieIndexes& trueSpeedTies = handle.get<SpeedTieIndexes>();

  REQUIRE(trueSpeedTies.val.size() == idealSpeedTies.val.size());
  for (const auto& idealSpeedTie : idealSpeedTies.val) {
    bool found =
      std::any_of(trueSpeedTies.val.begin(), trueSpeedTies.val.end(), [&idealSpeedTie](const auto& trueSpeedTie) {
        return trueSpeedTie.start == idealSpeedTie.start && trueSpeedTie.length == trueSpeedTie.length;
      });

    REQUIRE(found);
  }
};
}  // namespace

TEST_CASE("Simulate Turn: SpeedSort", "[Simulation][SimulateTurn]") {
  SECTION("One Queue Item") {
    SpeedSort emptySpeedSort{};
    runSpeedSortTest({emptySpeedSort}, {emptySpeedSort});
  }

  SECTION("Two Identical Items") {
    SpeedSort emptySpeedSort{};
    runSpeedSortTest(
      {emptySpeedSort, emptySpeedSort},
      {emptySpeedSort, emptySpeedSort},
      SpeedTieIndexes{
        {SpeedTieIndexes::Span{0U, 2U}},
      });
  }

  SECTION("Sort By Order") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::TEAM},
      SpeedSort{ActionOrder::START},
      SpeedSort{ActionOrder::BEFORE_TURN},
      SpeedSort{ActionOrder::SWITCH},
      SpeedSort{ActionOrder::MOVE},
      SpeedSort{ActionOrder::NONE},
    };

    runSpeedSortTest(
      {
        idealList[2],
        idealList[0],
        idealList[1],
        idealList[3],
        idealList[5],
        idealList[4],
      },
      idealList);
  }

  SECTION("Sort By Priority") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 5},
      SpeedSort{ActionOrder::MOVE, 3},
      SpeedSort{ActionOrder::MOVE, 1},
      SpeedSort{ActionOrder::MOVE, 0},
      SpeedSort{ActionOrder::MOVE, -2},
      SpeedSort{ActionOrder::MOVE, -3},
      SpeedSort{ActionOrder::MOVE, -7},
    };

    runSpeedSortTest(
      {
        idealList[1],
        idealList[0],
        idealList[2],
        idealList[3],
        idealList[6],
        idealList[5],
        idealList[4],
      },
      idealList);
  }

  SECTION("Sort By Priority and Fractional Priority") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 5, false},
      SpeedSort{ActionOrder::MOVE, 3, true},
      SpeedSort{ActionOrder::MOVE, 0, false},
      SpeedSort{ActionOrder::MOVE, 0, true},
      SpeedSort{ActionOrder::MOVE, -3, false},
      SpeedSort{ActionOrder::MOVE, -7, true},
    };

    runSpeedSortTest(
      {
        idealList[5],
        idealList[4],
        idealList[3],
        idealList[1],
        idealList[0],
        idealList[2],
      },
      idealList);
  }

  SECTION("Sort By Fractional Priority") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 0, false},
      SpeedSort{ActionOrder::MOVE, 0, true},
    };

    runSpeedSortTest({idealList[1], idealList[0]}, idealList);
  }

  SECTION("Sort By Speed") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::MOVE, 0, false, 772U},
      SpeedSort{ActionOrder::MOVE, 0, false, 621U},
      SpeedSort{ActionOrder::MOVE, 0, false, 584U},
      SpeedSort{ActionOrder::MOVE, 0, false, 444U},
      SpeedSort{ActionOrder::MOVE, 0, false, 305U},
      SpeedSort{ActionOrder::MOVE, 0, false, 152U},
      SpeedSort{ActionOrder::MOVE, 0, false, 90U},
      SpeedSort{ActionOrder::MOVE, 0, false, 11U},
    };

    runSpeedSortTest(
      {
        idealList[1],
        idealList[0],
        idealList[5],
        idealList[7],
        idealList[3],
        idealList[4],
        idealList[2],
        idealList[6],
      },
      idealList);
  }

  SECTION("Sort Combination") {
    SpeedSortList idealList = {
      SpeedSort{ActionOrder::TEAM},
      SpeedSort{ActionOrder::START},
      SpeedSort{ActionOrder::BEFORE_TURN, 0, false, 584U},
      SpeedSort{ActionOrder::BEFORE_TURN, 0, false, 444U},
      SpeedSort{ActionOrder::SWITCH, 0, false, 52U},
      SpeedSort{ActionOrder::SWITCH, 0, false, 40U},
      SpeedSort{ActionOrder::MOVE, 1, false, 152U},
      SpeedSort{ActionOrder::MOVE, 0, false, 315U},
      SpeedSort{ActionOrder::MOVE, -3, true, 700U},
    };

    runSpeedSortTest(
      {
        idealList[8],
        idealList[7],
        idealList[5],
        idealList[3],
        idealList[1],
        idealList[0],
        idealList[4],
        idealList[6],
        idealList[2],
      },
      idealList);
  }
}

constexpr std::array<DamageRollKind, 3U> branchingDamageRollOptions = {
  DamageRollKind::AVERAGE_DAMAGE,
  calc_damage::combineDamageKinds(DamageRollKind::AVERAGE_DAMAGE, DamageRollKind::GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(DamageRollKind::MIN_DAMAGE, DamageRollKind::MAX_DAMAGE),
};

constexpr std::array<DamageRollKind, 4U> fixedBranchDamageRollOptions = {
  DamageRollKind::ALL_DAMAGE_ROLLS,
  DamageRollKind::AVERAGE_DAMAGE,
  calc_damage::combineDamageKinds(DamageRollKind::AVERAGE_DAMAGE, DamageRollKind::GUARANTEED_CRIT_CHANCE),
  calc_damage::combineDamageKinds(DamageRollKind::MIN_DAMAGE, DamageRollKind::MAX_DAMAGE),
};

struct VerticalSlice1Checks : debug::Checks {
 private:
  const simulate_turn::Options* options;

  template <typename Selector>
  void specificallyCheckEntities(
    const debug::TypesToIgnore& typesToIgnore, const debug::TypesToIgnore& typesIgnoredOnConstants = {}) const {
    for (types::entity entity : registry->view<Selector>()) {
      types::entity original = debug::findCopyParent(originalToCopy, *registry, entity);
      if (!specificallyChecked.contains(original)) {
        continue;
      }

      bool shouldNotChange = !options->applyChangesToInputBattle && original == entity;
      debug::areEntitiesEqual(
        *registry,
        entity,
        registryOnInput,
        originalToCopy.at(original),
        shouldNotChange ? typesIgnoredOnConstants : typesToIgnore);
    }
  }

  void checkBattle() const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<simulate_turn::TurnOutcomeBattles>();

    debug::TypesToIgnore typesIgnoredOnConstants = typesToIgnore;
    typesToIgnore.add<Probability, ParentBattle, Turn, RootBattle>();

    if (!options->makeBranchesOnRandomEvents) {
      typesToIgnore.add<RngSeed>();
    }

    specificallyCheckEntities<tags::Battle>(typesToIgnore, typesIgnoredOnConstants);
  }

  void checkSides() const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<SideDecision>();

    specificallyCheckEntities<tags::Side>(typesToIgnore);
  }

  void checkPokemon() const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<stat::CurrentHp, LastUsedMove, StatusName, status::tags::Paralysis>();

    specificallyCheckEntities<tags::Pokemon>(typesToIgnore);
  }

  void checkMoves() const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<Pp>();

    specificallyCheckEntities<MoveName>(typesToIgnore);
  }

 public:
  VerticalSlice1Checks(const Simulation& _simulation)
      : debug::Checks(_simulation), options(&_simulation.simulateTurnOptions) {
    for (types::entity battle : registry->view<tags::Battle>()) {
      originalToCopy[battle] = debug::createEntityCopy(battle, *registry, registryOnInput);
    }

    for (types::entity side : registry->view<tags::Side>()) {
      originalToCopy[side] = debug::createEntityCopy(side, *registry, registryOnInput);
    }

    for (types::entity pokemon : registry->view<tags::Pokemon>()) {
      originalToCopy[pokemon] = debug::createEntityCopy(pokemon, *registry, registryOnInput);
    }

    for (const auto& [entity, move] : registry->view<MoveName>().each()) {
      if (move.name == dex::Move::KNOCK_OFF || move.name == dex::Move::THUNDERBOLT) {
        originalToCopy[entity] = debug::createEntityCopy(entity, *registry, registryOnInput);
      }
    }

    copyRemainingEntities();
  }

  void checkConstantEntities() const {
    checkBattle();
    checkSides();
    checkPokemon();
    checkMoves();
    checkRemainingOutputs();

    if (!options->makeBranchesOnRandomEvents) {
      types::entityIndex finalEntityCount = getFinalEntityCount();
      if (options->applyChangesToInputBattle) {
        REQUIRE(finalEntityCount == initialEntityCount);
      }
      else {
        REQUIRE(finalEntityCount == (initialEntityCount * 2U));
      }
    }
  }

  const RngSeed& originalRngSeed(types::entity battle) const {
    types::entity original = debug::findCopyParent(originalToCopy, *registry, battle);
    return registryOnInput.get<RngSeed>(originalToCopy.at(original));
  }

  types::entity originalBattle(types::entity battle) const {
    return debug::findCopyParent(originalToCopy, *registry, battle);
  }
};

struct DamageValueInfo {
 private:
  std::vector<types::damage> baseDamage;
  std::vector<types::damage> critDamage;
  types::damage averageRegularDamage;
  types::damage averageCritDamage;
  types::stat startingHp;
  DamageRollKind damageRollKind;
  bool checkParalysisChance;
  bool checkWasCrit;
  bool willChooseMinOrMax;

  static auto constexpr MIN_PROBABILITY = MechanicConstants::Probability::MIN;
  static auto constexpr MIN_PERCENT_CHANCE = MechanicConstants::PercentChance::MIN;
  static auto constexpr MAX_PERCENT_CHANCE = MechanicConstants::PercentChance::MAX;
  static auto constexpr CRIT_CHANCE = MechanicConstants::CRIT_CHANCE_DIVISORS[0];
  static auto constexpr THUNDERBOLT_PAR_CHANCE =
    dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>::targetSecondaryEffect::chance;

 public:
  DamageValueInfo(
    PlayerSideId sideId, const std::vector<types::damage>& _baseDamage, types::damage _averageRegularDamage,
    const std::vector<types::damage>& _critDamage, types::damage _averageCritDamage, types::stat _startingHp,
    DamageRollKind _damageRollKind, const simulate_turn::Options& options)
      : baseDamage(_baseDamage),
        critDamage(_critDamage),
        averageRegularDamage(_averageRegularDamage),
        averageCritDamage(_averageCritDamage),
        startingHp(_startingHp),
        damageRollKind(_damageRollKind),
        checkParalysisChance(sideId == PlayerSideId::P2),
        checkWasCrit(true),
        willChooseMinOrMax(options.makeBranchesOnRandomEvents) {
    auto lowerLimit = options.randomChanceLowerLimit.value_or(MIN_PERCENT_CHANCE);
    auto upperLimit = options.randomChanceUpperLimit.value_or(MAX_PERCENT_CHANCE);
    auto branchProbabilityLowerLimit = options.branchProbabilityLowerLimit.value_or(MIN_PROBABILITY);

    checkParalysisChance &= branchProbabilityLowerLimit < 1.0F / THUNDERBOLT_PAR_CHANCE;
    checkWasCrit &= branchProbabilityLowerLimit < 1.0F / CRIT_CHANCE;

    checkParalysisChance &= lowerLimit < MAX_PERCENT_CHANCE / THUNDERBOLT_PAR_CHANCE;
    checkWasCrit &= lowerLimit < MAX_PERCENT_CHANCE / CRIT_CHANCE;
    checkWasCrit &= damageRollKind != branchingDamageRollOptions[1];

    willChooseMinOrMax |= branchProbabilityLowerLimit < 0.5F && lowerLimit < (MAX_PERCENT_CHANCE / 2U) &&
                          upperLimit > (MAX_PERCENT_CHANCE / 2U);
    willChooseMinOrMax &= damageRollKind == branchingDamageRollOptions[2];
  }

  entt::dense_set<types::stat> possibleHpValues() const {
    entt::dense_set<types::stat> hpValues;
    auto addDamageValues = [&](const std::vector<types::damage>& damages) {
      for (types::damage damage : damages) {
        hpValues.insert(startingHp - damage);
      }
    };

    if (damageRollKind == fixedBranchDamageRollOptions[0]) {
      if (checkWasCrit) {
        addDamageValues(critDamage);
      }
      addDamageValues(baseDamage);
    }
    else if (damageRollKind == fixedBranchDamageRollOptions[1]) {
      if (checkWasCrit) {
        addDamageValues({averageCritDamage});
      }
      addDamageValues({averageRegularDamage});
    }
    else if (damageRollKind == fixedBranchDamageRollOptions[2]) {
      addDamageValues({averageCritDamage});
    }
    else if (damageRollKind == fixedBranchDamageRollOptions[3]) {
      if (checkWasCrit) {
        addDamageValues({critDamage[0], critDamage[15]});
      }
      addDamageValues({baseDamage[0], baseDamage[15]});
    }
    else {
      FAIL();
    }

    return hpValues;
  }

  std::size_t uniqueDamageCount() const {
    std::size_t damageCounts = 1U;
    for (std::size_t i = 1U; i < baseDamage.size(); i++) {
      if (baseDamage[i - 1] != baseDamage[i]) {
        damageCounts++;
      }
    }
    if (checkWasCrit) {
      damageCounts++;
      for (std::size_t i = 1U; i < critDamage.size(); i++) {
        if (critDamage[i - 1] != critDamage[i]) {
          damageCounts++;
        }
      }
    }
    return damageCounts;
  }

  std::size_t possibilities() const {
    std::size_t count = 1U;
    if (checkWasCrit) {
      count++;
    }
    if (checkParalysisChance) {
      count *= 2U;
    }
    if (willChooseMinOrMax) {
      count *= 2U;
    }
    if (damageRollKind == DamageRollKind::ALL_DAMAGE_ROLLS) {
      count *= uniqueDamageCount();
    }

    return count;
  }

  types::probability getProbability(types::stat afterTurnHp, bool causedParalysisFromThunderbolt) const {
    types::damage damage = startingHp - afterTurnHp;
    types::probability baseDamageRollInstances =
      (types::probability)std::count(baseDamage.begin(), baseDamage.end(), damage);
    types::probability critDamageRollInstances =
      (types::probability)std::count(critDamage.begin(), critDamage.end(), damage);
    REQUIRE(!!(baseDamageRollInstances || critDamageRollInstances));

    types::probability probability = 1.0F;
    if (checkWasCrit) {
      if (critDamageRollInstances != 0.0F) {
        probability /= CRIT_CHANCE;
      }
      else {
        probability *= 1.0F - 1.0F / CRIT_CHANCE;
      }
    }

    if (checkParalysisChance) {
      if (causedParalysisFromThunderbolt) {
        probability *= THUNDERBOLT_PAR_CHANCE / (types::probability)(MAX_PERCENT_CHANCE);
      }
      else {
        probability *= 1.0F - (THUNDERBOLT_PAR_CHANCE / (types::probability)(MAX_PERCENT_CHANCE));
      }
    }

    if (willChooseMinOrMax) {
      probability /= 2.0F;
    }
    if (damageRollKind == DamageRollKind::ALL_DAMAGE_ROLLS) {
      if (critDamageRollInstances != 0.0F) {
        probability *= critDamageRollInstances / MechanicConstants::DamageRollCount::MAX;
      }
      else {
        probability *= baseDamageRollInstances / MechanicConstants::DamageRollCount::MAX;
      }
    }

    return probability;
  }

  bool mightCauseParalysis() const { return checkParalysisChance; }
};

TEST_CASE("Simulate Turn: Vertical Slice 1", "[Simulation][SimulateTurn]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};

  p1SlotDecision.moveChoice = dex::Move::KNOCK_OFF;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::THUNDERBOLT;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};
  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};

  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);
  auto& p1Info = battleCreationInfo.p1.team[0];
  auto& p2Info = battleCreationInfo.p2.team[0];
  p1Info.status = dex::Status::NO_STATUS;
  p2Info.item = dex::Item::NO_ITEM;
  p2Info.nature = dex::Nature::HARDY;
  p2Info.stats = {295U, 165U, 190U, 255U, 210U, 145U};
  battleCreationInfo.runWithSimulateTurn = true;

  auto numberOfSamples = GENERATE(std::optional<types::entityIndex>{std::nullopt}, 1U, 5U);

  bool applyChangesToInputBattle = GENERATE(true, false);
  auto branchProbabilityLowerLimit = GENERATE(std::optional<types::probability>{std::nullopt}, 0.0F, 0.5F);
  auto randomChanceLowerLimit = GENERATE(
    std::optional<types::percentChance>{std::nullopt},
    (types::percentChance)0U,
    (types::percentChance)10U,
    (types::percentChance)50U);
  auto randomChanceUpperLimit = GENERATE(
    std::optional<types::percentChance>{std::nullopt},
    (types::percentChance)100U,
    (types::percentChance)90U,
    (types::percentChance)50U);

  DamageRollOptions damageRollOptions;
  if (numberOfSamples.has_value()) {
    damageRollOptions.p1 = GENERATE(from_range(fixedBranchDamageRollOptions));
    damageRollOptions.p2 = GENERATE(from_range(fixedBranchDamageRollOptions));
  }
  else {
    damageRollOptions.p1 = GENERATE(from_range(branchingDamageRollOptions));
    damageRollOptions.p2 = GENERATE(from_range(branchingDamageRollOptions));
  }

  CAPTURE(
    applyChangesToInputBattle,
    branchProbabilityLowerLimit,
    numberOfSamples,
    randomChanceLowerLimit,
    randomChanceUpperLimit,
    damageRollOptions.p1,
    damageRollOptions.p2);

  auto& options = simulation.simulateTurnOptions;
  options.applyChangesToInputBattle = applyChangesToInputBattle;
  options.branchProbabilityLowerLimit = branchProbabilityLowerLimit;
  options.makeBranchesOnRandomEvents = !numberOfSamples.has_value();
  options.randomChanceLowerLimit = randomChanceLowerLimit;
  options.randomChanceUpperLimit = randomChanceUpperLimit;
  options.damageRollsConsidered = damageRollOptions;

  const DamageValueInfo p1DamageInfo(
    PlayerSideId::P1,
    {174U, 170U, 168U, 168U, 164U, 164U, 162U, 158U, 158U, 156U, 156U, 152U, 152U, 150U, 146U, 146U},  // 10
    158U,
    {260U, 258U, 254U, 252U, 248U, 246U, 242U, 240U, 240U, 236U, 234U, 230U, 228U, 224U, 222U, 218U},  // 15
    240U,
    p1Info.stats.hp,
    damageRollOptions.p1,
    options);

  const DamageValueInfo p2DamageInfo(
    PlayerSideId::P2,
    {52U, 51U, 50U, 50U, 49U, 49U, 48U, 48U, 47U, 47U, 46U, 46U, 45U, 45U, 44U, 44U},  // 9
    48U,
    {78U, 77U, 76U, 75U, 74U, 74U, 73U, 72U, 71U, 70U, 70U, 69U, 68U, 67U, 67U, 66U},  // 13
    72U,
    p2Info.stats.hp,
    damageRollOptions.p2,
    options);

  std::size_t idealTurnOutcomeCount = 0U;
  std::size_t totalPossibilities = p1DamageInfo.possibilities() * p2DamageInfo.possibilities();

  if (options.makeBranchesOnRandomEvents) {
    idealTurnOutcomeCount = totalPossibilities;
  }
  else {
    idealTurnOutcomeCount = numberOfSamples.value();
  }

  const auto expectedP1Hp = p1DamageInfo.possibleHpValues();
  const auto expectedP2Hp = p2DamageInfo.possibleHpValues();

  std::vector<Simulation::BattleCreationInfo> battleCreationInfoList;
  for (types::entityIndex i = 0U; i < numberOfSamples.value_or(1U); i++) {
    battleCreationInfoList.push_back(battleCreationInfo);
  }

  simulation.createInitialStates(battleCreationInfoList);
  updateAllStats(simulation);
  const auto originalBattles = simulation.selectedBattleEntities();

  VerticalSlice1Checks checks{simulation};
  const auto result = simulation.simulateTurn();
  checks.checkConstantEntities();

  REQUIRE(result.turnOutcomeBattlesResults().size() == battleCreationInfoList.size());
  types::entityVector allTurnOutcomes;
  result.turnOutcomeBattlesResults().each([&allTurnOutcomes](const auto& turnOutcomes) {
    allTurnOutcomes.insert(allTurnOutcomes.end(), turnOutcomes.val.begin(), turnOutcomes.val.end());
  });
  REQUIRE(allTurnOutcomes.size() == idealTurnOutcomeCount);

  if (!applyChangesToInputBattle) {
    for (types::entity originalBattle : originalBattles) {
      bool originalInOutcome =
        std::find(allTurnOutcomes.begin(), allTurnOutcomes.end(), originalBattle) != allTurnOutcomes.end();
      REQUIRE_FALSE(originalInOutcome);
    }
  }

  entt::dense_set<types::stat> foundP1Hp;
  entt::dense_set<types::stat> foundP2Hp;

  const types::registry& registry = simulation.registry;
  for (types::entity battle : allTurnOutcomes) {
    const auto& [turn, probability, rngSeed, rootBattle, sides] =
      registry.get<Turn, Probability, RngSeed, RootBattle, Sides>(battle);

    types::entity p1Side = sides.p1();
    types::entity p2Side = sides.p2();
    types::entity p1Pokemon = registry.get<Team>(p1Side).val[0];
    types::entity p2Pokemon = registry.get<Team>(p2Side).val[0];
    types::entity p1Move = registry.get<MoveSlots>(p1Pokemon).val[1];
    types::entity p2Move = registry.get<MoveSlots>(p2Pokemon).val[0];

    bool p1Paralyzed = registry.all_of<status::tags::Paralysis>(p1Pokemon);
    const auto& [p1Hp, p1LastUsedMove] = registry.get<stat::CurrentHp, LastUsedMove>(p1Pokemon);
    const auto& [p2Hp, p2LastUsedMove] = registry.get<stat::CurrentHp, LastUsedMove>(p2Pokemon);
    const Pp& p1Pp = registry.get<Pp>(p1Move);
    const Pp& p2Pp = registry.get<Pp>(p2Move);

    if (!applyChangesToInputBattle) {
      REQUIRE_FALSE(registry.all_of<simulate_turn::TurnOutcomeBattles>(battle));
    }

    types::entity originalRootBattle = checks.originalBattle(battle);
    REQUIRE(rootBattle.val == originalRootBattle);

    REQUIRE(turn.val == 2U);
    const auto& originalRngSeed = checks.originalRngSeed(battle);
    if (options.makeBranchesOnRandomEvents || totalPossibilities == 1U) {
      REQUIRE(rngSeed.val == originalRngSeed.val);
    }
    else {
      REQUIRE_FALSE(rngSeed.val == originalRngSeed.val);
    }

    REQUIRE_FALSE(registry.all_of<SideDecision>(p1Side));
    REQUIRE_FALSE(registry.all_of<SideDecision>(p2Side));

    REQUIRE(p1LastUsedMove.val == p1Move);
    REQUIRE(p2LastUsedMove.val == p2Move);
    REQUIRE(p1Pp.val == (p1Info.moves[1].pp - 1U));
    REQUIRE(p2Pp.val == (p2Info.moves[0].pp - 1U));

    if (!p2DamageInfo.mightCauseParalysis()) {
      REQUIRE_FALSE(registry.all_of<StatusName>(p1Pokemon));
      REQUIRE_FALSE(p1Paralyzed);
    }
    else if (p1Paralyzed) {
      REQUIRE(registry.get<StatusName>(p1Pokemon).name == dex::Status::PAR);
    }

    REQUIRE_FALSE(registry.all_of<StatusName>(p2Pokemon));
    REQUIRE_FALSE(registry.all_of<status::tags::Paralysis>(p2Pokemon));

    CAPTURE(p1Paralyzed, p1Hp.val, p2Hp.val, expectedP1Hp, expectedP2Hp);

    REQUIRE(expectedP1Hp.contains(p1Hp.val));
    REQUIRE(expectedP2Hp.contains(p2Hp.val));

    foundP1Hp.insert(p1Hp.val);
    foundP2Hp.insert(p2Hp.val);

    types::probability idealProbability =
      p1DamageInfo.getProbability(p1Hp.val, false) * p2DamageInfo.getProbability(p2Hp.val, p1Paralyzed);
    REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(idealProbability));
  }

  if (options.makeBranchesOnRandomEvents) {
    REQUIRE(foundP1Hp.size() == expectedP1Hp.size());
    REQUIRE(foundP2Hp.size() == expectedP2Hp.size());
  }
}
}  // namespace pokesim