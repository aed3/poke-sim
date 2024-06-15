#pragma once

#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

namespace pokesim {
class Simulation;

namespace internal {
template <auto Function, typename... Tags>
struct RegistryLoop {
 private:
  template <class Signature>
  struct RegistryLoopInternal;

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::handle, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(const Simulation&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([&simulation](types::entity, auto&&... args) {
        Function(simulation, args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([&simulation](types::entity, auto&&... args) {
        Function(simulation, args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::handle, const Simulation&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry, &simulation](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, simulation, args...);
        });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry, &simulation](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, simulation, args...);
        });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::registry&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([&registry](types::entity, auto&&... args) {
        Function(registry, args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([&registry](types::entity, auto&&... args) {
        Function(registry, args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::registry&, const Simulation&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry, &simulation](types::entity, auto&&... args) { Function(registry, simulation, args...); });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry, &simulation](types::entity, auto&&... args) { Function(registry, simulation, args...); });
    }
  };

  using FunctionSig = std::decay_t<decltype(Function)>;

 public:
  template <typename... ViewArgs>
  static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
    RegistryLoopInternal<FunctionSig>::view(registry, simulation, viewArgs...);
  }

  template <typename... GroupArgs>
  static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
    RegistryLoopInternal<FunctionSig>::group(registry, simulation, groupArgs...);
  }
};
}  // namespace internal
}  // namespace pokesim