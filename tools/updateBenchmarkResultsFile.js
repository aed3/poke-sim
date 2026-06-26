const fs = require('fs');
const os = require('os');
const util = require('util');
const {execSync, exec} = require('child_process');
const {getFullPath} = require('./utils.js');

console.time('Total Time');
const currentOsType = os.type();
const currentCpus = os.cpus();
const currentMemory = Math.floor(os.totalmem() / 1e6);

const benchmarkFilePath = getFullPath('extras', 'RecentBenchmarkResults.md');

const parseBenchmarksLines = (lines, current, results = {}) => {
  const newInfoPerLine = new Array(lines.length);
  const splitToColumns = (line) => line.split('|').slice(1, -1).map(cell => cell.trim());

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (line.startsWith('## ')) {
      current.test = {name: line.substring(3)};
      current.columns = splitToColumns(lines[i + 1]);
      results[current.test.name] = current.test;

      i += 2;
      continue;
    }
    if (!line.startsWith('|')) {
      continue;
    }

    const [inputs, ...columns] = splitToColumns(line);
    if (isNaN(parseInt(inputs))) {
      continue;
    }

    const row = current.test[inputs] = {Inputs: parseInt(inputs)};
    for (let column = 0; column < columns.length; column++) {
      let columnValue = columns[column];
      const columnName = current.columns[column + 1];
      if (columnValue.endsWith('ms')) {
        columnValue = 1e6 * parseFloat(columnValue);
      }
      else if (columnValue.endsWith('us')) {
        columnValue = 1e3 * parseFloat(columnValue);
      }
      else {
        columnValue = parseFloat(columnValue);
      }

      row[columnName] = columnValue;
    }

    newInfoPerLine[i] = {...row, test: current.test.name};
  }

  return newInfoPerLine;
};

const parseExistingBenchmarkFile = () => {
  if (!fs.existsSync(benchmarkFilePath)) return null;

  const benchmarkFile = fs.readFileSync(benchmarkFilePath, 'utf8');
  const [/*Title*/, /*System*/, osType, cpu, totalMemory, /*Results Title*/, ...resultLines] =
    benchmarkFile.split('\n');
  const parseSystemLine = (line) => line.substring(line.indexOf(':') + 2);

  if (!currentCpus.every(cpu => cpu.model === currentCpus[0].model)) {
    return null;
  }

  if (currentOsType !== parseSystemLine(osType)) {
    return null;
  }
  if (currentCpus[0].model !== parseSystemLine(cpu)) {
    return null;
  }
  if (currentMemory !== parseInt(parseSystemLine(totalMemory))) {
    return null;
  }

  const results = {};
  const current = {test: {}, columns: []};

  parseBenchmarksLines(resultLines, current, results);

  return results;
};

const buildBenchmarks = () => {
  console.time('Build Time');
  execSync('cmake --preset benchmark-release', {cwd: getFullPath(), stdio: 'inherit'});
  execSync('cmake --build --preset benchmark-release', {cwd: getFullPath(), stdio: 'inherit'});
  console.timeEnd('Build Time');
};

const previousTest = parseExistingBenchmarkFile();
buildBenchmarks();
let newFile = `# Benchmarks
## System Info
- OS: ${currentOsType}
- CPU: ${currentCpus[0].model}
- Memory: ${currentMemory}MB
# Results
`;

console.time('Benchmark Time');
const benchmarkProcess = exec('./build/bin/release/PokeSimBenchmarks', {cwd: getFullPath()}, (error) => {
  console.timeEnd('Benchmark Time');
  if (!error) {
    fs.writeFileSync(benchmarkFilePath, newFile);
  }
  console.timeEnd('Total Time');
});

const current = {
  test: {},
  columns: {},
  text: '',
};

process.stdout.write(newFile);

benchmarkProcess.stdout.on('data', (chunk) => {
  const lastLineLength = current.text.length;
  current.text += chunk;
  newFile += chunk;
  if (current.text.includes('\n')) {
    const lines = current.text.split('\n');
    current.text = lines.pop();
    const newInfoPerLine = parseBenchmarksLines(lines, current);
    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      process.stdout.write(i == 0 ? line.substring(lastLineLength) : line);

      const newInfo = newInfoPerLine[i];
      const previousInfo = previousTest?.[newInfo?.test]?.[newInfo?.Inputs];
      if (newInfo?.['Mean / Input Count'] && previousInfo?.['Mean / Input Count']) {
        const diff = newInfo['Mean / Input Count'] - previousInfo['Mean / Input Count'];
        const percentDiff = Math.abs(diff / previousInfo['Mean / Input Count']);
        let valueDiff = diff;

        let unit = 'ns';
        if (Math.abs(valueDiff) >= 1e6) {
          valueDiff /= 1e6;
          unit = 'ms';
        }
        else if (Math.abs(valueDiff) >= 1e3) {
          valueDiff /= 1e3;
          unit = 'us';
        }

        valueDiff = valueDiff.toFixed(3);
        let color = '';
        let suffix = '';
        if (valueDiff > 0) {
          valueDiff = '+' + valueDiff;
          suffix = '% slower';
          color = 'red';
        }
        else {
          suffix = '% faster';
          color = 'green';
        }

        if (percentDiff <= 0.05) {
          color = 'reset';
        }

        process.stdout.write(
          util.styleText(color, (100 * percentDiff).toFixed(3).padStart(10) + suffix + ', ' + valueDiff + unit));
      }
      process.stdout.write('\n');
    }
    process.stdout.write(current.text);
  }
  else {
    process.stdout.write(chunk);
  }
});
