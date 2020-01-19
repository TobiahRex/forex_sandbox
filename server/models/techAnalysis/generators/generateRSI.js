const moment = require('moment');
const fs = require('fs');
const path = require('path');

const EMA = require('../indicators/ema');

function generateRSI(data, { writeToFiles = false }) {
  const netChangeVals = data.map((price, i) => {
    const lastPrice = data[i > 0 ? i - 1 : NaN];
    if (lastPrice) {
      const change = price.close - lastPrice.close;
      return Number(change.toFixed(4));
    }
    return 0;
  });

  const netChange = new EMA({
    values: netChangeVals,
    period: 14,
  });

  const totChgAvg = new EMA({
    values: netChangeVals.map(Math.abs),
    period: 14,
  });

  const changeRatios = totChgAvg.map((val, i) => {
    if (val) {
      const ratio = netChange[i] / val;
      return ratio;
    }
    return 0;
  });
  const rsi = changeRatios.map((ratio, i) => ({
    value: 50 * (ratio + 1),
    time: data[i].time,
  }));

  if (writeToFiles) {
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/raw/rsi-value.txt'),
      rsi.map(val => val.value + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/raw/time.txt'),
      rsi
        .map(
          val =>
            `${moment(new Date(val.time)).format('DD/MM/YYYY hh:mm:ss')}\n`,
        )
        .join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
  }

  return rsi;
}

module.exports = generateRSI;
