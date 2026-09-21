#include <algorithm>

#include "Tests.hpp"

namespace pokesim {
namespace {
GameMechanics constexpr TestMechanic = GameMechanics::SCARLET_VIOLET;

auto constexpr MAX_PROBABILITY = Constants::Probability::MAX;
auto constexpr MAX_PERCENT_CHANCE = Constants::PercentChance::MAX;
auto constexpr CHANCE_TO_PROBABILITY = Constants::PercentChanceToProbability;
auto constexpr PROBABILITY_TO_CHANCE = Constants::ProbabilityToPercentChance;
auto constexpr CRIT_PROBABILITY = MAX_PROBABILITY / MechanicConstants::CRIT_CHANCE_DIVISORS(TestMechanic)[0];
auto constexpr STAT_BOOST_STAGES = Constants::STAT_BOOST_STAGES;
auto constexpr MIN_HP = Constants::PokemonCurrentHpStat::MIN;

auto constexpr ALL_DAMAGE = DamageRollKind::ALL_DAMAGE_ROLLS;
auto constexpr AVERAGE_DAMAGE = DamageRollKind::AVERAGE_DAMAGE;
auto constexpr AVERAGE_CRIT_DAMAGE = DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE;
auto constexpr MIN_AND_MAX_DAMAGE = DamageRollKind::MIN_DAMAGE | DamageRollKind::MAX_DAMAGE;

constexpr std::array<DamageRollKind, 3U> branchingDamageRollOptions = {
  AVERAGE_DAMAGE,
  AVERAGE_CRIT_DAMAGE,
  MIN_AND_MAX_DAMAGE,
};

constexpr std::array<DamageRollKind, 4U> fixedBranchDamageRollOptions = {
  ALL_DAMAGE,
  AVERAGE_DAMAGE,
  AVERAGE_CRIT_DAMAGE,
  MIN_AND_MAX_DAMAGE,
};

auto runAndCheckSimulation(TestSimulation& test, std::size_t idealTurnOutcomeCount, std::size_t totalPossibilities) {
  test.initializeSimulation();

  types::registry& registry = test.registry();
  auto& options = test.simulateTurnOptions();
  auto originalBattles = test.simulation.battleEntities();

  auto results = test.simulateTurn(Tags<Probability, RngSeed>{}, Tags<FoesRemaining>{});
  auto rootBattles = results.rootBattles();

  if (!options.getApplyChangesToInputBattle()) {
    REQUIRE_THAT(rootBattles, Catch::Matchers::UnorderedEquals(originalBattles));
  }

  if (!options.getMakeBranchesOnRandomEvents()) {
    types::entityIndex finalEntityCount = test.checks.getFinalEntityCount();
    if (options.getApplyChangesToInputBattle()) {
      REQUIRE(finalEntityCount == test.checks.initialEntityCount);
    }
    else {
      REQUIRE(finalEntityCount == (test.checks.initialEntityCount * 2U));
    }
  }

  REQUIRE(rootBattles.size() == test.battleInfoList.size());
  types::entityVector allTurnOutcomes{results.battleOutcomes().begin(), results.battleOutcomes().end()};

  for (types::entity battle : allTurnOutcomes) {
    if (registry.all_of<RootBattle>(battle)) {
      types::entity parentBattle = test.checks.getParentEntity(battle);
      REQUIRE(registry.get<RootBattle>(battle).val == parentBattle);
    }

    Sides sides = registry.get<Sides>(battle);
    REQUIRE_FALSE(registry.all_of<SideDecision>(sides.val.p1()));
    REQUIRE_FALSE(registry.all_of<SideDecision>(sides.val.p2()));

    auto [initialRngSeed, currentRngSeed] = test.checks.getInitialAndCurrent<RngSeed>(battle);
    if (options.getMakeBranchesOnRandomEvents() || totalPossibilities == 1U) {
      REQUIRE(currentRngSeed.val == initialRngSeed.val);
    }
    else {
      REQUIRE_FALSE(currentRngSeed.val == initialRngSeed.val);
    }
  }
  REQUIRE(allTurnOutcomes.size() == idealTurnOutcomeCount);

  for (types::entity originalBattle : originalBattles) {
    if (options.getApplyChangesToInputBattle()) {
      REQUIRE_THAT(allTurnOutcomes, Catch::Matchers::Contains(originalBattle));
    }
    else {
      REQUIRE_THAT(allTurnOutcomes, !Catch::Matchers::Contains(originalBattle));
    }
  }

  return allTurnOutcomes;
}

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
      return 0U;
    }
    if (damageRollKind == MIN_AND_MAX_DAMAGE) {
      if (willChooseMinOrMaxDamage) {
        return std::min(rolls.front(), startingHp) == std::min(rolls.back(), startingHp) ? 1U : 2U;
      }
      return 1U;
    }
    if (willChooseAverageDamage) {
      return 1U;
    }

    types::damage lastDamageValue = rolls[0];
    std::size_t unique = 1U;

    for (std::size_t i = 1U; i < rolls.size(); i++) {
      types::damage damage = rolls[i];
      damage = std::min(damage, startingHp);
      if (damage != lastDamageValue) {
        unique++;
        lastDamageValue = damage;
      }
    }

    return unique;
  }

  std::size_t damageRollMatches(
    types::damage damageDone, const std::vector<types::damage>& rolls, types::damage averageDamage) const {
    if (AVERAGE_DAMAGE & damageRollKind && damageDone == std::min(averageDamage, startingHp)) {
      return 1U;
    }

    std::size_t matches = 0U;
    for (types::damage roll : rolls) {
      if (damageDone == std::min(roll, startingHp)) {
        matches++;
      }
    }

    return matches;
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

  types::probability getProbability(types::stat afterTurnHp) const {
    types::damage damage = startingHp - afterTurnHp;
    types::probability critDamageRollInstances =
      (types::probability)damageRollMatches(damage, critDamage, averageCritDamage);
    bool wasCrit = critDamageRollInstances != 0.0F;

    types::probability probability = MAX_PROBABILITY;
    if (checkWasCrit) {
      if (wasCrit) {
        probability *= CRIT_PROBABILITY;
      }
      else {
        probability *= MAX_PROBABILITY - CRIT_PROBABILITY;
      }
    }

    if (willChooseMinOrMaxDamage && afterTurnHp != MIN_HP) {
      probability /= 2.0F;
    }
    if (damageRollKind == ALL_DAMAGE) {
      types::probability baseDamageRollInstances =
        (types::probability)damageRollMatches(damage, baseDamage, averageRegularDamage);
      REQUIRE(!!(baseDamageRollInstances || critDamageRollInstances));

      if (wasCrit) {
        probability *= critDamageRollInstances / Constants::DamageRollCount::MAX;
      }
      else {
        probability *= baseDamageRollInstances / Constants::DamageRollCount::MAX;
      }
    }

    return probability;
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
        willCrit(damageRollKind == AVERAGE_CRIT_DAMAGE),
        willChooseAverageDamage(damageRollKind & AVERAGE_DAMAGE),
        willChooseMinOrMaxDamage(options.getMakeBranchesOnRandomEvents()),

        lowerLimit(options.getRandomChanceLowerLimit()),
        upperLimit(options.getRandomChanceUpperLimit()),
        branchProbabilityLowerLimit(options.getBranchProbabilityLowerLimit()) {
    checkWasCrit &= chanceWithinSimulationBounds((types::percentChance)(PROBABILITY_TO_CHANCE * CRIT_PROBABILITY));
    checkWasCrit &= damageRollKind != AVERAGE_CRIT_DAMAGE;

    willChooseMinOrMaxDamage |= chanceWithinSimulationBounds(MAX_PERCENT_CHANCE / 2U);
    willChooseMinOrMaxDamage &= damageRollKind == MIN_AND_MAX_DAMAGE;
  }

  entt::dense_set<types::stat> possibleHpValues() const {
    entt::dense_set<types::stat> hpValues;
    auto addDamageValues = [&](const std::vector<types::damage>& damages) {
      for (types::damage damage : damages) {
        hpValues.insert(damage + MIN_HP > startingHp ? MIN_HP : startingHp - damage);
      }
    };

    if (damageRollKind == ALL_DAMAGE) {
      if (checkWasCrit) {
        addDamageValues(critDamage);
      }
      addDamageValues(baseDamage);
    }
    else if (damageRollKind == AVERAGE_DAMAGE) {
      if (checkWasCrit) {
        addDamageValues({averageCritDamage});
      }
      addDamageValues({averageRegularDamage});
    }
    else if (damageRollKind == AVERAGE_CRIT_DAMAGE) {
      addDamageValues({averageCritDamage});
    }
    else if (damageRollKind == MIN_AND_MAX_DAMAGE) {
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

  bool mightCrit() const { return checkWasCrit; }
  bool guaranteedCrit() const { return willCrit; }
};
}  // namespace

TEST_CASE(
  "Simulate Turn: Vertical Slice 1, Single Battle", "[Simulation][SimulateTurn][VerticalSlice1][SingleBattle]") {
  static constexpr auto THUNDERBOLT_PAR_CHANCE = dex::Thunderbolt::targetSecondaryEffect::chance(TestMechanic);
  static constexpr auto PARALYSIS_SPEED_DIVISOR =
    dex::Paralysis::speedDivisor(TestMechanic) / dex::Paralysis::speedDividend(TestMechanic);

  struct DamageValueInfo : VerticalSliceDamageValueInfo {
   private:
    bool checkWasParalyzed;

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
      types::probability probability = VerticalSliceDamageValueInfo::getProbability(afterTurnHp);
      if (mightCauseParalysis()) {
        if (causedParalysisFromThunderbolt) {
          probability *= THUNDERBOLT_PAR_CHANCE * CHANCE_TO_PROBABILITY;
        }
        else {
          probability *= MAX_PROBABILITY - (THUNDERBOLT_PAR_CHANCE * CHANCE_TO_PROBABILITY);
        }
      }

      return probability;
    }

    bool mightCauseParalysis() const { return checkWasParalyzed; }
  };

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
    damageRollOptions.setP1(GENERATE(from_range(fixedBranchDamageRollOptions)));
    damageRollOptions.setP2(GENERATE(from_range(fixedBranchDamageRollOptions)));
  }
  else {
    damageRollOptions.setP1(GENERATE(from_range(branchingDamageRollOptions)));
    damageRollOptions.setP2(GENERATE(from_range(branchingDamageRollOptions)));
  }

  CAPTURE(applyChangesToInputBattle, branchProbabilityLowerLimit, numberOfSamples);
  INFO("randomChanceLowerLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceLowerLimit));
  INFO("randomChanceUpperLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceUpperLimit));
  CAPTURE(damageRollOptions.getP1(), damageRollOptions.getP2());

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  for (types::entityIndex i = 0U; i < numberOfSamples.value_or(1U); i++) {
    test.setupBattle(
      Turn{1U},
      test.side(test.pokemon(
        dex::Species::EMPOLEON,
        dex::Item::ASSAULT_VEST,
        dex::Ability::COMPETITIVE,
        dex::Gender::MALE,
        Level{99U},
        Evs{0U, 25U, 50U, 75U, 100U, 125U},
        dex::Move::FURY_ATTACK,
        dex::Move::KNOCK_OFF)),
      test.side(test.pokemon(
        dex::Species::AMPHAROS,
        dex::Ability::PLUS,
        dex::Gender::FEMALE,
        dex::Nature::HARDY,
        Ivs{5U, 10U, 15U, 20U, 25U, 30U},
        dex::Move::THUNDERBOLT)),
      test.turnDecision(dex::Move::KNOCK_OFF, dex::Move::THUNDERBOLT));
  }

  auto& options = test.simulateTurnOptions();
  options.setApplyChangesToInputBattle(applyChangesToInputBattle);
  if (branchProbabilityLowerLimit.has_value())
    options.setBranchProbabilityLowerLimit(branchProbabilityLowerLimit.value());
  options.setMakeBranchesOnRandomEvents(!numberOfSamples.has_value());
  if (randomChanceUpperLimit.has_value()) options.setRandomChanceUpperLimit(randomChanceUpperLimit.value());
  if (randomChanceLowerLimit.has_value()) options.setRandomChanceLowerLimit(randomChanceLowerLimit.value());
  options.setDamageRollsConsidered(damageRollOptions);

  const DamageValueInfo p1DamageInfo(
    PlayerSideId::P1,
    {174U, 170U, 168U, 168U, 164U, 164U, 162U, 158U, 158U, 156U, 156U, 152U, 152U, 150U, 146U, 146U},  // 10
    160U,
    {260U, 258U, 254U, 252U, 248U, 246U, 242U, 240U, 240U, 236U, 234U, 230U, 228U, 224U, 222U, 218U},  // 15
    240U,
    275U,
    damageRollOptions.getP1(),
    options);

  const DamageValueInfo p2DamageInfo(
    PlayerSideId::P2,
    {52U, 51U, 50U, 50U, 49U, 49U, 48U, 48U, 47U, 47U, 46U, 46U, 45U, 45U, 44U, 44U},  // 9
    48U,
    {78U, 77U, 76U, 75U, 74U, 74U, 73U, 72U, 71U, 70U, 70U, 69U, 68U, 67U, 67U, 66U},  // 13
    72U,
    295U,
    damageRollOptions.getP2(),
    options);

  std::size_t idealTurnOutcomeCount = 0U;
  std::size_t totalPossibilities = p1DamageInfo.possibilities() * p2DamageInfo.possibilities();

  if (options.getMakeBranchesOnRandomEvents()) {
    idealTurnOutcomeCount = totalPossibilities;
  }
  else {
    idealTurnOutcomeCount = numberOfSamples.value();
  }

  auto expectedP1Hp = p1DamageInfo.possibleHpValues();
  auto expectedP2Hp = p2DamageInfo.possibleHpValues();

  auto allTurnOutcomes = runAndCheckSimulation(test, idealTurnOutcomeCount, totalPossibilities);

  entt::dense_set<types::stat> foundP1Hp;
  entt::dense_set<types::stat> foundP2Hp;

  const types::registry& registry = test.registry();
  for (types::entity battle : allTurnOutcomes) {
    types::moveSlotIndex p1MoveIndex = 1U;
    auto entities = test.getBattleEntities(battle);

    bool p1Paralyzed = registry.all_of<dex::Paralysis>(entities.p1A);
    stat::CurrentHp p1Hp = registry.get<stat::CurrentHp>(entities.p1A);
    stat::CurrentHp p2Hp = registry.get<stat::CurrentHp>(entities.p2A);
    CAPTURE(p1Paralyzed, p1Hp.val, p2Hp.val, expectedP1Hp, expectedP2Hp);

    Probability probability = registry.get<Probability>(battle);
    REQUIRE(registry.get<Turn>(battle).val == 2U);

    REQUIRE(registry.get<FoesRemaining>(entities.p1Side).val == 1U);
    REQUIRE(registry.get<FoesRemaining>(entities.p2Side).val == 1U);

    if (!p2DamageInfo.mightCauseParalysis() || !p1Paralyzed) {
      test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A, p1MoveIndex);
    }
    if (p1Paralyzed) {
      auto [initialP1Speed, currentP1Speed] = test.checks.getInitialAndCurrent<stat::EffectiveSpe>(entities.p1A);
      REQUIRE(registry.all_of<tags::HasStatus>(entities.p1A));
      REQUIRE(currentP1Speed.val == initialP1Speed.val / PARALYSIS_SPEED_DIVISOR);
      test.checks.checkUsedMovePokemon<stat::CurrentHp, tags::HasStatus, dex::Paralysis, stat::EffectiveSpe>(
        entities.p1A,
        p1MoveIndex);
    }

    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

    REQUIRE(expectedP1Hp.contains(p1Hp.val));
    REQUIRE(expectedP2Hp.contains(p2Hp.val));

    foundP1Hp.insert(p1Hp.val);
    foundP2Hp.insert(p2Hp.val);

    types::probability idealProbability =
      p1DamageInfo.getProbability(p1Hp.val, false) * p2DamageInfo.getProbability(p2Hp.val, p1Paralyzed);
    REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(idealProbability));
  }

  if (options.getMakeBranchesOnRandomEvents()) {
    REQUIRE(foundP1Hp.size() == expectedP1Hp.size());
    REQUIRE(foundP2Hp.size() == expectedP2Hp.size());
  }
}

TEST_CASE(
  "Simulate Turn: Vertical Slice 1, Double Battle", "[Simulation][SimulateTurn][VerticalSlice1][DoubleBattle]") {
  static constexpr auto MOONBLAST_HIT_CHANCE = (types::percentChance)internal::chainValueToModifier(
    dex::Moonblast::accuracy(TestMechanic),
    dex::BrightPowder::onModifyAccuracyNumerator(TestMechanic),
    dex::BrightPowder::onModifyAccuracyDenominator(TestMechanic));
  static constexpr auto MOONBLAST_SPA_DROP_CHANCE = dex::Moonblast::targetSecondaryEffect::chance(TestMechanic);
  static constexpr auto WILL_O_WISP_ACCURACY = dex::WillOWisp::accuracy(TestMechanic);
  static constexpr auto BURN_HP_DIVISOR = dex::Burn::onResidualHpDecreaseDivisor(TestMechanic);
  static constexpr auto FOCUS_SASH_HP_TO_KEEP = dex::FocusSash::onAfterModifyDamageHpToKeep(TestMechanic);
  static constexpr auto LIFE_ORB_HP_DIVISOR = dex::LifeOrb::onAfterMoveUsedHpDecreaseDivisor(TestMechanic);
  static constexpr auto QUIVER_DANCE_SPA_BOOST = dex::QuiverDance::targetPrimaryEffect::spaBoost(TestMechanic);
  static constexpr auto QUIVER_DANCE_SPD_BOOST = dex::QuiverDance::targetPrimaryEffect::spdBoost(TestMechanic);
  static constexpr auto QUIVER_DANCE_SPE_BOOST = dex::QuiverDance::targetPrimaryEffect::speBoost(TestMechanic);

  static constexpr types::stat P1A_MAX_HP = 222U;
  static constexpr types::stat P2A_MAX_HP = 314U;
  static constexpr types::stat P1B_MAX_HP = 291U;
  static constexpr types::stat P2B_MAX_HP = 237U;

  struct DamageValueInfo : VerticalSliceDamageValueInfo {
   private:
    dex::Item item;
    bool checkMoveMissed;
    bool checkMoveDroppedSpa;

   public:
    DamageValueInfo(
      Slot slot, dex::Item _item, const std::vector<types::damage>& _baseDamage, types::damage _averageRegularDamage,
      const std::vector<types::damage>& _critDamage, types::damage _averageCritDamage, types::stat _startingHp,
      DamageRollKind _damageRollKind, const simulate_turn::Options& options)
        : VerticalSliceDamageValueInfo(
            _baseDamage, _averageRegularDamage, _critDamage, _averageCritDamage, _startingHp, _damageRollKind, options),
          item(_item),
          checkMoveMissed(slot == Slot::P2B),
          checkMoveDroppedSpa(slot == Slot::P2B) {
      checkMoveMissed &= chanceWithinSimulationBounds(MOONBLAST_HIT_CHANCE);
      checkMoveDroppedSpa &= chanceWithinSimulationBounds(MOONBLAST_SPA_DROP_CHANCE);
    }

    std::size_t possibilities() const {
      if (willCrit) {
        return checkMoveMissed ? 2U : 1U;
      }

      std::size_t count = uniqueDamageCount();

      if (moveMightDropSpa()) {
        count += uniqueRolls(baseDamage);
      }

      if (moveMightMiss()) {
        count++;
      }

      return count;
    }

    auto possibleHpValues() const {
      auto expectedHp = VerticalSliceDamageValueInfo::possibleHpValues();
      if (item == dex::Item::FOCUS_SASH && expectedHp.contains(MIN_HP)) {
        expectedHp.insert(dex::FocusSash::onAfterModifyDamageHpToKeep(TestMechanic));
        expectedHp.erase(MIN_HP);
      }
      if (moveMightMiss()) {
        expectedHp.insert(startingHp);
      }
      return expectedHp;
    }

    types::probability getProbability(types::stat afterTurnHp, bool p2BSpaBoosted) const {
      if (item == dex::Item::FOCUS_SASH && afterTurnHp == dex::FocusSash::onAfterModifyDamageHpToKeep(TestMechanic)) {
        afterTurnHp = MIN_HP;
      }

      if (afterTurnHp == startingHp) {
        REQUIRE(moveMightMiss());
        return (MAX_PERCENT_CHANCE - MOONBLAST_HIT_CHANCE) * CHANCE_TO_PROBABILITY;
      }

      types::probability probability = VerticalSliceDamageValueInfo::getProbability(afterTurnHp);

      if (moveMightMiss()) {
        probability *= MOONBLAST_HIT_CHANCE * CHANCE_TO_PROBABILITY;
      }
      if (moveMightDropSpa() && afterTurnHp != MIN_HP) {
        if (p2BSpaBoosted) {
          probability *= (MAX_PERCENT_CHANCE - MOONBLAST_SPA_DROP_CHANCE) * CHANCE_TO_PROBABILITY;
        }
        else {
          probability *= MOONBLAST_SPA_DROP_CHANCE * CHANCE_TO_PROBABILITY;
        }
      }

      return probability;
    }

    bool moveMightMiss() const { return checkMoveMissed; }
    bool moveMightDropSpa() const { return checkMoveDroppedSpa; }
  };

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
    damageRollOptions.setP1(GENERATE(from_range(fixedBranchDamageRollOptions)));
    damageRollOptions.setP2(GENERATE(from_range(fixedBranchDamageRollOptions)));
  }
  else {
    damageRollOptions.setP1(GENERATE(from_range(branchingDamageRollOptions)));
    damageRollOptions.setP2(GENERATE(from_range(branchingDamageRollOptions)));
  }

  CAPTURE(applyChangesToInputBattle, branchProbabilityLowerLimit, numberOfSamples);
  INFO("randomChanceLowerLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceLowerLimit));
  INFO("randomChanceUpperLimit := " + Catch::StringMaker<std::optional<int>>::convert(randomChanceUpperLimit));
  CAPTURE(damageRollOptions.getP1(), damageRollOptions.getP2());

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  auto p1AInfo = test.pokemon(
    dex::Species::GARDEVOIR,
    dex::Ability::TRACE,
    dex::Gender::FEMALE,
    dex::Item::CHOICE_SCARF,
    dex::Status::BRN,
    Level{90U},
    Evs{0U, 25U, 50U, 75U, 100U, 125U},
    dex::Move::MOONBLAST);
  auto p2AInfo = test.pokemon(
    dex::Species::PANGORO,
    dex::Ability::IRON_FIST,
    dex::Gender::MALE,
    dex::Item::LIFE_ORB,
    Level{95U},
    Evs{125U, 100U, 75U, 50U, 25U, 0U},
    dex::Move::KNOCK_OFF);
  auto p1BInfo = test.pokemon(
    dex::Species::DRAGAPULT,
    dex::Ability::INFILTRATOR,
    dex::Gender::FEMALE,
    dex::Item::FOCUS_SASH,
    dex::Nature::HASTY,
    Ivs{5U, 10U, 15U, 20U, 25U, 30U},
    dex::Move::WILL_O_WISP);
  auto p2BInfo = test.pokemon(
    dex::Species::RIBOMBEE,
    dex::Ability::SWEET_VEIL,
    dex::Gender::MALE,
    dex::Item::BRIGHT_POWDER,
    Level{91U},
    Ivs{30U, 25U, 20U, 15U, 10U, 5U},
    dex::Move::QUIVER_DANCE);

  for (types::entityIndex i = 0U; i < numberOfSamples.value_or(1U); i++) {
    test.setupBattle(
      Turn{2U},
      Probability{0.9F},
      RngSeed{0x12345678},
      test.side(p1AInfo, p1BInfo),
      test.side(p2AInfo, p2BInfo),
      test.turnDecision(
        MoveDecision{Slot::P1A, Slot::P2B, dex::Move::MOONBLAST},
        MoveDecision{Slot::P1B, Slot::P2A, dex::Move::WILL_O_WISP},
        MoveDecision{Slot::P2A, Slot::P1B, dex::Move::KNOCK_OFF},
        MoveDecision{Slot::P2B, Slot::P2B, dex::Move::QUIVER_DANCE}));
  }

  auto& options = test.simulateTurnOptions();
  options.setApplyChangesToInputBattle(applyChangesToInputBattle);
  if (branchProbabilityLowerLimit.has_value())
    options.setBranchProbabilityLowerLimit(branchProbabilityLowerLimit.value());
  if (randomChanceUpperLimit.has_value()) options.setRandomChanceUpperLimit(randomChanceUpperLimit.value());
  if (randomChanceLowerLimit.has_value()) options.setRandomChanceLowerLimit(randomChanceLowerLimit.value());
  options.setMakeBranchesOnRandomEvents(!numberOfSamples.has_value());
  options.setDamageRollsConsidered(damageRollOptions);

  DamageValueInfo p1BHalfDamageInfo(
    Slot::P1B,
    p1BInfo.item.value(),
    {262U, 259U, 257U, 253U, 251U, 250U, 246U, 243U, 242U, 238U, 235U, 234U, 230U, 227U, 226U, 222U},
    242U,
    {394U, 387U, 383U, 380U, 376U, 372U, 368U, 364U, 360U, 356U, 352U, 348U, 344U, 341U, 337U, 333U},
    364U,
    P1B_MAX_HP,
    damageRollOptions.getP1(),
    options);
  DamageValueInfo p1BFullDamageInfo(
    Slot::P1B,
    p1BInfo.item.value(),
    {525U, 517U, 515U, 507U, 502U, 499U, 491U, 486U, 484U, 476U, 471U, 468U, 460U, 455U, 452U, 445U},
    485U,
    {788U, 775U, 767U, 759U, 751U, 743U, 736U, 728U, 720U, 712U, 704U, 697U, 689U, 681U, 673U, 665U},
    728U,
    P1B_MAX_HP,
    damageRollOptions.getP1(),
    options);
  DamageValueInfo p2BDamageInfo(
    Slot::P2B,
    p2BInfo.item.value(),
    {190U, 187U, 186U, 184U, 181U, 180U, 178U, 177U, 174U, 172U, 171U, 169U, 166U, 165U, 163U, 160U},
    175U,
    {285U, 282U, 279U, 276U, 273U, 270U, 267U, 264U, 261U, 258U, 256U, 253U, 250U, 247U, 244U, 241U},
    263U,
    P2B_MAX_HP,
    damageRollOptions.getP2(),
    options);

  bool willOWispMightMiss =
    (!randomChanceUpperLimit.has_value() || randomChanceUpperLimit > WILL_O_WISP_ACCURACY) &&
    (!branchProbabilityLowerLimit.has_value() ||
     branchProbabilityLowerLimit < MAX_PROBABILITY / (MAX_PERCENT_CHANCE - WILL_O_WISP_ACCURACY));

  std::size_t idealTurnOutcomeCount = 0U;
  std::size_t totalPossibilities =
    (willOWispMightMiss ? (p2BDamageInfo.possibilities() * p1BFullDamageInfo.possibilities()) : 0U) +
    (p2BDamageInfo.possibilities() * p1BHalfDamageInfo.possibilities());

  if (options.getMakeBranchesOnRandomEvents()) {
    idealTurnOutcomeCount = totalPossibilities;
  }
  else {
    idealTurnOutcomeCount = numberOfSamples.value();
  }

  auto expectedP1BHalfHp = p1BHalfDamageInfo.possibleHpValues();
  auto expectedP2BHp = p2BDamageInfo.possibleHpValues();
  auto expectedP1BAllHp = p1BFullDamageInfo.possibleHpValues();
  expectedP1BAllHp.insert(expectedP1BHalfHp.begin(), expectedP1BHalfHp.end());
  CAPTURE(
    willOWispMightMiss,
    idealTurnOutcomeCount,
    totalPossibilities,
    expectedP1BHalfHp,
    expectedP1BAllHp,
    expectedP2BHp);

  auto allTurnOutcomes = runAndCheckSimulation(test, idealTurnOutcomeCount, totalPossibilities);

  entt::dense_set<types::stat> foundP1BHp;
  entt::dense_set<types::stat> foundP2AHp;
  entt::dense_set<types::stat> foundP2BHp;
  entt::dense_map<types::probability, entt::dense_set<types::probability>> foundUncertainProbabilities;
  types::probability startingProbability = test.battleInfoList[0].probability.value();
  types::probability sumOfProbability = 0.0F;

  const types::registry& registry = test.registry();
  for (types::entity battle : allTurnOutcomes) {
    auto entities = test.getBattleEntities(battle);

    bool p2ABurned = registry.all_of<dex::Burn>(entities.p2A);
    bool p2BFainted = registry.all_of<tags::Fainted>(entities.p2B);
    bool p2BSpaBoosted = registry.all_of<SpaBoost>(entities.p2B);

    const auto& [turn, probability] = registry.get<Turn, Probability>(battle);
    const auto& [initialP1AHp, currentP1AHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p1A);
    const auto& [initialP1BHp, currentP1BHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p1B);
    const auto& [initialP2AHp, currentP2AHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
    const auto& [initialP2BHp, currentP2BHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2B);

    CAPTURE(
      currentP1AHp.val,
      currentP1BHp.val,
      currentP2AHp.val,
      currentP2BHp.val,
      p2ABurned,
      p2BFainted,
      p2BSpaBoosted,
      probability.val);

    REQUIRE(initialP1AHp.val == P1A_MAX_HP);
    REQUIRE(initialP1BHp.val == P1B_MAX_HP);
    REQUIRE(initialP2AHp.val == P2A_MAX_HP);
    REQUIRE(initialP2BHp.val == P2B_MAX_HP);

    REQUIRE(turn.val == 3U);
    REQUIRE(registry.get<FoesRemaining>(entities.p2Side).val == 2U);

    types::probability idealProbability = startingProbability;
    if (totalPossibilities == 1U) {
      REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(idealProbability));
    }

    sumOfProbability += probability.val;

    test.checks.checkUsedMovePokemon<stat::CurrentHp, ChoiceLock, DisabledMoveSlots>(entities.p1A);
    test.checks.checkUsedMovePokemon<stat::CurrentHp, dex::FocusSash, tags::HasItem>(entities.p1B);
    test.checks.checkUsedMovePokemon<stat::CurrentHp, tags::HasStatus, dex::Burn>(entities.p2A);

    // P1A (Gardevoir) Specific Checks
    const auto& [p1AChoiceLock, p1ADisabledMoveSlots] = registry.get<ChoiceLock, DisabledMoveSlots>(entities.p1A);
    types::stat p1ABurnHpDecrease = P1A_MAX_HP / BURN_HP_DIVISOR;
    REQUIRE(currentP1AHp.val == P1A_MAX_HP - p1ABurnHpDecrease);
    REQUIRE(p1AChoiceLock.val == 0U);
    REQUIRE(p1ADisabledMoveSlots.val[p1AChoiceLock.val] == true);

    // P1B (Dragapult) Specific Checks
    REQUIRE_FALSE(registry.all_of<dex::FocusSash>(entities.p1B));
    REQUIRE_FALSE(registry.all_of<tags::HasItem>(entities.p1B));
    if (p2ABurned) {
      REQUIRE(expectedP1BHalfHp.contains(currentP1BHp.val));
      idealProbability *= p1BHalfDamageInfo.getProbability(currentP1BHp.val, p2BSpaBoosted);
    }
    else {
      REQUIRE(currentP1BHp.val == FOCUS_SASH_HP_TO_KEEP);
    }
    foundP1BHp.insert(currentP1BHp.val);

    // P2A (Pangoro) Specific Checks
    types::stat p2ALifeOrbHpDecrease = P2A_MAX_HP / LIFE_ORB_HP_DIVISOR;
    types::stat p2ABurnHpDecrease = P2A_MAX_HP / BURN_HP_DIVISOR;
    if (p2ABurned) {
      REQUIRE(registry.all_of<tags::HasStatus>(entities.p2A));
      REQUIRE(currentP2AHp.val == P2A_MAX_HP - p2ALifeOrbHpDecrease - p2ABurnHpDecrease);

      if (willOWispMightMiss) {
        idealProbability *= WILL_O_WISP_ACCURACY * CHANCE_TO_PROBABILITY;
      }
    }
    else {
      REQUIRE(willOWispMightMiss);
      REQUIRE_FALSE(registry.all_of<tags::HasStatus>(entities.p2A));
      REQUIRE(currentP2AHp.val == P2A_MAX_HP - p2ALifeOrbHpDecrease);

      idealProbability *= (MAX_PERCENT_CHANCE - WILL_O_WISP_ACCURACY) * CHANCE_TO_PROBABILITY;
    }
    foundP2AHp.insert(currentP2AHp.val);

    // P2B (Ribombee) Specific Checks
    REQUIRE(expectedP2BHp.contains(currentP2BHp.val));
    if (p2BFainted) {
      REQUIRE(registry.get<FoesRemaining>(entities.p1Side).val == 1U);

      test.checks.checkEntityForChanges<tags::ActivePokemon, tags::Fainted, stat::CurrentHp>(entities.p2B);
      REQUIRE_FALSE(registry.all_of<tags::ActivePokemon>(entities.p2B));
      REQUIRE_FALSE(p2BSpaBoosted);
      REQUIRE(currentP2BHp.val == MIN_HP);
      REQUIRE((p2BDamageInfo.mightCrit() || p2BDamageInfo.guaranteedCrit()));
    }
    else {
      REQUIRE(registry.get<FoesRemaining>(entities.p1Side).val == 2U);
      test.checks.checkUsedMovePokemon<
        stat::CurrentHp,
        SpaBoost,
        SpdBoost,
        SpeBoost,
        stat::EffectiveSpa,
        stat::EffectiveSpd,
        stat::EffectiveSpe>(entities.p2B);

      const auto& [p2BSpdBoost, p2BSpeBoost] = registry.get<SpdBoost, SpeBoost>(entities.p2B);
      const auto& [p2BInitialSpa, p2BCurrentSpa] = test.checks.getInitialAndCurrent<stat::EffectiveSpa>(entities.p2B);
      const auto& [p2BInitialSpd, p2BCurrentSpd] = test.checks.getInitialAndCurrent<stat::EffectiveSpd>(entities.p2B);
      const auto& [p2BInitialSpe, p2BCurrentSpe] = test.checks.getInitialAndCurrent<stat::EffectiveSpe>(entities.p2B);

      REQUIRE_FALSE(currentP2BHp.val == MIN_HP);

      REQUIRE(p2BSpdBoost.val == QUIVER_DANCE_SPD_BOOST);
      REQUIRE(p2BCurrentSpd.val == (types::stat)(p2BInitialSpd.val * STAT_BOOST_STAGES[QUIVER_DANCE_SPD_BOOST]));
      REQUIRE(p2BSpeBoost.val == QUIVER_DANCE_SPE_BOOST);
      REQUIRE(p2BCurrentSpe.val == (types::stat)(p2BInitialSpe.val * STAT_BOOST_STAGES[QUIVER_DANCE_SPE_BOOST]));
      if (p2BSpaBoosted) {
        const SpaBoost& p2BSpaBoost = registry.get<SpaBoost>(entities.p2B);
        REQUIRE(p2BSpaBoost.val == QUIVER_DANCE_SPA_BOOST);
        REQUIRE(p2BCurrentSpa.val == (types::stat)(p2BInitialSpa.val * STAT_BOOST_STAGES[QUIVER_DANCE_SPA_BOOST]));
      }
      else {
        REQUIRE(p2BCurrentSpa.val == p2BInitialSpa.val);
      }

      if (!p2BDamageInfo.moveMightDropSpa()) {
        REQUIRE(p2BSpaBoosted);
      }
      if (!p2BDamageInfo.moveMightMiss()) {
        REQUIRE_FALSE(currentP2BHp.val == P2B_MAX_HP);
      }
    }
    idealProbability *= p2BDamageInfo.getProbability(currentP2BHp.val, p2BSpaBoosted);
    foundP2BHp.insert(currentP2BHp.val);

    if (!p2ABurned && p1BFullDamageInfo.mightCrit()) {
      types::probability withP1BCritProbability = idealProbability * CRIT_PROBABILITY;
      types::probability withoutP1BCritProbability = idealProbability * (MAX_PROBABILITY - CRIT_PROBABILITY);
      CAPTURE(idealProbability, withP1BCritProbability, withoutP1BCritProbability);

      REQUIRE(
        (probability.val == Catch::Approx(withP1BCritProbability) ||
         probability.val == Catch::Approx(withoutP1BCritProbability)));

      foundUncertainProbabilities[idealProbability].insert(probability.val);
    }
    else {
      REQUIRE_THAT(idealProbability, Catch::Matchers::WithinRel(probability.val));
    }
  }

  if (options.getMakeBranchesOnRandomEvents()) {
    if (willOWispMightMiss) {
      REQUIRE(foundP1BHp.size() == expectedP1BAllHp.size());
    }
    else {
      REQUIRE(foundP1BHp.size() == expectedP1BHalfHp.size());
      REQUIRE(foundP2AHp.size() == 1U);
    }
    REQUIRE(foundP2BHp.size() == expectedP2BHp.size());

    REQUIRE_THAT(sumOfProbability, Catch::Matchers::WithinRel(startingProbability));
    for (const auto& uncertainProbabilities : foundUncertainProbabilities) {
      REQUIRE(uncertainProbabilities.second.size() == 2U);
    }
  }
}
}  // namespace pokesim
