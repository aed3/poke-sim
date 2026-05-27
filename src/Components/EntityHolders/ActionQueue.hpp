#pragma once

#include <Types/Constants.hpp>
#include <Types/Entity.hpp>
#include <Utilities/MaxSizedVector.hpp>

namespace pokesim {
// Contains the list of action entities queued up to be simulated for a battle's current turn.
struct ActionQueue {
  internal::maxSizedVector<types::entity, Constants::ActionQueueLength::MAX> val{};
};
}  // namespace pokesim
