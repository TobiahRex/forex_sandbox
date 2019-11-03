/* eslint-disable no-param-reassign */
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
      magnitude: {number},
      startState: {} // the state that starts the current ZZ trend
      endState: {} // the state that ends the current zz trend.
    }
  ]
*/

function generateMetrics(states) {
  const metrics = states.reduce(
    (acc, currentState, i) => {
      const prevState = states[i === 0 ? i : i - 1];

      if (prevState && currentState.mode === prevState.mode) {
        const currentDiff = (() => {
          let highValue;
          let lowValue;
          if (currentState.mode === 'uptrend') {
            lowValue = currentState.minPriceL;
            highValue = currentState.maxPriceH;
            return highValue - lowValue;
          }
          if (currentState.mode === 'downtrend') {
            highValue = currentState.minPriceL;
            lowValue = currentState.maxPriceH;
            return highValue - lowValue;
          }
          // mode = 'unknown'
          highValue = acc.startValue;
          lowValue = currentState.value;
          return highValue - lowValue;
        })();
        acc.current.slope = currentDiff / currentState.duration || 0;
      } else {
        acc.current.magnitude = (currentState.duration * acc.current.slope) / 2;
        acc.current.endState = { ...prevState };

        delete acc.current.startValue;
        acc.all.push(acc.current);

        const startValue = (() => {
          if (currentState.mode === 'uptrend') {
            return currentState.minPriceL; // NOTE: The newest low
          }
          if (currentState.mode === 'downtrend') {
            return currentState.maxPriceH; // NOTE: The newest high
          }
          return currentState.value; // WARNING: May need to traverse history and find the most recent trend and return the appropriate value.
        })();

        acc.current = {
          slope: currentState.value - startValue, // NOTE: initialize the slope as the next height difference (positive or negative) then divide by 1 (implied).
          startState: { ...currentState }, // NOTE: If i is less than the last value, the give the next state, otherwise give the current state.
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
        slope: 0,
        startState: states[0],
        startValue:
          states[0].mode === 'uptrend'
            ? states[0].maxPriceH
            : states[0].minPriceL,
      },
      all: [],
      endState: null,
    },
  );

  return metrics.all;
}

module.exports = generateMetrics;
