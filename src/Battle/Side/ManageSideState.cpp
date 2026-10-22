#include "ManageSideState.hpp"

#include <Pokedex/EnumToTag/SideConditionEnumToTag.hpp>
#include <Pokedex/Moves/Reflect.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/SideCondition.hpp>

namespace pokesim::internal {
namespace {
template <typename SideConditionType>
struct SetSideCondition {
  static void run(Simulation& simulation) { SideConditionType::onStart(simulation); }
};
}  // namespace

void setSideCondition(Simulation& simulation) {
  dex::forEachSideCondition<SetSideCondition>(simulation);
}
}  // namespace pokesim::internal
