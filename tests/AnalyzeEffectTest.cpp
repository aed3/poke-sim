#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, false);
  bool getKoUses = false;
  bool reconsiderActiveEffects = false;

  using Ideals = std::tuple<types::effectMultiplier, DamageRolls, calc_damage::UsesUntilKo>;
  entt::dense_map<types::entity, Ideals> idealMultipliers;
  DamageRolls idealBaseDamageRolls = {13, 12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 11, 11};
  DamageRolls idealHalvedDamageRolls = {6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5};

  types::effectMultiplier idealHalvedMultiplier =
    (types::effectMultiplier)idealHalvedDamageRolls.val[0].val / idealBaseDamageRolls.val[0].val;
  types::effectMultiplier idealDoubledMultiplier =
    (types::effectMultiplier)idealBaseDamageRolls.val[0].val / idealHalvedDamageRolls.val[0].val;

  calc_damage::UsesUntilKo idealBaseKoUses = {{{23, 0.0625F}, {25, 0.4375F}, {27, 0.5F}}};
  calc_damage::UsesUntilKo idealHalvedKoUses = {{{50, 0.5F}, {59, 0.5F}}};

  auto checkResults = [&](const analyze_effect::Results& results) {
    const auto multipliers = results.effectMultiplierResults();
    const auto damageRolls = results.multipliedDamageRollsResults();
    const auto maxDamages = results.multipliedMaxDamageResults();
    const auto koUses = results.multipliedUsesUntilKoResults();

    REQUIRE(multipliers.size() == idealMultipliers.size());
    REQUIRE(damageRolls.size() == idealMultipliers.size());
    REQUIRE(maxDamages.size() == idealMultipliers.size());
    REQUIRE(koUses.size() == (getKoUses ? idealMultipliers.size() : 0U));

    multipliers.each([&](types::entity entity, const analyze_effect::EffectMultiplier& multiplier) {
      REQUIRE(idealMultipliers.contains(entity));

      const auto& [idealMultiplier, idealDamageRolls, idealKoUses] = idealMultipliers.at(entity);
      REQUIRE(multiplier.val == idealMultiplier);

      REQUIRE(maxDamages.contains(entity));
      const auto& [maxDamage] = maxDamages.get(entity);
      REQUIRE(maxDamage.val == idealDamageRolls.val[0].val);

      if (getKoUses) {
        REQUIRE(damageRolls.contains(entity));
        const auto& [trueDamageRolls] = damageRolls.get(entity);
        REQUIRE(trueDamageRolls.val.size() == idealDamageRolls.val.size());
        for (std::size_t i = 0; i < trueDamageRolls.val.size(); i++) {
          REQUIRE(trueDamageRolls.val[i].val == idealDamageRolls.val[i].val);
        }

        REQUIRE(koUses.contains(entity));
        const auto& [trueKosUses] = koUses.get(entity);

        REQUIRE(trueKosUses.minHits() == idealKoUses.val[0]);
        REQUIRE(trueKosUses.maxHits() == idealKoUses.val.back());
        REQUIRE(trueKosUses.guaranteedKo() == false);

        REQUIRE(trueKosUses.val.size() == idealKoUses.val.size());
        for (std::size_t i = 0; i < trueKosUses.val.size(); i++) {
          REQUIRE(trueKosUses.val[i] == idealKoUses.val[i]);
        }
      }
    });
  };

  auto runSections = [&]() {
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

    SECTION("One Effect Applied") {
      battleCreationInfo.p1.team[0].status = dex::Status::BRN;

      battleCreationInfo.runWithAnalyzeEffect = true;
      battleCreationInfo.effectsToAnalyze = {{
        Slot::P1A,
        Slot::P2A,
        Slot::P1A,
        {dex::Move::FURY_ATTACK},
        dex::Status::BRN,
      }};
      simulation.createInitialStates({battleCreationInfo});

      types::entity inputEntity = simulation.registry.view<analyze_effect::tags::Input>()[0];
      idealMultipliers.emplace(
        inputEntity,
        Ideals{reconsiderActiveEffects ? idealHalvedMultiplier : 1.0F, idealHalvedDamageRolls, idealHalvedKoUses});

      auto results = simulation.analyzeEffect();
      checkResults(results);
    }

    SECTION("One Effect Not Applied") {
      battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;

      battleCreationInfo.runWithAnalyzeEffect = true;
      battleCreationInfo.effectsToAnalyze = {{
        Slot::P1A,
        Slot::P2A,
        Slot::P1A,
        {dex::Move::FURY_ATTACK},
        dex::Status::BRN,
      }};
      simulation.createInitialStates({battleCreationInfo});

      types::entity inputEntity = simulation.registry.view<analyze_effect::tags::Input>()[0];
      idealMultipliers.emplace(inputEntity, Ideals{idealHalvedMultiplier, idealHalvedDamageRolls, idealHalvedKoUses});

      auto results = simulation.analyzeEffect();
      checkResults(results);
    }

    SECTION("Multiple Effects, One Applied") {
      battleCreationInfo.p1.team[0].status = dex::Status::BRN;

      battleCreationInfo.runWithAnalyzeEffect = true;
      battleCreationInfo.effectsToAnalyze = {
        {
          Slot::P1A,
          Slot::P2A,
          Slot::P1A,
          {dex::Move::FURY_ATTACK},
          dex::Status::BRN,
        },
        {
          Slot::P1A,
          Slot::P2A,
          Slot::P1A,
          {dex::Move::FURY_ATTACK},
          dex::Status::PAR,
        },
      };
      simulation.createInitialStates({battleCreationInfo});

      simulation.registry.view<analyze_effect::tags::Input, StatusName>().each([&](
                                                                                 types::entity entity,
                                                                                 StatusName status) {
        if (status.name == dex::Status::BRN) {
          idealMultipliers.emplace(
            entity,
            Ideals{reconsiderActiveEffects ? idealHalvedMultiplier : 1.0F, idealHalvedDamageRolls, idealHalvedKoUses});
        }
        else {
          idealMultipliers.emplace(entity, Ideals{idealDoubledMultiplier, idealBaseDamageRolls, idealBaseKoUses});
        }
      });

      auto results = simulation.analyzeEffect();
      checkResults(results);
    }

    SECTION("Multiple Effects, None Applied") {
      battleCreationInfo.p1.team[0].status = dex::Status::NO_STATUS;

      battleCreationInfo.runWithAnalyzeEffect = true;
      battleCreationInfo.effectsToAnalyze = {
        {
          Slot::P1A,
          Slot::P2A,
          Slot::P1A,
          {dex::Move::FURY_ATTACK},
          dex::Status::BRN,
        },
        {
          Slot::P1A,
          Slot::P2A,
          Slot::P1A,
          {dex::Move::FURY_ATTACK},
          dex::Status::PAR,
        },
      };
      simulation.createInitialStates({battleCreationInfo});

      simulation.registry.view<analyze_effect::tags::Input, StatusName>().each(
        [&](types::entity entity, StatusName status) {
          if (status.name == dex::Status::BRN) {
            idealMultipliers.emplace(entity, Ideals{idealHalvedMultiplier, idealHalvedDamageRolls, idealHalvedKoUses});
          }
          else {
            idealMultipliers.emplace(entity, Ideals{1.0F, idealBaseDamageRolls, idealBaseKoUses});
          }
        });

      auto results = simulation.analyzeEffect();
      checkResults(results);
    }
  };

  auto alternateReconsideringEffects = [&]() {
    SECTION("Reconsider Effects") {
      reconsiderActiveEffects = true;
      runSections();
    }

    SECTION("Don't Reconsider Effects") {
      reconsiderActiveEffects = false;
      runSections();
    }
  };

  SECTION("With KO Uses") {
    getKoUses = true;
    alternateReconsideringEffects();
  }

  SECTION("Without KO Uses") {
    getKoUses = false;
    alternateReconsideringEffects();
  }
}
}  // namespace pokesim