#include "BattleStateSetup.hpp"

#include <Components/ActionQueue.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/ID.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/RecycledEntities.hpp>
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
  setTurn(Constants::TurnCount::DEFAULT);
  setProbability(Constants::Probability::DEFAULT);
}

void BattleStateSetup::setRecycledAction(types::entity recycledAction, types::entity recycledActionMove) {
  types::registry& registry = *handle.registry();

  handle.emplace<RecycledAction>(recycledAction);
  registry.emplace<tags::RecycledAction>(recycledAction);

  handle.emplace<RecycledActionMove>(recycledActionMove);
  registry.emplace<tags::RecycledActionMove>(recycledActionMove);
}

void BattleStateSetup::setAddedRecycledActionMoves(
  types::entity addedRecycledActionMove1, types::entity addedRecycledActionMove2) {
  types::registry& registry = *handle.registry();

  handle.emplace<AddedRecycledActionMove1>(addedRecycledActionMove1);
  registry.emplace<tags::AddedRecycledActionMove1>(addedRecycledActionMove1);

  handle.emplace<AddedRecycledActionMove2>(addedRecycledActionMove2);
  registry.emplace<tags::AddedRecycledActionMove2>(addedRecycledActionMove2);
}

void BattleStateSetup::setAutoID() {
  setID((types::stateId)handle.registry()->view<Sides>().size());
}

void BattleStateSetup::setID(types::stateId id) {
  handle.emplace<Id>(id);
}

void BattleStateSetup::setSide(types::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  PlayerSideId sideId = handle.registry()->get<PlayerSide>(sideEntity).val;
  switch (sideId) {
    case PlayerSideId::P1: sides.val.p1() = sideEntity; break;
    case PlayerSideId::P2: sides.val.p2() = sideEntity; break;

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

void BattleStateSetup::setActionQueue(const std::vector<ActionQueueItem>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleStateSetup::setTurn(types::battleTurn turn) {
  handle.emplace<Turn>(turn);
}

void BattleStateSetup::setProbability(types::probability probability) {
  handle.emplace<Probability>(probability);
}
}  // namespace pokesim
