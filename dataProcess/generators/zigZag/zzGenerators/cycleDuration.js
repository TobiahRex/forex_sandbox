function getCycleDuration(metrics) {
  return metrics.reduce(
    (acc, val) => {
      if (
        val.endState.mode === 'downtrend' &&
        acc.prevValue.mode === 'uptrend'
      ) {
        const combo = acc.prevValue.duration + val.endState.duration;
        acc.prevValue = null;
        acc.vals.push(combo);
      } else if (val.endState.mode === 'uptrend') {
        acc.prevValue = val.endState;
      }
      return acc;
    },
    {
      prevValue: null,
      vals: [],
    },
  ).vals;
}

module.exports = getCycleDuration;
