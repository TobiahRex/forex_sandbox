import * as types from './types';

const dataRequest = options => ({
  type: types.DATA_REQUEST,
  payload: options,
});

const dataResponse = options => ({
  type: types.DATA_RESPONSE,
  payload: options,
});

const dataError = ({ error, errorMsg }) => ({
  type: types.DATA_ERROR,
  payload: {
    errorMsg,
  },
  error,
});

export const async = {
  request: dataRequest,
  response: dataResponse,
  error: dataError,
};

export const getDataSuite = options => ({
  type: types.DATA_GET_SUITE,
  payload: options,
});

export const getPriceData = options => ({
  type: types.DATA_GET_PRICES,
  payload: options,
});

export const getDirectionData = options => ({
  type: types.DATA_GET_DIRECTION,
  payload: options,
});

export const getTriggerData = options => ({
  type: types.DATA_GET_TRIGGER,
  payload: options,
});

export const receivedDataSuite = response => ({
  type: types.DATA_RECEIVE_SUITE,
  payload: response,
});

export const receivedPriceData = response => ({
  type: types.DATA_RECEIVE_PRICES,
  payload: response,
});
export const receivedDirectionData = response => ({
  type: types.DATA_RECEIVE_DIRECTION,
  payload: response,
});
export const receivedTriggerData = response => ({
  type: types.DATA_RECEIVE_TRIGGER,
  payload: response,
});
