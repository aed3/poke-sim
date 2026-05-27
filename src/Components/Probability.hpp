#pragma once

#include <Types/Constants.hpp>
#include <Types/Random.hpp>

namespace pokesim {
/**
 * @brief The probability of all the previous actions in a battle's simulation occurring.
 *
 * Calculated by multiplying the various Accuracy and Chance numbers of a battle state's events.
 */
struct Probability {
  types::probability val = Constants::Probability::DEFAULT;
};
}  // namespace pokesim
