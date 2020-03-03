import * as types from './types';

const dataRequest = payload => ({
  type: types.DATA_REQUEST,
  payload,
});

const dataResponse = payload => ({
  type: types.DATA_RESPONSE,
  payload,
});

const dataError = payload => ({
  type: types.DATA_ERROR,
  payload,
  error: payload.error,
});

export const async = {
  request: dataRequest,
  response: dataResponse,
  error: dataError,
};

export const getDataSuite = payload => ({
  type: types.DATA_GET_SUITE,
  payload,
});

export const getPriceData = payload => ({
  type: types.DATA_GET_PRICES,
  payload,
});

export const getDirectionData = payload => ({
  type: types.DATA_GET_DIRECTION,
  payload,
});

export const getTriggerData = payload => ({
  type: types.DATA_GET_TRIGGER,
  payload,
});

export const receivedDataSuite = payload => ({
  type: types.DATA_RECEIVE_SUITE,
  payload,
});

export const receivedPriceData = payload => ({
  type: types.DATA_RECEIVE_PRICES,
  payload,
});
export const receivedDirectionData = payload => ({
  type: types.DATA_RECEIVE_DIRECTION,
  payload,
});
export const receivedTriggerData = payload => ({
  type: types.DATA_RECEIVE_TRIGGER,
  payload,
});
