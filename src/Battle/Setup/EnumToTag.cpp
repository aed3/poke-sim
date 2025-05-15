#include "EnumToTag.hpp"

#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/NatureTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim {
void ability::tags::enumToTag(dex::Ability ability, types::handle handle) {
  switch (ability) {
    case dex::Ability::DEFIANT: handle.emplace<Defiant>(); return;
    case dex::Ability::INFILTRATOR: handle.emplace<Infiltrator>(); return;
    case dex::Ability::IRON_FIST: handle.emplace<IronFist>(); return;
    case dex::Ability::STATIC: handle.emplace<Static>(); return;
    case dex::Ability::SWEET_VEIL: handle.emplace<SweetVeil>(); return;
    case dex::Ability::TRACE: handle.emplace<Trace>(); return;
    default: {
      POKESIM_REQUIRE(false, "Adding tag for ability that does not exist.");
    }
  }
}

void item::tags::enumToTag(dex::Item item, types::handle handle) {
  switch (item) {
    case dex::Item::ASSAULT_VEST: handle.emplace<AssaultVest>(); return;
    case dex::Item::BRIGHT_POWDER: handle.emplace<BrightPowder>(); return;
    case dex::Item::CHOICE_SCARF: handle.emplace<ChoiceScarf>(); return;
    case dex::Item::CHOICE_SPECS: handle.emplace<ChoiceSpecs>(); return;
    case dex::Item::FOCUS_SASH: handle.emplace<FocusSash>(); return;
    case dex::Item::LIFE_ORB: handle.emplace<LifeOrb>(); return;
    default: {
      POKESIM_REQUIRE(false, "Adding tag for item that does not exist.");
    }
  }
}

void nature::tags::enumToTag(dex::Nature nature, types::handle handle) {
  switch (nature) {
    case dex::Nature::ADAMANT: handle.emplace<Adamant>(); return;
    case dex::Nature::BASHFUL: handle.emplace<Bashful>(); return;
    case dex::Nature::BOLD: handle.emplace<Bold>(); return;
    case dex::Nature::BRAVE: handle.emplace<Brave>(); return;
    case dex::Nature::CALM: handle.emplace<Calm>(); return;
    case dex::Nature::CAREFUL: handle.emplace<Careful>(); return;
    case dex::Nature::DOCILE: handle.emplace<Docile>(); return;
    case dex::Nature::GENTLE: handle.emplace<Gentle>(); return;
    case dex::Nature::HARDY: handle.emplace<Hardy>(); return;
    case dex::Nature::HASTY: handle.emplace<Hasty>(); return;
    case dex::Nature::IMPISH: handle.emplace<Impish>(); return;
    case dex::Nature::JOLLY: handle.emplace<Jolly>(); return;
    case dex::Nature::LAX: handle.emplace<Lax>(); return;
    case dex::Nature::LONELY: handle.emplace<Lonely>(); return;
    case dex::Nature::MILD: handle.emplace<Mild>(); return;
    case dex::Nature::MODEST: handle.emplace<Modest>(); return;
    case dex::Nature::NAIVE: handle.emplace<Naive>(); return;
    case dex::Nature::NAUGHTY: handle.emplace<Naughty>(); return;
    case dex::Nature::QUIET: handle.emplace<Quiet>(); return;
    case dex::Nature::QUIRKY: handle.emplace<Quirky>(); return;
    case dex::Nature::RASH: handle.emplace<Rash>(); return;
    case dex::Nature::RELAXED: handle.emplace<Relaxed>(); return;
    case dex::Nature::SASSY: handle.emplace<Sassy>(); return;
    case dex::Nature::SERIOUS: handle.emplace<Serious>(); return;
    case dex::Nature::TIMID: handle.emplace<Timid>(); return;
    default: {
      POKESIM_REQUIRE(false, "Adding tag for nature that does not exist.");
    }
  }
}

void status::tags::enumToTag(dex::Status status, types::handle& handle) {
  switch (status) {
    case dex::Status::BRN: handle.emplace<Burn>(); return;
    case dex::Status::FRZ: handle.emplace<Freeze>(); return;
    case dex::Status::PAR: handle.emplace<Paralysis>(); return;
    case dex::Status::PSN: handle.emplace<Poison>(); return;
    case dex::Status::SLP: handle.emplace<Sleep>(); return;
    case dex::Status::TOX: handle.emplace<Toxic>(); return;
    default: {
      POKESIM_REQUIRE(false, "Adding tag for status that does not exist.");
    }
  }
}
}  // namespace pokesim
