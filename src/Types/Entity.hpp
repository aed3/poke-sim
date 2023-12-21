#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim::types {
using registry = entt::registry;
}  // namespace pokesim::types

namespace pokesim::types {
template <typename T, typename... Other>
using view = entt::view<entt::get_t<const T, const Other...> >;

using handle = entt::basic_handle<registry>;

using entity = entt::entity;
}  // namespace pokesim::types
