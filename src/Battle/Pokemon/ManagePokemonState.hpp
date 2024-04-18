#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct Pp;

void deductPp(Pp& pp);
void setLastMoveUsed(types::handle handle);
}  // namespace pokesim