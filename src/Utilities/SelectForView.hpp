#pragma once

#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/Selection.hpp>
#include <Simulation/RegistryContainer.hpp>
#include <Types/Entity.hpp>
#include <cstdint>
#include <entt/container/dense_set.hpp>
#include <vector>

namespace pokesim::internal {
template <typename Selection, typename... ComponentsToSelect>
struct SelectForView {
  SelectForView(RegistryContainer& registryContainer_)
      : registryContainer(&registryContainer_),
        selectedCount(registryContainer->select<Selection, ComponentsToSelect...>()) {
    if (hasNoneSelected()) {
      registryContainer = nullptr;
    }
  }

  ~SelectForView() { deselect(); }

  void deselect() {
    if (registryContainer) {
      registryContainer->deselect<Selection>();
      registryContainer = nullptr;
    }
  }

  bool hasNoneSelected() const { return selectedCount == 0; }

 private:
  RegistryContainer* registryContainer = nullptr;
  const std::size_t selectedCount = 0;
};

template <typename... ComponentsToSelect>
struct SelectForBattleView : SelectForView<tags::SelectedForViewBattle, Sides, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForSideView : SelectForView<tags::SelectedForViewSide, Team, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForPokemonView : SelectForView<tags::SelectedForViewPokemon, SpeciesName, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForCurrentActionMoveView
    : SelectForView<tags::SelectedForViewMove, tags::CurrentActionMove, ComponentsToSelect...> {};
}  // namespace pokesim::internal