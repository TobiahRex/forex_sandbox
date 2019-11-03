const fs = require('fs');
const path = require('path');

const {
  generateData,
  generateRSI,
  generateTrigger,
  generateDirection,
} = require('./generators');

function buildSignals(cb) {
  fs.readFile(path.resolve('./dataProcess/data.txt'), 'utf8', (err, data) => {
    if (err) cb(err);
    const cleanData = generateData(data);
    const rsi = generateRSI(cleanData);
    const triggerData = generateTrigger({
      rsi,
      data: cleanData,
    });

    const directionData = generateDirection({
      data: cleanData,
      timeLength: 10,
      rawRsi: rsi,
      zigZagReversal: 5,
    });

    const {
      average: trigAverage,
      averageSmall: trigAverageSmall,
      averageBig: trigAverageBig,
      zigZag: trigZigZag,
      zZCycleDuration: trigZzCycleDuration,
      zZCycleMagnitude: trigZzCycleMagnitude,
    } = triggerData;
    const {
      average: dirAverage,
      averageSmall: dirAverageSmall,
      averageBig: dirAverageBig,
      zigZag: dirZigZag,
      zZCycleDuration: dirZzCycleDuration,
      zZCycleMagnitude: dirZzCycleMagnitude,
    } = directionData;

    const signals = {
      priceData: cleanData,
      trigAverage,
      trigAverageSmall,
      trigAverageBig,
      trigZigZag,
      trigZzCycleDuration,
      trigZzCycleMagnitude,
      dirAverage,
      dirAverageSmall,
      dirAverageBig,
      dirZigZag,
      dirZzCycleDuration,
      dirZzCycleMagnitude,
    };

    cb(null, signals);
  });
}

module.exports = buildSignals;
