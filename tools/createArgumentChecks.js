const fs = require('fs');
const {fullPath, noBase} = require('./utils');
const {getComponents, createForwardDeclaration} = require('./getComponents');

const {components, componentData, declarationsByNamespace} = getComponents(false);
const headerPath = fullPath('src/Utilities/ArgumentChecks.hpp');
const sourcePath = fullPath('src/Utilities/ArgumentChecks.cpp');
const existingHeader = fs.readFileSync(headerPath, 'utf8').split('\n');
const existingSource = fs.readFileSync(sourcePath, 'utf8').split('\n');

const emptyTemplate = 'template <>';

const getExisting = (existingLines) => {
  const existingData = {};
  let startLine = 0;
  let endLine = existingLines.length - 1;
  let current = [];
  const addToExisting = (i) => {
    if (!startLine) {
      startLine = i;
    }
    if (!current.length) return;
    const component = /void check\(const (\S+)&/.exec(current[1])[1];
    existingData[component] = current.join('\n').trim();
    current = [];
  };

  let lastBlank = 0;
  for (let i = 0; i < existingLines.length; i++) {
    const line = existingLines[i];
    if (startLine && line.startsWith('}  // namespace ')) {
      endLine = i;
      break;
    }

    if (!line) lastBlank = i;

    if (line === emptyTemplate) {
      addToExisting(lastBlank);
      current.push(line);
    }
    else if (line.startsWith('// ' + emptyTemplate)) {
      addToExisting(lastBlank);
      current.push('', line);
      addToExisting(lastBlank);
    }
    else if (current.length) {
      current.push(line);
    }
  }
  addToExisting();

  return {existingData, startLine, endLine};
};

const getNewLines = (existingLines, {existingData, startLine, endLine}, ending) => {
  const newCode = existingLines.slice(0, startLine);
  newCode.push('');

  const allComponentCode = [];
  for (const component of components) {
    const inPokesimNamespaceComponent = noBase(component);
    let componentCode = existingData[inPokesimNamespaceComponent];
    delete existingData[inPokesimNamespaceComponent];
    if (componentCode === null) continue;

    if (!componentCode) {
      componentCode = `${emptyTemplate}\nvoid check(const ${inPokesimNamespaceComponent}&`;
      if (componentData[component].holdsEntity) {
        componentCode += ', const types::registry&';
      }
      componentCode += ')' + ending;
    }

    allComponentCode.push(componentCode);
  }

  for (const code of Object.values(existingData)) {
    allComponentCode.push(code);
  }

  newCode.push(allComponentCode.join('\n\n'), ...existingLines.slice(endLine));
  return newCode;
};

const existingHeaderData = getExisting(existingHeader);
const existingSourceData = getExisting(existingSource);

for (const [component, code] of Object.entries(existingHeaderData.existingData)) {
  if (code.startsWith('//')) {
    existingSourceData.existingData[component] = null;
  }
}

const newHeader = getNewLines(existingHeader, existingHeaderData, ';');
const newSource = getNewLines(existingSource, existingSourceData, ' {}');

const forward = createForwardDeclaration(declarationsByNamespace);
const existingForwardDeclarationStart = newHeader.indexOf('namespace pokesim {');
const existingForwardDeclarationEnd = newHeader.indexOf('}  // namespace pokesim');
newHeader.splice(
  existingForwardDeclarationStart, existingForwardDeclarationEnd - existingForwardDeclarationStart + 1, ...forward);

fs.writeFileSync(headerPath, newHeader.join('\n'));
fs.writeFileSync(sourcePath, newSource.join('\n'));