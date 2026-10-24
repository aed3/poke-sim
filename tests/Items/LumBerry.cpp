#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Lum Berry: Prevents status and is removed", "[Simulation][SimulateTurn][SingleBattle][Item][LumBerry]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::LUM_BERRY, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::GLARE)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::GLARE));

  auto entities = test.simulateOneNonBranchingBattle(Tags<ActiveWithUpdateEvents>{});
  test.checks.checkUsedMovePokemon<item::tags::Berry, dex::LumBerry, tags::HasItem>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  types::registry& registry = test.registry();
  REQUIRE_FALSE(registry.all_of<item::tags::Berry>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::LumBerry>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<tags::HasItem>(entities.p1A));
}

TEST_CASE("Lum Berry: Heals status and is removed", "[Simulation][SimulateTurn][SingleBattle][Item][LumBerry]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  dex::Status status = GENERATE(
    dex::Status::BRN,
    dex::Status::FRZ,
    dex::Status::PAR,
    dex::Status::PSN,
    dex::Status::SLP,
    dex::Status::TOX);
  CAPTURE(status);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::LUM_BERRY, status, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle(Tags<ActiveWithUpdateEvents>{});

  types::registry& registry = test.registry();
  REQUIRE_FALSE(registry.all_of<item::tags::Berry>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::LumBerry>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<tags::HasItem>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<tags::HasStatus>(entities.p1A));

  REQUIRE_FALSE(registry.all_of<dex::Burn>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::Freeze>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::Paralysis>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::Poison>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::Sleep>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::Toxic>(entities.p1A));
}

TEST_CASE(
  "Lum Berry: Causes speed tie if activated at the same time",
  "[Simulation][SimulateTurn][SingleBattle][Item][LumBerry]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::LUM_BERRY, dex::Status::BRN, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::LUM_BERRY, dex::Status::BRN, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  auto battleEntitiesList = test.simulateOneBranchingBattle(Tags<ActiveWithUpdateEvents>{});

  types::registry& registry = test.registry();
  for (auto& entities : battleEntitiesList) {
    Probability probability = registry.get<Probability>(entities.battle);
    REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(0.25F));

    test.checks.checkUsedMovePokemon<item::tags::Berry, dex::LumBerry, tags::HasItem, tags::HasStatus, dex::Burn>(
      entities.p1A);
    test.checks.checkUsedMovePokemon<item::tags::Berry, dex::LumBerry, tags::HasItem, tags::HasStatus, dex::Burn>(
      entities.p2A);
    REQUIRE_FALSE(registry.all_of<item::tags::Berry>(entities.p1A));
    REQUIRE_FALSE(registry.all_of<item::tags::Berry>(entities.p2A));
    REQUIRE_FALSE(registry.all_of<dex::LumBerry>(entities.p1A));
    REQUIRE_FALSE(registry.all_of<dex::LumBerry>(entities.p2A));
    REQUIRE_FALSE(registry.all_of<tags::HasItem>(entities.p1A));
    REQUIRE_FALSE(registry.all_of<tags::HasItem>(entities.p2A));
    REQUIRE_FALSE(registry.all_of<tags::HasStatus>(entities.p1A));
    REQUIRE_FALSE(registry.all_of<tags::HasStatus>(entities.p2A));
    REQUIRE_FALSE(registry.all_of<dex::Burn>(entities.p1A));
    REQUIRE_FALSE(registry.all_of<dex::Burn>(entities.p2A));
  }
}
}  // namespace pokesim
