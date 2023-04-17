#pragma once
#include <cstdint>

namespace pokesim {
namespace dex {
/** @brief Pokemon status condition name */
enum Status : std::uint8_t { NO_STATUS = 0, BRN, FRZ, PAR, PSN, SLP, TOX, /*, FRB, DRO, */ STATUS_TOTAL };
}  // namespace dex

struct StatusName {
  dex::Status name = dex::NO_STATUS;
};
}  // namespace pokesim