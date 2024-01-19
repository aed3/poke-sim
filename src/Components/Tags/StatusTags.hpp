#pragma once
#include <Types/Entity.hpp>
#include <Types/Enums/Status.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::status::tags {
struct Burn {};
struct Freeze {};
struct Paralysis {};
struct Poison {};
struct Sleep {};
struct Toxic {};

// Assigns a status' tag to a handle
/*_inline_*/ void enumToTag(dex::Status status, types::handle& handle);
}  // namespace pokesim::status::tags
