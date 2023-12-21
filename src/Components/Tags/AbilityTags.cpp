#include "AbilityTags.hpp"

#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::ability {
void enumToTag(dex::Ability ability, types::handle handle) {
  switch (ability) {
    case dex::Ability::DEFIANT: handle.emplace<Defiant>(); return;
    case dex::Ability::INFILTRATOR: handle.emplace<Infiltrator>(); return;
    case dex::Ability::IRON_FIST: handle.emplace<IronFist>(); return;
    case dex::Ability::STATIC: handle.emplace<Static>(); return;
    case dex::Ability::SWEET_VEIL: handle.emplace<SweetVeil>(); return;
    case dex::Ability::TRACE: handle.emplace<Trace>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for ability that does not exist");
    }
  }
}
}  // namespace pokesim::tags::ability
