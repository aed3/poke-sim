#pragma once

#include <Types/Entity.hpp>
#include <cstdint>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

#include "Tags.hpp"

namespace pokesim::internal {
template <auto Function, typename...>
struct RegistryLoop;

template <auto Function, typename... ExtraTags, typename... Exclude, typename... Include, typename... PassedInArgs>
struct RegistryLoop<
  Function, Tags<ExtraTags...>, entt::exclude_t<Exclude...>, entt::get_t<Include...>, PassedInArgs...> {
 private:
  template <class Signature>
  struct RegistryLoopInternal;
  template <bool, typename TupleFrom, typename TupleTo>
  struct ParameterShifter;

  static constexpr std::size_t excludeSize = sizeof...(Exclude);
  static constexpr std::size_t includeSize = sizeof...(Include);
  static constexpr std::size_t passedInArgsSize = sizeof...(PassedInArgs);

  template <typename TupleFromHead, typename... TupleFromTail, typename... TupleToTail>
  struct ParameterShifter<false, Tags<TupleFromHead, TupleFromTail...>, Tags<TupleToTail...>>
      : ParameterShifter<
          sizeof...(TupleFromTail) == passedInArgsSize, Tags<TupleFromTail...>, Tags<TupleToTail..., TupleFromHead>> {};

  template <typename... TupleFromTail, typename... RegistryArgs>
  struct ParameterShifter<true, Tags<TupleFromTail...>, Tags<RegistryArgs...>> {
   private:
    using FirstType = std::decay_t<std::tuple_element_t<0, std::tuple<RegistryArgs...>>>;

   public:
    static void view(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (std::is_same_v<FirstType, types::registry>) {
        viewWithRegistry<RegistryArgs...>(registry, passedInArgs...);
      }
      else if constexpr (std::is_same_v<FirstType, types::handle>) {
        viewWithHandle<RegistryArgs...>(registry, passedInArgs...);
      }
      else {
        basicView(registry, passedInArgs...);
      }
    }

    static void group(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (std::is_same_v<FirstType, types::registry>) {
        groupWithRegistry<RegistryArgs...>(registry, passedInArgs...);
      }
      else if constexpr (std::is_same_v<FirstType, types::handle>) {
        groupWithHandle<RegistryArgs...>(registry, passedInArgs...);
      }
      else {
        basicGroup(registry, passedInArgs...);
      }
    }

   private:
    template <typename... ViewArgs>
    static auto getView(types::registry& registry) {
      if constexpr (excludeSize) {
        return registry.view<ExtraTags..., std::decay_t<ViewArgs>...>(entt::exclude<Exclude...>);
      }
      else {
        return registry.view<ExtraTags..., std::decay_t<ViewArgs>...>();
      }
    }

    template <typename... GroupArgs>
    static auto getGroup(types::registry& registry) {
      if constexpr (excludeSize) {
        return registry.group<ExtraTags..., std::decay_t<GroupArgs>...>(
          entt::exclude<Exclude...>,
          entt::get<Include...>);
      }
      else if constexpr (includeSize) {
        return registry.group<ExtraTags..., std::decay_t<GroupArgs>...>(entt::get<Include...>);
      }
      else {
        return registry.group<ExtraTags..., std::decay_t<GroupArgs>...>();
      }
    }

    static void basicView(types::registry& registry, const PassedInArgs&... passedInArgs) {
      getView<RegistryArgs...>(registry).each(
        [&passedInArgs...](types::entity, auto&&... args) { Function(args..., passedInArgs...); });
    }

    template <typename, typename... Rest>
    static void viewWithHandle(types::registry& registry, const PassedInArgs&... passedInArgs) {
      getView<Rest...>(registry).each([&registry, &passedInArgs...](types::entity entity, auto&&... args) {
        Function(types::handle{registry, entity}, args..., passedInArgs...);
      });
    }

    template <typename, typename... Rest>
    static void viewWithRegistry(types::registry& registry, const PassedInArgs&... passedInArgs) {
      getView<Rest...>(registry).each(
        [&registry, &passedInArgs...](types::entity, auto&&... args) { Function(registry, args..., passedInArgs...); });
    }

    static void basicGroup(types::registry& registry, const PassedInArgs&... passedInArgs) {
      getGroup<RegistryArgs...>(registry).each(
        [&passedInArgs...](types::entity, auto&&... args) { Function(args..., passedInArgs...); });
    }

    template <typename, typename... Rest>
    static void groupWithHandle(types::registry& registry, const PassedInArgs&... passedInArgs) {
      getGroup<Rest...>(registry).each([&registry, &passedInArgs...](types::entity entity, auto&&... args) {
        Function(types::handle{registry, entity}, args..., passedInArgs...);
      });
    }

    template <typename, typename... Rest>
    static void groupWithRegistry(types::registry& registry, const PassedInArgs&... passedInArgs) {
      getGroup<Rest...>(registry).each(
        [&registry, &passedInArgs...](types::entity, auto&&... args) { Function(registry, args..., passedInArgs...); });
    }
  };

  template <class ReturnType, class... RegistryArgs>
  struct RegistryLoopInternal<ReturnType (*)(RegistryArgs...)> {
    using WithPassedArgs = ParameterShifter<false, Tags<RegistryArgs...>, Tags<>>;
    using NoPassedArgs = ParameterShifter<true, Tags<>, Tags<RegistryArgs...>>;

    static void view(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (passedInArgsSize) {
        WithPassedArgs::view(registry, passedInArgs...);
      }
      else {
        NoPassedArgs::view(registry, passedInArgs...);
      }
    }

    static void group(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (passedInArgsSize) {
        WithPassedArgs::group(registry, passedInArgs...);
      }
      else {
        NoPassedArgs::group(registry, passedInArgs...);
      }
    }
  };

  using FunctionSig = std::decay_t<decltype(Function)>;

 public:
  static void view(types::registry& registry, const PassedInArgs&... passedInArgs) {
    RegistryLoopInternal<FunctionSig>::view(registry, passedInArgs...);
  }

  static void group(types::registry& registry, const PassedInArgs&... passedInArgs) {
    RegistryLoopInternal<FunctionSig>::group(registry, passedInArgs...);
  }
};
}  // namespace pokesim::internal