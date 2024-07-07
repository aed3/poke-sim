#pragma once

#include <Components/Tags/Selection.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/RegistryLoop.hpp>
#include <Utilities/Tags.hpp>
#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <type_traits>
#include <vector>

namespace pokesim::internal {
class RegistryContainer {
 private:
  template <typename, typename...>
  friend struct SelectForView;

  using SelectionFunctionList = std::vector<entt::delegate<std::vector<types::entity>(const types::registry&)>>;
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

  template <typename Selection, typename... ComponentsToSelect>
  std::size_t select() {
    auto group = registry.group(entt::get<ComponentsToSelect...>);
    if (group.empty()) {
      return 0U;
    }

    bool narrowSelection = hasActiveSelection<Selection>();
    std::size_t finalSelectionSize = 0U;

    if (narrowSelection) {
      auto unmatchedSelections = registry.group(entt::get<Selection>, entt::exclude<ComponentsToSelect...>);
      std::size_t totalSelected = registry.view<Selection>().size();
      std::size_t unmatchedSelectionSize = unmatchedSelections.size();
      if (unmatchedSelectionSize == totalSelected) {
        return 0U;
      }

      registry.remove<Selection>(unmatchedSelections.begin(), unmatchedSelections.end());

      ENTT_ASSERT(totalSelected > unmatchedSelectionSize, "");
      finalSelectionSize = totalSelected - unmatchedSelectionSize;
    }
    else {
      registry.clear<Selection>();
      registry.insert<Selection>(group.begin(), group.end());
      finalSelectionSize = group.size();
    }

    selectedFunctions<Selection>().emplace_back([](const void*, const types::registry& reg) {
      auto view = reg.view<ComponentsToSelect...>();
      return std::vector<types::entity>{view.begin(), view.end()};
    });

    return finalSelectionSize;
  }

  template <typename Selection>
  void deselect() {
    ENTT_ASSERT(hasActiveSelection<Selection>(), "Selections must be present to deselect.");

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
      end = std::remove_if(
        filteredEntityList.begin(),
        filteredEntityList.end(),
        [&previouslySelected](types::entity entity) {
          return std::find(previouslySelected.begin(), previouslySelected.end(), entity) == previouslySelected.end();
        });
    }

    registry.insert<Selection>(filteredEntityList.begin(), end);
  }

 protected:
  template <typename Selection>
  bool hasActiveSelection() {
    return !selectedFunctions<Selection>().empty();
  }

 private:
  template <typename Selected, auto Function, typename...>
  struct ForSelected;

  template <
    typename Selected, auto Function, typename ExcludeContainer, typename IncludeContainer, typename... ExtraTags>
  struct ForSelected<Selected, Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer> {
    template <typename... PassedInArgs>
    static void view(RegistryContainer* container, const PassedInArgs&... passedInArgs) {
      if (container->hasActiveSelection<Selected>()) {
        container->view<Function, Tags<Selected, ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
      else {
        container->view<Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
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
    ForSelected<tags::SelectedForViewBattle, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedBattles(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewBattle, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedSides(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewSide, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedSides(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewSide, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedPokemon(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewPokemon, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedPokemon(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewPokemon, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedMoves(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewMove, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedMoves(const PassedInArgs&... passedInArgs) {
    ForSelected<tags::SelectedForViewMove, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
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

  template <typename Type, typename... ViewComponents>
  void removeFromEntities() {
    auto view = registry.view<ViewComponents...>();
    registry.remove<Type>(view.begin(), view.end());
  }
};
}  // namespace pokesim::internal