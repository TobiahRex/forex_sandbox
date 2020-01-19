import * as types from './types';

export const getData = options => ({
  type: types.GET_DATA,
  payload: options,
});

export const dataReceived = response => ({
  type: types.RECEIVED_DATA_SUITE,
  payload: response,
});

export const dataError = error => ({
  type: types.RECEIVED_ERROR,
  error,
});
