#pragma once

#include <Types/Entity.hpp>
#include <Types/MechanicConstants.hpp>
#include <Utilities/MaxSizedVector.hpp>
#include <vector>

namespace pokesim {
// Contains the list of action entities queued up to be simulated for a battle's current turn.
struct ActionQueue {
  internal::maxSizedVector<types::entity, MechanicConstants::MAX_ACTION_QUEUE_LENGTH> val{};
};
}  // namespace pokesim