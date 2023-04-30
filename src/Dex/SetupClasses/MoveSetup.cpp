#include "MoveSetup.hpp"

#include <Components/Accuracy.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/Chance.hpp>
#include <Components/EntityHolders/MoveEffect.hpp>
#include <Components/MultiHit.hpp>
#include <Components/PP.hpp>
#include <Components/Priority.hpp>
#include <Components/Tags/Move.hpp>

namespace pokesim::dex::internal {
void MoveSetup::setName(Move move) {
  handle.emplace<MoveName>(move);
}

void MoveSetup::setType(Type type) {
  handle.emplace<TypeName>(type);
}

void MoveSetup::setAccuracy(std::uint8_t accuracy) {
  handle.emplace<Accuracy>(accuracy);
}

void MoveSetup::setBasePower(std::uint8_t basePower) {
  handle.emplace<BasePower>(basePower);
}

void MoveSetup::setCategoryPhysical() {
  ENTT_ASSERT(!(handle.any_of<move::Special, move::Status>()), "A move can only have one category");
  handle.emplace<move::Physical>();
}

void MoveSetup::setCategorySpecial() {
  ENTT_ASSERT(!(handle.any_of<move::Physical, move::Status>()), "A move can only have one category");
  handle.emplace<move::Special>();
}

void MoveSetup::setCategoryStatus() {
  ENTT_ASSERT(!(handle.any_of<move::Physical, move::Special>()), "A move can only have one category");
  handle.emplace<move::Status>();
}

void MoveSetup::setBasePP(std::uint8_t pp) {
  handle.emplace<PP>(pp);
}

void MoveSetup::setPriority(std::uint8_t priority) {
  handle.emplace<Priority>(priority);
}

void MoveSetup::setMultiHit(std::uint8_t minHits, std::uint8_t maxHits) {
  handle.emplace<MultiHit>(minHits, maxHits);
}

void MoveSetup::setPrimaryEffect(entt::entity entity) {
  handle.emplace<MoveEffect>(true, entity);
}

void MoveSetup::setSecondaryEffect(entt::entity entity) {
  handle.emplace<MoveEffect>(false, entity);
}

void MoveEffectSetup::setChance(std::uint8_t chance) {
  handle.emplace<Chance>(chance);
}

void MoveEffectSetup::setEffectsSelf() {
  handle.emplace<move::effect::MoveSource>();
}

void MoveEffectSetup::setEffectsTarget() {
  handle.emplace<move::effect::MoveTarget>();
}

template <typename BoostType>
void MoveEffectSetup::setBoost(std::int8_t boost) {
  static_assert(
    std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() || std::is_same<SpaBoost, BoostType>() ||
    std::is_same<SpdBoost, BoostType>() || std::is_same<SpeBoost, BoostType>());
  handle.emplace<BoostType>(boost);
}
}  // namespace pokesim::dex::internal