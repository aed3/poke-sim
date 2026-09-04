#pragma once

#include <Components/EntityHolders/Current.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Registry.hpp>
#include <Utilities/Tags.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
void setupActionMoveBuild(
  types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
  types::entity actionMoveEntity, pokesim::dex::Move move, bool useExtended);

template <
  template <typename> typename RunStruct, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
  typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
void currentActionMovesAsSourceView(Simulation& simulation, const PassedInArgs&... passedInArgs) {
  simulation.view<RunStruct<CurrentActionMovesAsSource>::run, TagContainer, ExcludeContainer, IncludeContainer>(
    passedInArgs...);

  simulation.view<RunStruct<CurrentActionMovesAsSourceExtended>::run, TagContainer, ExcludeContainer, IncludeContainer>(
    passedInArgs...);
}

template <
  template <typename> typename RunStruct, typename TagContainer = Tags<>, typename ExcludeContainer = entt::exclude_t<>,
  typename IncludeContainer = entt::get_t<>, typename... PassedInArgs>
void currentActionMovesAsTargetView(Simulation& simulation, const PassedInArgs&... passedInArgs) {
  simulation.view<RunStruct<CurrentActionMovesAsTarget>::run, TagContainer, ExcludeContainer, IncludeContainer>(
    passedInArgs...);

  simulation.view<RunStruct<CurrentActionMovesAsTargetExtended>::run, TagContainer, ExcludeContainer, IncludeContainer>(
    passedInArgs...);
}
}  // namespace pokesim::internal
