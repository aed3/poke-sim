const fs = require('fs');
const path = require('path');

process.chdir(path.join(__dirname, '..'));

const srcFolder = 'src';
const externalFolder = 'external';
const testLibraryFolder = path.join(externalFolder, 'Catch2');
const mainHeader = path.join('src', 'PokeSim.hpp');

const includeRegex = /^\s*#include\s+["<]([^"<>]+)([">])/;
const pragmaOnceRegex = /^\s*#pragma\s+once/;
const inlineRegex = /\/\*\s*__inline__\s*\*\//g;

const fileText = {};
const dependencies = {};
const dependencyLines = {};
const dependencyOrder = [];

const singleFileHeader = ['#pragma once'];

const readSrcFiles = () => {
  const nextFiles = [srcFolder, externalFolder];

  while (nextFiles.length) {
    const file = nextFiles.pop();
    if (fs.statSync(file).isDirectory() && file !== testLibraryFolder) {
      fs.readdirSync(file).forEach(dirFile => nextFiles.push(path.join(file, dirFile)));
    }
    else {
      const ext = path.extname(file);
      if (['.cpp', '.hpp', '.h'].includes(ext)) {
        fileText[file] = fs.readFileSync(file, 'utf8').split('\n');
      }
    }
  }
};

const findDependencies = () => {
  for (const [file, lines] of Object.entries(fileText)) {
    const fileDependencies = dependencies[file] = [];
    const fileDependencyLines = dependencyLines[file] = new Set();
    const fileDirName = path.dirname(file);

    for (let i = 0; i < lines.length; i++) {
      const include = includeRegex.exec(lines[i]);
      if (!include) continue;
      const [, includePath, includeType] = include;
      let dependency = '';

      const srcFilePath = path.join(srcFolder, includePath);
      const extFilePath = path.join(externalFolder, includePath);
      if (includeType === '"') {
        dependency = path.normalize(path.join(fileDirName, includePath));
      }
      else if (fs.existsSync(srcFilePath)) {
        dependency = srcFilePath;
      }
      else if (fs.existsSync(extFilePath)) {
        dependency = extFilePath;
      }

      if (dependency) {
        fileDependencies.push(dependency);
        fileDependencyLines.add(i);
      }
    }
  }
};

const checkDependencyCycles = () => {
  const allDependenciesCache = {};
  for (const file of Object.keys(dependencies)) {
    const allDependencies = {};
    const nextFiles = [{dependency: file, list: [file]}];

    while (nextFiles.length) {
      const {dependency, list} = nextFiles.pop();

      for (const subDependency of dependencies[dependency]) {
        if (!allDependencies[subDependency]) {
          allDependencies[subDependency] = list;

          if (allDependenciesCache[subDependency]) {
            Object.entries(allDependenciesCache[subDependency])
              .forEach(cached => allDependencies[cached[0]] = list.concat(...cached[1]))
          }
          else {
            nextFiles.push({dependency: subDependency, list: list.concat(subDependency)});
          }
        }
      }
    }

    if (allDependencies[file]) {
      throw new Error(
        `Dependency Cycle found in ${file}\nDependency Order: ${[...allDependencies[file], file].join(' -> ')}\n`);
    }

    allDependenciesCache[file] = allDependencies;
  }
};

const findDependencyOrder = () => {
  const visitedFiles = new Set();

  const exploreDependency = (file) => {
    if (visitedFiles.has(file)) return;
    for (const dependency of dependencies[file].sort()) {
      exploreDependency(dependency);
    }

    visitedFiles.add(file);
    dependencyOrder.push(file);
  };

  const startingPoints = [...Object.keys(fileText).filter(file => file.endsWith('.cpp')), mainHeader];
  startingPoints.map(file => exploreDependency(file));
};

const pad = (str) => {
  const split = Math.floor((80 - str.length) / 2);
  return ' '.padStart(split, '/') + str + ' '.padEnd(split + (str.length % 2), '/');
};

const addToOneFileHeader = (files) => {
  singleFileHeader.push('', '/**', ' * FILE ORDER');
  for (const file of files) {
    singleFileHeader.push(' * ' + file);
  }
  singleFileHeader.push(' */', '');
  for (const file of files) {
    singleFileHeader.push('', pad(`START OF ${file}`), '');
    const lines = fileText[file];
    const fileDependencyLines = dependencyLines[file];

    for (let i = 0; i < lines.length; i++) {
      if (fileDependencyLines.has(i) || pragmaOnceRegex.test(lines[i])) continue;
      singleFileHeader.push(lines[i].replace(inlineRegex, 'inline'));
    }
    singleFileHeader.push(pad(`END OF ${file}`), '');
  }
};

readSrcFiles();
findDependencies();
checkDependencyCycles();
findDependencyOrder();

addToOneFileHeader(dependencyOrder);
// addToOneFileHeader(
//   Object.keys(fileText).filter(file => !dependencyOrder.includes(file) && !file.startsWith(externalFolder)));

fs.writeFileSync('include/PokeSim.hpp', singleFileHeader.join('\n'));
