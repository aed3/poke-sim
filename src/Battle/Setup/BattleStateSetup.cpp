#include "BattleStateSetup.hpp"

#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/ID.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Turn.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Utilities/MaxSizedVector.hpp>
#include <Utilities/RNG.hpp>
#include <atomic>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/view.hpp>

namespace pokesim {
BattleStateSetup::BattleStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {
  if (!handle.any_of<ActionQueue, tags::Battle>()) {
    handle.emplace<ActionQueue>();
    handle.emplace<tags::Battle>();
  }
}

void BattleStateSetup::initBlank() {
  handle.emplace<Sides>();
  handle.emplace<ActionQueue>();
  setAutoID();
  setTurn(MechanicConstants::TurnCount::MIN);
  setProbability(MechanicConstants::Probability::MAX);
}

void BattleStateSetup::setAutoID() {
  setID((types::stateId)handle.registry()->view<Sides>().size());
}

void BattleStateSetup::setID(types::stateId id) {
  handle.emplace_or_replace<Id>(id);
}

void BattleStateSetup::setSide(types::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  PlayerSideId sideId = handle.registry()->get<PlayerSide>(sideEntity).val;
  switch (sideId) {
    case PlayerSideId::P1: sides.p1() = sideEntity; break;
    case PlayerSideId::P2: sides.p2() = sideEntity; break;

    default: POKESIM_REQUIRE_FAIL("sideID must be assigned P1 or P2."); break;
  }
}

void BattleStateSetup::setRNGSeed(std::optional<types::rngState> seed) {
  if (!seed.has_value()) {
    static std::atomic_uint64_t state = 1U;
    seed = state;

    types::rngState newState = state;
    internal::nextRandomValue(newState);
    state = newState;
  }
  handle.emplace<RngSeed>(seed.value());
}

void BattleStateSetup::setActionQueue(const std::vector<types::entity>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleStateSetup::setTurn(types::battleTurn turn) {
  handle.emplace<Turn>(turn);
}

void BattleStateSetup::setCurrentActionTarget(types::targets<types::entity> actionTargets) {
  handle.emplace<CurrentActionTargets>(actionTargets);
  for (types::entity entity : actionTargets) {
    handle.registry()->emplace<tags::CurrentActionMoveTarget>(entity);
  }
}

void BattleStateSetup::setCurrentActionSource(types::entity actionSource) {
  handle.emplace<CurrentActionSource>(actionSource);
  handle.registry()->emplace<tags::CurrentActionMoveSource>(actionSource);
}

void BattleStateSetup::setCurrentActionMove(types::entity actionMove) {
  handle.emplace<CurrentActionMoves>().val.push_back(actionMove);
  handle.registry()->emplace<tags::CurrentActionMove>(actionMove);
}

void BattleStateSetup::setProbability(types::probability probability) {
  handle.emplace<Probability>(probability);
}
}  // namespace pokesim