#pragma once

#include <Types/Random.hpp>

namespace pokesim {
namespace tags {
struct RandomEventCheckPassed {};
struct RandomEventCheckFailed {};

struct RandomEventA {};
struct RandomEventB {};
struct RandomEventC {};
struct RandomEventD {};
struct RandomEventE {};
}  // namespace tags

// Indicates which of equally likely events was chosen randomly
struct RandomEventIndex {
  types::eventPossibilities val = 0U;
};
}  // namespace pokesim