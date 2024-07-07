#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Selection Levels", "[RegistryContainer]") {
  internal::RegistryContainer container;

  struct FilterA {};
  struct FilterB {};
  struct FilterC {};
  struct Select {};

  types::entity abc = container.registry.create();
  types::entity ab = container.registry.create();
  types::entity a = container.registry.create();
  container.registry.emplace<FilterA>(abc);
  container.registry.emplace<FilterA>(ab);
  container.registry.emplace<FilterA>(a);

  container.registry.emplace<FilterB>(abc);
  container.registry.emplace<FilterB>(ab);

  container.registry.emplace<FilterC>(abc);

  SECTION("Largest To Smallest") {
    internal::SelectForView<Select, FilterA> stageOne{container};
    REQUIRE_FALSE(stageOne.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 3);

    internal::SelectForView<Select, FilterB> stageTwo{container};
    REQUIRE_FALSE(stageTwo.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 2);

    internal::SelectForView<Select, FilterC> stageThree{container};
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
    internal::SelectForView<Select, FilterC> stageOne{container};
    REQUIRE_FALSE(stageOne.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    internal::SelectForView<Select, FilterB> stageTwo{container};
    REQUIRE_FALSE(stageTwo.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    internal::SelectForView<Select, FilterA> stageThree{container};
    REQUIRE_FALSE(stageThree.hasNoneSelected());
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageThree.deselect();
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageTwo.deselect();
    REQUIRE(container.registry.view<Select>().size() == 1);

    stageOne.deselect();
    REQUIRE(container.registry.view<Select>().size() == 0);
  }
}
}  // namespace pokesim