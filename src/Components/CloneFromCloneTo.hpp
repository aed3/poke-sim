#pragma once

#include <Types/State.hpp>

namespace pokesim {
namespace tags {
struct CloneFrom {};
}  // namespace tags

struct CloneTo {
  types::cloneIndex index;
};
}  // namespace pokesim