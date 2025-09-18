const path = require('path');
const {writeCppFile, noBase} = require('./utils');
const {getComponents, createForwardDeclaration} = require('./getComponents');

const {components, declarationsByNamespace} = getComponents();

const fileStart = `// NOLINTBEGIN
#if !defined(NDEBUG) && defined(POKESIM_ENTITY_VIEWER)
`;

const fileEnd = '#endif\n// NOLINTEND';

const headerText = (forwardDeclarations, getComponentDeclarations, getSizeDeclarations, getAllDeclarations) =>
  `${fileStart}
#pragma once

#include <Types/Entity.hpp>
#include <Types/Random.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/handle.hpp>
#include <vector>
#include <string>

${forwardDeclarations.join('\n')}

namespace pokesim::debug {
template <typename Registry>
class EntityViewerHandle;

class EntityViewerRegistry : public entt::registry {
  template <typename Registry>
  friend class EntityViewerHandle;
  std::vector<std::string_view> components(types::entity entity) const;
  std::vector<std::string_view> components(std::uint64_t entity) const {
    return components((types::entity)entity);
  };
  std::size_t count() const;
  ${getComponentDeclarations.join('')}
  ${
    getComponentDeclarations.map(declaration => declaration.replace('types::entity entity', 'std::uint64_t entity'))
      .join('')}
  ${getSizeDeclarations.join('')}
  ${getAllDeclarations.join('')}
};

template <typename Registry>
class EntityViewerHandle : public entt::basic_handle<Registry> {
 public:
  EntityViewerHandle() noexcept : entt::basic_handle<Registry>() { test(); };
  EntityViewerHandle(Registry& ref, pokesim::types::entity value) noexcept : entt::basic_handle<Registry>(ref, value) {
    test();
  }
  pokesim::types::entity entity() const noexcept {
    return entt::basic_handle<Registry>::entity();
  }
  Registry* registry() const noexcept {
    return entt::basic_handle<Registry>::registry();
  }

 private:
  void test() const;
  std::vector<std::string_view> components() const;
  ${getComponentDeclarations.map(decl => decl.replace('types::entity entity', '')).join('')}
  ${getSizeDeclarations.join('')}
  ${getAllDeclarations.join('')}
};
}
${fileEnd}`;


const implText = (getComponentDefinitions,
  getComponentFromNumDefinitions,
  getSizeDefinitions,
  getAllDefinitions,
  uses,
  handleAdapters) => `${fileStart}
#include "EntityViewer.hpp"

#include <Components/headers.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <vector>
#include <string>

namespace pokesim::debug {
std::vector<std::string_view> EntityViewerRegistry::components(types::entity entity) const {
  std::vector<std::string_view> list;
  for (auto [id, store] : storage()) {
    if (store.contains(entity)) {
      list.push_back(store.type().name());
    }
  }
  return list;
}

std::size_t EntityViewerRegistry::count() const {
  try {
    return view<types::entity>().size();
  }
  catch (...) {
    return 0;
  }
}

namespace {
template <typename T>
const T* get_type(types::entity entity, const EntityViewerRegistry* registry) {
  try {
    return registry->try_get<T>(entity);
  }
  catch (...) {
    return nullptr;
  }
}

template <typename T>
bool get_tag_type(types::entity entity, const EntityViewerRegistry* registry) {
  try {
    return registry->all_of<T>(entity);
  }
  catch (...) {
    return false;
  }
}

template <typename T>
std::size_t count_type(const EntityViewerRegistry* registry) {
  try {
    return registry->view<T>().size();
  }
  catch (...) {
    return 0;
  }
}

template <typename T>
std::vector<std::pair<entt::entity, T>> all(const EntityViewerRegistry* registry) {
  try {
    std::vector<std::pair<entt::entity, T>> ret{};
    for (const auto& pair : registry->view<T>().each()) {
      ret.emplace_back(std::get<0>(pair), std::get<1>(pair));
    }
    return ret;
  }
  catch (...) {
    return {};
  }
}

template <typename T>
std::vector<entt::entity> all_tag(const EntityViewerRegistry* registry) {
  try {
    return {registry->view<T>().begin(), registry->view<T>().end()};
  }
  catch (...) {
    return {};
  }
}
}

${getComponentDefinitions.join('')}
${getComponentFromNumDefinitions.join('')}
${getSizeDefinitions.join('')}
${getAllDefinitions.join('')}

template <typename Registry>
void EntityViewerHandle<Registry>::test() const {
  volatile int doNotOptimize = 0;
  if (doNotOptimize) {
    components();
    registry()->components(0);
    ${uses.join('\n')}
  }
}

template <typename Registry>
std::vector<std::string_view> EntityViewerHandle<Registry>::components() const {
  return registry()->components(entity());
}
${handleAdapters.join('\n')}
template class EntityViewerHandle<pokesim::types::registry>;
}
${fileEnd}`;

const isTag = (component) => component.includes('::tags::');
const toVariableName = (component) => noBase(component).replace(/::/g, '_').replace(/\<(\w+)\>/, '_$1');
const toFunctionName = (func, component) => func + '_' + toVariableName(component);
const getReturnType = (component) => isTag(component) ? 'bool' : `const ${component}*`;
const getAllReturnType = (component) =>
  isTag(component) ? 'std::vector<entt::entity>' : `std::vector<std::pair<entt::entity, ${component}>>`;

const makeSizeDecl = (component) => `std::size_t ${toFunctionName('count', component)}() const;`;

const makeGetComponentDecl = (component) =>
  `${getReturnType(component)} ${toFunctionName('get', component)}(types::entity entity) const;`;

const makeGetComponentFromNumDefinitions = (component) =>
  `${getReturnType(component)} EntityViewerRegistry::${toFunctionName('get', component)}(std::uint64_t entity) const {
  return ${toFunctionName('get', component)}((types::entity)entity);
}`;

const makeAllDecl = (component) => `${getAllReturnType(component)} ${toFunctionName('all', component)}() const;`;

const makeGetComponentDef = (component) => {
  if (isTag(component)) {
    return `${getReturnType(component)} EntityViewerRegistry::${
      toFunctionName('get', component)}(types::entity entity) const {
      return get_tag_type<${component}>(entity, this);
    }`;
  }
  else {
    return `${getReturnType(component)} EntityViewerRegistry::${
      toFunctionName('get', component)}(types::entity entity) const {
        return get_type<${component}>(entity, this);
      }`;
  }
};

const makeSizeDef = (component) => `
std::size_t EntityViewerRegistry::${toFunctionName('count', component)}() const {
  return count_type<${component}>(this);
}`;

const makeAllDef =
  (component) => {
    if (isTag(component)) {
      return `${getAllReturnType(component)} EntityViewerRegistry::${toFunctionName('all', component)}() const {
      return all_tag<${component}>(this);
    }`;
    }
    else {
      return `${getAllReturnType(component)} EntityViewerRegistry::${toFunctionName('all', component)}() const {
      return all<${component}>(this);
    }`;
    }
  }

const makeUses =
  (component) => { return ['get', 'count', 'all'].map(func => `${toFunctionName(func, component)}();`).join('\n'); };

const makeHandleAdapter = (component, declarationCreator, func) =>
  'template <typename Registry>\n' +
  declarationCreator(component)
    .replace('types::entity entity', '')
    .replace(toFunctionName(func, component), 'EntityViewerHandle<Registry>::' + toFunctionName(func, component))
    .replace(';', `{ return registry()->${toFunctionName(func, component)}(${func === 'get' ? 'entity()' : ''}); }`);

const createHeaderFile = () => {
  const forwardDeclarations = createForwardDeclaration(declarationsByNamespace, true);
  const getComponentDeclarations = components.map(makeGetComponentDecl);
  const getSizeDeclarations = components.map(makeSizeDecl);
  const getAllDeclarations = components.map(makeAllDecl);

  const header = headerText(forwardDeclarations, getComponentDeclarations, getSizeDeclarations, getAllDeclarations);
  writeCppFile('EntityViewer.hpp', path.join('src', 'Utilities'), header);
};


const createImplFile = () => {
  const getComponentDefinitions = components.map(makeGetComponentDef);
  const getComponentFromNumDefinitions = components.map(makeGetComponentFromNumDefinitions);
  const getSizeDefinitions = components.map(makeSizeDef);
  const getAllDefinitions = components.map(makeAllDef);
  const uses = components.map(makeUses);
  const handleAdapters = [[makeGetComponentDecl, 'get'], [makeSizeDecl, 'count'], [makeAllDecl, 'all']]
                           .map(args => components.map(component => makeHandleAdapter(component, ...args)))
                           .flat();

  const implementation = implText(getComponentDefinitions,
    getComponentFromNumDefinitions,
    getSizeDefinitions,
    getAllDefinitions,
    uses,
    handleAdapters);
  writeCppFile('EntityViewer.cpp', path.join('src', 'Utilities'), implementation);
};

createHeaderFile();
createImplFile();