#include "ManageBattleState.hpp"

#include <Components/EntityHolders/Active.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void setActiveTarget(types::handle handle, ActiveAction activeAction, ActiveSource activeSource) {}
void setActiveMove(types::handle handle, ActiveAction activeAction) {}

}  // namespace pokesim