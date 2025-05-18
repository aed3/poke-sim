#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <cstddef>
#include <entt/container/dense_set.hpp>
#include <entt/container/fwd.hpp>
#include <entt/core/type_info.hpp>

namespace pokesim {
struct ParentEntity;
namespace types {
class registry;
}
namespace internal {
template <typename T, std::uint8_t N>
class maxSizedVector;
}
}  // namespace pokesim

namespace pokesim::debug {
struct TypesToIgnore : private entt::dense_set<entt::id_type> {
  TypesToIgnore() : entt::dense_set<entt::id_type>() { add<ParentEntity>(); }

  template <typename... Types>
  void add() {
    (emplace(entt::type_hash<Types>()), ...);
  }

  using entt::dense_set<entt::id_type>::contains;
};

types::entity createEntityCopy(types::entity entity, const types::registry& src, types::registry& dst);

void hasSameComponents(
  const types::registry& currReg, types::entity currEntity, const types::registry& initReg, types::entity initEntity,
  const TypesToIgnore& typesToIgnore = {});

void areEntitiesEqual(
  const types::registry& currReg, types::entity currEntity, const types::registry& initReg, types::entity initEntity,
  const TypesToIgnore& typesToIgnore = {});

types::entity findCopyParent(
  const entt::dense_map<types::entity, types::entity>& initialEntities, const types::registry& registry,
  types::entity entity);

bool checkIfCopyParent(types::entity current, types::entity initial, const types::registry& registry);

template <typename Type>
class AssertComponentsEqual {
  template <typename, typename = void>
  struct val : std::false_type {};
  template <typename T>
  struct val<T, std::void_t<decltype(T::val)>> : std::true_type {};

  template <typename, typename = void>
  struct name : std::false_type {};
  template <typename T>
  struct name<T, std::void_t<decltype(T::name)>> : std::true_type {};

  template <class, class = void>
  struct equals : std::false_type {};
  template <class T>
  struct equals<T, std::void_t<decltype(&T::operator==)>> : std::true_type {};

  template <typename T>
  using hasEqualTo = std::disjunction<equals<T>, std::is_scalar<T>>;

  template <typename>
  struct isList;
  template <typename T, std::uint8_t N>
  struct isList<internal::maxSizedVector<T, N>> {};
  template <typename... Args>
  struct isList<std::vector<Args...>> {};
  template <typename T, auto N>
  struct isList<std::array<T, N>> {};

  template <typename Member>
  static void compareMember(const Member& current, const Member& initial, const types::registry& registry) {
    if constexpr (std::is_same_v<types::entity, Member>) {
      if (current == initial) {
        return;
      }

      bool initialIsParent = checkIfCopyParent(current, initial, registry);
      bool currentIsParent = checkIfCopyParent(initial, current, registry);
      POKESIM_REQUIRE_NM((initialIsParent && !currentIsParent));
    }
    else if constexpr (entt::is_complete_v<isList<Member>>) {
      POKESIM_REQUIRE_NM(current.size() == initial.size());
      for (std::size_t i = 0; i < current.size(); i++) {
        compareMember(current[i], initial[i], registry);
      }
    }
    else if constexpr (hasEqualTo<Member>::value) {
      POKESIM_REQUIRE_NM(current == initial);
    }
    else {
      // Not a static_assert so this only fails on types that actually get copied
      POKESIM_REQUIRE_FAIL("There's a type that needs a dedicated equals function.");
    }
  }

 public:
  static void check(const Type& current, const Type& initial, const types::registry& registry) {
    if constexpr (hasEqualTo<Type>::value) {
      compareMember(current, initial, registry);
      return;
    }
    else if constexpr (val<Type>::value) {
      compareMember(current.val, initial.val, registry);
      if constexpr (sizeof(current.val) == sizeof(current)) {
        return;
      }
    }
    else if constexpr (name<Type>::value) {
      compareMember(current.name, initial.name, registry);
      if constexpr (sizeof(current.name) == sizeof(current)) {
        return;
      }
    }

    // Not a static_assert so this only fails on types that actually get copied
    POKESIM_REQUIRE_FAIL("This component needs a dedicated equals function.");
  }
};
}  // namespace pokesim::debug

#endif