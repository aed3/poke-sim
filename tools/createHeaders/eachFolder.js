const fs = require('fs');
const path = require('path');
const {fullPath, toRelative} = require('../utils');

const headerFileName = 'headers.hpp';

const createHeadersFile = (folder) => {
  const folderHeadersFileName = path.join(folder, headerFileName);
  const headersFileExists = fs.existsSync(folderHeadersFileName);

  const files = fs.readdirSync(folder).map(file => path.join(folder, file));
  const headerFiles = [];

  for (const file of files) {
    if (fs.statSync(file).isDirectory()) {
      const subFolderHeaderFiles = createHeadersFile(file);
      if (subFolderHeaderFiles) {
        headerFiles.push(...subFolderHeaderFiles);
      }
      else {
        const sudFolderFiles = fs.readdirSync(file)
                                 .filter(sudFile => path.extname(sudFile) === '.hpp')
                                 .map(sudFile => path.join(file, sudFile));
        headerFiles.push(...sudFolderFiles);
      }
    }
    else if (path.extname(file) === '.hpp' && file !== folderHeadersFileName) {
      headerFiles.push(file);
    }
  }

  if (headersFileExists) {
    headerFiles.sort();
    fs.writeFileSync(folderHeadersFileName, '#pragma once\n\n');
    fs.appendFileSync(
      folderHeadersFileName, headerFiles.map(file => `#include "${toRelative(file, folder)}"`).join('\n'));
    return headerFiles;
  }

  return null;
};

createHeadersFile(fullPath('src'));