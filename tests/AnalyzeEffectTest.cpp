#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  battleCreationInfo.runWithAnalyzeEffect = true;
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);

  bool getKoUses = GENERATE(false, true);
  bool reconsiderActiveEffects = GENERATE(false, true);

  CAPTURE(getKoUses, reconsiderActiveEffects);

  using Ideals = std::tuple<types::effectMultiplier, DamageRolls, calc_damage::UsesUntilKo>;
  entt::dense_map<types::entity, Ideals> idealMultipliers;
  const DamageRolls idealBaseDamageRolls = {
    {13U, 12U, 12U, 12U, 12U, 12U, 12U, 12U, 11U, 11U, 11U, 11U, 11U, 11U, 11U, 11U}};
  const DamageRolls idealHalvedDamageRolls = {{6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U}};

  types::effectMultiplier idealHalvedMultiplier =
    (types::effectMultiplier)idealHalvedDamageRolls.val[0].val / idealBaseDamageRolls.val[0].val;
  types::effectMultiplier idealDoubledMultiplier =
    (types::effectMultiplier)idealBaseDamageRolls.val[0].val / idealHalvedDamageRolls.val[0].val;

  calc_damage::UsesUntilKo idealBaseKoUses = {{{23U, 1 / 16.0F}, {25U, 7 / 16.0F}, {27U, 8 / 16.0F}}};
  calc_damage::UsesUntilKo idealHalvedKoUses = {{{50U, 0.5F}, {59U, 0.5F}}};

  auto checkResults = [&](const analyze_effect::Results& results) {
    const auto multipliers = results.effectMultiplierResults();
    const auto damageRolls = results.multipliedDamageRollsResults();
    const auto koUses = results.multipliedUsesUntilKoResults();

    REQUIRE(multipliers.size() == idealMultipliers.size());
    REQUIRE(damageRolls.size() == idealMultipliers.size());
    REQUIRE(koUses.size() == (getKoUses ? idealMultipliers.size() : 0U));

    multipliers.each([&](types::entity entity, const analyze_effect::EffectMultiplier& multiplier) {
      REQUIRE(idealMultipliers.contains(entity));

      const auto& [idealMultiplier, idealDamageRolls, idealKoUses] = idealMultipliers.at(entity);
      REQUIRE(multiplier.val == idealMultiplier);

      if (getKoUses) {
        REQUIRE(damageRolls.contains(entity));
        const auto& [trueDamageRolls] = damageRolls.get(entity);
        REQUIRE(trueDamageRolls.val.size() == idealDamageRolls.val.size());
        for (types::damageRollIndex i = 0U; i < trueDamageRolls.val.size(); i++) {
          REQUIRE(trueDamageRolls.val[i].val == idealDamageRolls.val[i].val);
        }

        REQUIRE(koUses.contains(entity));
        const auto& [trueKosUses] = koUses.get(entity);

        REQUIRE(trueKosUses.minHits() == idealKoUses.val.front());
        REQUIRE(trueKosUses.maxHits() == idealKoUses.val.back());
        REQUIRE(trueKosUses.guaranteedKo() == false);

        REQUIRE(trueKosUses.val.size() == idealKoUses.val.size());
        for (types::damageRollIndex i = 0U; i < trueKosUses.val.size(); i++) {
          REQUIRE(trueKosUses.val[i] == idealKoUses.val[i]);
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

  SECTION("One Effect Applied") {
    battleCreationInfo.p1.team[0].status = dex::Status::BRN;

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
}
}  // namespace pokesim
