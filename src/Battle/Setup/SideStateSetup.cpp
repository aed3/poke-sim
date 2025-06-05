#include "SideStateSetup.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/State.hpp>
#include <cstddef>
#include <entt/entity/handle.hpp>

#include "PokemonStateSetup.hpp"

namespace pokesim {
SideStateSetup::SideStateSetup(types::registry& registry, types::entity entity, PlayerSideId playerSideId)
    : StateSetupBase(registry, entity) {
  handle.emplace<tags::Side>();
  handle.emplace<PlayerSide>(playerSideId);
}

void SideStateSetup::initBlank() {
  handle.emplace<Battle>();
  handle.emplace<Team>();
  handle.emplace<FoeSide>();
}

void SideStateSetup::setTeam(std::vector<PokemonStateSetup>& team) {
  Team& teamEntities = handle.emplace<Team>();
  Battle battle = handle.get<Battle>();
  POKESIM_REQUIRE(team.size() <= teamEntities.val.max_size(), "Cannot add more Pokemon to a team than MAX_TEAM_SIZE.");

  for (PokemonStateSetup& pokemonSetup : team) {
    teamEntities.val.push_back(pokemonSetup.entity());
    pokemonSetup.setPostion(teamEntities.val.size());
    pokemonSetup.setSide(entity());
    pokemonSetup.setBattle(battle.val);
  }
}

void SideStateSetup::setOpponent(types::entity entity) {
  handle.emplace<FoeSide>(entity);
}

void SideStateSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}

void SideStateSetup::setPlayerSide(PlayerSideId playerSideId) {
  handle.emplace<PlayerSide>(playerSideId);
}
}  // namespace pokesim