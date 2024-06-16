#pragma once

#include <cstdint>
#include <vector>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    std::size_t start = 0;
    std::size_t length = 0;
  };

  std::vector<Span> spans{};
};
}  // namespace pokesim