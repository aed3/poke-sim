const fs = require('fs');
const path = require('path');
const {fullPath, tempDir} = require('./utils');
const child_process = require('child_process');

/**@type {string[]} */
const components = [];
const declarationsByNamespace = {};
const componentFolder = fullPath('src/Components');

const getComponents = () => {
  const nextFiles = [componentFolder];

  const addToDeclarationNamespace = (forwardDeclaration, namespace) => {
    const namespaces = namespace.split('::');
    let current = declarationsByNamespace;
    for (const namespace of namespaces) {
      if (!current[namespace]) current[namespace] = {declarations: []};
      current = current[namespace];
    }
    current.declarations.push(forwardDeclaration);
  };

  while (nextFiles.length) {
    const file = nextFiles.pop();
    if (fs.statSync(file).isDirectory()) {
      fs.readdirSync(file).reverse().forEach(dirFile => nextFiles.push(path.join(file, dirFile)));
      continue;
    }

    const lines = fs.readFileSync(file, 'utf8').split('\n');
    let braceCount = 0;
    let namespace = '';
    const namespaceByBrace = [];

    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      const namespaceLine = /^namespace ([a-z_:]+) \{/.exec(line);
      if (namespaceLine) {
        const nameParts = namespaceLine[1].split('::');
        namespace = namespace.split('::').concat(nameParts).filter(n => n).join('::');
        namespaceByBrace.push({name: namespaceLine[1], braceCount, length: nameParts.length});
      }
      const structLine = /^struct (\w+) /.exec(line);
      if (structLine) {
        const forwardDeclaration = `struct ${structLine[1]};`;

        if (lines[i - 1].startsWith('template <types::eventPossibilities')) {
          for (let j = 2; j <= 5; j++) {
            components.push(namespace + '::' + structLine[1] + `<${j}U>`);
          }
          addToDeclarationNamespace(['template <types::eventPossibilities>', forwardDeclaration], namespace);
        }
        else if (!lines[i - 1].startsWith('template ')) {
          components.push(namespace + '::' + structLine[1]);
          addToDeclarationNamespace([forwardDeclaration], namespace);
        }
      }

      for (const char of line) {
        if (char === '{') {
          braceCount++;
        }
        else if (char === '}') {
          braceCount--;
          if (braceCount === namespaceByBrace.at(-1)?.braceCount) {
            const removedNamespace = namespaceByBrace.pop();
            namespace = namespace.split('::').slice(0, -removedNamespace.length).join('::');
          }
        }
      }
    }
  }
};

const fileStart = `
// AUTO GENERATED FILE, DO NOT EDIT
// NOLINTBEGIN
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
const noBase = (component) => component.replace(/^pokesim::/, '');
const toVariableName = (component) => noBase(component).replace(/::/g, '_').replace(/\<(\w+)\>/, '_$1');
const toFunctionName = (func, component) => func + '_' + toVariableName(component);
const getReturnType = (component) => isTag(component) ? 'bool' : `const ${component}*`;
const getAllReturnType = (component) =>
  isTag(component) ? 'std::vector<entt::entity>' : `std::vector<std::pair<entt::entity, ${component}>>`;

const makeSizeDecl = (component) => `std::size_t ${toFunctionName('count', component)}() const;`;

const createForwardDeclaration = () => {
  const forwardDeclarations = [];
  const addNamespace = ([name, namespace]) => {
    const {declarations, ...subNamespaces} = namespace;
    if (name) {
      forwardDeclarations.push(`namespace ${name} {`);
    }

    if (declarations) {
      forwardDeclarations.push(...declarations.flat());
    }

    Object.entries(subNamespaces).forEach(addNamespace);
    if (name) {
      forwardDeclarations.push('}');
    }
  };

  addNamespace([, declarationsByNamespace]);

  return forwardDeclarations;
};

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
      try {
        return all_of<${component}>(entity); 
      }
      catch (...) {
        return false;
      }
    }`;
  }
  else {
    return `${getReturnType(component)} EntityViewerRegistry::${
      toFunctionName('get', component)}(types::entity entity) const {
        try {
          return try_get<${component}>(entity);
        }
        catch (...) {
          return nullptr;
        }
      }`;
  }
};

const makeSizeDef = (component) => `
std::size_t EntityViewerRegistry::${toFunctionName('count', component)}() const {
  try {
    return view<${component}>().size();
  }
  catch (...) {
    return 0;
  }
}`;

const makeAllDef =
  (component) => {
    if (isTag(component)) {
      return `${getAllReturnType(component)} EntityViewerRegistry::${toFunctionName('all', component)}() const {
      try {
        return {view<${component}>().begin(), view<${component}>().end()};
      }
      catch (...) {
        return {};
      }
    }`;
    }
    else {
      return `${getAllReturnType(component)} EntityViewerRegistry::${toFunctionName('all', component)}() const {
      try {
        ${getAllReturnType(component)} ret{};
        for (const auto& pair : view<${component}>().each()) {
          ret.emplace_back(std::get<0>(pair), std::get<1>(pair));
        }
        return ret;
      }
      catch (...) {
        return {};
      }
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

const writeFile = (extension, text) => {
  const filename = 'EntityViewer.' + extension;
  const entityViewerPath = fullPath('src/Utilities/' + filename);
  const tempFilPath = path.join(tempDir, filename);
  fs.writeFileSync(tempFilPath, text);

  child_process.execSync(
    `clang-format --style="file:${fullPath('.clang-format')}" -i ${tempFilPath}`, {cwd: fullPath()});
  if (fs.existsSync(entityViewerPath)) {
    if (fs.readFileSync(entityViewerPath, 'utf8') === fs.readFileSync(tempFilPath, 'utf8')) {
      return;
    }
  }

  console.log(`Made changes to entity viewer ${extension} file`);
  fs.copyFileSync(tempFilPath, entityViewerPath);
};

const createHeaderFile = () => {
  const forwardDeclarations = createForwardDeclaration();
  const getComponentDeclarations = components.map(makeGetComponentDecl);
  const getSizeDeclarations = components.map(makeSizeDecl);
  const getAllDeclarations = components.map(makeAllDecl);

  const header = headerText(forwardDeclarations, getComponentDeclarations, getSizeDeclarations, getAllDeclarations);
  writeFile('hpp', header);
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

  const header = implText(getComponentDefinitions,
    getComponentFromNumDefinitions,
    getSizeDefinitions,
    getAllDefinitions,
    uses,
    handleAdapters);
  writeFile('cpp', header);
};

getComponents();
createHeaderFile();
createImplFile();