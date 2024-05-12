#pragma once

#include <Components/Selection.hpp>
#include <Simulation/Simulation.hpp>

namespace pokesim::internal {
template <typename Selection, typename... ComponentsToSelect>
struct SelectForView {
  SelectForView(Simulation& simulation_) : simulation(simulation_) {
    for (types::entity entity : simulation.registry.view<ComponentsToSelect...>()) {
      simulation.registry.get_or_emplace<Selection>(entity).count++;
    }
  };

  ~SelectForView() { deselect(); }

  void deselect() {
    for (types::entity entity : simulation.registry.view<Selection, ComponentsToSelect...>()) {
      Selection& selection = simulation.registry.get<Selection>(entity);
      selection.count--;

      if (!selection.count) {
        simulation.registry.remove<Selection>(entity);
      }
    }
  }

 private:
  Simulation& simulation;
};

template <typename... ComponentsToSelect>
struct SelectForBattleView : SelectForView<SelectedForViewBattle, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForSideView : SelectForView<SelectedForViewSide, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForPokemonView : SelectForView<SelectedForViewPokemon, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForMoveView : SelectForView<SelectedForViewMove, ComponentsToSelect...> {};
}  // namespace pokesim::internal