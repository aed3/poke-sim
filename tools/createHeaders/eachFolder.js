const fs = require('fs');
const path = require('path');
const {getFullPath, toRelative, ignoredFiles, writeCppFile} = require('../utils');

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
        continue;
      }

      for (const subFile of fs.readdirSync(file)) {
        if (path.extname(subFile) === '.hpp') {
          headerFiles.push(path.join(file, subFile));
        }
      }
      continue;
    }
    if (path.extname(file) === '.hpp' && file !== folderHeadersFileName &&
        !ignoredFiles.includes(path.basename(file))) {
      headerFiles.push(file);
    }
  }

  if (headersFileExists) {
    headerFiles.sort();
    const fileText = '#pragma once\n\n' + headerFiles.map(file => `#include "${toRelative(file, folder)}"`).join('\n');
    writeCppFile(headerFileName, toRelative(folder), fileText, false);
    return headerFiles;
  }

  return null;
};

createHeadersFile(getFullPath('src'));