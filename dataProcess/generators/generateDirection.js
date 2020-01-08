const fs = require('fs');
const path = require('path');

const generateHigherTimeRsi = require('./generateHigherTimeRsi');
const generateZigZag = require('./zigZag/generateZigZag');
const getCycleDuration = require('./zigZag/zzGenerators/cycleDuration');
const getCycleMagnitude = require('./zigZag/zzGenerators/cycleMagnitude');
const EMA = require('../indicators/ema');

function generateDirection(options) {
  const { zigZagReversal, data } = options;

  const fastRsiDirection = generateHigherTimeRsi({
    ...options,
    rsiEmaLength: 13,
  });
  const slowRsiDirection = generateHigherTimeRsi({
    ...options,
    rsiEmaLength: 21,
  });

  const zigZag = generateZigZag({
    times: data.map(({ time }) => time),
    data: fastRsiDirection,
    reversalAmount: zigZagReversal,
  });

  const rsiAverage = new EMA({
    values: slowRsiDirection,
    period: 21,
  });

  const {
    // states,
    metrics,
    plots,
  } = zigZag;

  const rsiBull = new EMA({
    values: plots.map(val => val),
    period: 13,
  });
  const rsiBear = new EMA({
    values: plots.map(val => val),
    period: 34,
  });

  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-rsi.txt'),
    slowRsiDirection.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );
  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-rsiAverage.txt'),
    rsiAverage.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );
  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-rsiBull.txt'),
    rsiBull.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );
  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-rsiBear.txt'),
    rsiBear.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );
  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-zigZag.txt'),
    plots.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );
  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-zigZag-metrics.txt'),
    metrics.map(val => JSON.stringify(val, null, 2) + '\n').join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );

  const zzDurations = getCycleDuration(metrics);

  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-zigZag-cycleDuration.txt'),
    zzDurations.map(duration => `${duration}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );

  const zzMagnitudes = getCycleMagnitude(metrics);

  fs.writeFile(
    path.resolve('./dataProcess/direction/direction-zigZag-cycleMagnitude.txt'),
    zzMagnitudes.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );

  return {
    average: rsiAverage,
    averageSmall: rsiBull,
    averageBig: rsiBear,
    zigZag,
    zzDurations,
    zzMagnitudes,
  };
}

module.exports = generateDirection;
