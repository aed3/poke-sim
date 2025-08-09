#pragma once

#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <optional>

namespace pokesim {
struct CloneTo;

types::ClonedEntityMap clone(types::registry& registry, std::optional<types::entityIndex> cloneCount);
void deleteClones(types::registry& registry);
}  // namespace pokesim