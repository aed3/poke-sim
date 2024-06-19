#pragma once

#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>
#include <iostream>

#include "Tags.hpp"

namespace pokesim::internal {
template <auto Function, typename...>
struct RegistryLoop;

template <auto Function, typename... ExtraTags, typename... Exclude, typename... Include, typename... PassedInArgs>
struct RegistryLoop<
  Function, Tags<ExtraTags...>, entt::exclude_t<Exclude...>, entt::get_t<Include...>, PassedInArgs...> {
  static_assert(std::conjunction_v<std::is_empty<ExtraTags>...>);
  static_assert(std::conjunction_v<std::is_class<ExtraTags>...>);
  static_assert(std::conjunction_v<std::is_class<Exclude>...>);
  static_assert(std::conjunction_v<std::is_class<Include>...>);

 private:
  template <typename Signature>
  struct RegistryLoopInternal;
  template <bool, typename TupleFrom, typename TupleTo>
  struct ParameterShifter;

  static constexpr bool usesExclude = sizeof...(Exclude) > 0;
  static constexpr bool usesInclude = sizeof...(Include) > 0;
  static constexpr auto passedInArgsSize = sizeof...(PassedInArgs);

  template <typename TupleFromHead, typename... TupleFromTail, typename... TupleToTail>
  struct ParameterShifter<false, Tags<TupleFromHead, TupleFromTail...>, Tags<TupleToTail...>>
      : ParameterShifter<
          sizeof...(TupleFromTail) == passedInArgsSize, Tags<TupleFromTail...>, Tags<TupleToTail..., TupleFromHead>> {};

  template <typename... TupleFromTail, typename... RegistryArgs>
  struct ParameterShifter<true, Tags<TupleFromTail...>, Tags<RegistryArgs...>> {
   private:
    using FirstType = std::tuple_element_t<0, std::tuple<RegistryArgs...>>;
    static constexpr bool hasRegistryFirst = std::is_same_v<FirstType, types::registry&>;
    static constexpr bool hasHandleFirst = std::is_same_v<FirstType, types::handle>;

    static_assert(sizeof...(RegistryArgs) > 0);
    // If the first argument is a registry, it must be a non-constant reference
    static_assert(!std::is_same_v<std::decay_t<FirstType>, types::registry> || hasRegistryFirst);
    // If the first argument is a handle, it must be a non-constant value
    static_assert(!std::is_same_v<std::decay_t<FirstType>, types::handle> || hasHandleFirst);

    static constexpr void paramShiftCheck() {
      static_assert(sizeof...(TupleFromTail) == passedInArgsSize);
      static_assert(std::conjunction_v<std::is_same<std::decay_t<TupleFromTail>, std::decay_t<PassedInArgs>>...>);
    }

   public:
    static void view(types::registry& registry, const PassedInArgs&... passedInArgs) {
      paramShiftCheck();

      if constexpr (hasRegistryFirst) {
        viewWithRegistry<RegistryArgs...>(registry, passedInArgs...);
      }
      else if constexpr (hasHandleFirst) {
        viewWithHandle<RegistryArgs...>(registry, passedInArgs...);
      }
      else {
        basicView(registry, passedInArgs...);
      }
    }

    static void group(types::registry& registry, const PassedInArgs&... passedInArgs) {
      paramShiftCheck();

      if constexpr (hasRegistryFirst) {
        groupWithRegistry<RegistryArgs...>(registry, passedInArgs...);
      }
      else if constexpr (hasHandleFirst) {
        groupWithHandle<RegistryArgs...>(registry, passedInArgs...);
      }
      else {
        basicGroup(registry, passedInArgs...);
      }
    }

   private:
    template <typename... Args>
    static constexpr void argChecks() {
      static_assert(sizeof...(Args) > 0);
      static_assert(std::conjunction_v<std::is_class<std::decay_t<Args>>...>);
      static_assert(std::conjunction_v<std::is_copy_assignable<std::decay_t<Args>>...>);
    }

    template <typename... ViewArgs>
    static auto getView(types::registry& registry) {
      argChecks<ViewArgs...>();

      if constexpr (usesExclude) {
        return registry.view<ExtraTags..., std::decay_t<ViewArgs>..., Include...>(entt::exclude<Exclude...>);
      }
      else {
        return registry.view<ExtraTags..., std::decay_t<ViewArgs>..., Include...>();
      }
    }

    template <typename... GroupArgs>
    static auto getGroup(types::registry& registry) {
      argChecks<GroupArgs...>();

      if constexpr (usesExclude) {
        return registry.group<ExtraTags..., std::decay_t<GroupArgs>...>(
          entt::exclude<Exclude...>,
          entt::get<Include...>);
      }
      else if constexpr (usesInclude) {
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

  template <typename... RegistryArgs>
  struct RegistryLoopInternal<void (*)(RegistryArgs...)> {
    using WithPassedArgs = ParameterShifter<false, Tags<RegistryArgs...>, Tags<>>;
    using NoPassedArgs = ParameterShifter<true, Tags<>, Tags<RegistryArgs...>>;

    static void view(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (passedInArgsSize > 0) {
        WithPassedArgs::view(registry, passedInArgs...);
      }
      else {
        NoPassedArgs::view(registry, passedInArgs...);
      }
    }

    static void group(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (passedInArgsSize > 0) {
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