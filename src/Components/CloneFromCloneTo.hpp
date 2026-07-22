#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
namespace tags {
struct CloneFrom {};
struct CloneToRemove {};
}  // namespace tags

struct CloneTo {
  types::entityIndex val{};
};
}  // namespace pokesim
