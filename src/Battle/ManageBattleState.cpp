#include "ManageBattleState.hpp"

#include <Components/EntityHolders/Active.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void setActiveTarget(types::handle /*handle*/, ActiveAction /*activeAction*/, ActiveSource /*activeSource*/) {}
void setActiveMove(types::handle /*handle*/, ActiveAction /*activeAction*/) {}
void clearActive(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<ActiveAction>();
  registry.clear<ActiveMove>();
  registry.clear<ActiveTarget>();
  registry.clear<ActiveSource>();
}
}  // namespace pokesim