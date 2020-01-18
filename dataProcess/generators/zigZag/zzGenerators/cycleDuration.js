/**
 @function getCycleDuration: utility function to count the overall duration,
 for a complete market cycle: change between an uptrend condition, and a downtrend
 condition. This result will be used to measure, a complete market cycle of uptrend + downtrend
 duration.
 The information will be used to determine the rate of change between uptrend conditions to
 downtrend conditions. If the market was uptrending for a long duration, then suddenly
 swinging to a downtrend condition very quickly, then that would objectively show a
 change in momentum, and this function serves to measure the change objectively.

 A smaller duration indicates a faster rate of change, and depending on the magnitude of that
 change, one could determine if the change in momentum is due to time, or due to force of
 magnitude.
 @param {array{objects}} metrics
 @return {array} vals
 */
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
