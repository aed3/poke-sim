#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace pokesim::internal {
template <std::uint64_t MaxValue>
using unsignedIntType = std::conditional_t<
  MaxValue <= std::numeric_limits<std::uint32_t>::max(),
  std::conditional_t<
    MaxValue <= std::numeric_limits<std::uint16_t>::max(),
    std::conditional_t<MaxValue <= std::numeric_limits<std::uint8_t>::max(), std::uint8_t, std::uint16_t>,
    std::uint32_t>,
  std::uint64_t>;

template <std::int64_t MaxValue, std::int64_t MinValue>
using signedIntType = std::conditional_t<
  MaxValue <= std::numeric_limits<std::int32_t>::max() && MinValue >= std::numeric_limits<std::int32_t>::min(),
  std::conditional_t<
    MaxValue <= std::numeric_limits<std::uint16_t>::max() && MinValue >= std::numeric_limits<std::int16_t>::min(),
    std::conditional_t<
      MaxValue <= std::numeric_limits<std::int8_t>::max() && MinValue >= std::numeric_limits<std::int8_t>::min(),
      std::int8_t, std::int16_t>,
    std::int32_t>,
  std::int64_t>;
}  // namespace pokesim::internal