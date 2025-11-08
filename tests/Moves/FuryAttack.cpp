#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Fury Attack: Multi-hit Branches", "[Simulation][SimulateTurn][Move][FuryAttack]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  Simulation simulation(pokedex, BattleFormat::SINGLES_BATTLE_FORMAT);
  const types::registry& registry = simulation.registry;

  Simulation::BattleCreationInfo battleCreationInfo;
  battleCreationInfo.p1 = {{createPredefinedPokemon(dex::Species::EMPOLEON)}};
  battleCreationInfo.p2 = {{createPredefinedPokemon(dex::Species::RIBOMBEE)}};
  battleCreationInfo.turn = 1U;
  loadPokedexForBattleInfo(battleCreationInfo, pokedex);

  battleCreationInfo.runWithSimulateTurn = true;
  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  SlotDecision p1SlotDecision{Slot::P1A, Slot::P2A};
  SlotDecision p2SlotDecision{Slot::P2A, Slot::P1A};
  p1SlotDecision.moveChoice = dex::Move::FURY_ATTACK;
  p1Decision.decisions = types::sideSlots<SlotDecision>{p1SlotDecision};
  p2SlotDecision.moveChoice = dex::Move::QUIVER_DANCE;
  p2Decision.decisions = types::sideSlots<SlotDecision>{p2SlotDecision};

  battleCreationInfo.decisionsToSimulate = {{p1Decision, p2Decision}};
  simulation.createInitialStates({battleCreationInfo});

  static constexpr auto minDamageKind = DamageRollKind::MIN_DAMAGE;
  static constexpr auto minCritDamageKind = DamageRollKind::MIN_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE;

  DamageRollOptions damageRollOptions{
    GENERATE(minCritDamageKind, minDamageKind),
    GENERATE(minCritDamageKind, minDamageKind),
  };
  CAPTURE(damageRollOptions.p1, damageRollOptions.p2);

  auto& options = simulation.simulateTurnOptions;
  options.damageRollsConsidered = {
    GENERATE(minCritDamageKind, minDamageKind),
    GENERATE(minCritDamageKind, minDamageKind),
  };
  options.applyChangesToInputBattle = true;
  options.makeBranchesOnRandomEvents = true;

  const bool alwaysCrits = options.damageRollsConsidered.p2 & DamageRollKind::GUARANTEED_CRIT_CHANCE;
  const bool twoDamageOutcomesPerHit = !alwaysCrits;

  constexpr types::probability passesAccuracyProbability =
    applyChainedModifier(
      dex::FuryAttack<GameMechanics::SCARLET_VIOLET>::accuracy,
      chainValueToModifier(
        MechanicConstants::FIXED_POINT_SCALE,
        dex::BrightPowder<GameMechanics::SCARLET_VIOLET>::onModifyAccuracyNumerator,
        dex::BrightPowder<GameMechanics::SCARLET_VIOLET>::onModifyAccuracyDenominator)) /
    100.0F;

  constexpr types::probability lowerMultiHitMoveChances = MechanicConstants::PROGRESSIVE_MULTI_HIT_CHANCES[0] / 100.0F;
  constexpr types::probability upperMultiHitMoveChances =
    (MechanicConstants::PROGRESSIVE_MULTI_HIT_CHANCES[2] - MechanicConstants::PROGRESSIVE_MULTI_HIT_CHANCES[1]) /
    100.0F;

  const types::probability critHitChance = alwaysCrits ? 1.0F : 1.0F / MechanicConstants::CRIT_CHANCE_DIVISORS[0];
  const types::probability baseHitChance = 1.0F - critHitChance;
  const types::damage critDamage = 22U;
  const types::damage baseDamage = 15U;
  const std::size_t damageOutcomesPerHit = 1U * (twoDamageOutcomesPerHit ? 2U : 1U);
  const std::size_t idealTurnOutcomeCount = 1U +                                               // The move misses
                                            (std::size_t)std::pow(damageOutcomesPerHit, 2U) +  // 2 Hits
                                            (std::size_t)std::pow(damageOutcomesPerHit, 3U) +  // 3 Hits
                                            (std::size_t)std::pow(damageOutcomesPerHit, 4U) +  // 4 Hits
                                            (std::size_t)std::pow(damageOutcomesPerHit, 5U);   // 5 Hits

  // The below strategy only works because all the damage outcomes from all the branches are unique
  entt::dense_map<types::stat, std::tuple<types::moveHits, types::moveHits, types::probability>>
    hitCombinationsFromP2Hp;
  const types::stat p2MaxHp = battleCreationInfo.p2.team[0].stats.hp;

  hitCombinationsFromP2Hp[p2MaxHp] = {(types::moveHits)0U, (types::moveHits)0U, 1.0F - passesAccuracyProbability};

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

  auto result = simulation.simulateTurn();

  REQUIRE(result.turnOutcomeBattlesResults().size() == 1U);
  const auto& turnOutcomeBattles = std::get<1>(*result.turnOutcomeBattlesResults().each().begin()).val;
  REQUIRE(turnOutcomeBattles.size() == idealTurnOutcomeCount);

  for (types::entity battle : turnOutcomeBattles) {
    const auto& [probability, sides] = registry.get<Probability, Sides>(battle);
    types::stat p2Hp = registry.get<stat::CurrentHp>(registry.get<Team>(sides.p2()).val[0]).val;
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