#include "../Tests.hpp"

namespace pokesim {
static constexpr auto ToProbability = Constants::PercentChanceToProbability;
static constexpr auto ProbabilityMax = Constants::Probability::MAX;
using TripleArrows = dex::TripleArrows;

TEST_CASE(
  "Triple Arrows: Higher crit chance, can drop defense, and can flinch",
  "[Simulation][SimulateTurn][SingleBattle][Move][TripleArrows][Flinch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::HISUIAN_DECIDUEYE,
      dex::Move::TRIPLE_ARROWS,
      test.evs({{dex::Stat::SPE, Constants::PokemonEv::MAX}}))),
    test.side(test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::TRIPLE_ARROWS, dex::Move::SPLASH));

  types::critStage critBoost = test.dexValue<TripleArrows::critStageBoost>();
  types::probability critProbability =
    ProbabilityMax / test.dexValue<MechanicConstants::CRIT_CHANCE_DIVISORS>()[critBoost];
  types::probability defDrop = test.dexValue<TripleArrows::targetSecondaryEffect::chance>() * ToProbability;
  types::probability flinch = test.dexValue<TripleArrows::targetSecondaryEffect::addedFlinchChance>() * ToProbability;

  test.simulateTurnOptions().setMakeBranchesOnRandomEvents(true).setDamageRollsConsidered({DamageRollKind::MAX_DAMAGE});
  auto turnOutcomeBattles = test.simulateOneBattle(Tags<Probability>{}).battleOutcomes();
  REQUIRE(turnOutcomeBattles.size() == 8U);

  std::size_t critCount = 0U;
  std::size_t defDropCount = 0U;
  std::size_t flinchCount = 0U;
  const types::registry& registry = test.registry();
  for (types::entity battle : turnOutcomeBattles) {
    auto entities = test.getBattleEntities(battle);
    test.checks.checkUsedMovePokemon(entities.p1A);

    bool moveCrit = registry.get<stat::CurrentHp>(entities.p2A).val == (246U - 73U);
    bool defDropped = registry.all_of<DefBoost>(entities.p2A);
    bool flinched = !registry.all_of<LastUsedMove>(entities.p2A);

    Probability probability = registry.get<Probability>(battle);
    CAPTURE(moveCrit, defDropped, flinched, probability.val);

    types::probability idealProbability = Constants::Probability::DEFAULT;
    if (moveCrit) {
      critCount++;
      idealProbability *= critProbability;
    }
    else {
      idealProbability *= ProbabilityMax - critProbability;
    }

    if (defDropped) {
      REQUIRE(
        registry.get<DefBoost>(entities.p2A).val == test.dexValue<TripleArrows::targetSecondaryEffect::defBoost>());
      defDropCount++;
      idealProbability *= defDrop;
    }
    else {
      idealProbability *= ProbabilityMax - defDrop;
    }

    if (flinched) {
      test.checks.checkEntityForChanges<stat::CurrentHp, DefBoost, stat::EffectiveDef>(entities.p2A);
      flinchCount++;
      idealProbability *= flinch;
    }
    else {
      test.checks.checkUsedMovePokemon<stat::CurrentHp, DefBoost, stat::EffectiveDef>(entities.p2A);
      idealProbability *= ProbabilityMax - flinch;
    }

    REQUIRE_THAT(idealProbability, Catch::Matchers::WithinRel(probability.val));
  }

  REQUIRE(critCount == 4U);
  REQUIRE(defDropCount == 4U);
  REQUIRE(flinchCount == 4U);
};
}  // namespace pokesim
