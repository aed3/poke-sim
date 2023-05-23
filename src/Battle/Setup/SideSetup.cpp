#include "SideSetup.hpp"

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Position.hpp>

namespace pokesim {
void SideSetup::initBlank() {
  handle.emplace<Battle>();
  handle.emplace<Team>();
  handle.emplace<FoeSide>();
}

void SideSetup::setTeam(const std::vector<entt::entity>& team) {
  handle.emplace<Team>(team);
}

void SideSetup::setOpponent(entt::entity entity) {
  handle.emplace<FoeSide>(entity);
}

void SideSetup::setBattle(entt::entity entity) {
  handle.emplace<Battle>(entity);
}

void SideSetup::addTeamMember(entt::entity entity) {
  Team& team = handle.get_or_emplace<Team>();
  team.team.push_back(entity);
  handle.registry()->emplace<Position>(entity, (std::uint8_t)team.team.size());
}
}  // namespace pokesim