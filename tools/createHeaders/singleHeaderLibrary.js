const fs = require('fs');
const os = require('os');
const path = require('path');
const child_process = require('child_process');
const {fullPath, toRelative} = require('../utils')

const srcFolder = fullPath('src');
const externalFolder = fullPath('external');
const testLibraryFolder = path.join(externalFolder, 'Catch2');
const mainHeader = path.join(srcFolder, 'PokeSim.hpp');

const includeRegex = /^\s*#include\s+["<]([^"<>]+)([">])/;
const pragmaOnceRegex = /^\s*#pragma\s+once/;

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
        const lines = fileText[file] = fs.readFileSync(file, 'utf8').split('\n');

        while (!lines[lines.length - 1]) lines.pop();
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

const addToSingleFileHeader = (files) => {
  files = files.filter(file => path.basename(file) !== 'headers.hpp');

  singleFileHeader.push('', '/**', ' * FILE ORDER');
  for (const file of files) {
    singleFileHeader.push(' * ' + toRelative(file));
  }
  singleFileHeader.push(' */', '');

  for (const file of files) {
    const normalizedFile = toRelative(file);
    singleFileHeader.push('', pad(`START OF ${normalizedFile}`), '');
    const lines = fileText[file];
    const fileDependencyLines = dependencyLines[file];

    let startOfFile = true;
    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      if ((!line && startOfFile) || fileDependencyLines.has(i) || pragmaOnceRegex.test(line)) continue;
      startOfFile = false;
      singleFileHeader.push(line);
    }

    singleFileHeader.push('', pad(`END OF ${normalizedFile}`));
  }
};

const createSingleFileHeader = () => {
  try {
    child_process.execSync(`${os.platform() === 'win32' ? 'where' : 'which'} clang-query`);
  }
  catch (e) {
    throw new Error('clang-query was not found as is required to make the single header file.');
  }

  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), '_'));

  try {
    const tempHeaderPath = path.join(tempDir, 'PokeSim.hpp');
    fs.writeFileSync(tempHeaderPath, singleFileHeader.join('\n'));
    const query = `
    set bind-root false
    l inline functionDecl(
      unless(cxxConstructorDecl(isDefinition())),
      unless(isDefinition()),
      unless(isInline()),
      unless(isImplicit())
    ).bind("inline")
    l method cxxMethodDecl(inline)
    l function functionDecl(inline)

    m namespaceDecl(
      isExpansionInMainFile(),
      matchesName("pokesim"),
      forEach(cxxRecordDecl(forEach(method)))
    )
    m namespaceDecl(
      isExpansionInMainFile(),
      matchesName("pokesim"),
      forEach(cxxRecordDecl(
        forEach(functionTemplateDecl(forEach(method)))
      ))
    )
    m namespaceDecl(
      isExpansionInMainFile(),
      matchesName("pokesim"),
      forEach(function)
    )
    m namespaceDecl(
      isExpansionInMainFile(),
      matchesName("pokesim"),
      forEach(functionTemplateDecl(forEach(function)))
    )`;

    const queryPath = path.join(tempDir, 'query');
    fs.writeFileSync(queryPath, query);

    let buildArguments = '--extra-arg="-std=c++17"';
    if (fs.existsSync(fullPath('build'))) {
      buildArguments = `-p "${fullPath('build')}"`;
    }

    const result =
      child_process
        .execSync(`clang-query -f "${queryPath}" ${buildArguments} "${tempHeaderPath}"`, {stdio: 'pipe'})
        .toString();

    const inlineLocations = result.match(/\d+:\d+: note: "inline" binds here/g).map(resultLine => {
      const [, line, column] = /(\d+):(\d+):/.exec(resultLine);
      return {line: parseInt(line) - 1, column: parseInt(column) - 1};
    });

    inlineLocations.forEach(({line, column}) => {
      const headerLine = singleFileHeader[line];
      const newLine = `${headerLine.substring(0, column)}inline ${headerLine.substring(column)}`;
      singleFileHeader[line] = newLine;
    });

    const headerPath = fullPath('include', 'PokeSim.hpp');
    if (fs.existsSync(headerPath)) {
      const originalHeader = fs.readFileSync(headerPath, 'utf8');
      if (originalHeader !== singleFileHeader.join('\n')) {
        console.log('Made changes to single header file');
      }
    }

    fs.writeFileSync(fullPath('include', 'PokeSim.hpp'), singleFileHeader.join('\n'));
  }
  catch (e) {
    console.error(e);
  }

  fs.rmSync(tempDir, {recursive: true, force: true});
};

readSrcFiles();
findDependencies();
checkDependencyCycles();
findDependencyOrder();

addToSingleFileHeader(dependencyOrder);
createSingleFileHeader();
