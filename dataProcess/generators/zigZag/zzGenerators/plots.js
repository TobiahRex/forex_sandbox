function generatePlots(metrics) {
  const plots = metrics.reduce((acc, metric) => {
    const { duration, mode, maxPriceH: high, minPriceL: low } = metric.endState;

    let count = duration;
    let runningValue = mode === 'uptrend' ? low : high;
    while (count > 0) {
      runningValue += metric.slope;
      acc.push(runningValue);
      count -= 1;
    }
    const endValue = mode === 'uptrend' ? high : low;
    if (endValue.toFixed(3) !== runningValue.toFixed(3)) {
      throw new Error('values didnt sync');
    }
    return acc;
  }, []);

  return plots;
}

module.exports = generatePlots;
