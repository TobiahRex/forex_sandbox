/**
  @function generatePlots Responsible for generating the final data points
  used to graph the zigZag line pattern given the metric data points that
  are dynamically generated.
  @param {array} metrics the result of metrics.js output.
  @return {array} plots - the plot points, that should be used to create a
  zig zag line plot on a graph.
 */

function generatePlots(metrics) {
  const plots = metrics.reduce((acc, metric) => {
    const { duration, mode, maxPriceH: high, minPriceL: low } = metric.endState;

    let count = duration;
    let runningValue = mode === 'uptrend' ? low : high;
    while (count > 0) {
      runningValue += metric.meta.slope;
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
