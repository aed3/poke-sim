#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct SideDecision;

void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
}  // namespace pokesim