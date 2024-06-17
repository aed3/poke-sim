#pragma once

#include <Types/Entity.hpp>
#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <vector>

// TODO (aed3): THIS WILL NOT BE OK WHEN RUNNING MULTIPLE SIMULATION OBJECTS ON DIFFERENT THREADS!
namespace pokesim {
struct SelectedForViewBattle {
  static inline std::vector<entt::delegate<std::vector<types::entity>(const types::registry&)>>
    depth{};  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewSide {
  static inline std::vector<entt::delegate<std::vector<types::entity>(const types::registry&)>>
    depth{};  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewPokemon {
  static inline std::vector<entt::delegate<std::vector<types::entity>(const types::registry&)>>
    depth{};  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewMove {
  static inline std::vector<entt::delegate<std::vector<types::entity>(const types::registry&)>>
    depth{};  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
}  // namespace pokesim
