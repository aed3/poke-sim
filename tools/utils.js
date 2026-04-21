const fs = require('fs');
const os = require('os');
const path = require('path');
const {execSync} = require('child_process');
const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), '_'));
process.on('exit', () => fs.rmSync(tempDir, {recursive: true, force: true}));

/** @type {(...paths: string[]) => string} */
const getFullPath = path.join.bind({}, __dirname, '..');
const toRelative = (to, from = getFullPath()) => path.relative(from, to).split(path.sep).join('/');

const ignoredFiles = ['EntityViewer.cpp', 'EntityViewer.hpp'];

const writeCppFile = (filename, relativePath, text, format = true) => {
  text = '// AUTO GENERATED FILE, DO NOT EDIT\n\n' + text;

  if (format) {
    const tempFolderPath = path.join(tempDir, relativePath);
    if (!fs.existsSync(tempFolderPath)) {
      fs.mkdirSync(tempFolderPath, {recursive: true});
    }

    const tempFilPath = path.join(tempFolderPath, filename);
    fs.writeFileSync(tempFilPath, text);

    execSync(`clang-format --style="file:${getFullPath('.clang-format')}" -i ${tempFilPath}`, {cwd: getFullPath()});
    text = fs.readFileSync(tempFilPath, 'utf8');
  }

  const fullPath = getFullPath(relativePath, filename);
  if (fs.existsSync(fullPath) && fs.readFileSync(fullPath, 'utf8') === text) {
    return;
  }

  fs.writeFileSync(fullPath, text);
  console.log(`Made changes to ${path.join(relativePath, filename)}`);
};


/** @param {string} component */
const noBase = (component) => component.replace(/^pokesim::/, '');

module.exports = {
  getFullPath,
  toRelative,
  tempDir,
  ignoredFiles,
  writeCppFile,
  noBase,
}
