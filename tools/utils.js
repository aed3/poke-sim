const fs = require('fs');
const os = require('os');
const path = require('path');
const {execSync} = require('child_process');
const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), '_'));

/** @type {(...paths: string[]) => string} */
const fullPath = path.join.bind({}, __dirname, '..');
const toRelative = (to, from = fullPath()) => path.relative(from, to).split(path.sep).join('/');

const ignoredFile = ['EntityViewer.cpp', 'EntityViewer.hpp'];
const clangBuildArguments = fs.existsSync(fullPath('build')) ? `-p "${fullPath('build')}"` : '--extra-arg="-std=c++17"';

let clangChecked = false;

const runClangQuery = (query, file, bound) => {
  if (!clangChecked) {
    try {
      execSync(`${os.platform() === 'win32' ? 'where' : 'which'} clang-query`);
      clangChecked = true;
    }
    catch (e) {
      throw new Error('clang-query was not found as is required to make the single header file.');
    }
  }

  const queryPath = path.join(tempDir, 'query');
  fs.writeFileSync(queryPath, query);

  const result = execSync(`clang-query -f "${queryPath}" ${clangBuildArguments} "${file}"`, {
    stdio: ['ignore', 'pipe', 'ignore'],
  }).toString();

  const matches = result.match(new RegExp(`\\d+:\\d+: note: "${bound}" binds here.*\n.*`, 'g'));

  if (!matches) {
    return [];
  }

  return matches.map(resultLine => {
    const [, line, column] = /(\d+):(\d+):/.exec(resultLine);
    return {line: parseInt(line) - 1, column: parseInt(column) - 1, text: resultLine.split('\n')[1]?.trim()};
  });
};

module.exports = {
  fullPath,
  toRelative,
  tempDir,
  ignoredFile,
  runClangQuery,
}