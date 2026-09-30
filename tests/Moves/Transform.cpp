#include "../Tests.hpp"

namespace pokesim {
namespace {
template <typename NewAbility, typename... OtherComponents>
void checkTransformerChanges(TestSimulation& test, types::entity transformer) {
  test.checks.checkEntityForChanges<
    TransformedFrom,
    SpeciesName,
    SpeciesTypes,
    dex::Imposter,
    NewAbility,
    MoveSlots,
    stat::Atk,
    stat::Def,
    stat::Spa,
    stat::Spd,
    stat::Spe,
    stat::EffectiveAtk,
    stat::EffectiveDef,
    stat::EffectiveSpa,
    stat::EffectiveSpd,
    stat::EffectiveSpe,
    OtherComponents...>(transformer);
}

template <typename NewAbility>
void checkCorrectTransform(
  TestSimulation& test, types::handle transformer, types::handle transformTarget, bool checkEffectiveStats) {
  TransformedFrom transformedFrom = transformer.get<TransformedFrom>();
  REQUIRE(transformedFrom.species == dex::Species::DITTO);
  REQUIRE(transformedFrom.ability == dex::Ability::IMPOSTER);
  REQUIRE_THAT(
    transformedFrom.moves,
    Catch::Matchers::RangeEquals({MoveSlot{
      dex::Move::TRANSFORM,
      (types::pp)(test.dexValue<dex::Transform::basePp>() - Constants::PP_USE_DEDUCTION),
      test.dexValue<dex::Transform::basePp>(),
    }}));

  REQUIRE(transformer.get<SpeciesName>().val == transformTarget.get<SpeciesName>().val);
  REQUIRE(transformer.get<SpeciesTypes>().val == transformTarget.get<SpeciesTypes>().val);
  REQUIRE(transformer.all_of<NewAbility>());
  REQUIRE_FALSE(transformer.all_of<dex::Imposter>());

  MoveSlots transformedFromMoveSlots = transformTarget.get<MoveSlots>();
  MoveSlots idealMoveSlots;
  for (MoveSlot transformedFromMoveSlot : transformedFromMoveSlots.val) {
    idealMoveSlots.val.push_back({
      transformedFromMoveSlot.move,
      Constants::MoveMaxPp::COPIED_WITH_TRANSFORM,
      Constants::MoveMaxPp::COPIED_WITH_TRANSFORM,
    });
  }

  REQUIRE_THAT(transformer.get<MoveSlots>().val, Catch::Matchers::RangeEquals(idealMoveSlots.val));

  REQUIRE(transformer.get<stat::Atk>().val == transformTarget.get<stat::Atk>().val);
  REQUIRE(transformer.get<stat::Def>().val == transformTarget.get<stat::Def>().val);
  REQUIRE(transformer.get<stat::Spa>().val == transformTarget.get<stat::Spa>().val);
  REQUIRE(transformer.get<stat::Spd>().val == transformTarget.get<stat::Spd>().val);
  REQUIRE(transformer.get<stat::Spe>().val == transformTarget.get<stat::Spe>().val);

  if (checkEffectiveStats) {
    REQUIRE(transformer.get<stat::EffectiveAtk>().val == transformTarget.get<stat::EffectiveAtk>().val);
    REQUIRE(transformer.get<stat::EffectiveDef>().val == transformTarget.get<stat::EffectiveDef>().val);
    REQUIRE(transformer.get<stat::EffectiveSpa>().val == transformTarget.get<stat::EffectiveSpa>().val);
    REQUIRE(transformer.get<stat::EffectiveSpd>().val == transformTarget.get<stat::EffectiveSpd>().val);
    REQUIRE(transformer.get<stat::EffectiveSpe>().val == transformTarget.get<stat::EffectiveSpe>().val);
  }
}
}  // namespace

TEST_CASE(
  "Transform: Copies species, ability, moves, and stats", "[Simulation][SimulateTurn][SingleBattle][Move][Transform]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::TRANSFORM)),
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::TORRENT,
      dex::Nature::MODEST,
      Evs{10U, 20U, 30U, 40U, 50U, 60U},
      Ivs{30U, 25U, 20U, 15U, 10U, 5U},
      dex::Move::SPLASH,
      dex::Move::FURY_ATTACK,
      dex::Move::FLASH_CANNON)),
    test.turnDecision(dex::Move::TRANSFORM, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();

  checkTransformerChanges<dex::Torrent>(test, entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  checkCorrectTransform<dex::Torrent>(test, {test.registry(), entities.p1A}, {test.registry(), entities.p2A}, true);
};

TEST_CASE("Transform: Revert transform on switch out", "[Simulation][SimulateTurn][SingleBattle][Move][Transform]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::TRANSFORM),
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPLASH)),
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::TORRENT,
      dex::Nature::MODEST,
      Evs{10U, 20U, 30U, 40U, 50U, 60U},
      Ivs{30U, 25U, 20U, 15U, 10U, 5U},
      dex::Move::SPLASH,
      dex::Move::FURY_ATTACK,
      dex::Move::FLASH_CANNON)),
    test.turnDecision(dex::Move::TRANSFORM, dex::Move::SPLASH));

  test.resetChecksBetweenDecisions = false;
  test.applyDecision(test.simulateOneNonBranchingBattle().battle, test.turnDecision(Slot::P1B, dex::Move::SPLASH));
  auto entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  test.checks.checkEntityForChanges<tags::ActivePokemon>(entities.p1A);
  test.checks.checkEntityForChanges<tags::ActivePokemon, MoveSlots>(entities.p1B);
  test.checks.checkEntityForChanges<LastUsedMove, MoveSlots>(entities.p2A);

  REQUIRE_THAT(
    test.registry().get<MoveSlots>(entities.p1B).val,
    Catch::Matchers::RangeEquals({MoveSlot{
      dex::Move::TRANSFORM,
      (types::pp)(test.dexValue<dex::Transform::basePp>() - Constants::PP_USE_DEDUCTION),
      test.dexValue<dex::Transform::basePp>(),
    }}));
};

TEST_CASE("Transform: Copy unmodified stats", "[Simulation][SimulateTurn][SingleBattle][Move][Transform]") {
  dex::Item p2Item = GENERATE(dex::Item::ASSAULT_VEST, dex::Item::CHOICE_SCARF, dex::Item::CHOICE_SPECS);
  CAPTURE(p2Item);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::TRANSFORM)),
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::TORRENT,
      p2Item,
      dex::Move::SPLASH,
      dex::Move::FURY_ATTACK,
      dex::Move::FLASH_CANNON)),
    test.turnDecision(dex::Move::TRANSFORM, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();

  checkTransformerChanges<dex::Torrent>(test, entities.p1A);
  test.checks.checkUsedMovePokemon<ChoiceLock, DisabledMoveSlots>(entities.p2A);

  types::handle p1Handle{test.registry(), entities.p1A};
  types::handle p2Handle{test.registry(), entities.p2A};
  checkCorrectTransform<dex::Torrent>(test, p1Handle, p2Handle, false);

  REQUIRE(p1Handle.get<stat::EffectiveAtk>().val == p2Handle.get<stat::Atk>().val);
  REQUIRE(p1Handle.get<stat::EffectiveDef>().val == p2Handle.get<stat::Def>().val);
  REQUIRE(p1Handle.get<stat::EffectiveSpa>().val == p2Handle.get<stat::Spa>().val);
  REQUIRE(p1Handle.get<stat::EffectiveSpd>().val == p2Handle.get<stat::Spd>().val);
  REQUIRE(p1Handle.get<stat::EffectiveSpe>().val == p2Handle.get<stat::Spe>().val);
};

TEST_CASE("Transform: Keep item stat modifiers", "[Simulation][SimulateTurn][SingleBattle][Move][Transform]") {
  dex::Item p1Item = GENERATE(dex::Item::ASSAULT_VEST, dex::Item::CHOICE_SCARF, dex::Item::CHOICE_SPECS);
  CAPTURE(p1Item);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, p1Item, dex::Move::TRANSFORM)),
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::TORRENT,
      dex::Move::SPLASH,
      dex::Move::FURY_ATTACK,
      dex::Move::FLASH_CANNON)),
    test.turnDecision(dex::Move::TRANSFORM, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();

  checkTransformerChanges<dex::Torrent>(test, entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  types::handle p1Handle{test.registry(), entities.p1A};
  types::handle p2Handle{test.registry(), entities.p2A};
  checkCorrectTransform<dex::Torrent>(test, p1Handle, p2Handle, false);

  REQUIRE(p1Handle.get<stat::EffectiveAtk>().val == p2Handle.get<stat::Atk>().val);
  REQUIRE(p1Handle.get<stat::EffectiveDef>().val == p2Handle.get<stat::Def>().val);

  auto [p1Spa, p1Spd, p1Spe] = p1Handle.get<stat::EffectiveSpa, stat::EffectiveSpd, stat::EffectiveSpe>();
  auto [p2Spa, p2Spd, p2Spe] = p2Handle.get<stat::EffectiveSpa, stat::EffectiveSpd, stat::EffectiveSpe>();
  if (p1Item == dex::Item::CHOICE_SPECS) {
    REQUIRE(
      p1Spa.val == internal::fixedPointMultiply(p2Spa.val, test.dexValue<dex::ChoiceSpecs::onModifySpaModifier>()));
  }
  else {
    REQUIRE(p1Spa.val == p2Spa.val);
  }
  if (p1Item == dex::Item::ASSAULT_VEST) {
    REQUIRE(
      p1Spd.val == internal::fixedPointMultiply(p2Spd.val, test.dexValue<dex::AssaultVest::onModifySpdModifier>()));
  }
  else {
    REQUIRE(p1Spd.val == p2Spd.val);
  }
  if (p1Item == dex::Item::CHOICE_SCARF) {
    REQUIRE(
      p1Spe.val == internal::fixedPointMultiply(p2Spe.val, test.dexValue<dex::ChoiceScarf::onModifySpeModifier>()));
  }
  else {
    REQUIRE(p1Spe.val == p2Spe.val);
  }
};

TEST_CASE("Transform: Copy stat boosts", "[Simulation][SimulateTurn][SingleBattle][Move][Transform]") {
  AtkBoost p2AtkBoost{5};
  DefBoost p2DefBoost{-4};
  SpaBoost p2SpaBoost{3};
  SpdBoost p2SpdBoost{-2};
  SpeBoost p2SpeBoost{1};

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(
      dex::Species::DITTO,
      dex::Ability::IMPOSTER,
      AtkBoost{-1},
      DefBoost{2},
      SpaBoost{-3},
      SpdBoost{4},
      SpeBoost{-5},
      dex::Move::TRANSFORM)),
    test.side(test.pokemon(
      dex::Species::EMPOLEON,
      dex::Ability::TORRENT,
      p2AtkBoost,
      p2DefBoost,
      p2SpaBoost,
      p2SpdBoost,
      p2SpeBoost,
      dex::Move::SPLASH,
      dex::Move::FURY_ATTACK,
      dex::Move::FLASH_CANNON)),
    test.turnDecision(dex::Move::TRANSFORM, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();

  checkTransformerChanges<dex::Torrent, AtkBoost, DefBoost, SpaBoost, SpdBoost, SpeBoost>(test, entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  types::handle p1Handle{test.registry(), entities.p1A};
  types::handle p2Handle{test.registry(), entities.p2A};
  checkCorrectTransform<dex::Torrent>(test, p1Handle, p2Handle, true);

  REQUIRE(p1Handle.get<AtkBoost>().val == p2AtkBoost.val);
  REQUIRE(p1Handle.get<DefBoost>().val == p2DefBoost.val);
  REQUIRE(p1Handle.get<SpaBoost>().val == p2SpaBoost.val);
  REQUIRE(p1Handle.get<SpdBoost>().val == p2SpdBoost.val);
  REQUIRE(p1Handle.get<SpeBoost>().val == p2SpeBoost.val);

  auto [idealAtk, idealDef, idealSpa, idealSpd, idealSpe] =
    p2Handle.get<stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>();
  internal::applyStatBoost(idealAtk.val, p2AtkBoost.val);
  internal::applyStatBoost(idealDef.val, p2DefBoost.val);
  internal::applyStatBoost(idealSpa.val, p2SpaBoost.val);
  internal::applyStatBoost(idealSpd.val, p2SpdBoost.val);
  internal::applyStatBoost(idealSpe.val, p2SpeBoost.val);

  REQUIRE(p1Handle.get<stat::EffectiveAtk>().val == idealAtk.val);
  REQUIRE(p1Handle.get<stat::EffectiveDef>().val == idealDef.val);
  REQUIRE(p1Handle.get<stat::EffectiveSpa>().val == idealSpa.val);
  REQUIRE(p1Handle.get<stat::EffectiveSpd>().val == idealSpd.val);
  REQUIRE(p1Handle.get<stat::EffectiveSpe>().val == idealSpe.val);
};

TEST_CASE("Transform: Fail against already transformed", "[Simulation][SimulateTurn][SingleBattle][Move][Transform]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DITTO, dex::Ability::IMPOSTER, dex::Move::TRANSFORM)),
    test.side(test.pokemon(dex::Species::EMPOLEON, Level{10U}, dex::Ability::TORRENT, dex::Move::TRANSFORM)),
    test.turnDecision(dex::Move::TRANSFORM, dex::Move::TRANSFORM));

  auto entities = test.simulateOneNonBranchingBattle();

  checkTransformerChanges<dex::Torrent>(test, entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  checkCorrectTransform<dex::Torrent>(test, {test.registry(), entities.p1A}, {test.registry(), entities.p2A}, true);
};
}  // namespace pokesim
