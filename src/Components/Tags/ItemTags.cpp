#include "ItemTags.hpp"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::item {
void enumToTag(dex::Item item, entt::handle handle) {
  switch (item) {
    case dex::ASSAULT_VEST: handle.emplace<AssaultVest>(); return;
    case dex::BRIGHT_POWDER: handle.emplace<BrightPowder>(); return;
    case dex::CHOICE_SCARF: handle.emplace<ChoiceScarf>(); return;
    case dex::CHOICE_SPECS: handle.emplace<ChoiceSpecs>(); return;
    case dex::FOCUS_SASH: handle.emplace<FocusSash>(); return;
    case dex::LIFE_ORB: handle.emplace<LifeOrb>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for item that does not exist");
    }
  }
}
}  // namespace pokesim::tags::item
