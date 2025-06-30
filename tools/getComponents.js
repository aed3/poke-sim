const fs = require('fs');
const path = require('path');
const {fullPath} = require('./utils');

const componentFolder = fullPath('src/Components');

/**@typedef {{declarations: string[][], subNamespaces: {[name: string]: Namespace}}} Namespace */

const getComponents = (includeTags = true) => {
  /**@type {string[]} */
  const components = [];
  /**@type {{[component: string]: {holdsEntity: boolean}}} */
  const componentData = {};
  /**@type {Namespace} */
  const declarationsByNamespace = {declarations: [], subNamespaces: {}};

  const nextFiles = [componentFolder];

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
    let data = {holdsEntity: false};
    let inTagNamespace = false;
    const namespaceByBrace = [];

    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      const namespaceLine = /^namespace ([a-z_:]+) \{/.exec(line);
      if (namespaceLine) {
        const nameParts = namespaceLine[1].split('::');
        const namespaceParts = namespace.split('::').concat(nameParts).filter(n => n);
        inTagNamespace = namespaceParts.some(part => part === 'tags');
        namespace = namespaceParts.join('::');
        namespaceByBrace.push({name: namespaceLine[1], braceCount, length: nameParts.length});
      }

      data.holdsEntity ||= line.startsWith('  types::entity');
      const structLine = /^struct (\w+) /.exec(line);
      if (structLine && !(!includeTags && inTagNamespace)) {
        const forwardDeclaration = `struct ${structLine[1]};`;
        data = {holdsEntity: false};

        if (lines[i - 1].startsWith('template <types::eventPossibilities')) {
          for (let j = 2; j <= 5; j++) {
            const name = namespace + '::' + structLine[1] + `<${j}U>`;
            componentData[name] = data;
            components.push(name);
          }
          addToDeclarationNamespace(['template <types::eventPossibilities>', forwardDeclaration], namespace);
        }
        else if (!lines[i - 1].startsWith('template ')) {
          const name = namespace + '::' + structLine[1];
          components.push(name);
          componentData[name] = data;
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
            const namespaceParts = namespace.split('::').slice(0, -removedNamespace.length);
            inTagNamespace = namespaceParts.some(part => part === 'tags');
            namespace = namespaceParts.join('::');
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