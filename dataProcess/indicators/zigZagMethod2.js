function generateZigZag() {
  const defaultState = {
    mode: 'init', // NOTE: 'init', 'uptrend', 'downtrend', 'unknown'
    maxPriceH: -Infinity,
    minPriceL: Infinity,
    newMax: false,
    newMin: false,
    prevMaxH: -Infinity,
    prevMinL: Infinity,
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

  const metrics = states.reduce(
    (acc, state, i) => {
      const prevState = states[i - 1 > 0 ? i - 1 : 0];
      acc.current.duration += 1;
      if (state.mode !== prevState.mode) {
        let slope;
        if (state.mode === 'uptrend') {
          slope = (state.maxPriceH - state.minPriceL) / acc.current.duration;
        } else {
          slope = (state.minPriceL - state.maxPriceH) / acc.current.duration;
        }
        acc.current.slope = slope;
        acc.all.push({
          ...state,
          ...acc.current,
        });

        acc.current = {
          slope: 0,
          duration: 0,
        };
      }
      return acc;
    },
    {
      current: {
        slope: 0,
        duration: 0,
      },
      all: [],
    },
  );

  const plots = metrics.all.reduce((acc, metric) => {
    let count = metric.duration;
    let runningValue = metric.slope > 0 ? metric.minPriceL : metric.maxPriceH;
    acc.push(runningValue);
    while (count > 1) {
      runningValue += metric.slope;
      acc.push(runningValue);
      count -= 1;
    }
    return acc;
  }, []);

  return {
    states,
    metrics: metrics.all,
    plots,
  };
}
