#pragma once

#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Selection.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <cstdint>
#include <entt/container/dense_set.hpp>
#include <vector>

namespace pokesim::internal {
template <typename Selection, typename... ComponentsToSelect>
struct SelectForView {
  SelectForView(Simulation& simulation_)
      : simulation(&simulation_), selectedCount(simulation->registry.group(entt::get<ComponentsToSelect...>).size()) {
    if (hasNoneSelected()) {
      // simulation = nullptr;
      // return;
    }

    Selection::depth.emplace_back([](const void*, const types::registry& registry) {
      auto view = registry.view<ComponentsToSelect...>();
      return std::vector<types::entity>{view.begin(), view.end()};
    });

    simulation->registry.clear<Selection>();

    for (types::entity entity : simulation->registry.view<ComponentsToSelect...>()) {
      simulation->registry.emplace<Selection>(entity);
    }
  }

  ~SelectForView() { deselect(); }

  void deselect() {
    if (!simulation) {
      return;
    }

    simulation->registry.clear<Selection>();

    ENTT_ASSERT(!Selection::depth.empty(), "Selection depth cannot go negative.");
    Selection::depth.pop_back();

    for (const auto delegate : Selection::depth) {
      std::vector<types::entity> previouslySelected = delegate(simulation->registry);
      for (types::entity entity : previouslySelected) {
        if (simulation->registry.valid(entity)) {
          simulation->registry.get_or_emplace<Selection>(entity);
        }
      }
    }

    simulation = nullptr;
  }

  bool hasNoneSelected() const { return selectedCount == 0; }

 private:
  Simulation* simulation = nullptr;
  const std::size_t selectedCount = 0;
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