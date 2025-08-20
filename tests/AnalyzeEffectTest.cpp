#include "Tests.hpp"

namespace pokesim {
struct IdealDamageResults {
  DamageRolls rolls;
  calc_damage::UsesUntilKo koUses;

  IdealDamageResults() {}
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  IdealDamageResults(std::vector<types::damage> _rolls, const calc_damage::UsesUntilKo& _koUses)
      : rolls(_rolls), koUses(_koUses) {}
};

struct Ideals {
 private:
  IdealDamageResults before;
  IdealDamageResults after;

 public:
  Ideals() {}
  Ideals(const IdealDamageResults& _before, const IdealDamageResults& _after) : before(_before), after(_after) {}

  const auto& damageRolls() const { return after.rolls.val; }
  const auto& koUses() const { return after.koUses.val; }
  auto multiplier() const { return (types::effectMultiplier)after.rolls.max() / before.rolls.max(); }
};

TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  battleCreationInfo.runWithAnalyzeEffect = true;
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);
  const types::registry& registry = simulation.registry;

  bool getKoUses = GENERATE(false, true);
  bool reconsiderActiveEffects = GENERATE(false, true);

  CAPTURE(getKoUses, reconsiderActiveEffects);

  IdealDamageResults p1FuryAttack = {
    {13U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 11U, 11U, 11U, 11U, 11U, 11U, 11U, 11U},
    {{{23U, 1 / 16.0F}, {25U, 7 / 16.0F}, {27U, 8 / 16.0F}}},
  };
  IdealDamageResults p1FuryAttackBurn = {
    {6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U},
    {{{50U, 0.5F}, {59U, 0.5F}}},
  };
  IdealDamageResults p1FuryAttackBurnPlus2Atk = {
    {12U, 12U, 12U, 12U, 12U, 11U, 11U, 11U, 11U, 11U, 11U, 11U, 11U, 10U, 10U, 10U},
    {{{25U, 5 / 16.0F}, {27U, 8 / 16.0F}, {30U, 3 / 16.0F}}},
  };
  IdealDamageResults p1KnockOff = {
    {52U, 51U, 50U, 50U, 49U, 49U, 48U, 48U, 47U, 47U, 46U, 46U, 45U, 45U, 44U, 44U},
    {{{6U, 4 / 16.0F}, {7U, 12 / 16.0F}}},
  };
  IdealDamageResults p1KnockOffBurn = {
    {26U, 25U, 25U, 25U, 24U, 24U, 24U, 24U, 23U, 23U, 23U, 23U, 22U, 22U, 22U, 22U},
    {{{12U, 4 / 16.0F}, {13U, 8 / 16.0F}, {14U, 4 / 16.0F}}},
  };
  IdealDamageResults p1KnockOffMinus1Atk = {
    {35U, 34U, 34U, 33U, 33U, 33U, 32U, 32U, 32U, 31U, 31U, 31U, 30U, 30U, 30U, 29U},
    {{{9U, 6 / 16.0F}, {10U, 9 / 16.0F}, {11U, 1 / 16.0F}}},
  };
  IdealDamageResults p1Thunderbolt = {
    {43U, 43U, 42U, 42U, 41U, 41U, 40U, 40U, 40U, 39U, 39U, 38U, 38U, 37U, 37U, 36U},
    {{{7U, 2 / 16.0F}, {8U, 13 / 16.0F}, {9U, 1 / 16.0F}}},
  };

  IdealDamageResults p2FuryAttack = {
    {5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 4U, 4U, 4U, 4U, 4U, 4U},
    {{{55U, 10 / 16.0F}, {69U, 6 / 16.0F}}},
  };
  IdealDamageResults p2KnockOff = {
    {44U, 43U, 43U, 42U, 42U, 41U, 41U, 40U, 40U, 40U, 39U, 39U, 38U, 38U, 37U, 37U},
    {{{7U, 10 / 16.0F}, {8U, 6 / 16.0F}}},
  };
  IdealDamageResults p2KnockOffBurnPlus2Atk = {
    {43U, 42U, 42U, 41U, 41U, 40U, 40U, 39U, 39U, 39U, 38U, 38U, 37U, 37U, 36U, 36U},
    {{{7U, 7 / 16.0F}, {8U, 9 / 16.0F}}},
  };
  IdealDamageResults p2KnockOffP1Plus2Def = {
    {23U, 22U, 22U, 22U, 22U, 21U, 21U, 21U, 21U, 20U, 20U, 20U, 20U, 20U, 19U, 19U},
    {{{12U, 1 / 16.0F}, {13U, 4 / 16.0F}, {14U, 9 / 16.0F}, {15U, 2 / 16.0F}}},
  };
  IdealDamageResults p2Thunderbolt = {
    {282U, 278U, 276U, 272U, 270U, 266U, 264U, 260U, 258U, 254U, 252U, 248U, 246U, 242U, 240U, 236U},
    {{{1U, 3 / 16.0F}, {2U, 13 / 16.0F}}},
  };

  IdealDamageResults p2ThunderboltPlus3Spa = {
    {698U, 690U, 684U, 678U, 668U, 662U, 656U, 648U, 642U, 636U, 626U, 620U, 614U, 606U, 600U, 594U},
    {{{1U, 16 / 16.0F}}},
  };

  entt::dense_map<types::entity, Ideals> idealResults;
  auto checkResults = [&](const analyze_effect::Results& results) {
    const auto multipliers = results.effectMultiplierResults();
    const auto damageRolls = results.multipliedDamageRollsResults();
    const auto koUses = results.multipliedUsesUntilKoResults();

    REQUIRE(multipliers.size() == idealResults.size());
    REQUIRE(damageRolls.size() == idealResults.size());
    REQUIRE(koUses.size() == (getKoUses ? idealResults.size() : 0U));

    multipliers.each([&](types::entity entity, const analyze_effect::EffectMultiplier& multiplier) {
      auto [attacker, defenders, effectTarget, move] = registry.get<
        analyze_effect::Attacker,
        analyze_effect::Defenders,
        analyze_effect::EffectTarget,
        analyze_effect::EffectMove>(entity);

      auto attackerSide = registry.get<PlayerSide>(registry.get<Side>(attacker.val).val).val;
      auto defenderSide = registry.get<PlayerSide>(registry.get<Side>(defenders.only()).val).val;
      auto effectTargetSide = registry.get<PlayerSide>(registry.get<Side>(effectTarget.val).val).val;

      auto effectStatus = registry.try_get<StatusName>(entity) ? registry.get<StatusName>(entity) : StatusName{};
      auto effectAtkBoost = registry.try_get<AtkBoost>(entity) ? registry.get<AtkBoost>(entity) : AtkBoost{};
      auto effectDefBoost = registry.try_get<DefBoost>(entity) ? registry.get<DefBoost>(entity) : DefBoost{};
      auto effectSpaBoost = registry.try_get<SpaBoost>(entity) ? registry.get<SpaBoost>(entity) : SpaBoost{};
      auto effectSpeBoost = registry.try_get<SpeBoost>(entity) ? registry.get<SpeBoost>(entity) : SpeBoost{};

      CAPTURE(
        attackerSide,
        defenderSide,
        effectTargetSide,
        move.val,
        effectStatus.name,
        effectAtkBoost.val,
        effectDefBoost.val,
        effectSpaBoost.val,
        effectSpeBoost.val);

      REQUIRE(idealResults.contains(entity));

      const Ideals& ideals = idealResults.at(entity);
      REQUIRE(multiplier.val == ideals.multiplier());

      if (getKoUses) {
        REQUIRE(damageRolls.contains(entity));
        const auto& [trueDamageRolls] = damageRolls.get(entity);
        REQUIRE(trueDamageRolls.val.size() == ideals.damageRolls().size());
        for (types::damageRollIndex i = 0U; i < trueDamageRolls.val.size(); i++) {
          REQUIRE(trueDamageRolls.val[i].val == ideals.damageRolls()[i].val);
        }

        REQUIRE(koUses.contains(entity));
        const auto& [trueKosUses] = koUses.get(entity);

        REQUIRE(trueKosUses.minHits() == ideals.koUses().front());
        REQUIRE(trueKosUses.maxHits() == ideals.koUses().back());
        REQUIRE(trueKosUses.guaranteedKo() == (ideals.koUses().size() == 1U));

        REQUIRE(trueKosUses.val.size() == ideals.koUses().size());
        for (types::damageRollIndex i = 0U; i < trueKosUses.val.size(); i++) {
          REQUIRE(trueKosUses.val[i] == ideals.koUses()[i]);
        }
      }
    });
  };

  auto& options = simulation.analyzeEffectOptions;

  options.reconsiderActiveEffects = reconsiderActiveEffects;
  if (getKoUses) {
    options.damageRollOptions.p1 = DamageRollKind::ALL_DAMAGE_ROLLS;
    options.damageRollOptions.p2 = DamageRollKind::ALL_DAMAGE_ROLLS;
    options.noKoChanceCalculation = false;
  }
  else {
    options.noKoChanceCalculation = true;
  }

  SECTION("One Input, One Effect Applied") {
    battleCreationInfo.p1.team[0].status = dex::Status::BRN;

    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };
    simulation.createInitialStates({battleCreationInfo});

    types::entity inputEntity = simulation.registry.view<analyze_effect::tags::Input>()[0];
    idealResults.emplace(
      inputEntity,
      Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, No Effect Applied") {
    battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };
    simulation.createInitialStates({battleCreationInfo});

    types::entity inputEntity = simulation.registry.view<analyze_effect::tags::Input>()[0];
    idealResults.emplace(inputEntity, Ideals{p1FuryAttack, p1FuryAttackBurn});

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("Multiple Inputs, One Effect Applied") {
    battleCreationInfo.p1.team[0].status = dex::Status::BRN;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::PAR},
    };
    simulation.createInitialStates({battleCreationInfo});

    simulation.registry.view<analyze_effect::tags::Input, StatusName>().each(
      [&](types::entity entity, StatusName status) {
        if (status.name == dex::Status::BRN) {
          idealResults.emplace(
            entity,
            Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});
        }
        else {
          idealResults.emplace(entity, Ideals{p1FuryAttackBurn, p1FuryAttack});
        }
      });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, No Effect Applied, Multiple Calculations") {
    battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };
    simulation.createInitialStates({battleCreationInfo});

    simulation.registry.view<analyze_effect::tags::Input>().each(
      [&](types::entity entity) { idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttackBurn}); });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, One Effect Applied, Multiple Calculations") {
    battleCreationInfo.p1.team[0].status = dex::Status::BRN;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };
    simulation.createInitialStates({battleCreationInfo});

    simulation.registry.view<analyze_effect::tags::Input>().each([&](types::entity entity) {
      idealResults.emplace(entity, Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});
    });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, No Effect Applied, Multiple Calculations, Multiple Attacks Per Input, Same Attack") {
    battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK, dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };
    simulation.createInitialStates({battleCreationInfo});

    simulation.registry.view<analyze_effect::tags::Input>().each(
      [&](types::entity entity) { idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttackBurn}); });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, One Effect Applied, Multiple Calculations, Multiple Attacks Per Input, Same Attack") {
    battleCreationInfo.p1.team[0].status = dex::Status::BRN;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK, dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };
    simulation.createInitialStates({battleCreationInfo});

    simulation.registry.view<analyze_effect::tags::Input>().each([&](types::entity entity) {
      idealResults.emplace(entity, Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});
    });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, Multiple Effects, No Effects Applied") {
    battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::PAR},
    };
    simulation.createInitialStates({battleCreationInfo});

    simulation.registry.view<analyze_effect::tags::Input, StatusName>().each(
      [&](types::entity entity, StatusName status) {
        if (status.name == dex::Status::BRN) {
          idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttackBurn});
        }
        else {
          idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttack});
        }
      });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("One Input, Multiple Effects Per Input, No Effects Applied") {
    battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {
        Slot::P1A,
        Slot::P2A,
        Slot::P1A,
        {dex::Move::FURY_ATTACK},
        dex::Status::BRN,
        {{dex::Stat::ATK, 2}},
      },
    };
    simulation.createInitialStates({battleCreationInfo});

    types::entity inputEntity = simulation.registry.view<analyze_effect::tags::Input>()[0];
    idealResults.emplace(inputEntity, Ideals{p1FuryAttack, p1FuryAttackBurnPlus2Atk});

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }

  SECTION("Multiple Inputs, Multiple Effects") {
    battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;
    battleCreationInfo.p1.team[0].item = dex::Item::NO_ITEM;
    battleCreationInfo.p2.team[0].item = dex::Item::NO_ITEM;

    battleCreationInfo.effectsToAnalyze = {
      {
        Slot::P1A,
        Slot::P2A,
        Slot::P1A,
        {dex::Move::FURY_ATTACK, dex::Move::KNOCK_OFF, dex::Move::THUNDERBOLT},
        dex::Status::BRN,
      },
      {
        Slot::P1A,
        Slot::P2A,
        Slot::P1A,
        {dex::Move::KNOCK_OFF, dex::Move::THUNDERBOLT},
        std::nullopt,
        {{dex::Stat::ATK, -1}},
      },
      {
        Slot::P2A,
        Slot::P1A,
        Slot::P2A,
        {dex::Move::FURY_ATTACK, dex::Move::THUNDERBOLT},
        std::nullopt,
        {{dex::Stat::SPE, 5}},
      },
      {
        Slot::P2A,
        Slot::P1A,
        Slot::P2A,
        {dex::Move::FURY_ATTACK},
        dex::Status::PSN,
      },
      {
        Slot::P1A,
        Slot::P2A,
        Slot::P2A,
        {dex::Move::KNOCK_OFF},
        dex::Status::BRN,
      },
      {
        Slot::P2A,
        Slot::P1A,
        Slot::P2A,
        {dex::Move::THUNDERBOLT},
        std::nullopt,
        {{dex::Stat::SPA, 3}},
      },
      {
        Slot::P2A,
        Slot::P1A,
        Slot::P1A,
        {dex::Move::THUNDERBOLT, dex::Move::KNOCK_OFF},
        std::nullopt,
        {{dex::Stat::DEF, 2}},
      },
      {
        Slot::P2A,
        Slot::P1A,
        Slot::P1A,
        {dex::Move::THUNDERBOLT, dex::Move::KNOCK_OFF, dex::Move::FURY_ATTACK},
        dex::Status::TOX,
      },
    };
    simulation.createInitialStates({battleCreationInfo});

    auto inputs = registry.view<analyze_effect::tags::Input>();
    REQUIRE(inputs.size() == 15U);

    inputs.each([&](types::entity input) {
      auto [attacker, effectTarget, move] =
        registry.get<analyze_effect::Attacker, analyze_effect::EffectTarget, analyze_effect::EffectMove>(input);
      types::entity attackerSide = registry.get<Side>(attacker.val).val;
      types::entity effectTargetSide = registry.get<Side>(effectTarget.val).val;
      bool attackerIsP1 = registry.get<PlayerSide>(attackerSide).val == PlayerSideId::P1;
      bool effectTargetIsP1 = registry.get<PlayerSide>(effectTargetSide).val == PlayerSideId::P1;

      auto effectStatus = registry.all_of<StatusName>(input) ? registry.get<StatusName>(input) : StatusName{};
      auto effectAtkBoost = registry.all_of<AtkBoost>(input) ? registry.get<AtkBoost>(input) : AtkBoost{};
      auto effectDefBoost = registry.all_of<DefBoost>(input) ? registry.get<DefBoost>(input) : DefBoost{};
      auto effectSpaBoost = registry.all_of<SpaBoost>(input) ? registry.get<SpaBoost>(input) : SpaBoost{};
      auto effectSpeBoost = registry.all_of<SpeBoost>(input) ? registry.get<SpeBoost>(input) : SpeBoost{};

      Ideals pickedIdeals;
      if (attackerIsP1 && effectTargetIsP1 && effectStatus.name == dex::Status::BRN) {
        switch (move.val) {
          case dex::Move::FURY_ATTACK: pickedIdeals = {p1FuryAttack, p1FuryAttackBurn}; break;
          case dex::Move::KNOCK_OFF:   pickedIdeals = {p1KnockOff, p1KnockOffBurn}; break;
          case dex::Move::THUNDERBOLT: pickedIdeals = {p1Thunderbolt, p1Thunderbolt}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (attackerIsP1 && effectTargetIsP1 && effectAtkBoost.val == -1) {
        switch (move.val) {
          case dex::Move::KNOCK_OFF:   pickedIdeals = {p1KnockOff, p1KnockOffMinus1Atk}; break;
          case dex::Move::THUNDERBOLT: pickedIdeals = {p1Thunderbolt, p1Thunderbolt}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (!attackerIsP1 && !effectTargetIsP1 && effectSpeBoost.val == 5) {
        switch (move.val) {
          case dex::Move::FURY_ATTACK: pickedIdeals = {p2FuryAttack, p2FuryAttack}; break;
          case dex::Move::THUNDERBOLT: pickedIdeals = {p2Thunderbolt, p2Thunderbolt}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (!attackerIsP1 && !effectTargetIsP1 && effectStatus.name == dex::Status::PSN) {
        switch (move.val) {
          case dex::Move::FURY_ATTACK: pickedIdeals = {p2FuryAttack, p2FuryAttack}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (attackerIsP1 && !effectTargetIsP1 && effectStatus.name == dex::Status::BRN) {
        switch (move.val) {
          case dex::Move::KNOCK_OFF: pickedIdeals = {p1KnockOff, p1KnockOff}; break;
          default:                   FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (!attackerIsP1 && !effectTargetIsP1 && effectSpaBoost.val == 3) {
        switch (move.val) {
          case dex::Move::THUNDERBOLT: pickedIdeals = {p2Thunderbolt, p2ThunderboltPlus3Spa}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (!attackerIsP1 && effectTargetIsP1 && effectDefBoost.val == 2) {
        switch (move.val) {
          case dex::Move::KNOCK_OFF:   pickedIdeals = {p2KnockOff, p2KnockOffP1Plus2Def}; break;
          case dex::Move::THUNDERBOLT: pickedIdeals = {p2Thunderbolt, p2Thunderbolt}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (!attackerIsP1 && effectTargetIsP1 && effectStatus.name == dex::Status::TOX) {
        switch (move.val) {
          case dex::Move::FURY_ATTACK: pickedIdeals = {p2FuryAttack, p2FuryAttack}; break;
          case dex::Move::KNOCK_OFF:   pickedIdeals = {p2KnockOff, p2KnockOff}; break;
          case dex::Move::THUNDERBOLT: pickedIdeals = {p2Thunderbolt, p2Thunderbolt}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else {
        FAIL("Test is missing damages for an input scenario.");
      }

      idealResults.emplace(input, pickedIdeals);
    });

    auto results = simulation.analyzeEffect();
    checkResults(results);
  }
}
}  // namespace pokesim
