#include "MoveDexDataSetup.hpp"

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
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
void MoveDexDataSetup::setName(Move move) {
  handle.emplace<MoveName>(move);
}

void MoveDexDataSetup::setType(Type type) {
  handle.emplace<TypeName>(type);
}

void MoveDexDataSetup::setAccuracy(types::BaseAccuracy accuracy) {
  handle.emplace<Accuracy>(accuracy);
}

void MoveDexDataSetup::setBasePower(types::BasePower basePower) {
  handle.emplace<BasePower>(basePower);
}

void MoveDexDataSetup::setCategoryPhysical() {
  ENTT_ASSERT(!(handle.any_of<tags::move::Special, tags::move::Status>()), "A move can only have one category");
  handle.emplace<tags::move::Physical>();
}

void MoveDexDataSetup::setCategorySpecial() {
  ENTT_ASSERT(!(handle.any_of<tags::move::Physical, tags::move::Status>()), "A move can only have one category");
  handle.emplace<tags::move::Special>();
}

void MoveDexDataSetup::setCategoryStatus() {
  ENTT_ASSERT(!(handle.any_of<tags::move::Physical, tags::move::Special>()), "A move can only have one category");
  handle.emplace<tags::move::Status>();
}

void MoveDexDataSetup::setBasePp(types::Pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveDexDataSetup::setPriority(types::Priority priority) {
  handle.emplace<MovePriority>(priority);
}

void MoveDexDataSetup::setMultiHit(types::MoveHits minHits, types::MoveHits maxHits) {
  handle.emplace<MultiHit>(minHits, maxHits);
}

void MoveDexDataSetup::setPrimaryEffect(entt::entity entity) {
  handle.emplace<MoveEffect>(true, entity);
}

void MoveDexDataSetup::setSecondaryEffect(entt::entity entity) {
  handle.emplace<MoveEffect>(false, entity);
}

void MoveEffectSetup::setChance(types::BaseEffectChance chance) {
  handle.emplace<Chance>(chance);
}

void MoveEffectSetup::setEffectsSelf() {
  handle.emplace<tags::move::effect::MoveSource>();
}

void MoveEffectSetup::setEffectsTarget() {
  handle.emplace<tags::move::effect::MoveTarget>();
}
}  // namespace pokesim::dex::internal