import { fromJS } from 'immutable';

import identity from 'utils/identity';
import * as types from './types';

export const defaultAsyncState = {
  error: false,
  errorMsg: '',
  isFetching: 0,
};

export const defaultState = fromJS({
  prices: [],
  direction: [],
  trigger: [],
}).merge(fromJS(defaultAsyncState));

export default function dataPageReducer(state = defaultState, action) {
  switch (action.type) {
    case types.DATA_REQUEST:
      return handleDataRequest(state, action);
    case types.DATA_RESPONSE:
      return handleDataResponse(state, action);
    case types.DATA_ERROR:
      return handleDataError(state, action);
    case types.DATA_RECEIVE_SUITE:
      return handleDataReceiveSuite(state, action);
    case types.DATA_RECEIVE_PRICES:
      return handleDataReceivePrices(state, action);
    case types.DATA_RECEIVE_DIRECTION:
      return handleDataReceiveDirection(state, action);
    case types.DATA_RECEIVE_TRIGGER:
      return handleReceiveTrigger(state, action);
    default:
      return state;
  }
}

function handleDataRequest(state) {
  return state.merge(
    fromJS({
      ...defaultAsyncState,
      isFetching: state.get('isFetching', 0) + 1,
    }),
  );
}
function handleDataResponse(state) {
  return state.merge(
    fromJS({
      ...defaultAsyncState,
      isFetching: state.get('isFetching', 0) - 1,
    }),
  );
}
function handleDataError(state, action) {
  const {
    error,
    payload: { errorMsg },
  } = action;

  const prevFetching = state.get('isFetching', 0);

  if (typeof error === 'boolean') {
    return state.merge(
      fromJS({
        isFetching: prevFetching - 1,
        error,
        errorMsg,
      }),
    );
  }
  if (typeof error === 'string' && identity.isTruthyString(error)) {
    return state.merge(
      fromJS({
        isFetching: prevFetching - 1,
        error: true,
        errorMsg: error,
      }),
    );
  }
  return state;
}
function handleDataReceiveSuite(state, action) {
  // debugger; // TODO: reconfig payload to redux state
  return state.merge(fromJS(action.payload));
}
function handleDataReceivePrices(state, action) {
  return state.set('prices', fromJS(action.payload));
}
function handleDataReceiveDirection(state, action) {
  return state.set('direction', fromJS(action.payload));
}
function handleReceiveTrigger(state, action) {
  return state.set('trigger', fromJS(action.payload));
}
