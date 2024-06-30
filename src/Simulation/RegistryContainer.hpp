#pragma once

#include <Components/Selection.hpp>
#include <Types/Entity.hpp>
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
    if constexpr (std::is_same_v<SelectedForViewBattle, Selection>) {
      return battleSelection;
    }
    else if constexpr (std::is_same_v<SelectedForViewSide, Selection>) {
      return sideSelection;
    }
    else if constexpr (std::is_same_v<SelectedForViewPokemon, Selection>) {
      return pokemonSelection;
    }
    else if constexpr (std::is_same_v<SelectedForViewMove, Selection>) {
      return moveSelection;
    }
  }

  template <typename Selection, typename... ComponentsToSelect>
  std::size_t select() {
    auto group = registry.group(entt::get<ComponentsToSelect...>);
    if (group.empty()) {
      return 0U;
    }

    selectedFunctions<Selection>().emplace_back([](const void*, const types::registry& reg) {
      auto view = reg.view<ComponentsToSelect...>();
      return std::vector<types::entity>{view.begin(), view.end()};
    });

    registry.clear<Selection>();
    registry.insert<Selection>(group.begin(), group.end());

    return group.size();
  }

  template <typename Selection>
  void deselect() {
    ENTT_ASSERT(hasActiveSelection<Selection>(), "Selections must be present to deselect.");

    registry.clear<Selection>();
    SelectionFunctionList& functions = selectedFunctions<Selection>();
    functions.pop_back();

    for (const auto delegate : functions) {
      std::vector<types::entity> previouslySelected = delegate(registry);
      for (types::entity entity : previouslySelected) {
        if (registry.valid(entity)) {
          registry.get_or_emplace<Selection>(entity);
        }
      }
    }
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
    ForSelected<SelectedForViewBattle, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedBattles(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewBattle, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedSides(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewSide, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedSides(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewSide, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedPokemon(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewPokemon, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedPokemon(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewPokemon, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void viewForSelectedMoves(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewMove, Function, TagContainer, ExcludeContainer, IncludeContainer>::view(
      this,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void groupForSelectedMoves(const PassedInArgs&... passedInArgs) {
    ForSelected<SelectedForViewMove, Function, TagContainer, ExcludeContainer, IncludeContainer>::group(
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