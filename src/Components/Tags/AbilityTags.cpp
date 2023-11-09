#include "AbilityTags.hpp"

#include <Types/Ability.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::ability {
void enumToTag(dex::Ability ability, entt::handle handle) {
  switch (ability) {
    case dex::DEFIANT: handle.emplace<Defiant>(); return;
    case dex::INFILTRATOR: handle.emplace<Infiltrator>(); return;
    case dex::IRON_FIST: handle.emplace<IronFist>(); return;
    case dex::STATIC: handle.emplace<Static>(); return;
    case dex::SWEET_VEIL: handle.emplace<SweetVeil>(); return;
    case dex::TRACE: handle.emplace<Trace>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for ability that does not exist");
    }
  }
}
}  // namespace pokesim::tags::ability
