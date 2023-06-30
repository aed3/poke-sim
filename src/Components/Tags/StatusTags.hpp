#pragma once
#include <Types/Status.hpp>
#include <entt/entity/fwd.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::status {
struct Burn {};
struct Freeze {};
struct Paralysis {};
struct Poison {};
struct Sleep {};
struct Toxic {};

/*_inline_*/ void enumToTag(dex::Status status, entt::handle& handle);
}  // namespace pokesim::tags::status
