#include <algorithm>

#include "Tests.hpp"

namespace pokesim {
static auto constexpr MIN_PROBABILITY = MechanicConstants::Probability::MIN;
static auto constexpr MIN_PERCENT_CHANCE = MechanicConstants::PercentChance::MIN;
static auto constexpr MAX_PERCENT_CHANCE = MechanicConstants::PercentChance::MAX;
static auto constexpr CRIT_CHANCE = MechanicConstants::CRIT_CHANCE_DIVISORS[0];

constexpr std::array<DamageRollKind, 3U> branchingDamageRollOptions = {
  DamageRollKind::AVERAGE_DAMAGE,
  DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
  DamageRollKind::MIN_DAMAGE | DamageRollKind::MAX_DAMAGE,
};

constexpr std::array<DamageRollKind, 4U> fixedBranchDamageRollOptions = {
  DamageRollKind::ALL_DAMAGE_ROLLS,
  DamageRollKind::AVERAGE_DAMAGE,
  DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE,
  DamageRollKind::MIN_DAMAGE | DamageRollKind::MAX_DAMAGE,
};

struct VerticalSliceChecks : TestChecks {
 private:
  const simulate_turn::Options* options;
  entt::delegate<void()> checkPokemon;

  void checkBattle() const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<simulate_turn::TurnOutcomeBattles, simulate_turn::tags::SpeedSortNeeded>();

    debug::TypesToIgnore typesIgnoredOnConstants = typesToIgnore;
    typesToIgnore.add<Probability, ParentBattle, Turn, RootBattle>();

    if (!options->makeBranchesOnRandomEvents) {
      typesToIgnore.add<RngSeed>();
    }

    for (types::entity entity : registry->view<tags::Battle>()) {
      if (!isSpecificallyChecked(entity)) {
        continue;
      }
      types::entity original = debug::findCopyParent(currentEntitiesToInitial, *registry, entity);
      bool shouldNotChange = !options->applyChangesToInputBattle && original == entity;
      debug::areEntitiesEqual(
        *registry,
        entity,
        registryOnInput,
        getInitialEntity(entity),
        shouldNotChange ? typesIgnoredOnConstants : typesToIgnore);
    }
  }

  void checkSides() const { specificallyCheckEntities<tags::Side, SideDecision>(); }

  void checkMoves() const { specificallyCheckEntities<MoveName, Pp>(); }

 public:
  VerticalSliceChecks(
    const Simulation& _simulation, const types::entityVector& specificallyCheckedEntities,
    entt::delegate<void()> _checkPokemon)
      : TestChecks(_simulation, specificallyCheckedEntities),
        options(&_simulation.simulateTurnOptions),
        checkPokemon(_checkPokemon) {}

  void checkEntities() const {
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

  types::stat initialSpeed(types::entity pokemon) const {
    types::entity initial = getInitialEntity(pokemon);
    return registryOnInput.get<stat::EffectiveSpe>(initial).val;
  }

  const RngSeed& initialRngSeed(types::entity battle) const {
    types::entity initial = getInitialEntity(battle);
    return registryOnInput.get<RngSeed>(initial);
  }

  types::entity parentBattle(types::entity battle) const {
    return debug::findCopyParent(currentEntitiesToInitial, *registry, battle);
  }
};

struct VerticalSliceDamageValueInfo {
 protected:
  std::vector<types::damage> baseDamage;
  std::vector<types::damage> critDamage;
  types::damage averageRegularDamage;
  types::damage averageCritDamage;
  types::stat startingHp;
  DamageRollKind damageRollKind;

  bool checkWasCrit;
  bool willCrit;
  bool willChooseAverageDamage;
  bool willChooseMinOrMaxDamage;

  types::percentChance lowerLimit;
  types::percentChance upperLimit;
  types::probability branchProbabilityLowerLimit;

  std::size_t uniqueRolls(const std::vector<types::damage>& rolls) const {
    if (rolls.empty()) {
      return 0UL;
    }
    if (damageRollKind == branchingDamageRollOptions[2]) {
      if (willChooseMinOrMaxDamage) {
        return std::min(rolls.front(), startingHp) == std::min(rolls.back(), startingHp) ? 1UL : 2UL;
      }
      return 1UL;
    }
    if (willChooseAverageDamage) {
      return 1UL;
    }

    types::damage lastDamageValue = rolls[0];
    std::size_t unique = 1UL;

    for (std::size_t i = 1UL; i < rolls.size(); i++) {
      types::damage damage = rolls[i];
      damage = std::min(damage, startingHp);
      if (damage != lastDamageValue) {
        unique++;
        lastDamageValue = damage;
      }
    }

    return unique;
  }

  bool chanceWithinSimulationBounds(types::percentChance percentChance) const {
    if (upperLimit <= percentChance) {
      return false;
    }

    if (lowerLimit >= percentChance) {
      return false;
    }

    types::percentChance lowerBrachPercentChance = std::min(MAX_PERCENT_CHANCE, percentChance);
    lowerBrachPercentChance =
      std::min(lowerBrachPercentChance, (types::percentChance)(MAX_PERCENT_CHANCE - lowerBrachPercentChance));

    return branchProbabilityLowerLimit * MAX_PERCENT_CHANCE < lowerBrachPercentChance;
  }

 public:
  VerticalSliceDamageValueInfo(
    const std::vector<types::damage>& _baseDamage, types::damage _averageRegularDamage,
    const std::vector<types::damage>& _critDamage, types::damage _averageCritDamage, types::stat _startingHp,
    DamageRollKind _damageRollKind, const simulate_turn::Options& options)
      : baseDamage(_baseDamage),
        critDamage(_critDamage),
        averageRegularDamage(_averageRegularDamage),
        averageCritDamage(_averageCritDamage),
        startingHp(_startingHp),
        damageRollKind(_damageRollKind),

        checkWasCrit(true),
        willCrit(damageRollKind == branchingDamageRollOptions[1]),
        willChooseAverageDamage(damageRollKind & DamageRollKind::AVERAGE_DAMAGE),
        willChooseMinOrMaxDamage(options.makeBranchesOnRandomEvents),

        lowerLimit(options.randomChanceLowerLimit.value_or(MIN_PERCENT_CHANCE)),
        upperLimit(options.randomChanceUpperLimit.value_or(MAX_PERCENT_CHANCE)),
        branchProbabilityLowerLimit(options.branchProbabilityLowerLimit.value_or(MIN_PROBABILITY)) {
    checkWasCrit &= chanceWithinSimulationBounds(MAX_PERCENT_CHANCE / CRIT_CHANCE);
    checkWasCrit &= damageRollKind != branchingDamageRollOptions[1];

    willChooseMinOrMaxDamage |= chanceWithinSimulationBounds(MAX_PERCENT_CHANCE / 2U);
    willChooseMinOrMaxDamage &= damageRollKind == branchingDamageRollOptions[2];
  }

  entt::dense_set<types::stat> possibleHpValues() const {
    entt::dense_set<types::stat> hpValues;
    auto addDamageValues = [&](const std::vector<types::damage>& damages) {
      for (types::damage damage : damages) {
        hpValues.insert(damage > startingHp ? 0U : startingHp - damage);
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
    std::size_t uniqueRegularDamage = uniqueRolls(baseDamage);
    std::size_t uniqueCritDamage = uniqueRolls(critDamage);
    if (willCrit) {
      return uniqueCritDamage;
    }
    if (checkWasCrit) {
      return uniqueCritDamage + uniqueRegularDamage;
    }
    return uniqueRegularDamage;
  }
};

TEST_CASE("Simulate Turn: Vertical Slice 1, Single Battle", "[Simulation][SimulateTurn][SingleBattle]") {
  struct Checks : VerticalSliceChecks {
    Checks(const Simulation& _simulation, const types::entityVector& specificallyCheckedEntities)
        : VerticalSliceChecks(
            _simulation, specificallyCheckedEntities, {entt::connect_arg<&Checks::checkPokemon>, this}) {}

    void checkPokemon() const {
      specificallyCheckEntities<
        tags::Pokemon,
        stat::CurrentHp,
        LastUsedMove,
        StatusName,
        status::tags::Paralysis,
        stat::EffectiveSpe>();
    }
  };

  struct DamageValueInfo : VerticalSliceDamageValueInfo {
   private:
    bool checkWasParalyzed;

    types::percentChance THUNDERBOLT_PAR_CHANCE =
      dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>::targetSecondaryEffect::chance;

   public:
    DamageValueInfo(
      PlayerSideId sideId, const std::vector<types::damage>& _baseDamage, types::damage _averageRegularDamage,
      const std::vector<types::damage>& _critDamage, types::damage _averageCritDamage, types::stat _startingHp,
      DamageRollKind _damageRollKind, const simulate_turn::Options& options)
        : VerticalSliceDamageValueInfo(
            _baseDamage, _averageRegularDamage, _critDamage, _averageCritDamage, _startingHp, _damageRollKind, options),
          checkWasParalyzed(sideId == PlayerSideId::P2) {
      checkWasParalyzed &= chanceWithinSimulationBounds(THUNDERBOLT_PAR_CHANCE);
    }

    std::size_t possibilities() const {
      std::size_t count = 1U;

      if (checkWasParalyzed) {
        count *= 2U;
      }
      count *= uniqueDamageCount();

      return count;
    }

    types::probability getProbability(types::stat afterTurnHp, bool causedParalysisFromThunderbolt) const {
      types::damage damage = startingHp - afterTurnHp;
      types::probability critDamageRollInstances =
        (types::probability)std::count(critDamage.begin(), critDamage.end(), damage);
      bool wasCrit = critDamageRollInstances != 0.0F ||
                     (damageRollKind & DamageRollKind::AVERAGE_DAMAGE && damage == averageCritDamage);

      types::probability probability = 1.0F;
      if (checkWasCrit) {
        if (wasCrit) {
          probability /= CRIT_CHANCE;
        }
        else {
          probability *= 1.0F - (1.0F / CRIT_CHANCE);
        }
      }

      if (mightCauseParalysis()) {
        if (causedParalysisFromThunderbolt) {
          probability *= THUNDERBOLT_PAR_CHANCE / (types::probability)(MAX_PERCENT_CHANCE);
        }
        else {
          probability *= 1.0F - (THUNDERBOLT_PAR_CHANCE / (types::probability)(MAX_PERCENT_CHANCE));
        }
      }

      if (willChooseMinOrMaxDamage) {
        probability /= 2.0F;
      }
      if (damageRollKind == DamageRollKind::ALL_DAMAGE_ROLLS) {
        types::probability baseDamageRollInstances =
          (types::probability)std::count(baseDamage.begin(), baseDamage.end(), damage);
        REQUIRE(!!(baseDamageRollInstances || critDamageRollInstances));
        if (wasCrit) {
          probability *= critDamageRollInstances / MechanicConstants::DamageRollCount::MAX;
        }
        else {
          probability *= baseDamageRollInstances / MechanicConstants::DamageRollCount::MAX;
        }
      }

      return probability;
    }

    bool mightCauseParalysis() const { return checkWasParalyzed; }
  };

  static Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleCreationInfo;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};

  p1SlotDecision.moveChoice = dex::Move::KNOCK_OFF;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::THUNDERBOLT;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};
  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};

  Simulation simulation = createSingleBattleSimulation(pokedex, battleCreationInfo);
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

  CAPTURE(applyChangesToInputBattle, branchProbabilityLowerLimit, numberOfSamples);
  INFO("randomChanceLowerLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceLowerLimit));
  INFO("randomChanceUpperLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceUpperLimit));
  CAPTURE(damageRollOptions.p1, damageRollOptions.p2);

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
    160U,
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

  const types::registry& registry = simulation.registry;

  types::entityVector specificallyCheckedEntities;
  for (types::entity battle : registry.view<tags::Battle>()) {
    specificallyCheckedEntities.push_back(battle);
  }

  for (types::entity side : registry.view<tags::Side>()) {
    specificallyCheckedEntities.push_back(side);
  }

  for (types::entity pokemon : registry.view<tags::Pokemon>()) {
    specificallyCheckedEntities.push_back(pokemon);
  }

  for (const auto& [entity, move] : registry.view<MoveName>().each()) {
    if (move.name == dex::Move::KNOCK_OFF || move.name == dex::Move::THUNDERBOLT) {
      specificallyCheckedEntities.push_back(entity);
    }
  }

  Checks checks{simulation, specificallyCheckedEntities};
  const auto result = simulation.simulateTurn();
  checks.checkEntities();

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
    const auto& [p1Hp, p1LastUsedMove, p1Speed] =
      registry.get<stat::CurrentHp, LastUsedMove, stat::EffectiveSpe>(p1Pokemon);
    const auto& [p2Hp, p2LastUsedMove, p2Speed] =
      registry.get<stat::CurrentHp, LastUsedMove, stat::EffectiveSpe>(p2Pokemon);
    const Pp& p1Pp = registry.get<Pp>(p1Move);
    const Pp& p2Pp = registry.get<Pp>(p2Move);

    types::stat initialP1Speed = checks.initialSpeed(p1Pokemon);
    types::stat initialP2Speed = checks.initialSpeed(p2Pokemon);

    if (!applyChangesToInputBattle) {
      REQUIRE_FALSE(registry.all_of<simulate_turn::TurnOutcomeBattles>(battle));
    }

    types::entity parentBattle = checks.parentBattle(battle);
    REQUIRE(rootBattle.val == parentBattle);

    REQUIRE(turn.val == 2U);
    const auto& initialRngSeed = checks.initialRngSeed(battle);
    if (options.makeBranchesOnRandomEvents || totalPossibilities == 1U) {
      REQUIRE(rngSeed.val == initialRngSeed.val);
    }
    else {
      REQUIRE_FALSE(rngSeed.val == initialRngSeed.val);
    }

    REQUIRE_FALSE(registry.all_of<SideDecision>(p1Side));
    REQUIRE_FALSE(registry.all_of<SideDecision>(p2Side));

    REQUIRE(p1LastUsedMove.val == p1Move);
    REQUIRE(p2LastUsedMove.val == p2Move);
    REQUIRE(p1Pp.val == (p1Info.moves[1].pp - 1U));
    REQUIRE(p2Pp.val == (p2Info.moves[0].pp - 1U));

    if (!p2DamageInfo.mightCauseParalysis() || !p1Paralyzed) {
      REQUIRE_FALSE(registry.all_of<StatusName>(p1Pokemon));
      REQUIRE_FALSE(p1Paralyzed);
      REQUIRE(p1Speed.val == initialP1Speed);
    }
    if (p1Paralyzed) {
      REQUIRE(registry.get<StatusName>(p1Pokemon).name == dex::Status::PAR);
      REQUIRE(p1Speed.val == (initialP1Speed / 2U));
    }

    REQUIRE_FALSE(registry.all_of<StatusName>(p2Pokemon));
    REQUIRE_FALSE(registry.all_of<status::tags::Paralysis>(p2Pokemon));
    REQUIRE(p2Speed.val == initialP2Speed);

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

TEST_CASE("Simulate Turn: Vertical Slice 1, Double Battle", "[Simulation][SimulateTurn][DoubleBattle]") {
  struct Checks : VerticalSliceChecks {
    Checks(const Simulation& _simulation, const types::entityVector& specificallyCheckedEntities)
        : VerticalSliceChecks(
            _simulation, specificallyCheckedEntities, {entt::connect_arg<&Checks::checkPokemon>, this}) {}

    void checkPokemon() const {}
  };

  struct DamageValueInfo : VerticalSliceDamageValueInfo {
   private:
    bool checkMoveMissed;
    bool checkMoveDroppedSpa;

    types::percentChance MOONBLAST_HIT_CHANCE = chainValueToModifier(
      dex::Moonblast<GameMechanics::SCARLET_VIOLET>::accuracy,
      dex::BrightPowder<GameMechanics::SCARLET_VIOLET>::onModifyAccuracyNumerator,
      dex::BrightPowder<GameMechanics::SCARLET_VIOLET>::onModifyAccuracyDenominator);

    types::percentChance MOONBLAST_SPA_DROP_CHANCE =
      dex::Moonblast<GameMechanics::SCARLET_VIOLET>::targetSecondaryEffect::chance;

   public:
    DamageValueInfo(
      Slot slot, const std::vector<types::damage>& _baseDamage, types::damage _averageRegularDamage,
      const std::vector<types::damage>& _critDamage, types::damage _averageCritDamage, types::stat _startingHp,
      DamageRollKind _damageRollKind, const simulate_turn::Options& options)
        : VerticalSliceDamageValueInfo(
            _baseDamage, _averageRegularDamage, _critDamage, _averageCritDamage, _startingHp, _damageRollKind, options),
          checkMoveMissed(slot == Slot::P2B),
          checkMoveDroppedSpa(slot == Slot::P2B) {
      if (slot != Slot::P2B) {
        return;
      }

      checkMoveMissed &= chanceWithinSimulationBounds(MOONBLAST_HIT_CHANCE);
      checkMoveDroppedSpa &= chanceWithinSimulationBounds(MOONBLAST_SPA_DROP_CHANCE);
    }

    std::size_t possibilities() const {
      if (willCrit) {
        return checkMoveMissed ? 2UL : 1UL;
      }

      std::size_t count = uniqueDamageCount();

      if (checkMoveDroppedSpa) {
        count += uniqueRolls(baseDamage);
      }

      if (checkMoveMissed) {
        count++;
      }

      return count;
    }
  };

  static Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation::BattleCreationInfo battleCreationInfo;

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

  auto simulation = createDoubleBattleSimulation(pokedex, battleCreationInfo);
  auto& p1AInfo = battleCreationInfo.p1.team[0];
  auto& p1BInfo = battleCreationInfo.p1.team[1];
  auto& p2AInfo = battleCreationInfo.p2.team[0];
  auto& p2BInfo = battleCreationInfo.p2.team[1];
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

  CAPTURE(applyChangesToInputBattle, branchProbabilityLowerLimit, numberOfSamples);
  INFO("randomChanceLowerLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceLowerLimit));
  INFO("randomChanceUpperLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceUpperLimit));
  CAPTURE(damageRollOptions.p1, damageRollOptions.p2);

  auto& options = simulation.simulateTurnOptions;
  options.applyChangesToInputBattle = applyChangesToInputBattle;
  options.branchProbabilityLowerLimit = branchProbabilityLowerLimit;
  options.makeBranchesOnRandomEvents = !numberOfSamples.has_value();
  options.randomChanceLowerLimit = randomChanceLowerLimit;
  options.randomChanceUpperLimit = randomChanceUpperLimit;
  options.damageRollsConsidered = damageRollOptions;

  DamageValueInfo p1BFullDamageInfo(
    Slot::P1B,
    {525U, 517U, 515U, 507U, 502U, 499U, 491U, 486U, 484U, 476U, 471U, 468U, 460U, 455U, 452U, 445U},
    485U,
    {788U, 775U, 767U, 759U, 751U, 743U, 736U, 728U, 720U, 712U, 704U, 697U, 689U, 681U, 673U, 665U},
    728U,
    p1BInfo.stats.hp,
    damageRollOptions.p1,
    options);
  DamageValueInfo p1BHalfDamageInfo(
    Slot::P1B,
    {263U, 259U, 257U, 253U, 251U, 250U, 246U, 243U, 242U, 238U, 235U, 234U, 230U, 227U, 226U, 222U},
    243U,
    {394U, 387U, 383U, 380U, 376U, 372U, 368U, 364U, 360U, 356U, 352U, 348U, 344U, 341U, 337U, 333U},
    364U,
    p1BInfo.stats.hp,
    damageRollOptions.p1,
    options);
  DamageValueInfo p2BDamageInfo(
    Slot::P2B,
    {190U, 187U, 186U, 184U, 181U, 180U, 178U, 177U, 174U, 172U, 171U, 169U, 166U, 165U, 163U, 160U},
    175U,
    {285U, 282U, 279U, 276U, 273U, 270U, 267U, 264U, 261U, 258U, 256U, 253U, 250U, 247U, 244U, 241U},
    263U,
    p2BInfo.stats.hp,
    damageRollOptions.p2,
    options);

  const types::percentChance willOWispHitChance = dex::WillOWisp<GameMechanics::SCARLET_VIOLET>::accuracy;
  const bool canWillOWispMiss = (!randomChanceUpperLimit.has_value() || randomChanceUpperLimit > willOWispHitChance) &&
                                (!branchProbabilityLowerLimit.has_value() ||
                                 branchProbabilityLowerLimit < 1.0F / (MAX_PERCENT_CHANCE - willOWispHitChance));

  std::vector<Simulation::BattleCreationInfo> battleCreationInfoList;
  for (types::entityIndex i = 0U; i < numberOfSamples.value_or(1U); i++) {
    battleCreationInfoList.push_back(battleCreationInfo);
  }

  std::size_t idealTurnOutcomeCount = 0U;
  std::size_t totalPossibilities =
    (canWillOWispMiss ? (p2BDamageInfo.possibilities() * p1BFullDamageInfo.possibilities()) : 0UL) +
    (p2BDamageInfo.possibilities() * p1BHalfDamageInfo.possibilities());

  if (options.makeBranchesOnRandomEvents) {
    idealTurnOutcomeCount = totalPossibilities;
  }
  else {
    idealTurnOutcomeCount = numberOfSamples.value();
  }

  simulation.createInitialStates(battleCreationInfoList);
  updateAllStats(simulation);

  const auto originalBattles = simulation.selectedBattleEntities();
  const types::registry& registry = simulation.registry;

  types::entityVector specificallyCheckedEntities;
  for (types::entity battle : registry.view<tags::Battle>()) {
    specificallyCheckedEntities.push_back(battle);
  }

  for (types::entity side : registry.view<tags::Side>()) {
    specificallyCheckedEntities.push_back(side);
  }

  for (types::entity pokemon : registry.view<tags::Pokemon>()) {
    specificallyCheckedEntities.push_back(pokemon);
  }

  for (types::entity move : registry.view<MoveName>()) {
    specificallyCheckedEntities.push_back(move);
  }

  Checks checks{simulation, specificallyCheckedEntities};
  const auto result = simulation.simulateTurn();
  checks.checkEntities();

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
}
}  // namespace pokesim
