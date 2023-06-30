#include "DexDataMoveSetup.hpp"

#include <Components/Accuracy.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/Chance.hpp>
#include <Components/EntityHolders/MoveEffect.hpp>
#include <Components/MultiHit.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/PP.hpp>
#include <Components/Priority.hpp>
#include <Components/Tags/MoveTags.hpp>

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
  ENTT_ASSERT(!(handle.any_of<tags::move::Special, tags::move::Status>()), "A move can only have one category");
  handle.emplace<tags::move::Physical>();
}

void MoveSetup::setCategorySpecial() {
  ENTT_ASSERT(!(handle.any_of<tags::move::Physical, tags::move::Status>()), "A move can only have one category");
  handle.emplace<tags::move::Special>();
}

void MoveSetup::setCategoryStatus() {
  ENTT_ASSERT(!(handle.any_of<tags::move::Physical, tags::move::Special>()), "A move can only have one category");
  handle.emplace<tags::move::Status>();
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
  handle.emplace<tags::move::effect::MoveSource>();
}

void MoveEffectSetup::setEffectsTarget() {
  handle.emplace<tags::move::effect::MoveTarget>();
}
}  // namespace pokesim::dex::internal