#include "../Tests.hpp"

namespace pokesim {
static constexpr auto ToProbability = Constants::PercentChanceToProbability;
static constexpr auto ProbabilityMax = Constants::Probability::MAX;

TEST_CASE("Kings Rock: Can cause move failure", "[Simulation][SimulateTurn][SingleBattle][Item][KingsRock][Flinch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Item::KINGS_ROCK, dex::Move::KNOCK_OFF)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::KNOCK_OFF, dex::Move::SPLASH));

  test.simulateTurnOptions().setMakeBranchesOnRandomEvents(true);
  auto turnOutcomeBattles = test.simulateOneBattle(Tags<Probability>{});
  REQUIRE(turnOutcomeBattles.size() == 2U);

  types::probability flinchChance = test.dexValue<dex::KingsRock::addedFlinchChance>() * ToProbability;

  entt::dense_set<types::probability> foundProbabilities;
  for (types::entity battle : turnOutcomeBattles) {
    auto entities = test.getBattleEntities(battle);
    Probability probability = test.registry().get<Probability>(battle);

    auto [initialRngSeed, currentRngSeed] = test.checks.getInitialAndCurrent<RngSeed>(battle);
    REQUIRE(currentRngSeed.val == initialRngSeed.val);

    bool flinchStoppedP2Move = Catch::Approx(flinchChance) == probability.val;
    bool p2Moved = Catch::Approx(ProbabilityMax - flinchChance) == probability.val;

    REQUIRE((flinchStoppedP2Move || p2Moved));

    if (flinchStoppedP2Move) {
      test.checks.checkEntityForChanges<stat::CurrentHp>(entities.p2A);
    }

    if (p2Moved) {
      test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
    }

    test.checks.checkUsedMovePokemon(entities.p1A);
    foundProbabilities.insert(probability.val);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}

TEST_CASE(
  "Kings Rock: Adds to secondary effects", "[Simulation][SimulateTurn][SingleBattle][Item][KingsRock][Flinch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Item::KINGS_ROCK, dex::Move::MOONBLAST)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::MOONBLAST, dex::Move::SPLASH));

  test.simulateTurnOptions().setMakeBranchesOnRandomEvents(true);
  auto turnOutcomeBattles = test.simulateOneBattle(Tags<Probability>{});
  REQUIRE(turnOutcomeBattles.size() == 4U);

  types::probability flinchChance = test.dexValue<dex::KingsRock::addedFlinchChance>() * ToProbability;
  types::probability spaDropChance = test.dexValue<dex::Moonblast::targetSecondaryEffect::chance>() * ToProbability;
  types::boost spaBoost = test.dexValue<dex::Moonblast::targetSecondaryEffect::spaBoost>();

  entt::dense_set<types::probability> foundProbabilities;
  for (types::entity battle : turnOutcomeBattles) {
    auto entities = test.getBattleEntities(battle);
    Probability probability = test.registry().get<Probability>(battle);
    auto approxProbability = Catch::Approx(probability.val);

    bool spaDropFlinch = flinchChance * spaDropChance == approxProbability;
    bool spaDropNoFlinch = (ProbabilityMax - flinchChance) * spaDropChance == approxProbability;
    bool noSpaDropFlinch = flinchChance * (ProbabilityMax - spaDropChance) == approxProbability;
    bool noSpaDropNoFlinch = (ProbabilityMax - flinchChance) * (ProbabilityMax - spaDropChance) == approxProbability;

    REQUIRE((spaDropFlinch || spaDropNoFlinch || noSpaDropFlinch || noSpaDropNoFlinch));

    if (spaDropFlinch) {
      test.checks.checkEntityForChanges<stat::CurrentHp, SpaBoost, stat::EffectiveSpa>(entities.p2A);
    }
    if (spaDropNoFlinch) {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, SpaBoost, stat::EffectiveSpa>(entities.p2A);
    }
    if (noSpaDropFlinch) {
      test.checks.checkEntityForChanges<stat::CurrentHp>(entities.p2A);
    }
    if (noSpaDropNoFlinch) {
      test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
    }

    if (spaDropFlinch || spaDropNoFlinch) {
      REQUIRE(test.registry().get<SpaBoost>(entities.p2A).val == spaBoost);
    }

    test.checks.checkUsedMovePokemon(entities.p1A);
    foundProbabilities.insert(probability.val);
  }

  REQUIRE(foundProbabilities.size() == 4U);
}

TEST_CASE("Kings Rock: Works on multi-hit moves", "[Simulation][SimulateTurn][SingleBattle][Item][KingsRock][Flinch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Item::KINGS_ROCK, dex::Move::FURY_ATTACK)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::FURY_ATTACK, dex::Move::SPLASH));

  static constexpr float TwoHits = Constants::PROGRESSIVE_MULTI_HIT_CHANCES[0] * ToProbability;
  static constexpr float ThreeHits = (Constants::PROGRESSIVE_MULTI_HIT_CHANCES[1] * ToProbability) - TwoHits;
  static constexpr float FourHits =
    (Constants::PROGRESSIVE_MULTI_HIT_CHANCES[2] * ToProbability) - (TwoHits + ThreeHits);
  static constexpr float FiveHits =
    (Constants::PROGRESSIVE_MULTI_HIT_CHANCES[3] * ToProbability) - (TwoHits + ThreeHits + FourHits);
  types::probability hit = test.dexValue<dex::FuryAttack::accuracy>() * ToProbability;
  types::probability miss = ProbabilityMax - hit;
  types::probability flinch = test.dexValue<dex::KingsRock::addedFlinchChance>() * ToProbability;
  types::probability noFlinch = ProbabilityMax - flinch;

  std::vector<float> missProbabilities = {
    miss,
  };
  std::vector<float> twoHitProbabilities = {
    hit * TwoHits * flinch,
    hit * TwoHits * noFlinch * flinch,
    hit * TwoHits * noFlinch * noFlinch,
  };
  std::vector<float> threeHitProbabilities = {
    hit * ThreeHits * flinch,
    hit * ThreeHits * noFlinch * flinch,
    hit * ThreeHits * noFlinch * noFlinch * flinch,
    hit * ThreeHits * noFlinch * noFlinch * noFlinch,
  };
  std::vector<float> fourHitProbabilities = {
    hit * FourHits * flinch,
    hit * FourHits * noFlinch * flinch,
    hit * FourHits * noFlinch * noFlinch * flinch,
    hit * FourHits * noFlinch * noFlinch * noFlinch * flinch,
    hit * FourHits * noFlinch * noFlinch * noFlinch * noFlinch,
  };
  std::vector<float> fiveHitProbabilities = {
    hit * FiveHits * flinch,
    hit * FiveHits * noFlinch * flinch,
    hit * FiveHits * noFlinch * noFlinch * flinch,
    hit * FiveHits * noFlinch * noFlinch * noFlinch * flinch,
    hit * FiveHits * noFlinch * noFlinch * noFlinch * noFlinch * flinch,
    hit * FiveHits * noFlinch * noFlinch * noFlinch * noFlinch * noFlinch,
  };

  std::vector<float> noFlinchProbabilities = {
    missProbabilities[0],
    twoHitProbabilities[2],
    threeHitProbabilities[3],
    fourHitProbabilities[4],
    fiveHitProbabilities[5],
  };

  test.simulateTurnOptions().setMakeBranchesOnRandomEvents(true);
  auto turnOutcomeBattles = test.simulateOneBattle(Tags<Probability>{});
  REQUIRE(
    turnOutcomeBattles.size() == missProbabilities.size() + twoHitProbabilities.size() + threeHitProbabilities.size() +
                                   fourHitProbabilities.size() + fiveHitProbabilities.size());

  entt::dense_map<types::moveHits, entt::dense_set<types::probability>> foundProbabilities;
  for (types::entity battle : turnOutcomeBattles) {
    auto entities = test.getBattleEntities(battle);
    Probability probability = test.registry().get<Probability>(battle);
    auto approxProbability = Catch::Approx(probability.val);

    test.checks.checkUsedMovePokemon(entities.p1A);
    if (Catch::Matchers::Contains(approxProbability).match(noFlinchProbabilities)) {
      test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
    }
    else {
      test.checks.checkEntityForChanges<stat::CurrentHp>(entities.p2A);
    }

    auto [initialHp, currentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
    types::moveHits moveHits = (initialHp.val - currentHp.val) / 5U;
    switch (moveHits) {
      case 0:  REQUIRE(missProbabilities[0] == approxProbability); break;
      case 2:  REQUIRE_THAT(twoHitProbabilities, Catch::Matchers::Contains(approxProbability)); break;
      case 3:  REQUIRE_THAT(threeHitProbabilities, Catch::Matchers::Contains(approxProbability)); break;
      case 4:  REQUIRE_THAT(fourHitProbabilities, Catch::Matchers::Contains(approxProbability)); break;
      case 5:  REQUIRE_THAT(fiveHitProbabilities, Catch::Matchers::Contains(approxProbability)); break;
      default: FAIL(); break;
    }

    auto foundProbabilitiesForMoveHits = foundProbabilities[moveHits];
    REQUIRE_FALSE(Catch::Matchers::Contains(approxProbability).match(foundProbabilitiesForMoveHits));
    foundProbabilitiesForMoveHits.insert(probability.val);
  }
}
}  // namespace pokesim
