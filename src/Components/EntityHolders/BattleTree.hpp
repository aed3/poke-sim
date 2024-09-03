#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct ParentBattle {
  types::entity val{};
};

struct RootBattle {
  types::entity val{};
};

#ifndef NDEBUG
struct ParentEntity {
  types::entity val{};
};
#endif
}  // namespace pokesim