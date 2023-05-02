const fs = require('fs');
const path = require('path');

process.chdir(path.join(__dirname, '..', 'src'));

const headerFileName = 'headers.hpp';

const createHeadersFile = (folder) => {
  const folderHeadersFileName = path.join(folder, headerFileName);
  const headersFileExists = fs.existsSync(folderHeadersFileName);

  const files = fs.readdirSync(folder).map(file => path.join(folder, file));
  const headerFiles = [];

  for (const file of files) {
    if (fs.statSync(file).isDirectory()) {
      const subFolderHeaderFile = createHeadersFile(file);
      if (subFolderHeaderFile) headerFiles.push(subFolderHeaderFile);
    }
    else if (path.extname(file) === '.hpp' && file !== folderHeadersFileName) {
      headerFiles.push(file);
    }
  }

  if (headersFileExists) {
    headerFiles.sort();
    fs.writeFileSync(folderHeadersFileName, '#pragma once\n\n');
    fs.appendFileSync(folderHeadersFileName,
      headerFiles.map(file => `#include "${path.relative(folder, file).split(path.sep).join('/')}"`).join('\n'));
    return folderHeadersFileName;
  }

  return null;
};

createHeadersFile('.');