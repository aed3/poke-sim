#pragma once

#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

namespace pokesim::internal {
template <auto Function, typename... Tags>
struct RegistryLoop {
 public:
  template <class Signature>
  struct RegistryLoopInternal;

  template <class Signature, class... Args>
  struct RegistryLoopInternal<Signature (*)(Args...)> {
    template <typename... Selected, typename... ViewArgs>
    static void view(types::registry& registry, const ViewArgs&... viewArgs) {
      registry.view<Tags..., Selected..., std::decay_t<Args>...>(viewArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }

    template <typename... Selected, typename... GroupArgs>
    static void group(types::registry& registry, const GroupArgs&... groupArgs) {
      registry.group<Tags..., Selected..., std::decay_t<Args>...>(groupArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }
  };

  template <class Signature, class... Args>
  struct RegistryLoopInternal<Signature (*)(types::handle, Args...)> {
    template <typename... Selected, typename... ViewArgs>
    static void view(types::registry& registry, const ViewArgs&... viewArgs) {
      registry.view<Tags..., Selected..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }

    template <typename... Selected, typename... GroupArgs>
    static void group(types::registry& registry, const GroupArgs&... groupArgs) {
      registry.group<Tags..., Selected..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }
  };

  template <class Signature, class... Args>
  struct RegistryLoopInternal<Signature (*)(types::registry&, Args...)> {
    template <typename... Selected, typename... ViewArgs>
    static void view(types::registry& registry, const ViewArgs&... viewArgs) {
      registry.view<Tags..., Selected..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry](types::entity, auto&&... args) { Function(registry, args...); });
    }

    template <typename... Selected, typename... GroupArgs>
    static void group(types::registry& registry, const GroupArgs&... groupArgs) {
      registry.group<Tags..., Selected..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry](types::entity, auto&&... args) { Function(registry, args...); });
    }
  };

  using FunctionSig = std::decay_t<decltype(Function)>;

 public:
  template <typename... Selected, typename... ViewArgs>
  static void view(types::registry& registry, const ViewArgs&... viewArgs) {
    RegistryLoopInternal<FunctionSig>::template view<Selected...>(registry, viewArgs...);
  }

  template <typename... Selected, typename... GroupArgs>
  static void group(types::registry& registry, const GroupArgs&... groupArgs) {
    RegistryLoopInternal<FunctionSig>::template group<Selected...>(registry, groupArgs...);
  }
};
}  // namespace pokesim::internal