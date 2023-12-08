#pragma once
#include <Types/Enums/Ability.hpp>
#include <entt/entity/fwd.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::ability {
struct Defiant {};
struct Infiltrator {};
struct IronFist {};
struct Static {};
struct SweetVeil {};
struct Trace {};

// Assigns an ability's tag to a handle
/*_inline_*/ void enumToTag(dex::Ability ability, entt::handle handle);
}  // namespace pokesim::tags::ability
