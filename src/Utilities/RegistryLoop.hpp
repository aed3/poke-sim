#pragma once

#include <Types/Entity.hpp>
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

    template <typename... Args>
    static constexpr void argChecks() {
      static_assert(std::conjunction_v<std::is_class<std::decay_t<Args>>...>);
      static_assert(std::conjunction_v<std::is_copy_assignable<std::decay_t<Args>>...>);
      static_assert(sizeof...(Args) + sizeof...(ExtraTags) + sizeof...(Include) > 0);
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

    template <auto getList>
    static auto run(types::registry& registry, const PassedInArgs&... passedInArgs) {
      auto list = getList(registry);
      list.each([&registry, &passedInArgs...](types::entity entity, auto&&... args) {
        if constexpr (hasRegistryFirst) {
          Function(registry, args..., passedInArgs...);
        }
        else if constexpr (hasHandleFirst) {
          Function(types::handle{registry, entity}, args..., passedInArgs...);
        }
        else {
          Function(args..., passedInArgs...);
        }
      });
      return list;
    }

    template <typename, typename... Rest>
    static auto runViewNoFirstArg(types::registry& registry, const PassedInArgs&... passedInArgs) {
      return run<getView<Rest...>>(registry, passedInArgs...);
    }

    template <typename, typename... Rest>
    static auto runGroupNoFirstArg(types::registry& registry, const PassedInArgs&... passedInArgs) {
      return run<getGroup<Rest...>>(registry, passedInArgs...);
    }

    static constexpr void paramShiftCheck() {
      static_assert(sizeof...(TupleFromTail) == passedInArgsSize);
      static_assert(std::conjunction_v<std::is_same<std::decay_t<TupleFromTail>, std::decay_t<PassedInArgs>>...>);
    }

   public:
    static auto view(types::registry& registry, const PassedInArgs&... passedInArgs) {
      paramShiftCheck();
      if constexpr (hasRegistryFirst || hasHandleFirst) {
        return runViewNoFirstArg<RegistryArgs...>(registry, passedInArgs...);
      }
      else {
        return run<getView<RegistryArgs...>>(registry, passedInArgs...);
      }
    }

    static auto group(types::registry& registry, const PassedInArgs&... passedInArgs) {
      paramShiftCheck();
      if constexpr (hasRegistryFirst || hasHandleFirst) {
        return runGroupNoFirstArg<RegistryArgs...>(registry, passedInArgs...);
      }
      else {
        return run<getGroup<RegistryArgs...>>(registry, passedInArgs...);
      }
    }
  };

  template <typename... RegistryArgs>
  struct RegistryLoopInternal<void (*)(RegistryArgs...)> {
    using WithPassedArgs = ParameterShifter<false, Tags<RegistryArgs...>, Tags<>>;
    using NoPassedArgs = ParameterShifter<true, Tags<>, Tags<RegistryArgs...>>;

    static auto view(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (passedInArgsSize > 0) {
        return WithPassedArgs::view(registry, passedInArgs...);
      }
      else {
        return NoPassedArgs::view(registry, passedInArgs...);
      }
    }

    static auto group(types::registry& registry, const PassedInArgs&... passedInArgs) {
      if constexpr (passedInArgsSize > 0) {
        return WithPassedArgs::group(registry, passedInArgs...);
      }
      else {
        return NoPassedArgs::group(registry, passedInArgs...);
      }
    }
  };

  using FunctionSig = std::decay_t<decltype(Function)>;

 public:
  static auto view(types::registry& registry, const PassedInArgs&... passedInArgs) {
    return RegistryLoopInternal<FunctionSig>::view(registry, passedInArgs...);
  }

  static auto group(types::registry& registry, const PassedInArgs&... passedInArgs) {
    return RegistryLoopInternal<FunctionSig>::group(registry, passedInArgs...);
  }
};
}  // namespace pokesim::internal