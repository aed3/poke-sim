#include "SideStateSetup.hpp"

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/Team.hpp>

namespace pokesim {
void SideStateSetup::initBlank() {
  handle.emplace<Battle>();
  handle.emplace<Team>();
  handle.emplace<FoeSide>();
}

void SideStateSetup::setTeam(std::vector<PokemonStateSetup>& team) {
  Team& teamEntities = handle.emplace<Team>();
  teamEntities.team.reserve(team.size());
  Battle battle = handle.get<Battle>();

  for (std::size_t i = 0; i < team.size(); i++) {
    teamEntities.team.push_back(team[i].entity());
    team[i].setPostion((std::uint8_t)(i + 1));
    team[i].setSide(entity());
    team[i].setBattle(battle.battle);
  }
}

void SideStateSetup::setOpponent(entt::entity entity) {
  handle.emplace<FoeSide>(entity);
}

void SideStateSetup::setBattle(entt::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim