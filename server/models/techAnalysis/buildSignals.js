import fs from 'fs';
import path from 'path';
import * as generators from './generators';

const {
  generatePriceData,
  generateRSI,
  generateTrigger,
  generateDirection,
} = generators;

export default function buildSignals(req, cb) {
  fs.readFile(
    path.resolve('./server/models/techAnalysis/data.txt'),
    'utf8',
    (err, rawPrices) => {
      if (err) cb(err);

      const cleanData = generatePriceData(rawPrices, req);

      if (req.query.type === 'prices') {
        cb(cleanData);
      }

      const rsi = generateRSI(cleanData, req);
      if (req.query.type === 'rsi') {
        cb(rsi);
      }

      const triggerData = generateTrigger({ rsi, data: cleanData }, req);
      if (req.query.type === 'trigger') {
        cb(triggerData);
      }

      const directionData = generateDirection(
        {
          data: cleanData,
          timeLength: 10,
          rawRsi: rsi,
          zigZagReversal: 5,
        },
        req,
      );
      if (req.query.type === 'direction') {
        cb(directionData);
      }

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
