#pragma once
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::ability::tags {
struct Defiant {};
struct Infiltrator {};
struct IronFist {};
struct Static {};
struct SweetVeil {};
struct Trace {};

// Assigns an ability's tag to a handle
void enumToTag(dex::Ability ability, types::handle handle);
}  // namespace pokesim::ability::tags
