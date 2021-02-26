/* eslint-disable no-underscore-dangle, no-param-reassign */
const generateMetrics = require('./zzGenerators/metrics');
const generatePlots = require('./zzGenerators/plots');
/**
  @function generateZigZag creates array of zig zag values.
  @param {object} _data { data, reversalAmount, teimes }
  @return {array {object}} [{ states, metrics, plots }...]
*/
function generateZigZag(_data) {
  const { data, reversalAmount, times } = _data;

  const states = [];
  const defaultState = {
    mode: 'init', // NOTE: 'init', 'uptrend', 'downtrend', 'unknown'
    maxPriceH: 0,
    minPriceL: 0,
    newMax: false,
    newMin: false,
    prevMaxH: 0,
    prevMinL: 0,
    duration: 1,
    startTime: 0,
    endTime: 0,
    i: null,
    value: null,
    time: null,
  };

  // NOTE: populates "states" with object { ...defaultState }
  data.forEach((value, i) => {
    const priceH = value;
    const priceL = value;
    const time = times[i];
    const hlPivot = reversalAmount / 100;

    const state = { ...defaultState, value, time, i };
    const Trend = seedTrend({ states, priceH, priceL, time });

    if (i === 0) {
      state.maxPriceH = value;
      state.minPriceL = value;
      state.prevMaxH = value;
      state.prevMinL = value;
      state.newMax = true;
      state.newMin = true;
      state.mode = 'unknown';
      state.startTime = time;
    } else {
      state.mode = states[i - 1].mode;
      state.prevMaxH = states[i - 1].maxPriceH || states[i - 1].value;
      state.prevMinL = states[i - 1].minPriceL || states[i - 1].value;
    }

    states.push(state);

    if (states[i].mode === 'unknown') {
      if (priceH >= state.prevMaxH) {
        Trend.updateState(state, 'uptrend');
        Trend.updatePrevTime();
      } else if (priceL <= state.prevMinL) {
        Trend.updateState(state, 'downtrend');
        Trend.updatePrevTime();
      } else {
        Trend.updateState(state, 'unknown');
      }
    } else if (state.mode === 'downtrend') {
      if (priceH >= state.prevMinL + state.prevMinL * hlPivot) {
        // NOTE: downtrend but now a uptrend
        Trend.updateState(state, 'uptrend');
        Trend.updatePrevTime();
      } else {
        Trend.updateState(state, 'downtrend', true);
        // NOTE: Continuation of the downtrend
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
        Trend.updateState(state, 'downtrend');
        Trend.updatePrevTime();
      } else {
        // NOTE: Continuation of the uptrend
        Trend.updateState(state, 'uptrend', true);
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
  const metrics = generateMetrics(states);
  const plots = generatePlots(metrics);

  return {
    states,
    plots,
    metrics,
  };
}

/**
 * @function seedTrend
 * Initialization function to return an object with various methods to call to modify & update
 * the running sliding window state. Can update trend mode to "uptrend" or "downtrend" etc.
 * @param
 * @return
 */
function seedTrend({ states, priceH, priceL, time }) {
  const Ix = states.length ? states.length - 1 : 0;
  const state = states[Ix] || { mode: 'unknown', duration: 0 };

  /**
   * @function updateDuration
   * updates counter that tracks how long the current trend length is active.
   * @param {string} "uptrend" | "downtrend" | "unknown"
   * @return {number}
   */
  function updateDuration(thisMode) {
    if (thisMode === state.mode) {
      return state.duration + 1;
    }
    return 1;
  }

  /**
   * @function updateState
   * Updates a running state with several flags & contextual data values.
   * The state object is encapsulating several meta values about the current trend,
   * as well as the global trend (maxPriceH, maxPriceL).
   * @param {object} _state
   * @param {string} trend
   * @param {bool} continuation
   * @return {null} * updates argument "state" by reference.
   */
  function updateState(_state, trend, continuation = false) {
    if (trend === 'unknown') {
      _state.duration = updateDuration('unknown');
      _state.mode = 'unknown';
      _state.maxPriceH = _state.prevMaxH;
      _state.maxPriceL = _state.prevMinL;
      _state.newMax = false;
      _state.newMin = false;
    }

    if (!continuation) {
      _state.duration = updateDuration(trend);
      _state.mode = trend;
      _state.startTime = time;
      if (_state.mode === 'uptrend') {
        _state.minPriceL = _state.prevMinL;
        _state.maxPriceH = priceH;
        _state.newMax = true;
      } else {
        // downtrend
        _state.maxPriceH = _state.prevMaxH;
        _state.minPriceL = priceL;
        _state.newMin = true;
      }
    } else {
      _state.duration = updateDuration(trend);
      _state.mode = trend;
      if (_state.mode === 'uptrend') {
        _state.minPriceL = _state.prevMinL;
        _state.newMin = false;
      } else {
        // downtrend
        _state.maxPriceH = _state.prevMaxH;
        _state.newMax = false;
      }
    }
  }

  function updatePrevTime() {
    states[Ix].endTime = time;
  }

  return {
    updateDuration,
    updateState,
    updatePrevTime,
  };
}

module.exports = generateZigZag;
