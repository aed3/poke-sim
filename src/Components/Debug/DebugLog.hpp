#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_TEST_CHECK_UTILITIES

#include <Components/ActionQueue.hpp>
#include <Components/CalcDamage/TemporaryMoveProperties.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/State.hpp>
#include <Types/Variant.hpp>

namespace pokesim::debug {
struct DebugMoveData {
  bool wasCrit = false;
  types::damage damage{};
  internal::calc_damage::DamageFormulaVariables damageFormulaVariables;

  types::stateId attacker{};
  types::stateId defender{};
  dex::Move move = dex::Move::NO_MOVE;

  bool operator==(const DebugMoveData& other) const {
    return wasCrit == other.wasCrit && move == other.move && damage == other.damage;
  }
};

struct DebugLogData {
  types::variant<DebugMoveData, ActionQueueItem> val{};

  bool operator==(const DebugLogData& other) const { return val == other.val; }
};

struct DebugLog {
  types::maxSizedVector<DebugLogData, Constants::MAX_ENTITIES> val{};
};
}  // namespace pokesim::debug

#endif
