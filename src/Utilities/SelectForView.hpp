#pragma once

#include <Components/Tags/SelectionTags.hpp>
#include <Simulation/Simulation.hpp>

namespace pokesim::internal {
template <typename SelectionTag, typename... ComponentsToSelect>
struct SelectForView {
  SelectForView(Simulation& simulation_) : simulation(simulation_) {
    static_assert(
      std::is_same<tags::SelectedForViewBattle, SelectionTag>() ||
      std::is_same<tags::SelectedForViewSide, SelectionTag>() ||
      std::is_same<tags::SelectedForViewPokemon, SelectionTag>() ||
      std::is_same<tags::SelectedForViewMove, SelectionTag>());

    auto view = simulation.registry.view<ComponentsToSelect...>();
    simulation.registry.insert<SelectionTag>(view.begin(), view.end());
  };

  ~SelectForView() { deselect(); }

  void deselect() {
    auto view = simulation.registry.view<SelectionTag, ComponentsToSelect...>();
    simulation.registry.erase<SelectionTag>(view.begin(), view.end());
  }

 private:
  Simulation& simulation;
};

template <typename... ComponentsToSelect>
struct SelectForBattleView : SelectForView<tags::SelectedForViewBattle, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForSideView : SelectForView<tags::SelectedForViewSide, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForPokemonView : SelectForView<tags::SelectedForViewPokemon, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForMoveView : SelectForView<tags::SelectedForViewMove, ComponentsToSelect...> {};
}  // namespace pokesim::internal