#include "BattleStateSetup.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/ID.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Turn.hpp>
#include <chrono>

namespace pokesim {
void BattleStateSetup::initBlank() {
  handle.emplace<Sides>();
  handle.emplace<ActionQueue>();
  setAutoID();
  setTurn(0);
  setProbability(1);
}

void BattleStateSetup::setAutoID() {
  setID((uint16_t)handle.registry()->view<Sides>().size());
}

void BattleStateSetup::setID(std::uint16_t id) {
  handle.emplace<ID>(id);
}

void BattleStateSetup::setSide(Side::PlayerSideID sideID, entt::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  switch (sideID) {
    case Side::P1: sides.p1 = sideEntity; break;
    case Side::P2: sides.p2 = sideEntity; break;
  }
}

void BattleStateSetup::setRNGSeed(std::uint32_t seed) {
  if (seed == 0) {
    seed = (std::uint32_t)std::chrono::high_resolution_clock::now().time_since_epoch().count();
  }

  handle.emplace<RNGSeed>(seed);
}

void BattleStateSetup::setActionQueue(const std::vector<entt::entity>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleStateSetup::setTurn(std::uint16_t turn) {
  handle.emplace<Turn>(turn);
}

void BattleStateSetup::setActiveMove(entt::entity activeMove) {
  handle.registry()->emplace<tags::ActiveMove>(activeMove);
}

void BattleStateSetup::setActivePokemon(entt::entity activePokemon) {
  handle.registry()->emplace<tags::ActivePokemon>(activePokemon);
}

void BattleStateSetup::setActiveTarget(entt::entity activeTarget) {
  handle.registry()->emplace<tags::ActiveMoveTarget>(activeTarget);
}

void BattleStateSetup::setActiveUser(entt::entity activeSource) {
  handle.registry()->emplace<tags::ActiveMoveUser>(activeSource);
}

void BattleStateSetup::setProbability(float probability) {
  handle.emplace<Probability>(probability);
}
}  // namespace pokesim