#pragma once

namespace pokesim {
/**
 * @brief The probability of all the previous actions in a battle's simulation occurring.
 *
 * Calculated by multiplying the various Accuracy and Chance numbers of a battle state's events.
 */
struct Probability {
  float probability = 1;
};
}  // namespace pokesim