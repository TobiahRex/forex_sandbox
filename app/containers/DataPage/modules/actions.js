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
    error,
    errorMsg,
  },
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

export const receivedDataSuite = response => ({
  type: types.DATA_RECEIVE_SUITE,
  payload: response,
});
