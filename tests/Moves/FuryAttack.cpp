#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Fury Attack: Multi-hit Branches", "[Simulation][SimulateTurn][SingleBattle][Move][FuryAttack]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::FURY_ATTACK)),
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::FURY_ATTACK, dex::Move::SPLASH));

  const Pokedex& pokedex = test.pokedex;
  auto& options = test.simulateTurnOptions();

  static constexpr auto minDamageKind = DamageRollKind::MIN_DAMAGE;
  static constexpr auto minCritDamageKind = DamageRollKind::MIN_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE;
  static constexpr types::probability lowerMultiHitMoveChances = Constants::PROGRESSIVE_MULTI_HIT_CHANCES[0] / 100.0F;
  static constexpr types::probability upperMultiHitMoveChances =
    (Constants::PROGRESSIVE_MULTI_HIT_CHANCES[2] - Constants::PROGRESSIVE_MULTI_HIT_CHANCES[1]) / 100.0F;

  DamageRollOptions damageRollOptions{
    GENERATE(minCritDamageKind, minDamageKind),
    GENERATE(minCritDamageKind, minDamageKind),
  };
  CAPTURE(damageRollOptions.getP1(), damageRollOptions.getP2());

  options
    .setDamageRollsConsidered({
      GENERATE(minCritDamageKind, minDamageKind),
      GENERATE(minCritDamageKind, minDamageKind),
    })
    .setMakeBranchesOnRandomEvents(true);

  bool alwaysCrits = options.getDamageRollsConsidered().getP2() & DamageRollKind::GUARANTEED_CRIT_CHANCE;
  bool twoDamageOutcomesPerHit = !alwaysCrits;
  types::probability passesAccuracyProbability = pokedex.getStaticValue<dex::FuryAttack::accuracy>() / 100.0F;

  types::probability critHitChance =
    alwaysCrits ? Constants::Probability::MAX
                : Constants::Probability::MAX / pokedex.getStaticValue<MechanicConstants::CRIT_CHANCE_DIVISORS>()[0];
  types::probability baseHitChance = Constants::Probability::MAX - critHitChance;
  types::damage critDamage = 23U;
  types::damage baseDamage = 16U;
  types::stat p2MaxHp = computeStatFromBaseStat(
    dex::Stat::HP,
    pokedex.getStaticValue<dex::Ribombee::hp>(),
    Constants::PokemonLevel::DEFAULT,
    dex::Nature::NO_NATURE,
    {},
    {});
  std::size_t damageOutcomesPerHit = twoDamageOutcomesPerHit ? 2U : 1U;
  std::size_t idealTurnOutcomeCount = 1U +                                               // The move misses
                                      (std::size_t)std::pow(damageOutcomesPerHit, 2U) +  // 2 Hits
                                      (std::size_t)std::pow(damageOutcomesPerHit, 3U) +  // 3 Hits
                                      (std::size_t)std::pow(damageOutcomesPerHit, 4U) +  // 4 Hits
                                      (std::size_t)std::pow(damageOutcomesPerHit, 5U);   // 5 Hits

  // The below strategy only works because all the damage outcomes from all the branches are unique
  entt::dense_map<types::stat, std::tuple<types::moveHits, types::moveHits, types::probability>>
    hitCombinationsFromP2Hp;

  hitCombinationsFromP2Hp[p2MaxHp] = {
    (types::moveHits)0U,
    (types::moveHits)0U,
    Constants::Probability::MAX - passesAccuracyProbability,
  };

  for (types::moveHits totalHits = 2U; totalHits <= 5U; totalHits++) {
    types::probability idealProbability = passesAccuracyProbability;
    if (totalHits == 2U || totalHits == 3U) {
      idealProbability *= lowerMultiHitMoveChances;
    }
    else {
      idealProbability *= upperMultiHitMoveChances;
    }

    for (types::moveHits critDamageHits = 0U; critDamageHits <= totalHits; critDamageHits++) {
      types::moveHits baseDamageHits = totalHits - critDamageHits;
      types::damage totalDamage = (baseDamage * baseDamageHits) + (critDamage * critDamageHits);
      types::stat hpAfterDamage = p2MaxHp - totalDamage;

      hitCombinationsFromP2Hp[hpAfterDamage] = {baseDamageHits, critDamageHits, idealProbability};
      std::get<2>(hitCombinationsFromP2Hp[hpAfterDamage]) *=
        (types::probability)std::pow(baseHitChance, baseDamageHits) *
        (types::probability)std::pow(critHitChance, critDamageHits);
    }
  }

  auto turnOutcomeBattles = test.simulateOneBattle(Tags<Probability>{});
  REQUIRE(turnOutcomeBattles.size() == idealTurnOutcomeCount);

  for (types::entity battle : turnOutcomeBattles) {
    Probability probability = test.registry().get<Probability>(battle);
    auto entities = test.getBattleEntities(battle);
    types::stat p2Hp = test.registry().get<stat::CurrentHp>(entities.p2A).val;
    CAPTURE(p2Hp);
    REQUIRE(hitCombinationsFromP2Hp.contains(p2Hp));

    const auto [baseDamageHits, critDamageHits, idealProbability] = hitCombinationsFromP2Hp.at(p2Hp);
    CAPTURE(baseDamageHits, critDamageHits, idealProbability);
    if (alwaysCrits) {
      REQUIRE(baseDamageHits == 0U);
    }

    REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(idealProbability));
  }
};
}  // namespace pokesim
