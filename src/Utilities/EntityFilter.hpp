#pragma once

#include <Simulation/Simulation.hpp>
#include <type_traits>

namespace pokesim::internal {
template <typename SelectionTag, typename... OtherSelectionTags>
struct EntityFilter {
 private:
  template <auto Function, typename...>
  struct Filter;

  template <auto Function, typename ExcludeContainer, typename IncludeContainer, typename... ExtraTags>
  struct Filter<Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer> {
    template <typename... PassedInArgs>
    static void view(Simulation* simulation, const PassedInArgs&... passedInArgs) {
      simulation
        ->view<Function, Tags<SelectionTag, OtherSelectionTags..., ExtraTags...>, ExcludeContainer, IncludeContainer>(
          passedInArgs...);
    }

    template <typename... PassedInArgs>
    static void group(Simulation* simulation, const PassedInArgs&... passedInArgs) {
      simulation
        ->group<Function, Tags<SelectionTag, OtherSelectionTags..., ExtraTags...>, ExcludeContainer, IncludeContainer>(
          passedInArgs...);
    }
  };

 public:
  EntityFilter(Simulation& simulation_) : simulation(&simulation_) {}

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  auto view(const PassedInArgs&... passedInArgs) {
    Filter<Function, TagContainer, ExcludeContainer, IncludeContainer>::view(simulation, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  auto group(const PassedInArgs&... passedInArgs) {
    Filter<Function, TagContainer, ExcludeContainer, IncludeContainer>::group(simulation, passedInArgs...);
  }

  template <typename... IncludeComponents, typename... ExcludeComponents>
  void applySelectionTags(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) {
    static_assert(
      !(sizeof...(ExcludeComponents) == 0U && std::conjunction_v<std::is_empty<IncludeComponents>...>),
      "If there are no excludes and all included components are tags, use those tags as the SelectionTags directly.");
    auto view = simulation->registry.view<IncludeComponents...>(exclude);
    simulation->registry.insert<SelectionTag, OtherSelectionTags...>(view.begin(), view.end());
  }

  bool hasNoneSelected() {
    auto view = simulation->registry.view<SelectionTag, OtherSelectionTags...>();
    return view.begin() == view.end();
  }

  void clearSelectionTags() { simulation->registry.clear<SelectionTag, OtherSelectionTags...>(); }

 private:
  Simulation* simulation = nullptr;
};
}  // namespace pokesim::internal
