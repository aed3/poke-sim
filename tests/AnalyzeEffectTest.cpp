#include "Tests.hpp"

namespace pokesim {
namespace {
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

const IdealDamageResults p1FuryAttack{
  {14U, 13U, 13U, 13U, 13U, 13U, 13U, 13U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 11U},
  {{{21U, 1U}, {23U, 7U}, {25U, 7U}, {27U, 1U}}},
};

const IdealDamageResults p1FuryAttackBurn{
  {7U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 5U},
  {{{42U, 1U}, {49U, 14U}, {58U, 1U}}},
};

const IdealDamageResults p1FuryAttackBurnPlus2Atk{
  {13U, 13U, 13U, 13U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 11U, 11U, 11U, 11U},
  {{{23U, 4U}, {25U, 8U}, {27U, 4U}}},
};

const IdealDamageResults p1KnockOff{
  {84U, 83U, 82U, 81U, 80U, 79U, 78U, 78U, 77U, 76U, 75U, 74U, 73U, 73U, 72U, 71U},
  {{{4U, 14U}, {5U, 2U}}},
};

const IdealDamageResults p1KnockOffBurn{
  {42U, 41U, 41U, 40U, 40U, 39U, 39U, 39U, 38U, 38U, 37U, 37U, 36U, 36U, 36U, 35U},
  {{{7U, 1U}, {8U, 11U}, {9U, 4U}}},
};

const IdealDamageResults p1KnockOffMinus1Atk{
  {56U, 55U, 54U, 54U, 53U, 53U, 52U, 52U, 51U, 50U, 50U, 49U, 49U, 48U, 48U, 47U},
  {{{6U, 13U}, {7U, 3U}}},
};

const IdealDamageResults p1Thunderbolt{
  {47U, 46U, 46U, 45U, 45U, 44U, 44U, 43U, 43U, 42U, 42U, 41U, 41U, 40U, 40U, 39U},
  {{{7U, 11U}, {8U, 5U}}},
};

const IdealDamageResults p2FuryAttack{
  {5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 4U, 4U, 4U, 4U, 4U, 4U},
  {{{56U, 10U}, {70U, 6U}}},
};

const IdealDamageResults p2KnockOff{
  {64U, 63U, 62U, 62U, 61U, 60U, 60U, 59U, 58U, 58U, 57U, 56U, 56U, 55U, 55U, 54U},
  {{{5U, 13U}, {6U, 3U}}},
};

const IdealDamageResults p2KnockOffBurnPlus2Atk{
  {63U, 62U, 62U, 61U, 60U, 60U, 59U, 59U, 58U, 57U, 57U, 56U, 55U, 55U, 54U, 53U},
  {{{5U, 12U}, {6U, 4U}}},
};

const IdealDamageResults p2KnockOffP1Plus2Def{
  {33U, 32U, 32U, 32U, 31U, 31U, 31U, 30U, 30U, 30U, 29U, 29U, 29U, 28U, 28U, 28U},
  {{{9U, 7U}, {10U, 9U}}},
};

const IdealDamageResults p2Thunderbolt{
  {288U, 284U, 282U, 278U, 276U, 272U, 270U, 266U, 264U, 260U, 258U, 254U, 252U, 248U, 246U, 242U},
  {{{1U, 4U}, {2U, 12U}}},
};

const IdealDamageResults p2ThunderboltPlus3Spa{
  {710U, 702U, 696U, 686U, 680U, 674U, 666U, 660U, 654U, 644U, 638U, 630U, 624U, 618U, 608U, 602U},
  {{{1U, 16U}}},
};
}  // namespace

TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect][SingleBattle]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Item::ASSAULT_VEST,
      dex::Move::FURY_ATTACK,
      dex::Move::KNOCK_OFF,
      dex::Move::THUNDERBOLT)),
    test.side(test.pokemon(
      dex::Species::AMPHAROS,
      dex::Item::CHOICE_SPECS,
      dex::Nature::MODEST,
      dex::Move::FURY_ATTACK,
      dex::Move::KNOCK_OFF,
      dex::Move::THUNDERBOLT)));

  BattleCreationInfo& battleCreationInfo = test.battleInfoList.front();
  battleCreationInfo.runWithAnalyzeEffect = true;
  auto [p1Info, p2Info] = test.getPokemonCreationInfo(Slot::P1A, Slot::P2A);
  types::registry& registry = test.registry();

  bool getKoUses = GENERATE(false, true);
  bool reconsiderActiveEffects = GENERATE(false, true);

  CAPTURE(getKoUses, reconsiderActiveEffects);

  entt::dense_map<types::entity, Ideals> idealResults;
  auto checkResults = [&](const analyze_effect::Results& results) {
    const auto multipliers = results.effectMultiplierResults();
    const auto damageRolls = results.multipliedDamageRollsResults();
    const auto koUses = results.multipliedUsesUntilKoResults();

    REQUIRE(multipliers.size() == idealResults.size());
    REQUIRE(damageRolls.size() == idealResults.size());
    REQUIRE(koUses.size() == (getKoUses ? idealResults.size() : 0U));

    multipliers.each([&](types::entity entity, const analyze_effect::EffectMultiplier& multiplier) {
      auto [attacker, defender, effectTarget, move] = registry.get<
        analyze_effect::Attacker,
        analyze_effect::Defender,
        analyze_effect::EffectTarget,
        analyze_effect::EffectMove>(entity);

      auto attackerSide = registry.get<PlayerSide>(registry.get<Side>(attacker.val).val).val;
      auto defenderSide = registry.get<PlayerSide>(registry.get<Side>(defender.val).val).val;
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
        effectStatus.val,
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

        REQUIRE(trueKosUses.minUses() == ideals.koUses().front());
        REQUIRE(trueKosUses.maxUses() == ideals.koUses().back());
        REQUIRE(trueKosUses.guaranteedKo() == (ideals.koUses().size() == 1U));

        REQUIRE(trueKosUses.val.size() == ideals.koUses().size());
        for (types::damageRollIndex i = 0U; i < trueKosUses.val.size(); i++) {
          REQUIRE(trueKosUses.val[i] == ideals.koUses()[i]);
        }
      }
    });
  };

  auto& options = test.analyzeEffectOptions();
  options.setReconsiderActiveEffects(reconsiderActiveEffects);
  if (getKoUses) {
    options.setDamageRollOptions({DamageRollKind::ALL_DAMAGE_ROLLS});
    options.setNoKoChanceCalculation(false);
  }
  else {
    options.setNoKoChanceCalculation(true);
  }

  SECTION("One Input, One Effect Applied") {
    p1Info.status = dex::Status::BRN;

    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };

    auto results = test.analyzeEffect();
    types::entity inputEntity = registry.view<analyze_effect::tags::Input>().front();
    idealResults.emplace(
      inputEntity,
      Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});

    checkResults(results);
  }

  SECTION("One Input, No Effect Applied") {
    p1Info.status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };

    auto results = test.analyzeEffect();
    types::entity inputEntity = registry.view<analyze_effect::tags::Input>().front();
    idealResults.emplace(inputEntity, Ideals{p1FuryAttack, p1FuryAttackBurn});

    checkResults(results);
  }

  SECTION("Multiple Inputs, One Effect Applied") {
    p1Info.status = dex::Status::BRN;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::PAR},
    };

    auto results = test.analyzeEffect();
    registry.view<analyze_effect::tags::Input, StatusName>().each([&](types::entity entity, StatusName status) {
      if (status.val == dex::Status::BRN) {
        idealResults.emplace(
          entity,
          Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});
      }
      else {
        idealResults.emplace(entity, Ideals{p1FuryAttackBurn, p1FuryAttack});
      }
    });

    checkResults(results);
  }

  SECTION("One Input, No Effect Applied, Multiple Calculations") {
    p1Info.status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };

    auto results = test.analyzeEffect();
    registry.view<analyze_effect::tags::Input>().each(
      [&](types::entity entity) { idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttackBurn}); });

    checkResults(results);
  }

  SECTION("One Input, One Effect Applied, Multiple Calculations") {
    p1Info.status = dex::Status::BRN;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };

    auto results = test.analyzeEffect();
    registry.view<analyze_effect::tags::Input>().each([&](types::entity entity) {
      idealResults.emplace(entity, Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});
    });

    checkResults(results);
  }

  SECTION("One Input, No Effect Applied, Multiple Calculations, Multiple Attacks Per Input, Same Attack") {
    p1Info.status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK, dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };

    auto results = test.analyzeEffect();
    registry.view<analyze_effect::tags::Input>().each(
      [&](types::entity entity) { idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttackBurn}); });

    checkResults(results);
  }

  SECTION("One Input, One Effect Applied, Multiple Calculations, Multiple Attacks Per Input, Same Attack") {
    p1Info.status = dex::Status::BRN;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK, dex::Move::FURY_ATTACK}, dex::Status::BRN},
    };

    auto results = test.analyzeEffect();
    registry.view<analyze_effect::tags::Input>().each([&](types::entity entity) {
      idealResults.emplace(entity, Ideals{reconsiderActiveEffects ? p1FuryAttack : p1FuryAttackBurn, p1FuryAttackBurn});
    });

    checkResults(results);
  }

  SECTION("One Input, Multiple Effects, No Effects Applied") {
    p1Info.status = dex::Status::NO_STATUS;
    battleCreationInfo.effectsToAnalyze = {
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::BRN},
      {Slot::P1A, Slot::P2A, Slot::P1A, {dex::Move::FURY_ATTACK}, dex::Status::PAR},
    };

    auto results = test.analyzeEffect();
    registry.view<analyze_effect::tags::Input, StatusName>().each([&](types::entity entity, StatusName status) {
      if (status.val == dex::Status::BRN) {
        idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttackBurn});
      }
      else {
        idealResults.emplace(entity, Ideals{p1FuryAttack, p1FuryAttack});
      }
    });

    checkResults(results);
  }

  SECTION("One Input, Multiple Effects Per Input, No Effects Applied") {
    p1Info.status = dex::Status::NO_STATUS;
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

    auto results = test.analyzeEffect();
    types::entity inputEntity = registry.view<analyze_effect::tags::Input>().front();
    idealResults.emplace(inputEntity, Ideals{p1FuryAttack, p1FuryAttackBurnPlus2Atk});

    checkResults(results);
  }

  SECTION("Multiple Inputs, Multiple Effects") {
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
        {dex::Move::KNOCK_OFF},
        dex::Status::BRN,
        {{dex::Stat::ATK, 2}},
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

    auto results = test.analyzeEffect();
    auto inputs = registry.view<analyze_effect::tags::Input>();
    REQUIRE(inputs.size() == 16U);

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
      if (attackerIsP1 && effectTargetIsP1 && effectStatus.val == dex::Status::BRN) {
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
      else if (!attackerIsP1 && !effectTargetIsP1 && effectStatus.val == dex::Status::PSN) {
        switch (move.val) {
          case dex::Move::FURY_ATTACK: pickedIdeals = {p2FuryAttack, p2FuryAttack}; break;
          default:                     FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (attackerIsP1 && !effectTargetIsP1 && effectStatus.val == dex::Status::BRN) {
        switch (move.val) {
          case dex::Move::KNOCK_OFF: pickedIdeals = {p1KnockOff, p1KnockOff}; break;
          default:                   FAIL("Test is missing damages for a move."); break;
        }
      }
      else if (!attackerIsP1 && !effectTargetIsP1 && effectStatus.val == dex::Status::BRN && effectAtkBoost.val == 2) {
        switch (move.val) {
          case dex::Move::KNOCK_OFF: pickedIdeals = {p2KnockOff, p2KnockOffBurnPlus2Atk}; break;
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
      else if (!attackerIsP1 && effectTargetIsP1 && effectStatus.val == dex::Status::TOX) {
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

    checkResults(results);
  }
}
}  // namespace pokesim
