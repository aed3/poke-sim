#pragma once

#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

namespace pokesim {
class Pokedex;

namespace internal {
template <auto Function, typename... Tags>
struct RegistryLoop {
 public:
  template <class Signature>
  struct RegistryLoopInternal;

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Pokedex&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Pokedex&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::handle, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Pokedex&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Pokedex&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(const Pokedex&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Pokedex& pokedex, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([&pokedex](types::entity, auto&&... args) {
        Function(pokedex, args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Pokedex& pokedex, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([&pokedex](types::entity, auto&&... args) {
        Function(pokedex, args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::handle, const Pokedex&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Pokedex& pokedex, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry, &pokedex](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, pokedex, args...);
        });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Pokedex& pokedex, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry, &pokedex](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, pokedex, args...);
        });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::registry&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Pokedex&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([&registry](types::entity, auto&&... args) {
        Function(registry, args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Pokedex&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([&registry](types::entity, auto&&... args) {
        Function(registry, args...);
      });
    }
  };

  using FunctionSig = std::decay_t<decltype(Function)>;

 public:
  template <typename... ViewArgs>
  static void view(types::registry& registry, const Pokedex& pokedex, const ViewArgs&... viewArgs) {
    RegistryLoopInternal<FunctionSig>::view(registry, pokedex, viewArgs...);
  }

  template <typename... GroupArgs>
  static void group(types::registry& registry, const Pokedex& pokedex, const GroupArgs&... groupArgs) {
    RegistryLoopInternal<FunctionSig>::group(registry, pokedex, groupArgs...);
  }
};
}  // namespace internal
}  // namespace pokesim