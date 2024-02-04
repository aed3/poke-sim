#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct ActionQueue;
void speedSort(types::handle handle, ActionQueue& actionQueue);
}  // namespace pokesim