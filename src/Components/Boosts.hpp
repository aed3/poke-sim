#pragma once

#include <cstdint>

namespace pokesim {
struct AtkBoost {
  std::int8_t boost = 0;
};

struct DefBoost {
  std::int8_t boost = 0;
};

struct SpaBoost {
  std::int8_t boost = 0;
};

struct SpdBoost {
  std::int8_t boost = 0;
};

struct SpeBoost {
  std::int8_t boost = 0;
};
}  // namespace pokesim