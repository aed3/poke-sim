#pragma once

#include <cstdint>
#include <vector>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    std::size_t start;
    std::size_t length;
  };

  std::vector<Span> spans;
};
}  // namespace pokesim