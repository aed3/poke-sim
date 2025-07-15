#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon status condition name
enum class Status : std::uint8_t {
  NO_STATUS = 0U,
  BRN,
  FRZ,
  PAR,
  PSN,
  SLP,
  TOX,
  /*, FRB, DRO, */ STATUS_TOTAL,
};

static constexpr std::uint8_t TOTAL_STATUS_COUNT = (std::uint8_t)Status::STATUS_TOTAL - 1U;
}  // namespace pokesim::dex
