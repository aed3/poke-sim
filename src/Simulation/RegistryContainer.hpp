#pragma once

#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/RegistryLoop.hpp>
#include <Utilities/Tags.hpp>
#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <iterator>
#include <type_traits>
#include <vector>

namespace pokesim::internal {
class RegistryContainer {
 public:
  using SelectionFunction = entt::delegate<std::vector<types::entity>(const types::registry&)>;

 private:
  template <typename, typename, typename...>
  friend struct SelectForView;

  using SelectionFunctionList = std::vector<SelectionFunction>;
  SelectionFunctionList battleSelection{};
  SelectionFunctionList sideSelection{};
  SelectionFunctionList pokemonSelection{};
  SelectionFunctionList moveSelection{};

  template <typename Selection>
  SelectionFunctionList& selectedFunctions() {
    if constexpr (std::is_same_v<tags::SelectedForViewBattle, Selection>) {
      return battleSelection;
    }
    else if constexpr (std::is_same_v<tags::SelectedForViewSide, Selection>) {
      return sideSelection;
    }
    else if constexpr (std::is_same_v<tags::SelectedForViewPokemon, Selection>) {
      return pokemonSelection;
    }
    else {
      return moveSelection;
    }
  }

  template <typename Selection>
  const SelectionFunctionList& selectedFunctions() const {
    if constexpr (std::is_same_v<tags::SelectedForViewBattle, Selection>) {
      return battleSelection;
    }
    else if constexpr (std::is_same_v<tags::SelectedForViewSide, Selection>) {
      return sideSelection;
    }
    else if constexpr (std::is_same_v<tags::SelectedForViewPokemon, Selection>) {
      return pokemonSelection;
    }
    else {
      return moveSelection;
    }
  }

  template <typename Selection, typename GetNewSelection, typename GetUnmatchedSelection>
  std::size_t select(
    GetNewSelection getNewSelection, GetUnmatchedSelection getUnmatchedSelection, SelectionFunction selectionFunction,
    bool isEmptySelection = false) {
    auto list = getNewSelection(registry);
    if (list.empty()) {
      return 0U;
    }

    bool narrowSelection = hasActiveSelection<Selection>();
    std::size_t finalSelectionSize = 0U;

    if (narrowSelection && isEmptySelection) {
      selectedFunctions<Selection>().push_back(selectionFunction);
      return registry.view<Selection>().size();
    }
    if (narrowSelection) {
      auto unmatchedSelections = getUnmatchedSelection(registry);
      std::size_t totalSelected = registry.view<Selection>().size();
      std::size_t unmatchedSelectionSize = unmatchedSelections.size();
      if (unmatchedSelectionSize == totalSelected) {
        return 0U;
      }

      registry.remove<Selection>(unmatchedSelections.begin(), unmatchedSelections.end());

      POKESIM_ASSERT(
        unmatchedSelectionSize < totalSelected,
        "The number of elements removed from the active selection must be less than the number of elements selected.");
      finalSelectionSize = totalSelected - unmatchedSelectionSize;
    }
    else {
      registry.clear<Selection>();
      registry.insert<Selection>(list.begin(), list.end());
      finalSelectionSize = list.size();
    }

    selectedFunctions<Selection>().push_back(selectionFunction);

    return finalSelectionSize;
  }

  template <typename Selection, typename Required, typename... ComponentsToSelect, typename... ComponentsToExclude>
  std::size_t select(entt::exclude_t<ComponentsToExclude...> exclude) {
    auto getNewSelection = [&exclude](types::registry& reg) {
      return reg.group<>(entt::get<Required, ComponentsToSelect...>, exclude);
    };
    auto getUnmatchedSelection = [](types::registry& reg) {
      return reg.group<>(entt::get<Selection, ComponentsToExclude...>, entt::exclude<ComponentsToSelect...>);
    };
    SelectionFunction selectionFunction{[](const void*, const types::registry& reg) {
      auto view = reg.view<Required, ComponentsToSelect...>(entt::exclude<ComponentsToExclude...>);
      return std::vector<types::entity>{view.begin(), view.end()};
    }};

    return select<Selection>(
      getNewSelection,
      getUnmatchedSelection,
      selectionFunction,
      sizeof...(ComponentsToSelect) == 0);
  }

  template <typename Selection>
  std::size_t select(SelectionFunction selectionFunction) {
    auto getUnmatchedSelections = [&selectionFunction](const types::registry& reg) -> std::vector<types::entity> {
      auto upcomingSelection = selectionFunction(reg);
      auto currentSelection = reg.view<Selection>();
      auto end =
        std::remove_if(upcomingSelection.begin(), upcomingSelection.end(), [&currentSelection](types::entity entity) {
          return !currentSelection.contains(entity);
        });
      return {upcomingSelection.begin(), end};
    };

    return select<Selection>(selectionFunction, getUnmatchedSelections, selectionFunction);
  }

  template <typename Selection>
  void deselect() {
    POKESIM_ASSERT(hasActiveSelection<Selection>(), "Selections must be present to deselect.");

    registry.clear<Selection>();
    SelectionFunctionList& functions = selectedFunctions<Selection>();
    functions.pop_back();

    if (functions.empty()) {
      return;
    }

    std::vector<types::entity> filteredEntityList = functions[0](registry);
    auto end = filteredEntityList.end();
    for (std::size_t i = 1; i < functions.size(); i++) {
      std::vector<types::entity> previouslySelected = functions[i](registry);
      end = std::remove_if(filteredEntityList.begin(), end, [&previouslySelected](types::entity entity) {
        return std::find(previouslySelected.begin(), previouslySelected.end(), entity) == previouslySelected.end();
      });
    }

    registry.insert<Selection>(filteredEntityList.begin(), end);
  }

 protected:
  template <typename Selection>
  bool hasActiveSelection() const {
    return !selectedFunctions<Selection>().empty();
  }

 private:
  template <typename Selected, typename Required, auto Function, typename...>
  struct ForSelected;

  template <
    typename Selected, typename Required, auto Function, typename ExcludeContainer, typename IncludeContainer,
    typename... ExtraTags>
  struct ForSelected<Selected, Required, Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer> {
    template <typename... PassedInArgs>
    static void view(RegistryContainer* container, const PassedInArgs&... passedInArgs) {
      if (container->hasActiveSelection<Selected>()) {
        container->view<Function, Tags<Selected, Required, ExtraTags...>, ExcludeContainer, IncludeContainer>(
          passedInArgs...);
      }
      else {
        container->view<Function, Tags<Required, ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
    }

    template <typename... PassedInArgs>
    static void group(RegistryContainer* container, const PassedInArgs&... passedInArgs) {
      if (container->hasActiveSelection<Selected>()) {
        container->view<Function, Tags<Selected, ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
      else {
        container->group<Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
    }
  };

 public:
  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedBattles(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewBattle, tags::Battle, Function, TagContainer, ExcludeContainer, IncludeContainer>::
      view(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedBattles(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewBattle, tags::Battle, Function, TagContainer, ExcludeContainer, IncludeContainer>::
      group(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedSides(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewSide, tags::Side, Function, TagContainer, ExcludeContainer, IncludeContainer>::
      view(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedSides(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewSide, tags::Side, Function, TagContainer, ExcludeContainer, IncludeContainer>::
      group(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedPokemon(const PassedInArgs&... passedInArgs) {
    ForSelected<
      tags::SelectedForViewPokemon,
      tags::Pokemon,
      Function,
      TagContainer,
      ExcludeContainer,
      IncludeContainer>::view(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedPokemon(const PassedInArgs&... passedInArgs) {
    ForSelected<
      tags::SelectedForViewPokemon,
      tags::Pokemon,
      Function,
      TagContainer,
      ExcludeContainer,
      IncludeContainer>::group(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedMoves(const PassedInArgs&... passedInArgs) {
    ForSelected<
      tags::SelectedForViewMove,
      tags::CurrentActionMove,
      Function,
      TagContainer,
      ExcludeContainer,
      IncludeContainer>::view(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedMoves(const PassedInArgs&... passedInArgs) {
    ForSelected<
      tags::SelectedForViewMove,
      tags::CurrentActionMove,
      Function,
      TagContainer,
      ExcludeContainer,
      IncludeContainer>::group(this, passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  auto view(const PassedInArgs&... passedInArgs) {
    return internal::RegistryLoop<Function, TagContainer, ExcludeContainer, IncludeContainer, PassedInArgs...>::view(
      registry,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  auto group(const PassedInArgs&... passedInArgs) {
    return internal::RegistryLoop<Function, TagContainer, ExcludeContainer, IncludeContainer, PassedInArgs...>::group(
      registry,
      passedInArgs...);
  }

 public:
  types::registry registry{};

  template <typename Type, typename... ViewComponents, typename... Args>
  void addToEntities(Args&&... args) {
    auto view = registry.view<ViewComponents...>();
    registry.insert<Type>(view.begin(), view.end(), args...);
  }

  template <typename Type, typename... ViewComponents, typename... ExcludeComponents>
  void removeFromEntities(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) {
    auto view = registry.view<Type, ViewComponents...>(exclude);
    registry.remove<Type>(view.begin(), view.end());
  }
};
}  // namespace pokesim::internal