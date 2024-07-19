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

  types::entity abc = container.registry.create();
  types::entity ab = container.registry.create();
  types::entity a = container.registry.create();
  container.registry.emplace<Required>(abc);
  container.registry.emplace<Required>(ab);
  container.registry.emplace<Required>(a);

  container.registry.emplace<FilterA>(abc);
  container.registry.emplace<FilterA>(ab);
  container.registry.emplace<FilterA>(a);

  container.registry.emplace<FilterB>(abc);
  container.registry.emplace<FilterB>(ab);

  container.registry.emplace<FilterC>(abc);

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
    REQUIRE(container.registry.view<Select>().size() == 0);
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
    REQUIRE(container.registry.view<Select>().size() == 0);
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