const fs = require('fs');
const path = require('path');
const {getFullPath} = require('./utils');

const componentFolder = getFullPath('src', 'Components');
const pokedexFolder = getFullPath('src', 'Pokedex');

/**@typedef {{declarations: string[][], subNamespaces: {[name: string]: Namespace}}} Namespace */

const getComponents = (includeTags = true) => {
  /**@type {string[]} */
  const components = [];
  /**@type {{[component: string]: {holdsEntity: boolean, hasMember: boolean}}} */
  const componentData = {};
  /**@type {Namespace} */
  const declarationsByNamespace = {declarations: [], subNamespaces: {}};

  const nextFiles = [
    componentFolder,
    ...['Abilities', 'Effects', 'Items', 'Moves', 'Species', 'Types'].map(folder => path.join(pokedexFolder, folder)),
  ];

  const addToDeclarationNamespace = (forwardDeclaration, namespace) => {
    const namespaces = namespace.split('::');
    let current = declarationsByNamespace;
    for (const namespace of namespaces) {
      if (!current.subNamespaces[namespace]) current.subNamespaces[namespace] = {declarations: [], subNamespaces: {}};
      current = current.subNamespaces[namespace];
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
    let isTag = false;
    let isDex = false;
    const namespaceByBrace = [];
    const structByBrace = [];

    const setNamespace = (namespaceParts) => {
      isTag = namespaceParts.some(part => part === 'tags');
      isDex = namespaceParts.some(part => part === 'dex');
      namespace = namespaceParts.join('::');
    };

    const addNewStruct = () => {
      let {structName, holdsEntity, lineNumber, hasMember, inherits} = structByBrace.pop();

      if (inherits) {
        const inheritedData = componentData[namespace + '::' + inherits] ?? componentData['pokesim::' + inherits];
        holdsEntity ||= inheritedData?.holdsEntity ?? true;
        hasMember ||= inheritedData?.hasMember ?? true;
      }

      let name = namespace + '::' + structName;
      const data = {holdsEntity, hasMember};
      if (!includeTags && !hasMember) {
        componentData[name] = data;
        return;
      }

      const forwardDeclaration = `struct ${structName};`;

      if (lines[lineNumber - 1].startsWith('template <types::eventPossibilities')) {
        for (let j = 2; j <= 5; j++) {
          name = namespace + '::' + structName + `<${j}U>`;
          componentData[name] = data;
          components.push(name);
        }
        addToDeclarationNamespace(['template <types::eventPossibilities>', forwardDeclaration], namespace);
      }
      else if (!lines[lineNumber - 1].startsWith('template ')) {
        components.push(name);
        componentData[name] = data;
        addToDeclarationNamespace([forwardDeclaration], namespace);
      }
    };

    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      const namespaceLine = /^namespace ([a-z_:]+) \{/.exec(line);
      if (namespaceLine) {
        const nameParts = namespaceLine[1].split('::');
        setNamespace(namespace.split('::').concat(nameParts).filter(n => n));
        namespaceByBrace.push({name: namespaceLine[1], braceCount, length: nameParts.length});
      }

      const structLine = /^struct (\w+) /.exec(line);
      if (structLine) {
        structByBrace.push({
          structName: structLine[1],
          lineNumber: i,
          holdsEntity: false,
          braceCount,
          hasMember: !(isTag || isDex),
          inherits: false,
        });
      }

      if (structByBrace.length) {
        const currentStruct = structByBrace.at(-1);
        const trimmedLine = line.trimStart();
        currentStruct.inherits ||= /^struct \w+ : (.+) \{/.exec(line)?.[1];
        currentStruct.holdsEntity ||= trimmedLine.startsWith('types::entity');
        if (isDex && !currentStruct.hasMember) {
          currentStruct.hasMember = !['using', 'static', 'constexpr'].some(start => trimmedLine.startsWith(start)) &&
                                    /^  [\w:]+ \w+( = |\{\})/.test(line);
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
            setNamespace(namespace.split('::').slice(0, -removedNamespace.length));
          }
          else if (braceCount === structByBrace.at(-1)?.braceCount) {
            addNewStruct();
          }
        }
      }
    }
  }

  return {components, componentData, declarationsByNamespace};
};

/**
 * @param {Namespace} declarationsByNamespace
 * @returns {string[]}
 */
const createForwardDeclaration = (declarationsByNamespace) => {
  const forwardDeclarations = [];

  /**
   * @param {string} name
   * @param {Namespace} namespace
   * @param {string[]} parentNamespaceLines
   */
  const addNamespace = (name, namespace, parentNamespaceLines) => {
    const namespaceLines = [];

    const {declarations, subNamespaces} = namespace;
    if (name) {
      namespaceLines.push(`namespace ${name} {`);
    }

    if (declarations) {
      namespaceLines.push(...declarations.flat());
    }

    Object.entries(subNamespaces)
      .forEach(subNamespace => addNamespace(subNamespace[0], subNamespace[1], namespaceLines));

    if (name) {
      namespaceLines.push(`}  // namespace ${name}`);
    }

    if (namespaceLines.length > 2) {
      parentNamespaceLines.push(...namespaceLines);
    }
  };

  addNamespace('', declarationsByNamespace, forwardDeclarations);

  return forwardDeclarations;
};

module.exports = {
  getComponents,
  createForwardDeclaration,
};
