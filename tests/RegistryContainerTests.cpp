#include "Tests.hpp"

namespace pokesim {
struct FilterA {};
struct FilterB {};
struct FilterC {};
struct Select {};
struct Required {};

template <typename... Components>
using SelectForTest = internal::SelectForView<Select, Required, Components...>;

TEST_CASE("Selection Levels", "[RegistryContainer]") {
  internal::RegistryContainer container;

  types::entity filtersABC = container.registry.create();
  types::entity filtersAB = container.registry.create();
  types::entity filterA = container.registry.create();
  container.registry.emplace<Required>(filtersABC);
  container.registry.emplace<Required>(filtersAB);
  container.registry.emplace<Required>(filterA);

  container.registry.emplace<FilterA>(filtersABC);
  container.registry.emplace<FilterA>(filtersAB);
  container.registry.emplace<FilterA>(filterA);

  container.registry.emplace<FilterB>(filtersABC);
  container.registry.emplace<FilterB>(filtersAB);

  container.registry.emplace<FilterC>(filtersABC);

  SECTION("Largest To Smallest") {
    SelectForTest<FilterA> stageOne{container};
    REQUIRE_FALSE(stageOne.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 3);

    SelectForTest<FilterB> stageTwo{container};
    REQUIRE_FALSE(stageTwo.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 2);

    SelectForTest<FilterC> stageThree{container};
    REQUIRE_FALSE(stageThree.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageThree.deselect();
    REQUIRE(container.registry.view<Select>().size() == 2);

    stageTwo.deselect();
    REQUIRE(container.registry.view<Select>().size() == 3);

    stageOne.deselect();
    REQUIRE(container.registry.view<Select>().empty());
  }

  SECTION("Smallest to Largest") {
    SelectForTest<FilterC> stageOne{container};
    REQUIRE_FALSE(stageOne.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    SelectForTest<FilterB> stageTwo{container};
    REQUIRE_FALSE(stageTwo.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    SelectForTest<FilterA> stageThree{container};
    REQUIRE_FALSE(stageThree.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageThree.deselect();
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageTwo.deselect();
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageOne.deselect();
    REQUIRE(container.registry.view<Select>().empty());
  }

  SECTION("Blank Selection") {
    SelectForTest<> stageZero{container};
    REQUIRE_FALSE(stageZero.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 3);

    stageZero.deselect();

    SelectForTest<FilterB> stageOne{container};
    REQUIRE_FALSE(stageOne.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 2);

    SelectForTest<> stageTwo{container};
    REQUIRE_FALSE(stageTwo.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 2);

    stageTwo.deselect();

    REQUIRE(container.registry.view<Select>().size() == 2);
  }
}
}  // namespace pokesim