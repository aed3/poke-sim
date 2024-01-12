#include "ItemTags.hpp"

#include <Types/Entity.hpp>
#include <Types/Enums/Item.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::item::tags {
void enumToTag(dex::Item item, types::handle handle) {
  switch (item) {
    case dex::Item::ASSAULT_VEST: handle.emplace<AssaultVest>(); return;
    case dex::Item::BRIGHT_POWDER: handle.emplace<BrightPowder>(); return;
    case dex::Item::CHOICE_SCARF: handle.emplace<ChoiceScarf>(); return;
    case dex::Item::CHOICE_SPECS: handle.emplace<ChoiceSpecs>(); return;
    case dex::Item::FOCUS_SASH: handle.emplace<FocusSash>(); return;
    case dex::Item::LIFE_ORB: handle.emplace<LifeOrb>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for item that does not exist");
    }
  }
}
}  // namespace pokesim::item::tags
