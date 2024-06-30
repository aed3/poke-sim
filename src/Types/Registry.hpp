#pragma once

#include <entt/container/fwd.hpp>
#include <entt/entity/fwd.hpp>

namespace pokesim::types {
using registry = entt::registry;
}  // namespace pokesim::types

namespace pokesim::types {
using handle = entt::basic_handle<registry>;
}  // namespace pokesim::types
