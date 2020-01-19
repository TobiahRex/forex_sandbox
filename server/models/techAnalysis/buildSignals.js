import fs from 'fs';
import path from 'path';
import * as generators from './generators';

const { generatePriceData, generateRSI, generateTrigger, generateDirection } = generators;

export default function buildSignals(req, cb) {
  fs.readFile(path.resolve('./server/models/techAnalysis/data.txt'), 'utf8', (err, rawPrices) => {
    if (err) cb(err);

    const cleanData = generatePriceData(rawPrices, req);
    const rsi = generateRSI(cleanData, req);

    const triggerData = generateTrigger({ rsi, data: cleanData }, req);
    const {
      average: trigAverage,
      averageSmall: trigAverageSmall,
      averageBig: trigAverageBig,
      zigZag: trigZigZag,
      zzDurations: trigZZDurations,
      zzMagnitudes: trigZZMagnitudes,
    } = triggerData;

    const directionData = generateDirection(
      {
        data: cleanData,
        timeLength: 10,
        rawRsi: rsi,
        zigZagReversal: 5,
      },
      req,
    );
    const {
      average: dirAverage,
      averageSmall: dirAverageSmall,
      averageBig: dirAverageBig,
      zigZag: dirZigZag,
      zzDurations: dirZZDurations,
      zzMagnitudes: dirZZMagnitudes,
    } = directionData;

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
  });
}
