#pragma once

#include <Config/Config.hpp>
#include <Types/Entity.hpp>

namespace pokesim {
struct ParentBattle {
  types::entity val{};
};

struct RootBattle {
  types::entity val{};
};

#ifdef POKESIM_DEBUG_CHECK_UTILITIES
struct ParentEntity {
  types::entity val{};
};
#endif
}  // namespace pokesim