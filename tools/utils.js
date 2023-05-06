const path = require('path');

/** @type {(...paths: string[]) => string} */
const fullPath = path.join.bind({}, __dirname, '..');

const toRelative = (to, from = fullPath()) => path.relative(from, to).split(path.sep).join('/');

module.exports = {
  fullPath,
  toRelative,
}