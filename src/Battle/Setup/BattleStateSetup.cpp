#include "BattleStateSetup.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/ID.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Turn.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <chrono>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/view.hpp>

namespace pokesim {
BattleStateSetup::BattleStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {
  if (!handle.any_of<ActionQueue>()) {
    handle.emplace<ActionQueue>();
  }
}

void BattleStateSetup::initBlank() {
  handle.emplace<Sides>();
  handle.emplace<ActionQueue>();
  setAutoID();
  setTurn(0);
  setProbability(1);
}

void BattleStateSetup::setAutoID() {
  setID((types::stateId)handle.registry()->view<Sides>().size());
}

void BattleStateSetup::setID(types::stateId id) {
  handle.emplace<Id>(id);
}

void BattleStateSetup::setSide(PlayerSideId sideID, types::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  switch (sideID) {
    case PlayerSideId::P1: sides.p1 = sideEntity; break;
    case PlayerSideId::P2: sides.p2 = sideEntity; break;
    default: ENTT_FAIL("sideID must be assigned P1 or P2"); break;
  }
}

void BattleStateSetup::setRNGSeed(std::optional<types::stateRngSeed> seed) {
  handle.emplace<RngSeed>(
    seed.value_or((types::stateRngSeed)std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

void BattleStateSetup::setActionQueue(const std::vector<types::entity>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleStateSetup::setTurn(types::battleTurn turn) {
  handle.emplace<Turn>(turn);
}

void BattleStateSetup::setActiveMove(types::entity activeMove) {
  handle.registry()->emplace<tags::ActiveMove>(activeMove);
}

void BattleStateSetup::setActivePokemon(types::entity activePokemon) {
  handle.registry()->emplace<tags::ActivePokemon>(activePokemon);
}

void BattleStateSetup::setActiveTarget(types::entity activeTarget) {
  handle.registry()->emplace<tags::ActiveMoveTarget>(activeTarget);
}

void BattleStateSetup::setActiveUser(types::entity activeSource) {
  handle.registry()->emplace<tags::ActiveMoveUser>(activeSource);
}

void BattleStateSetup::setProbability(types::stateProbability probability) {
  handle.emplace<Probability>(probability);
}

std::vector<BattleStateSetup> BattleStateSetup::clone(std::optional<types::cloneIndex> cloneCount) {
  types::registry& registry = *handle.registry();

  handle.emplace<tags::CloneFrom>();
  const types::ClonedEntityMap entityMap = pokesim::clone(registry, cloneCount);

  const auto& clonedBattles = entityMap.at(handle.entity());
  std::vector<BattleStateSetup> clonedSetups;
  clonedSetups.reserve(clonedBattles.size());

  for (types::entity entity : clonedBattles) {
    clonedSetups.emplace_back(registry, entity);
  }

  return clonedSetups;
}
}  // namespace pokesim