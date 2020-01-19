/**
 @function getCycleMagnitude: utility function to count the overall magnitude,
 for a complete market cycle: change between an uptrend condition, and a downtrend
 condition. This result will be used to measure, a complete market cycle of uptrend + downtrend.
 The information will be used to determine the rate of change between uptrend conditions to
 downtrend conditions. If the market was uptrending for a long duration, then suddenly
 swinging to a downtrend condition very quickly, then that would objectively show a
 change in momentum, and this function serves to measure the change objectively.
 @param {array{objects}} metrics
 @return {array} vals
 */
function getCycleMagnitude(metrics) {
  return metrics.reduce(
    (acc, val) => {
      if (!acc.prevValue || val.endState.mode === 'uptrend') {
        acc.prevValue = val;
      } else if (acc.prevValue.endState.mode === 'uptrend' && val.endState.mode === 'downtrend') {
        const combo = Math.abs(acc.prevValue.magnitude) + Math.abs(val.meta.magnitude);
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
