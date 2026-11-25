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

#ifdef POKESIM_TEST_CHECK_UTILITIES
namespace debug {
struct ParentEntity {
  types::entity val{};
};
}  // namespace debug
#endif
}  // namespace pokesim
