#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon status condition name
enum class Status : std::uint8_t { NO_STATUS = 0, BRN, FRZ, PAR, PSN, SLP, TOX, /*, FRB, DRO, */ STATUS_TOTAL };
}  // namespace pokesim::dex
