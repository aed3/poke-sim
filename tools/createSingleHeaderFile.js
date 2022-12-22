const fs = require('fs');
const path = require('path');

process.chdir(path.join(__dirname, '..'));

const srcFolder = 'src';
const mainHeader = path.join('src', 'PokeSim.hpp');

const includeRegex = /^\s*#include\s+["<]([^"<>]+)([">])/;
const pragmaOnceRegex = /^\s*#pragma\s+once/;
const inlineRegex = /\/\*\s*SHF_inline\s*\*\//g;

const fileData = {};
const dependencies = {};
const dependencyLines = {};
const dependencyOrder = [];

const singleFileHeader = ['#pragma once'];

const readSrcFiles = () => {
  const nextFiles = [srcFolder];

  while (nextFiles.length) {
    const file = nextFiles.pop();
    if (fs.statSync(file).isDirectory()) {
      fs.readdirSync(file).forEach(dirFile => nextFiles.push(path.join(file, dirFile)));
    }
    else {
      const ext = path.extname(file);
      if (ext === '.cpp' || ext === '.hpp') {
        fileData[file] = fs.readFileSync(file, 'utf8').split('\n');
      }
    }
  }
};

const findDependencies = () => {
  for (const [file, lines] of Object.entries(fileData)) {
    const fileDependencies = dependencies[file] = [];
    const fileDependencyLines = dependencyLines[file] = new Set();
    const fileDirName = path.dirname(file);

    for (let i = 0; i < lines.length; i++) {
      const include = includeRegex.exec(lines[i]);
      if (!include) continue;
      const [, includePath, includeType] = include;
      let dependency = '';

      if (includeType === '"') {
        dependency = path.normalize(path.join(fileDirName, includePath));
      }

      const srcFilePath = path.join(srcFolder, includePath);
      if (fs.existsSync(srcFilePath)) {
        dependency = srcFilePath;
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
      throw new Error(`Dependency Cycle found in ${file}\nDependency Order: ${[...allDependencies[file], file].join(' -> ')}\n`);
    }

    allDependenciesCache[file] = allDependencies;
  }
};

const findDependencyOrder = () => {
  const visitedFiles = new Set();

  const exploreDependency = (file) => {
    if (visitedFiles.has(file)) return;
    for (const dependency of dependencies[file])
      exploreDependency(dependency);

    visitedFiles.add(file);
    dependencyOrder.push(file);
  };

  exploreDependency(mainHeader);
};

const pad = (str) => {
  const split = Math.floor((80 - str.length) / 2);
  return ' '.padStart(split, '/') + str + ' '.padEnd(split + (str.length % 2), '/');
};

const addToOneFileHeader = (files) => {
  for (const file of files) {
    singleFileHeader.push('', pad(`START OF ${file}`), '');
    const lines = fileData[file];
    const fileDependencyLines = dependencyLines[file];

    for (let i = 0; i < lines.length; i++) {
      if (fileDependencyLines.has(i) || pragmaOnceRegex.test(lines[i])) continue;
      singleFileHeader.push(lines[i].replace(inlineRegex, 'inline'))
    }
    singleFileHeader.push('', pad(`END OF ${file}`), '');
  }
};

readSrcFiles();
findDependencies();
checkDependencyCycles();
findDependencyOrder();

addToOneFileHeader(dependencyOrder);
addToOneFileHeader(Object.keys(fileData).filter(file => !dependencyOrder.includes(file)));

fs.writeFileSync('include/PokeSim.hpp', singleFileHeader.join('\n'));
