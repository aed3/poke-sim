#pragma once

#include <cstddef>
#include <cstdint>

namespace pokesim::dex {
// Pokemon status condition name
enum class Status : std::uint8_t {
  NO_STATUS = 0,
  BRN,
  FRZ,
  PAR,
  PSN,
  SLP,
  TOX,
  /*, FRB, DRO, */ STATUS_TOTAL,
};

static constexpr std::size_t TOTAL_STATUS_COUNT = (std::size_t)Status::STATUS_TOTAL;
}  // namespace pokesim::dex
