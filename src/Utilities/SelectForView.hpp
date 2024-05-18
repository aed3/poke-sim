#pragma once

#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Selection.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <cstdint>
#include <vector>

namespace pokesim::internal {
template <typename Selection, typename... ComponentsToSelect>
struct SelectForView {
  SelectForView(Simulation& simulation_) : simulation(simulation_), depth(&Selection::depth) {
    auto view = simulation.registry.view<Selection>();
    if (!view.empty()) {
      previouslySelected.insert(previouslySelected.begin(), view.begin(), view.end());
      simulation.registry.clear<Selection>();
    }

    for (types::entity entity : simulation.registry.view<ComponentsToSelect...>()) {
      simulation.registry.emplace<Selection>(entity);
    }

    ENTT_ASSERT(depth != nullptr, "Depth needs to be defined.");
    *depth = *depth + 1;
  }

  ~SelectForView() { deselect(); }

  void deselect() {
    simulation.registry.clear<Selection>();

    for (types::entity entity : previouslySelected) {
      if (simulation.registry.valid(entity)) {
        simulation.registry.emplace<Selection>(entity);
      }
    }
    previouslySelected.clear();

    if (depth) {
      ENTT_ASSERT(*depth, "Selection depth cannot go negative.");
      *depth = *depth - 1;
      depth = nullptr;
    }
  }

 private:
  Simulation& simulation;
  std::uint8_t* depth = nullptr;
  std::vector<types::entity> previouslySelected;
};

template <typename... ComponentsToSelect>
struct SelectForBattleView : SelectForView<SelectedForViewBattle, Sides, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForSideView : SelectForView<SelectedForViewSide, Team, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForPokemonView : SelectForView<SelectedForViewPokemon, SpeciesName, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForMoveView : SelectForView<SelectedForViewMove, MoveName, ComponentsToSelect...> {};
}  // namespace pokesim::internal