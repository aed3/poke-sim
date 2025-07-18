#pragma once

#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Simulation/RegistryContainer.hpp>
#include <Types/Entity.hpp>
#include <cstddef>
#include <cstdint>
#include <entt/container/dense_set.hpp>
#include <vector>

namespace pokesim::internal {
template <typename Selection, typename Required, typename... ComponentsToSelect>
struct SelectForView {
  template <typename... ComponentsToExclude>
  SelectForView(
    RegistryContainer& registryContainer_,
    entt::exclude_t<ComponentsToExclude...> exclude = entt::exclude<ComponentsToExclude...>)
      : registryContainer(&registryContainer_),
        constantSelectedCount(registryContainer->select<Selection, Required, ComponentsToSelect...>(exclude)) {
    if (hasNoneSelected()) {
      registryContainer = nullptr;
    }
  }

  SelectForView(RegistryContainer& registryContainer_, RegistryContainer::SelectionFunction selectionFunction)
      : registryContainer(&registryContainer_),
        constantSelectedCount(registryContainer->select<Selection>(selectionFunction)) {
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

  bool hasNoneSelected() const { return constantSelectedCount == 0U; }

 private:
  RegistryContainer* registryContainer = nullptr;
  std::size_t constantSelectedCount = 0U;
};

template <typename... ComponentsToSelect>
using SelectForBattleView = SelectForView<tags::SelectedForViewBattle, tags::Battle, ComponentsToSelect...>;
template <typename... ComponentsToSelect>
using SelectForSideView = SelectForView<tags::SelectedForViewSide, tags::Side, ComponentsToSelect...>;
template <typename... ComponentsToSelect>
using SelectForPokemonView = SelectForView<tags::SelectedForViewPokemon, tags::Pokemon, ComponentsToSelect...>;
template <typename... ComponentsToSelect>
using SelectForCurrentActionMoveView =
  SelectForView<tags::SelectedForViewMove, tags::CurrentActionMove, ComponentsToSelect...>;
}  // namespace pokesim::internal