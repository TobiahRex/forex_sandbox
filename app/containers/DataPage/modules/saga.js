import { all, call, put, takeLatest } from 'redux-saga/effects';

import request from 'utils/request';
import * as types from './types';
import { receivedDataSuite, async } from './actions';
// import { reposLoaded, repoLoadingError } from 'containers/App/actions';

export default function* githubData() {
  yield takeLatest(types.DATA_GET_SUITE, getDataSuite);
}

export function* getDataSuite(action) {
  const { payload } = action;
  const requestURL = `/data/generate/all?log=${payload.log}&return=${payload.return}&write=${payload.write}`; // eslint-disable-line
  debugger;
  try {
    const [data] = yield all([call(request, requestURL), put(async.request())]);
    yield put(receivedDataSuite(data));
  } catch (err) {
    /**
     * TODO: render out this error properly!!!!
     * TODO: figure out why - hunch = response is too big | start by returning tiny chunk from server
     */
    debugger;
    yield put(async.error(err));
  }
}
