#include "MoveDexDataSetup.hpp"

#include <Components/Accuracy.hpp>
#include <Components/AddedTargets.hpp>
#include <Components/BasePower.hpp>
#include <Components/Boosts.hpp>
#include <Components/Chance.hpp>
#include <Components/EntityHolders/MoveEffect.hpp>
#include <Components/HitCount.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/PP.hpp>
#include <Components/Priority.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Config/Require.hpp>
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

void MoveDexDataSetup::addAddedTargets(AddedTargetOptions addedTargets) {
  AddedTargets& existingTargets = handle.get_or_emplace<AddedTargets>();
  existingTargets.val = static_cast<AddedTargetOptions>(
    static_cast<std::uint8_t>(existingTargets.val) | static_cast<std::uint8_t>(addedTargets));

  switch (addedTargets) {
    case AddedTargetOptions::TARGET_ALLY: {
      setProperty<move::added_targets::tags::TargetAlly>();
      break;
    }
    case AddedTargetOptions::USER_ALLY: {
      setProperty<move::added_targets::tags::UserAlly>();
      break;
    }
    case AddedTargetOptions::TARGET_SIDE: {
      setProperty<move::added_targets::tags::TargetSide>();
      break;
    }
    case AddedTargetOptions::USER_SIDE: {
      setProperty<move::added_targets::tags::UserSide>();
      break;
    }
    case AddedTargetOptions::FIELD: {
      setProperty<move::added_targets::tags::Field>();
      break;
    }
    default: break;
  }
}

void MoveDexDataSetup::setAccuracy(types::baseAccuracy accuracy) {
  handle.emplace<Accuracy>(accuracy);
}

void MoveDexDataSetup::setBasePower(types::basePower basePower) {
  handle.emplace<BasePower>(basePower);
}

void MoveDexDataSetup::setCategoryPhysical() {
  POKESIM_REQUIRE(!(handle.any_of<move::tags::Special, move::tags::Status>()), "A move can only have one category.");
  setProperty<move::tags::Physical>();
}

void MoveDexDataSetup::setCategorySpecial() {
  POKESIM_REQUIRE(!(handle.any_of<move::tags::Physical, move::tags::Status>()), "A move can only have one category.");
  setProperty<move::tags::Special>();
}

void MoveDexDataSetup::setCategoryStatus() {
  POKESIM_REQUIRE(!(handle.any_of<move::tags::Physical, move::tags::Special>()), "A move can only have one category.");
  setProperty<move::tags::Status>();
}

void MoveDexDataSetup::setBasePp(types::pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveDexDataSetup::setPriority(types::priority priority) {
  handle.emplace<MovePriority>(priority);
}

void MoveDexDataSetup::setHitCount(types::moveHits hitCount) {
  handle.emplace<HitCount>(hitCount);
}

void MoveDexDataSetup::setPrimaryEffect(types::entity entity) {
  handle.emplace<MoveEffect>(true, entity);
}

void MoveDexDataSetup::setSecondaryEffect(types::entity entity) {
  handle.emplace<MoveEffect>(false, entity);
}

void MoveEffectSetup::setChance(types::baseEffectChance chance) {
  handle.emplace<Chance>(chance);
}

void MoveEffectSetup::setEffectsSelf() {
  handle.emplace<move::tags::effect::MoveSource>();
}

void MoveEffectSetup::setEffectsTarget() {
  handle.emplace<move::tags::effect::MoveTarget>();
}
}  // namespace pokesim::dex::internal