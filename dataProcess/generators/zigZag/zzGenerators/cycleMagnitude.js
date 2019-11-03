function getCycleMagnitude(metrics) {
  return metrics.reduce(
    (acc, val) => {
      if (!acc.prevValue || val.endState.mode === 'uptrend') {
        acc.prevValue = val;
      } else if (
        acc.prevValue.endState.mode === 'uptrend' &&
        val.endState.mode === 'downtrend'
      ) {
        const combo =
          Math.abs(acc.prevValue.magnitude) + Math.abs(val.magnitude);
        acc.prevValue = null;
        acc.vals.push(combo);
      }

      return acc;
    },
    {
      prevValue: null,
      vals: [],
    },
  ).vals;
}

module.exports = getCycleMagnitude;
