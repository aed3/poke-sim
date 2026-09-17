#include "Tests.hpp"

namespace pokesim {
namespace {
struct IdealPP_MaxPP {
  types::pp pp;
  types::pp maxPp;
};
}  // namespace

TEST_CASE("Battle State: Single Battle", "[Simulation][Setup][SingleBattle]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Ability::COMPETITIVE, dex::Status::FRZ, dex::Move::FURY_ATTACK)),
    test.side(test.pokemon(
      dex::Species::AMPHAROS,
      dex::Ability::PLUS,
      dex::Nature::HARDY,
      dex::Item::CHOICE_SPECS,
      dex::Move::THUNDERBOLT)));

  test.initializeSimulation();

  const types::registry& registry = test.registry();
  auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == 1U);
  auto entities = test.getBattleEntities(battles.front());

  REQUIRE(registry.all_of<dex::Competitive>(entities.p1A));
  REQUIRE(registry.all_of<dex::Freeze>(entities.p1A));
  REQUIRE(registry.all_of<dex::Plus>(entities.p2A));
  REQUIRE(registry.all_of<nature::tags::Hardy>(entities.p2A));
  REQUIRE(registry.all_of<dex::ChoiceSpecs>(entities.p2A));
  REQUIRE(registry.all_of<item::tags::Choice>(entities.p2A));
}

TEST_CASE("Battle State: Double Battle", "[Simulation][Setup][DoubleBattle]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    test.side(
      test.pokemon(
        dex::Species::GARDEVOIR,
        dex::Ability::TRACE,
        dex::Status::BRN,
        dex::Item::CHOICE_SCARF,
        dex::Move::MOONBLAST),
      test.pokemon(
        dex::Species::DRAGAPULT,
        dex::Ability::INFILTRATOR,
        dex::Nature::HASTY,
        dex::Item::FOCUS_SASH,
        dex::Move::WILL_O_WISP)),
    test.side(
      test.pokemon(dex::Species::PANGORO, dex::Ability::IRON_FIST, dex::Item::LIFE_ORB, dex::Move::KNOCK_OFF),
      test
        .pokemon(dex::Species::RIBOMBEE, dex::Ability::SWEET_VEIL, dex::Item::BRIGHT_POWDER, dex::Move::QUIVER_DANCE)));

  test.initializeSimulation();

  const types::registry& registry = test.registry();
  auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == 1U);
  auto entities = test.getBattleEntities(battles.front());

  REQUIRE(registry.all_of<dex::Trace>(entities.p1A));
  REQUIRE(registry.all_of<dex::Burn>(entities.p1A));
  REQUIRE(registry.all_of<dex::ChoiceScarf>(entities.p1A));
  REQUIRE(registry.all_of<dex::IronFist>(entities.p2A));
  REQUIRE(registry.all_of<dex::LifeOrb>(entities.p2A));

  REQUIRE(registry.all_of<dex::Infiltrator>(entities.p1B));
  REQUIRE(registry.all_of<nature::tags::Hasty>(entities.p1B));
  REQUIRE(registry.all_of<dex::FocusSash>(entities.p1B));
  REQUIRE(registry.all_of<dex::SweetVeil>(entities.p2B));
  REQUIRE(registry.all_of<dex::BrightPowder>(entities.p2B));
}

TEST_CASE("Battle State: Multiple Battles", "[Simulation][Setup][SingleBattle]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  auto side1 = test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::FURY_ATTACK));
  auto side2 = test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::THUNDERBOLT));

  test.setupBattle(Turn{12U}, Probability{0.1F}, RngSeed{0xFFFFFFFF}, side1, side2);
  test.setupBattle(Turn{7U}, Probability{0.75F}, RngSeed{9826U}, side1, side2);
  test.initializeSimulation();

  const types::registry& registry = test.registry();
  auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == 2U);

  for (types::entity battle : battles) {
    if (registry.get<Turn>(battle).val == 12U) {
      REQUIRE(registry.get<Probability>(battle).val == 0.1F);
      REQUIRE(registry.get<RngSeed>(battle).val == 0xFFFFFFFF);
    }
    else {
      REQUIRE(registry.get<Turn>(battle).val == 7U);
      REQUIRE(registry.get<Probability>(battle).val == 0.75F);
      REQUIRE(registry.get<RngSeed>(battle).val == 9826U);
    }
  }
}

TEST_CASE("Clone Battles", "[Simulation][Setup]") {
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  types::entityIndex cloneCount = GENERATE(1U, 5U, 100U);
  CAPTURE(battleFormat, cloneCount);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, battleFormat};
  BattleCreationInfo& battleCreationInfo = test.setupBattle(
    test.side(
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::MOONBLAST),
      test.pokemon(dex::Species::DRAGAPULT, dex::Move::WILL_O_WISP)),
    test.side(
      test.pokemon(dex::Species::PANGORO, dex::Move::KNOCK_OFF),
      test.pokemon(dex::Species::RIBOMBEE, dex::Move::QUIVER_DANCE)));
  test.initializeSimulation();

  types::registry& registry = test.registry();
  entt::dense_set<types::entity> existingEntities;
  for (types::entity existingEntity : registry.view<types::entity>()) {
    existingEntities.insert(existingEntity);
  }
  existingEntities.reserve(existingEntities.size() * (cloneCount + 1U));

  types::handle baseHandle(registry, registry.view<tags::Battle>().front());
  baseHandle.emplace<tags::CloneFrom>();

  clone(registry, cloneCount);

  const auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == cloneCount + 1U);
  REQUIRE(registry.view<tags::CloneFrom>().empty());
  REQUIRE(registry.view<CloneTo>().empty());

  const auto baseSides = baseHandle.get<Sides>();

  for (auto battle : battles) {
    const types::handle cloneHandle{registry, battle};
    debug::SimulationSetupChecks::checkBattle(test.simulation, battle, battleCreationInfo);

    if (battle == baseHandle.entity()) continue;

    REQUIRE_FALSE(existingEntities.contains(battle));
    existingEntities.insert(battle);
    const auto cloneSides = cloneHandle.get<Sides>();
    auto checkSideEntities = [&](types::entity baseSide, types::entity cloneSide) {
      REQUIRE_FALSE(existingEntities.contains(cloneSide));
      existingEntities.insert(cloneSide);

      const auto& [baseTeam, baseFoeSide, baseSideBattle] = registry.get<Team, FoeSide, Battle>(baseSide);
      const auto& [cloneTeam, cloneFoeSide, cloneSideBattle] = registry.get<Team, FoeSide, Battle>(cloneSide);
      REQUIRE(baseFoeSide.val != cloneFoeSide.val);
      REQUIRE(cloneSide != cloneFoeSide.val);
      if (cloneSide == cloneSides.val.p1()) {
        REQUIRE(cloneSides.val.p2() == cloneFoeSide.val);
      }
      else if (cloneSide == cloneSides.val.p2()) {
        REQUIRE(cloneSides.val.p1() == cloneFoeSide.val);
      }

      REQUIRE(baseSideBattle.val != cloneSideBattle.val);
      REQUIRE(battle == cloneSideBattle.val);

      REQUIRE(baseTeam.val.size() == cloneTeam.val.size());

      for (types::teamPositionIndex i = 0U; i < baseTeam.val.size(); i++) {
        types::entity basePokemon = baseTeam.val[i];
        types::entity clonePokemon = cloneTeam.val[i];
        existingEntities.insert(basePokemon);
        REQUIRE_FALSE(existingEntities.contains(clonePokemon));
        existingEntities.insert(clonePokemon);

        const auto& [basePokemonSide, basePokemonBattle] = registry.get<Side, Battle>(basePokemon);
        const auto& [clonePokemonSide, clonePokemonBattle] = registry.get<Side, Battle>(clonePokemon);

        REQUIRE(basePokemonSide.val != clonePokemonSide.val);
        REQUIRE(cloneSide == clonePokemonSide.val);

        REQUIRE(basePokemonBattle.val != clonePokemonBattle.val);
        REQUIRE(battle == clonePokemonBattle.val);
      }
    };

    REQUIRE(baseSides.val.size() == cloneSides.val.size());
    for (types::sideIndex i = 0U; i < baseSides.val.size(); i++) {
      REQUIRE(baseSides.val[i] != cloneSides.val[i]);
      checkSideEntities(baseSides.val[i], cloneSides.val[i]);
    }
  }
}
}  // namespace pokesim
