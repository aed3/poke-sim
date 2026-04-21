const fs = require('fs');
const path = require('path');
const {getFullPath, toRelative, ignoredFiles, writeCppFile} = require('./utils');

require('./createFolderHeaderFiles.js');
require('./createArgumentChecks.js');

const srcFolder = getFullPath('src');
const externalFolder = getFullPath('external');
const testLibraryFolder = path.join(externalFolder, 'Catch2');
const mainHeader = path.join(srcFolder, 'PokeSim.hpp');

const includeRegex = /^\s*#include\s+["<]([^"<>]+)([">])/;
const pragmaOnceRegex = /^\s*#pragma\s+once/;

const fileText = {};
const dependencies = {};
const dependencyLines = {};
const dependencyOrder = [];

const combinedHeaderFile = ['#pragma once'];
const combinedSourceFile = ['#include "PokeSim.hpp"'];

const readSrcFiles = () => {
  const nextFiles = [srcFolder, externalFolder];

  while (nextFiles.length) {
    const file = nextFiles.pop();
    if (fs.statSync(file).isDirectory() && file !== testLibraryFolder) {
      fs.readdirSync(file).forEach(dirFile => nextFiles.push(path.join(file, dirFile)));
      continue;
    }

    const ext = path.extname(file);
    const basename = path.basename(file);
    if (['.cpp', '.hpp', '.h'].includes(ext) && !ignoredFiles.includes(basename)) {
      const lines = fileText[file] = fs.readFileSync(file, 'utf8').split('\n');

      while (!fileText[file][lines.length - 1]) {
        lines.pop();
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

      if (dependency && !ignoredFiles.includes(path.basename(dependency))) {
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

      if (!dependencies[dependency]) {
        throw new Error(`Dependencies for ${dependency} not found`);
      }

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

const addToCombinedFiles = (files, isSource) => {
  const combinedFileLines = isSource ? combinedSourceFile : combinedHeaderFile;
  files = files.filter(file => path.basename(file) !== 'headers.hpp' && (path.extname(file) === '.cpp') === isSource);

  combinedFileLines.push('', '/**', ' * FILE ORDER');
  for (const file of files) {
    combinedFileLines.push(' * ' + toRelative(file));
  }
  combinedFileLines.push(' */', '');

  const libraryIncludes = new Set();
  for (const file of files) {
    const normalizedFile = toRelative(file);
    const lines = fileText[file];
    const fileDependencyLines = dependencyLines[file];

    const savedFileLines = [];
    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      if ((!line && !savedFileLines.at(-1)) || fileDependencyLines.has(i) || pragmaOnceRegex.test(line)) continue;

      const includePath = includeRegex.exec(line)?.[1];
      if (includePath) {
        if (libraryIncludes.has(includePath)) continue;
        libraryIncludes.add(includePath);
      }

      savedFileLines.push(line);
    }

    if (savedFileLines.length) {
      combinedFileLines.push(
        '',
        pad(`START OF ${normalizedFile}`),
        '',
        ...savedFileLines,
        '',
        pad(`END OF ${normalizedFile}`),
      );
    }
  }

  writeCppFile('PokeSim.' + (isSource ? 'cpp' : 'hpp'), 'extras', combinedFileLines.join('\n'), false);
};

readSrcFiles();
findDependencies();
checkDependencyCycles();
findDependencyOrder();

addToCombinedFiles(dependencyOrder, false);
addToCombinedFiles(dependencyOrder, true);
