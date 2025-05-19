#pragma once

#include <Config/Config.hpp>

#ifndef POKESIM_DEBUG_CHECK_UTILITIES
#include <memory>

#ifndef ENTT_ID_TYPE
#include <cstdint>
#define ENTT_ID_TYPE std::uint32_t
#endif

namespace entt {
enum class entity : ENTT_ID_TYPE;
template <typename, typename>
class basic_registry;
using registry = basic_registry<entity, std::allocator<entity> >;

template <typename, typename...>
struct basic_handle;
}  // namespace entt

namespace pokesim::types {
using registry = entt::registry;
using handle = entt::basic_handle<registry>;
}  // namespace pokesim::types
#else

#include <Types/Entity.hpp>
#include <Utilities/AssertComponentsEqual.hpp>
#include <entt/entity/registry.hpp>
#include <entt/meta/factory.hpp>
#include <type_traits>

namespace pokesim::types::internal {
using BackingRegistry = entt::registry;
template <typename Registry>
using BackingHandle = entt::basic_handle<Registry>;
}  // namespace pokesim::types::internal
#endif

namespace pokesim::types {
class registry : public internal::BackingRegistry {
 public:
  enum MetaFunctions : entt::id_type {
    COPY_TO_OTHER_REGISTRY,
    ENTITY_COMPONENTS_EQUAL,
  };

 private:
  using entt::registry::emplace;
  using entt::registry::emplace_or_replace;
  using entt::registry::get_or_emplace;
  using entt::registry::insert;

  template <typename Type>
  static void copyToOtherRegistry(
    const registry* srcReg, types::entity srcEntity, registry* dstReg, types::entity dstEntity) {
    if constexpr (std::is_empty_v<Type>) {
      dstReg->emplace<Type>(dstEntity);
    }
    else {
      const Type& value = srcReg->get<Type>(srcEntity);
      dstReg->emplace<Type>(dstEntity, value);
    }
  }

  template <typename Type>
  static void entityComponentsEqual(
    const registry* currReg, types::entity currEntity, const registry* initReg, types::entity initEntity) {
    if constexpr (!std::is_empty_v<Type>) {
      const Type& currValue = currReg->get<Type>(currEntity);
      const Type& initValue = initReg->get<Type>(initEntity);
      debug::AssertComponentsEqual<Type>::check(currValue, initValue, *currReg);
    }
  }

  template <typename Type>
  void createMetaFunctions() const {
    static_assert(std::is_class_v<Type>, "Only classes or structs should be added to an entity.");
    entt::meta<Type>()
      .template func<&registry::copyToOtherRegistry<Type> >(MetaFunctions::COPY_TO_OTHER_REGISTRY)
      .template func<&registry::entityComponentsEqual<Type> >(MetaFunctions::ENTITY_COMPONENTS_EQUAL);
  }

 public:
  template <typename Type, typename... Args>
  decltype(auto) emplace(const entity_type entt, Args&&... args) {
    createMetaFunctions<Type>();
    return entt::registry::emplace<Type>(entt, std::forward<Args>(args)...);
  }

  template <typename Type, typename... Args>
  decltype(auto) emplace_or_replace(const entity_type entt, Args&&... args) {
    createMetaFunctions<Type>();
    return entt::registry::emplace_or_replace<Type>(entt, std::forward<Args>(args)...);
  }

  template <typename Type, typename... Args>
  [[nodiscard]] decltype(auto) get_or_emplace(const entity_type entt, Args&&... args) {
    createMetaFunctions<Type>();
    return entt::registry::get_or_emplace<Type>(entt, std::forward<Args>(args)...);
  }

  template <typename Type, typename It>
  void insert(It first, It last, const Type& value = {}) {
    createMetaFunctions<Type>();
    entt::registry::insert<Type>(std::move(first), std::move(last), value);
  }

  template <
    typename Type, typename EIt, typename CIt,
    typename = std::enable_if_t<std::is_same_v<typename std::iterator_traits<CIt>::value_type, Type> > >
  void insert(EIt first, EIt last, CIt from) {
    createMetaFunctions<Type>();
    entt::registry::insert<Type>(first, last, from);
  }
};

using handle = internal::BackingHandle<registry>;
}  // namespace pokesim::types
