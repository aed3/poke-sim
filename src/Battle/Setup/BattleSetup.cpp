#include "BattleSetup.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/ID.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/Tags/Battle.hpp>
#include <Components/Tags/Pokemon.hpp>
#include <Components/Turn.hpp>

namespace pokesim {
void BattleSetup::initBlank() {
  setID(handle.registry()->view<Sides>().size());
  handle.emplace<Sides>();
  handle.emplace<ActionQueue>();
  setTurn(0);
  setProbability(1);
}

void BattleSetup::setID(std::uint16_t id) {
  handle.emplace<ID>(id);
}

void BattleSetup::setSide(Side::PlayerSideID sideID, entt::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  switch (sideID) {
    case Side::P1: sides.p1 = sideEntity; break;
    case Side::P2: sides.p2 = sideEntity; break;
  }
}

void BattleSetup::setRNGSeed(std::uint32_t seed) {
  handle.emplace<RNGSeed>(seed);
}

void BattleSetup::setActionQueue(const std::vector<entt::entity>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleSetup::setTurn(std::uint16_t turn) {
  handle.emplace<Turn>(turn);
}

void BattleSetup::setActiveMove(entt::entity activeMove) {
  handle.registry()->emplace<ActiveMove>(activeMove);
}

void BattleSetup::setActivePokemon(entt::entity activePokemon) {
  handle.registry()->emplace<ActivePokemon>(activePokemon);
}

void BattleSetup::setActiveTarget(entt::entity activeTarget) {
  handle.registry()->emplace<ActiveMoveTarget>(activeTarget);
}

void BattleSetup::setActiveSource(entt::entity activeSource) {
  handle.registry()->emplace<ActiveMoveSource>(activeSource);
}

void BattleSetup::setProbability(float probability) {
  handle.emplace<Probability>(probability);
}
}  // namespace pokesim