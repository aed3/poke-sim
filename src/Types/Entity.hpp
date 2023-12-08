#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim::types {
template <typename T, typename... Other>
using view = entt::view<entt::get_t<const T, const Other...>>;
}  // namespace pokesim::types