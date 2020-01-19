const fs = require('fs');
const path = require('path');

const EMA = require('../indicators/ema');
const generateZigZag = require('./zigZag/generateZigZag');
const getCycleMagnitude = require('./zigZag/zzGenerators/cycleMagnitude');
const getCycleDuration = require('./zigZag/zzGenerators/cycleDuration');

function generateTrigger(seedData, { writeToFiles = true }) {
  const { rsi, data } = seedData;

  const rsiAvgSmall = new EMA({
    values: rsi.map(({ value }) => value * 1.7),
    period: 8,
  });
  const rsiAvgBig = new EMA({
    values: rsi.map(({ value }) => value * 1.7),
    period: 34,
  });

  const rsiAvg = rsiAvgSmall.map((val, i) => val - rsiAvgBig[i]).filter(Boolean);
  const bulletValues = rsiAvg.map((val, i) => {
    if (rsiAvg[i + 1]) {
      return (val - rsiAvg[i + 1]) * 10;
    }
    return '';
  });
  const bullet = new EMA({
    values: bulletValues,
    period: 5,
  });
  const zigZag = generateZigZag({
    times: data.map(({ time }) => time),
    data: bullet,
    reversalAmount: 10,
  });

  const { states, metrics, plots } = zigZag;

  const bulletFastEma = new EMA({
    values: bullet,
    period: 8,
  });
  const bulletSlowEma = new EMA({
    values: plots.map(val => val * 3),
    period: 34,
  });

  const zzDurations = getCycleDuration(metrics);
  const zzMagnitudes = getCycleMagnitude(metrics);

  if (writeToFiles) {
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-bullet.txt'),
      bullet.map(val => val + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-bulletFastEma.txt'),
      bulletFastEma.map(val => val + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-bulletSlowEma.txt'),
      bulletSlowEma.map(val => val + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-mode.txt'),
      states.map(val => (val.mode === 'uptrend' ? `${1}\n` : `${-1}\n`)).join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-plots.txt'),
      plots.map(val => val + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-metrics.txt'),
      metrics.map(val => JSON.stringify(val, null, 2) + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-magnitude.txt'),
      metrics.map(val => val.magnitude + '\n').join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-duration.txt'),
      metrics
        .map(val =>
          val.slope > 0 ? `${val.endState.duration}\n` : `${val.endState.duration * -1}\n`,
        )
        .join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-cycleDuration.txt'),
      zzDurations.map(val => `${val}\n`).join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
    fs.writeFile(
      path.resolve('./server/models/techAnalysis/trigger/trigger-zigZag-cycleMagnitude.txt'),
      zzMagnitudes.map(val => `${val}\n`).join(''), // eslint-disable-line
      _err => {
        if (_err) throw new Error('_err: ', _err);
      },
    );
  }

  return {
    average: rsiAvg,
    averageSmall: rsiAvgSmall,
    averageBig: rsiAvgBig,
    zigZag,
    zzDurations,
    zzMagnitudes,
  };
}

module.exports = generateTrigger;
