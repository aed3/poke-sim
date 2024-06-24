#pragma once

#include <Components/Selection.hpp>
#include <Types/Entity.hpp>
#include <Utilities/RegistryLoop.hpp>
#include <Utilities/Tags.hpp>
#include <vector>

namespace pokesim {
class RegistryContainer {
 private:
  template <typename Selected, auto Function, typename...>
  struct ForSelected;

  template <
    typename Selected, auto Function, typename ExcludeContainer, typename IncludeContainer, typename... ExtraTags>
  struct ForSelected<Selected, Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer> {
    template <typename... PassedInArgs>
    static void view(RegistryContainer* container, const PassedInArgs&... passedInArgs) {
      if (Selected::depth.empty()) {
        container->view<Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
      else {
        container->view<Function, Tags<Selected, ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
    }

    template <typename... PassedInArgs>
    static void group(RegistryContainer* container, const PassedInArgs&... passedInArgs) {
      if (Selected::depth.empty()) {
        container->group<Function, Tags<ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
      }
      else {
        container->view<Function, Tags<Selected, ExtraTags...>, ExcludeContainer, IncludeContainer>(passedInArgs...);
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
  void view(const PassedInArgs&... passedInArgs) {
    internal::RegistryLoop<Function, TagContainer, ExcludeContainer, IncludeContainer, PassedInArgs...>::view(
      registry,
      passedInArgs...);
  }

  template <
    auto Function, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
    typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
  void group(const PassedInArgs&... passedInArgs) {
    internal::RegistryLoop<Function, TagContainer, ExcludeContainer, IncludeContainer, PassedInArgs...>::group(
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
}  // namespace pokesim