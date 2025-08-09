#pragma once

#include <Types/State.hpp>

namespace pokesim {
namespace tags {
struct CloneFrom {};
struct CloneToRemove {};
}  // namespace tags

struct CloneTo {
  types::entityIndex val{};
};
}  // namespace pokesim