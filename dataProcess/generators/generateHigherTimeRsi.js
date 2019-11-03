const EMA = require('../indicators/ema');

function generateHigherTimeRsi({ data, timeLength, rsiEmaLength }) {
  const values = data
    .map((_price, i) => {
      // NOTE: calculate average price across the time length
      const { close } = _price;
      const pastPrice = data[i - timeLength > 0 ? i - timeLength : 0];

      let averageClose = _price.close;
      if (pastPrice) {
        averageClose = (close + pastPrice.close) / 2;
      }
      const price = { ..._price, averageClose };
      return price;
    })
    .map((_price, i, netChangeArr) => {
      // NOTE: calculate net change
      const lastPrice = netChangeArr[i > 0 ? i - 1 : i];
      const change = _price.averageClose - lastPrice.averageClose;
      const price = {
        ..._price,
        netChange: change,
        totChange: Math.abs(change),
      };
      return price;
    });

  const avgNetChange = new EMA({
    values: values.map(({ netChange }) => netChange),
    period: timeLength * rsiEmaLength,
  });

  const avgTotalChange = new EMA({
    values: values.map(({ totChange }) => totChange),
    period: timeLength * rsiEmaLength,
  });

  const changeRatios = avgNetChange.map((netChange, i) => {
    if (avgTotalChange[i]) {
      return netChange / avgTotalChange[i];
    }
    return 0;
  });

  const rsi = changeRatios.map(changeRatio => 50 * (1 + changeRatio) - 50);
  return rsi;
}

module.exports = generateHigherTimeRsi;
