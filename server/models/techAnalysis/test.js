let count = 100;
const numbers = [];
while (count > 50) {
  numbers.push(count);
  count -= 1;
}
while (count < 100) {
  numbers.push(count);
  count += 1;
}
const result = generateZigZag({ data: numbers, reversalAmount: 0.01 });

// result.plots.forEach(val => console.log(val));

function generateZigZag({ data, reversalAmount }) {
  const defaultState = {
    mode: 'init', // NOTE: 'init', 'uptrend', 'downtrend', 'unknown'
    maxPriceH: -Infinity,
    minPriceL: Infinity,
    newMax: false,
    newMin: false,
    prevMaxH: -Infinity,
    prevMinL: Infinity,
    newState: false,
  };
  const states = [];
  data.forEach((value, i) => {
    const priceH = value;
    const priceL = value;
    const hlPivot = reversalAmount / 100;

    const state = { ...defaultState, value };

    if (i === 0) {
      state.maxPriceH = value;
      state.minPriceL = value;
      state.newMax = true;
      state.newMin = true;
      state.mode = 'unknown';
    } else {
      state.mode = states[i - 1].mode;
      state.prevMaxH = states[i - 1].maxPriceH;
      state.prevMinL = states[i - 1].minPriceL;
    }
    states.push(state);

    if (states[i].mode === 'unknown') {
      if (priceH >= state.prevMaxH) {
        state.mode = 'uptrend';
        state.maxPriceH = priceH;
        state.minPriceL = state.prevMinL;
        state.newMax = true;
        state.newMin = false;
      } else if (priceL <= state.prevMinL) {
        state.mode = 'downtrend';
        state.maxPriceH = state.prevMaxH;
        state.minPriceL = priceL;
        state.newMax = false;
        state.newMin = true;
      } else {
        state.mode = 'unknown';
        state.maxPriceH = state.prevMaxH;
        state.maxPriceL = state.prevMinL;
        state.newMax = false;
        state.newMin = false;
      }
    } else if (state.mode === 'downtrend') {
      if (priceH >= state.prevMinL + state.prevMinL * hlPivot) {
        state.mode = 'uptrend';
        state.maxPriceH = priceH;
        state.minPriceL = state.prevMinL;
        state.newMax = true;
        state.newMin = false;
      } else {
        state.mode = 'downtrend';
        state.maxPriceH = state.prevMaxH;
        state.newMax = false;

        if (priceL <= state.prevMinL) {
          state.minPriceL = priceL;
          state.newMin = true;
        } else {
          state.minPriceL = state.prevMinL;
          state.newMin = false;
        }
      }
    } else if (state.mode === 'uptrend') {
      if (priceL <= state.prevMaxH - state.prevMaxH * hlPivot) {
        // NOTE: Uptrend but now a downtrend
        state.mode = 'downtrend';
        state.maxPriceH = state.prevMaxH;
        state.minPriceL = priceL;
        state.newMax = false;
        state.newMin = true;
      } else {
        state.mode = 'uptrend'; // NOTE: Continuation of the uptrend
        state.minPriceL = state.prevMinL;
        state.newMin = false;

        if (priceH >= state.prevMaxH) {
          state.maxPriceH = priceH;
          state.newMax = true;
        } else {
          state.maxPriceH = state.prevMaxH;
          state.newMax = false;
        }
      }
    }
  });

  /**
    Generate chunks that describe the beginning of a trend, and the slope for that trend.
    For every data point within that chunk, describe the plot point by adding the slope to the
    startValue.  For example: if the start point is 1 and the slope is +2, and the duration
    is 3, then the result of the chunk would be: [1, 3, 6].
    @param {Array {Object}} states - { mode: 'downtrend', maxPriceH: 100, minPriceL: 5, newMax: false, newMin: true, prevMaxH: 100, prevMinL: 6, newState: false, value: 5 };
    @return {Object}
  */
  const metrics = states.reduce(
    (acc, state, i) => {
      const prevState = states[i - 1 > 0 ? i - 1 : 0];
      const diff =
        i > 0
          ? state.value - prevState.value
          : states[i + 1].value - state.value;

      if (state.mode === prevState.mode) {
        acc.current.duration += 1;
        acc.current.changeSize = Math.abs(diff) + acc.current.prevChangeSize; // NOTE: Describes the vertical size of the trend
        acc.current.prevChangeSize = acc.current.changeSize;
      } else if (prevState) {
        if (acc.current.changeSize === 0)
          acc.current.changeSize = Math.abs(diff);

        acc.current.magnitude = // NOTE: Distance travelled by the vertical size divided by 2.
          (acc.current.duration * acc.current.changeSize) / 2;
        acc.current.slope =
          (acc.current.changeSize / acc.current.duration) *
          (state.value - acc.current.startValue > 0 ? 1 : -1);
        delete acc.current.prevChangeSize; // NOTE: Deleting because we don't need it for the follow-on iterative step.
        acc.all.push(acc.current);
        acc.current = {
          startValue: state.value,
          changeSize: 0,
          slope: state.value - prevState.value, // NOTE: rise over run distance of one.
          prevChangeSize: state.value - prevState.value,
          duration: 1,
        };
      }

      if (i === states.length - 1) {
        if (acc.current.changeSize === 0) {
          acc.current.changeSize = Math.abs(state.value - states[i + 1].value);
        }
        acc.current.magnitude = // NOTE: Distance travelled by the vertical size divided by 2.
          (acc.current.duration * acc.current.changeSize) / 2;
        acc.current.slope =
          (acc.current.changeSize / acc.current.duration) *
          (state.value - acc.current.startValue > 0 ? 1 : -1);
        delete acc.current.prevChangeSize; // NOTE: Deleting because we don't need it for the follow-on iterative step.
        acc.all.push(acc.current);
      }
      return acc;
    },
    {
      current: {
        startValue: 0,
        changeSize: 0,
        prevChangeSize: 0,
        duration: 0,
        slope: 0,
        magnitude: 0,
      },
      all: [],
    },
  );

  const plots = metrics.all.reduce((acc, metric) => {
    let iterations = metric.duration;
    let runningValue = metric.startValue;
    while (iterations > 0) {
      runningValue += metric.slope;
      acc.push(runningValue);
      iterations -= 1;
    }
    return acc;
  }, []);

  return {
    states,
    metrics: metrics.all,
    plots,
  };
}
