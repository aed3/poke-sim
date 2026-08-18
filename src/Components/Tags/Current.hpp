#pragma once

namespace pokesim::tags {
// Current Action Tag: The move action being processed by the simulator
struct CurrentActionMove {};
// Current Action Tag: The move action that was being processed but failed
struct FailedCurrentActionMove {};
// A move that actively hitting a target and is or will be processed by calcDamage
struct CurrentMoveHit {};
struct FailedCurrentMoveHit {};

// Current Action Tag: The move slot the current action's move was chosen and will deduct PP from
struct CurrentActionMoveSlot {};
struct CurrentActionTarget {};
struct CurrentActionSource {};
}  // namespace pokesim::tags
