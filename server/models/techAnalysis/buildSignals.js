const fs = require('fs');
const path = require('path');

const {
  generateData,
  generateRSI,
  generateTrigger,
  generateDirection,
} = require('./generators');

function buildSignals(cb) {
  fs.readFile(
    path.resolve('./server/models/techAnalysis/data.txt'),
    'utf8',
    (err, data) => {
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
        zzDurations: trigZZDurations,
        zzMagnitudes: trigZZMagnitudes,
      } = triggerData;
      const {
        average: dirAverage,
        averageSmall: dirAverageSmall,
        averageBig: dirAverageBig,
        zigZag: dirZigZag,
        zzDurations: dirZZDurations,
        zzMagnitudes: dirZZMagnitudes,
      } = directionData;
      // console.log('directionData: ', directionData);

      const signals = {
        priceData: cleanData,
        trigAverage,
        trigAverageSmall,
        trigAverageBig,
        trigZigZag,
        trigZZDurations,
        trigZZMagnitudes,
        dirAverage,
        dirAverageSmall,
        dirAverageBig,
        dirZigZag,
        dirZZDurations,
        dirZZMagnitudes,
      };

      cb(null, signals);
    },
  );
}

module.exports = buildSignals;
