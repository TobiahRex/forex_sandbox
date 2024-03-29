/* eslint-disable no-param-reassign */
/**
  NOTE: ZigZag metrics is mainly responsible for generating the countable occurences,
  of various meta data. They are based on simple geometric ideas such as,
  rise, run, slope, area under line, slope magnitude, and overal slope direction.
  Given these data points, we can make discrete an otherwise continuous data set,
  and thus categorize that various data points into different categories,
  and by so doing, extract probability of those data points occuring in the future.
  Metrics generation is at the heart of generating these discrete data points.
 */

/**
  @function generateMetrics - calculates the slope and magnitude for the zigZag trend.
    - diff = difference between the highest and lowest prices
    - slope is calculated as (diff / duration) aka (rise / run) | dynamically calculated for every continuation data point.
    - magnitude is the diff * slope, divided by 2.
  @param {array} states - output of initial zig zag calculations.
  @return {array {object}} -
  [
    {
      slope: {number},
      magnitude: {number},=
      startState: {} // the state that starts the current ZZ trend
      endState: {} // the state that ends the current zz trend.
    }
  ]
*/
function generateMetrics(states) {
  const metrics = states.reduce(
    (acc, currentState, i) => {
      const prevState = states[i === 0 ? i : i - 1];

      // NOTE: if the trend is continuing.
      if (prevState && currentState.mode === prevState.mode) {
        const currentDiff = (() => {
          let highValue;
          let lowValue;

          if (currentState.mode === 'uptrend') {
            highValue = currentState.maxPriceH;
            lowValue = currentState.minPriceL;
          } else if (currentState.mode === 'downtrend') {
            highValue = currentState.minPriceL;
            lowValue = currentState.maxPriceH;
          } else {
            // mode = 'unknown'
            highValue = acc.startValue;
            lowValue = currentState.value;
          }
          return highValue - lowValue;
        })();

        acc.current.meta.vertical = currentDiff;
        acc.current.meta.slope = currentDiff / currentState.duration || 0;
        acc.current.meta.distance =
          Math.sqrt(currentDiff ** 2 + currentState.duration ** 2) * (currentDiff > 0 ? 1 : -1);
        const y = (() => {
          if (currentDiff > 0) {
            if (currentDiff < 1) return currentDiff + 1;
            return currentDiff;
          }
          if (currentDiff > -1) return currentDiff - 1;
          return currentDiff;
        })();
        acc.current.meta.magnitude = (y * currentState.duration) / 2;
      } else {
        // NOTE: if the trend has not continued.
        acc.current.meta.duration = prevState.duration;

        // NOTE: edge case for single duration values.
        if (acc.current.meta.duration === 1) {
          acc.current.meta.magnitude = acc.current.meta.vertical / 2;
        }

        // NOTE: Calculates Force of momentum:
        acc.current.meta.force = (() => {
          // TODO: Rebuild this from acceleration formula. v2^2 - v1^2 / 2d
          const { magnitude, distance, duration } = acc.current.meta;
          const velocity = distance / duration;
          return (magnitude * velocity) / duration;
        })();

        acc.current.endState = { ...prevState };
        delete acc.current.startValue;
        acc.all.push(acc.current);

        // NOTE: Setup next value object.
        const startValue = currentState.mode === 'uptrend'
          ? currentState.minPriceL :  // NOTE: The newest low
            currentState.mode === 'downtrend'
              ? currentState.maxPriceH // NOTE: The newest high
              : currentState.value // WARNING: May need to traverse history and find the most recent trend and return the appropriate value.

        const seedVal = currentState.value - startValue; // NOTE: initialize the slope as the next height difference (positive or negative) then divide by 1 (implied).
        const polarity = (seedVal > 0 ? 1 : -1);
        const diagonalDistance = Math.sqrt(seedVal ** 2 + 1);
        acc.current = {
          meta: {
            slope: seedVal,
            vertical: seedVal,
            distance: diagonalDistance * polarity,
            magnitude: 0,
            duration: 0,
          },
          startState: { ...currentState }, // NOTE: If i is less than the last value, then give the next state, otherwise give the current state.
          endState: null,
          /*
            NOTE:
            - if the new trend is down (currentState.mode === 'downtrend'), then take the prevMaxH.
            - else if the new trend is up (currentState.mode === 'uptrend'), then take the prevMaxL.

            We can't take the value, because it is the start of the previous trends data point.
          */
          startValue,
        };
      }
      return acc;
    },
    {
      current: {
        meta: {
          vertical: 0,
          slope: 0,
          distance: 0,
          magnitude: 0,
        },
        startState: states[0],
        startValue: states[0].mode === 'unknown'
          ? 0 :
            states[0].mode === 'uptrend' ? states[0].maxPriceH : states[0].minPriceL,
      },
      all: [],
      endState: null,
    },
  );

  return metrics.all;
}

module.exports = generateMetrics;
