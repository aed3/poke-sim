#pragma once
#include <Types/Ability.hpp>
#include <entt/entity/fwd.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::ability {
struct Defiant {};
struct Infiltrator {};
struct IronFist {};
struct Static {};
struct SweetVeil {};
struct Trace {};

/*_inline_*/ void enumToTag(dex::Ability ability, entt::handle handle);
}  // namespace pokesim::tags::ability
