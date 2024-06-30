#pragma once

#include <entt/container/fwd.hpp>
#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim::types {
template <typename T, typename... Other>
using view = entt::view<entt::get_t<const T, const Other...>>;

using entity = entt::entity;

using ClonedEntityMap = entt::dense_map<entity, std::vector<entity>>;
}  // namespace pokesim::types
