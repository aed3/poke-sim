#pragma once

#include <Types/Constants.hpp>
#include <Types/MaxSizedVector.hpp>
#include <entt/container/fwd.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/storage.hpp>
#include <type_traits>

namespace pokesim::types {
template <typename T, typename... Other>
using view = entt::view<entt::get_t<const T, const Other...>>;

using entity = entt::entity;
using entityVector = maxSizedVector<entity, Constants::MAX_ENTITIES>;
using entityIndex = std::underlying_type_t<entity>;

using ClonedEntityMap = entt::dense_map<entity, entityVector>;
}  // namespace pokesim::types
