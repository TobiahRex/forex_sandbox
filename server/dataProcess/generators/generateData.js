const fs = require('fs');
const path = require('path');

const generateZigZag = require('./zigZag/generateZigZag');

/**
  @function generateData converts array of strings, into array of objects. Also writes results to raw.prices.txt and generates ZigZag values.
  @param {array} data array of strings: ['20191001 000000;117.868000;117.877000;117.848000;117.848000;0']
  @return {object} cleanData { open, high, low, close, time }
*/
function generateData(data) {
  const cleanData = data
    .split('\n')
    .filter(Boolean)
    .map(dayString => {
      const [time, open, high, low, close] = dayString.split(';');
      const [date, clock] = time.split(' ');

      const year = date.slice(0, 4);
      const month = date.slice(4, 6);
      const day = date.slice(6, 8);
      const hour = String(clock).slice(0, 2);
      const min = String(clock).slice(2, 4);
      const sec = String(clock).slice(4, 6);
      const cleanTime = new Date(
        `${year}-${month}-${day} ${hour}:${min}:${sec}`,
      );

      return {
        dateString: cleanTime,
        time: cleanTime.getTime(),
        open: Number(open),
        high: Number(high),
        low: Number(low),
        close: Number(close),
      };
    });

  fs.writeFile(
    path.resolve('./dataProcess/raw/prices.txt'),
    cleanData.map(({ close }) => close + '\n').join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error(_err);
    },
  );

  const priceZigZag = generateZigZag({
    times: cleanData.map(({ time }) => time),
    data: cleanData.map(({ close }) => close),
    reversalAmount: 0.01,
  });

  fs.writeFile(
    path.resolve('./dataProcess/raw/price-zigZag.txt'),
    priceZigZag.plots.map(val => `${val}\n`).join(''), // eslint-disable-line
    _err => {
      if (_err) throw new Error('_err: ', _err);
    },
  );

  return cleanData;
}

module.exports = generateData;
