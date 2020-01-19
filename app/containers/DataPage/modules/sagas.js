import { call, put, takeLatest } from 'redux-saga/effects';

import request from 'utils/request';
import * as types from './types';
import { dataReceived, dataError } from './actions';
// import { reposLoaded, repoLoadingError } from 'containers/App/actions';

export default function* githubData() {
  yield takeLatest(types.GET_DATA_SUITE, getDataSuite);
}

export function* getDataSuite(action) {
  const { payload } = action;
  const requestURL = `/data/generate/all?log=${payload.log}&return=${payload.return}&write=${payload.write}`;

  try {
    const data = yield call(request, requestURL);
    yield put(dataReceived(data));
  } catch (err) {
    yield put(dataError(err));
  }
}
